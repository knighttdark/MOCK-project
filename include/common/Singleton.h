#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        static T instance;
        return instance;
    }
    Singleton(const Singleton &) = delete;
    void operator=(const Singleton &) = delete;

protected:
    Singleton() {}
};

#endif
