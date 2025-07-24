
#ifndef FLOWONNX_SINGLETON_H
#define FLOWONNX_SINGLETON_H

template <class T>
class Singleton {
public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    static T *get_instance() {
        static T instance;
        return &instance;
    }

protected:
    Singleton() {}

private:
    struct T_Instance : public T {
        T_Instance()
            : T() {}
    };
};

/*
Usage:

class A: public Singleton<A> {
    using Singleton<A>::Singleton;
    //Rest of functionality for class A
};

*/

#endif