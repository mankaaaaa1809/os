// MarkerArray.java
//
// lab3 Java 21
//
// created by Anhelina Mankewich, 2 course, group 12

import java.util.Scanner;

public class Main {
    private static int readPositiveInt(Scanner scanner, String prompt) {
        int value = 0;
        while (true) {
            System.out.print(prompt);
            if (scanner.hasNextInt()) {
                value = scanner.nextInt();
                if (value > 0) {
                    return value;
                } else {
                    System.err.println("Error: number must be > 0");
                }
            } else {
                scanner.next();
                System.err.println("Error: enter a valid number");
            }
        }
    }

    private static int readMarkerId(Scanner scanner, int maxId) {
        int id = 0;
        while (true) {
            System.out.print("Enter marker id to terminate: ");
            if (scanner.hasNextInt()) {
                id = scanner.nextInt();
                if (id >= 1 && id <= maxId) {
                    return id;
                } else {
                    System.err.println("Marker id must be in range [1.." + maxId + "]");
                }
            } else {
                scanner.next();
                System.err.println("Invalid input! Please enter a number");
            }
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        try {
            int size = readPositiveInt(scanner, "Enter array size: ");
            MarkerController controller = new MarkerController(size);

            int count = readPositiveInt(scanner, "Enter number of marker threads: ");
            controller.startMarkers(count);
            controller.signalStartAll();

            while (controller.hasActiveMarkers()) {
                controller.waitAllBlocked();

                System.out.println("\nArray state BEFORE termination:");
                controller.printArray();

                if (controller.aliveCount() == 1) {
                    int last = controller.findFirstAlive();
                    if (last != 0) {
                        System.out.println("Only one marker (" + last + ") remains - terminating it automatically");
                        controller.terminateMarker(last);
                        System.out.println("\nArray state AFTER termination:");
                        controller.printArray();
                        break;
                    }
                }

                int id = readMarkerId(scanner, count);

                if (!controller.terminateMarker(id)) {
                    System.err.println("Invalid or already terminated marker. Try another id");
                    continue;
                }

                System.out.println("\nArray state AFTER termination:");
                controller.printArray();

                if (!controller.hasActiveMarkers()) {
                    break;
                }

                controller.resumeAll();
            }

            controller.joinAll();
            System.out.println("\nAll marker threads finished");

        } catch (Exception e) {
            System.err.println("Unhandled exception: " + e.getMessage());
            e.printStackTrace();
        } finally {
            scanner.close();
        }
    }
}