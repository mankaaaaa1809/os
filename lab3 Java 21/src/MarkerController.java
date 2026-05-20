// MarkerController.java
//
// lab3 Java 21
//
// created by Anhelina Mankewich, 2 course, group 12

package src;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class MarkerController {
    private final MarkerArray array;
    private final List<MarkerInfo> markers;
    private final ReentrantLock ctrlLock;
    private final Condition ctrlCondition;
    private final AtomicBoolean started;
    private final ReentrantLock startLock;
    private final Condition startCondition;

    private static class MarkerInfo {
        Thread thread;
        AtomicBoolean waiting;
        AtomicBoolean terminate;
        AtomicBoolean alive;
        ReentrantLock cvLock;
        Condition condition;

        MarkerInfo() {
            this.waiting = new AtomicBoolean(false);
            this.terminate = new AtomicBoolean(false);
            this.alive = new AtomicBoolean(true);
            this.cvLock = new ReentrantLock();
            this.condition = this.cvLock.newCondition();
        }
    }

    public MarkerController(int arraySize) {
        if (arraySize <= 0) {
            throw new IllegalArgumentException("Array size must be positive");
        }
        this.array = new MarkerArray(arraySize);
        this.markers = new ArrayList<>();
        this.ctrlLock = new ReentrantLock();
        this.ctrlCondition = ctrlLock.newCondition();
        this.started = new AtomicBoolean(false);
        this.startLock = new ReentrantLock();
        this.startCondition = startLock.newCondition();
    }

    public void startMarkers(int count) {
        if (count <= 0) return;

        ctrlLock.lock();
        try {
            markers.clear();
            for (int i = 0; i < count; i++) {
                markers.add(new MarkerInfo());
            }

            for (int i = 0; i < count; i++) {
                final int id = i + 1;
                MarkerInfo info = markers.get(i);
                info.thread = new Thread(() -> markerWorker(id));
                info.thread.start();
            }
        } finally {
            ctrlLock.unlock();
        }
    }

    public void signalStartAll() {
        startLock.lock();
        try {
            started.set(true);
            startCondition.signalAll();
        } finally {
            startLock.unlock();
        }
    }

    private void markerWorker(int id) {
        int arraySize = array.size();
        Random random = new Random(id);

        startLock.lock();
        try {
            while (!started.get()) {
                startCondition.await();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return;
        } finally {
            startLock.unlock();
        }

        while (true) {
            ctrlLock.lock();
            try {
                if (markers.get(id - 1).terminate.get()) {
                    break;
                }
            } finally {
                ctrlLock.unlock();
            }

            int index = random.nextInt(arraySize);

            if (array.get(index) == 0) {
                sleepMs(5);
                if (array.tryMark(index, id)) {
                    sleepMs(5);
                    continue;
                }
            }

            int markedCount = array.countMarkedBy(id);

            synchronized (System.out) {
                System.out.println("Marker " + id + " | Marked: " + markedCount + " | Blocked at index: " + index);
            }

            markers.get(id - 1).waiting.set(true);
            
            ctrlLock.lock();
            try {
                ctrlCondition.signal();
            } finally {
                ctrlLock.unlock();
            }

            MarkerInfo info = markers.get(id - 1);
            info.cvLock.lock();
            try {
                while (!info.terminate.get() && info.waiting.get()) {
                    info.condition.await();
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            } finally {
                info.cvLock.unlock();
            }

            if (markers.get(id - 1).terminate.get()) {
                break;
            }
        }

        array.clearMarks(id);
        markers.get(id - 1).alive.set(false);
        
        ctrlLock.lock();
        try {
            ctrlCondition.signal();
        } finally {
            ctrlLock.unlock();
        }

        synchronized (System.out) {
            System.out.println("Marker " + id + " terminated and cleared its marks");
        }
    }

    public void waitAllBlocked() {
        ctrlLock.lock();
        try {
            while (true) {
                int active = 0;
                int waiting = 0;
                for (MarkerInfo marker : markers) {
                    if (marker.alive.get()) {
                        active++;
                        if (marker.waiting.get()) {
                            waiting++;
                        }
                    }
                }
                if (active > 0 && waiting == active) {
                    break;
                }
                ctrlCondition.await();
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            ctrlLock.unlock();
        }
    }

    public boolean terminateMarker(int markerId) {
        if (markerId < 1 || markerId > markers.size()) {
            return false;
        }

        ctrlLock.lock();
        try {
            if (!markers.get(markerId - 1).alive.get()) {
                return false;
            }
            markers.get(markerId - 1).terminate.set(true);
            
            MarkerInfo info = markers.get(markerId - 1);
            info.cvLock.lock();
            try {
                info.condition.signal();
            } finally {
                info.cvLock.unlock();
            }
        } finally {
            ctrlLock.unlock();
        }

        try {
            markers.get(markerId - 1).thread.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return false;
        }

        return true;
    }

    public void resumeAll() {
        ctrlLock.lock();
        try {
            for (MarkerInfo marker : markers) {
                if (marker.waiting.get() && marker.alive.get() && !marker.terminate.get()) {
                    marker.waiting.set(false);
                    marker.cvLock.lock();
                    try {
                        marker.condition.signal();
                    } finally {
                        marker.cvLock.unlock();
                    }
                }
            }
        } finally {
            ctrlLock.unlock();
        }
    }

    public void printArray() {
        int[] snapshot = array.copy();
        for (int i = 0; i < snapshot.length; i++) {
            System.out.print(snapshot[i]);
            if (i + 1 < snapshot.length) {
                System.out.print(" ");
            }
        }
        System.out.println();
    }

    public boolean hasActiveMarkers() {
        for (MarkerInfo marker : markers) {
            if (marker.alive.get()) {
                return true;
            }
        }
        return false;
    }

    public int aliveCount() {
        int count = 0;
        for (MarkerInfo marker : markers) {
            if (marker.alive.get()) {
                count++;
            }
        }
        return count;
    }

    public int findFirstAlive() {
        for (int i = 0; i < markers.size(); i++) {
            if (markers.get(i).alive.get()) {
                return i + 1;
            }
        }
        return 0;
    }

    public void joinAll() {
        ctrlLock.lock();
        try {
            for (MarkerInfo marker : markers) {
                if (marker == null) continue;
                marker.terminate.set(true);
                marker.cvLock.lock();
                try {
                    marker.condition.signal();
                } finally {
                    marker.cvLock.unlock();
                }
            }
        } finally {
            ctrlLock.unlock();
        }

        for (MarkerInfo marker : markers) {
            if (marker != null && marker.thread != null) {
                try {
                    marker.thread.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
        markers.clear();
    }

    private void sleepMs(int ms) {
        try {
            Thread.sleep(ms);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}