#pragma once

template<typename Derived>
class Singleton {
public:
    static Derived &get() {
        static Derived instance{};
        return instance;
    }

    Singleton(Singleton const &) = delete;

    Singleton &operator=(Singleton const &) = delete;

protected:
    Singleton() = default;
};
