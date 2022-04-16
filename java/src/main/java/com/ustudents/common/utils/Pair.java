package com.ustudents.common.utils;

/** A container for a pair of objects. */
@SuppressWarnings({"unused"})
public class Pair<T, U> {
    /** The first object of the pair. */
    private T key;

    /** The second object of the pair. */
    private U value;

    public Pair() {

    }

    /**
     * Class constructor.
     *
     * @param object1 The first object.
     * @param object2 The second object.
     */
    public Pair(T object1, U object2) {
        this.key = object1;
        this.value = object2;
    }

    /** @return the first object of the pair. */
    public T getKey() {
        return key;
    }

    /** @return the second object of the pair. */
    public U getValue() {
        return value;
    }

    /**
     * Sets the first object of the pair.
     *
     * @param object The value.
     */
    public void setKey(T object) {
        key = object;
    }

    /**
     * Sets the second object of the pair.
     *
     * @param object The value.
     */
    public void setValue(U object) {
        value = object;
    }

    @Override
    public String toString() {
        return "Pair{" +
                "object1=" + key +
                ", object2=" + value +
                '}';
    }
}
