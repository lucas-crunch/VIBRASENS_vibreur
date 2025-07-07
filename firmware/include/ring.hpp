#include <Arduino.h>

class RingBuffer
{
public:
    RingBuffer(size_t size, unsigned long debounce = 50) : size(size), head(0), tail(0), count(0), lastUpdateTime(0), updateInterval(debounce)
    {
        buffer = new int[size];
    }

    ~RingBuffer()
    {
        delete[] buffer;
    }

    void push(int value)
    {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime < updateInterval)
        {
            return;
        }

        buffer[head] = value;
        head = (head + 1) % size;
        if (count < size)
            count++;
        else
            tail = (tail + 1) % size;
        lastUpdateTime = currentTime;
    }

    int getAverage()
    {
        int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            sum += buffer[(tail + i) % size];
        }
        return sum / count;
    }

    int getStableValue()
    {
        int stableValue = getAverage();
        return stableValue;
    }

private:
    int *buffer;
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
    unsigned long lastUpdateTime;
    unsigned long updateInterval;
};
