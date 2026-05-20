// MarkerArray.java
//
// lab3 Java 21
//
// created by Anhelina Mankewich, 2 course, group 12

package src;

import java.util.concurrent.locks.ReentrantLock;

public class MarkerArray {
    private final int[] data;
    private final ReentrantLock lock;

    public MarkerArray(int size) {
        if (size <= 0) {
            throw new IllegalArgumentException("Size must be positive");
        }
        this.data = new int[size];
        this.lock = new ReentrantLock();
    }

    public boolean tryMark(int index, int markerId) {
        lock.lock();
        try {
            if (index < 0 || index >= data.length) {
                return false;
            }
            if (data[index] == 0) {
                data[index] = markerId;
                return true;
            }
            return false;
        } finally {
            lock.unlock();
        }
    }

    public void clearMarks(int markerId) {
        lock.lock();
        try {
            for (int i = 0; i < data.length; i++) {
                if (data[i] == markerId) {
                    data[i] = 0;
                }
            }
        } finally {
            lock.unlock();
        }
    }

    public int[] copy() {
        lock.lock();
        try {
            int[] copy = new int[data.length];
            System.arraycopy(data, 0, copy, 0, data.length);
            return copy;
        } finally {
            lock.unlock();
        }
    }

    public int size() {
        return data.length;
    }

    public int countMarkedBy(int markerId) {
        lock.lock();
        try {
            int count = 0;
            for (int value : data) {
                if (value == markerId) {
                    count++;
                }
            }
            return count;
        } finally {
            lock.unlock();
        }
    }

    public int get(int index) {
        lock.lock();
        try {
            if (index < 0 || index >= data.length) {
                return -1;
            }
            return data[index];
        } finally {
            lock.unlock();
        }
    }
}