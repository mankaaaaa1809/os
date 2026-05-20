// MarkerArrayTest.java
//
// lab3 Java 21
//
// created by Anhelina Mankewich, 2 course, group 12

package tests;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import src.MarkerArray;

public class MarkerArrayTest {

    @Test
    public void testStartsWithZeros() {
        MarkerArray arr = new MarkerArray(5);
        int[] snapshot = arr.copy();
        for (int value : snapshot) {
            assertEquals(0, value);
        }
    }

    @Test
    public void testTryMarkSuccess() {
        MarkerArray arr = new MarkerArray(3);
        assertTrue(arr.tryMark(1, 2));
        assertEquals(2, arr.get(1));
    }

    @Test
    public void testTryMarkFailsWhenOccupied() {
        MarkerArray arr = new MarkerArray(3);
        arr.tryMark(1, 1);
        assertFalse(arr.tryMark(1, 2));
        assertEquals(1, arr.get(1));
    }

    @Test
    public void testClearMarksOnlyOwn() {
        MarkerArray arr = new MarkerArray(5);
        arr.tryMark(0, 1);
        arr.tryMark(1, 2);
        arr.tryMark(2, 1);
        arr.tryMark(3, 2);

        arr.clearMarks(1);

        assertEquals(0, arr.get(0));
        assertEquals(2, arr.get(1));
        assertEquals(0, arr.get(2));
        assertEquals(2, arr.get(3));
    }

    @Test
    public void testCountMarkedBy() {
        MarkerArray arr = new MarkerArray(6);
        arr.tryMark(0, 3);
        arr.tryMark(1, 3);
        arr.tryMark(2, 2);
        arr.tryMark(4, 3);

        assertEquals(3, arr.countMarkedBy(3));
        assertEquals(1, arr.countMarkedBy(2));
        assertEquals(0, arr.countMarkedBy(1));
    }

    @Test
    public void testGetOutOfBoundsReturnsMinusOne() {
        MarkerArray arr = new MarkerArray(3);
        assertEquals(-1, arr.get(100));
        assertEquals(-1, arr.get(-5));
    }

    @Test
    public void testTryMarkOutOfBoundsReturnsFalse() {
        MarkerArray arr = new MarkerArray(3);
        assertFalse(arr.tryMark(100, 1));
        assertFalse(arr.tryMark(-5, 1));
    }

    @Test
    public void testClearNonExistentIdSafe() {
        MarkerArray arr = new MarkerArray(4);
        arr.tryMark(0, 1);
        arr.tryMark(1, 1);
        arr.clearMarks(999);
        assertEquals(1, arr.get(0));
        assertEquals(1, arr.get(1));
    }

    @Test
    public void testDoubleClearSafe() {
        MarkerArray arr = new MarkerArray(3);
        arr.tryMark(0, 5);
        arr.clearMarks(5);
        arr.clearMarks(5);
        assertEquals(0, arr.get(0));
    }

    @Test
    public void testCopyReturnsIndependentCopy() {
        MarkerArray arr = new MarkerArray(3);
        arr.tryMark(0, 1);
        arr.tryMark(2, 2);

        int[] snapshot = arr.copy();
        snapshot[0] = 999;
        snapshot[1] = 888;

        assertEquals(1, arr.get(0));
        assertEquals(2, arr.get(2));
        assertEquals(999, snapshot[0]);
    }

    @Test
    public void testSizeReturnsCorrectValue() {
        MarkerArray arr = new MarkerArray(10);
        assertEquals(10, arr.size());
    }
}