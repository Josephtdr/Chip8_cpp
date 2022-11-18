#ifndef H_STACK
#define H_STACK

#include <vector>
#include <stdexcept> // for std::runtime_error

template <typename T>
class Stack
{
private:
    std::vector<T> stack;
    int sp{}; //points to top of the stack, i.e the first empty space
    int size{};

public:
    Stack(int stackSize)
        : sp{ 0 }, size{ stackSize }
    {
        stack.resize(size);
    }

    bool isFull() { return sp>=size; }
    bool isEmpty() { return sp; }

    const T& pop()
    {   
        if (isEmpty()) { throw std::runtime_error("Trying to pop from empty stack!"); }
        return stack[--sp];
    }

    void push(const T& data)
    {
        if (isFull()) { throw std::runtime_error("Trying to push to full stack!"); }
        stack[sp++] = data;
    }
};

#endif