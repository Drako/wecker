#pragma once

#include <pico/sync.h>

namespace detail {
    template<typename MutexT>
    struct LockTraits;

    template<>
    struct LockTraits<mutex_t> final {
        static void enter_blocking(mutex_t *const mutex) {
            mutex_enter_blocking(mutex);
        }

        static void exit(mutex_t *const mutex) {
            mutex_exit(mutex);
        }
    };

    template<>
    struct LockTraits<recursive_mutex_t> final {
        static void enter_blocking(recursive_mutex_t *const mutex) {
            recursive_mutex_enter_blocking(mutex);
        }

        static void exit(recursive_mutex_t *const mutex) {
            recursive_mutex_exit(mutex);
        }
    };
}

template<typename MutexT>
class LockGuard final {
public:
    explicit LockGuard(MutexT &mutex): mutex_{&mutex} {
        detail::LockTraits<MutexT>::enter_blocking(mutex_);
    }

    ~LockGuard() {
        detail::LockTraits<MutexT>::exit(mutex_);
    }

private:
    MutexT *mutex_;
};
