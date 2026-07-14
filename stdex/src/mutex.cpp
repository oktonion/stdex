// stdex includes
#include "../include/mutex.hpp"

// POSIX includes
/*none*/

// std includes
/*none*/

namespace stdex
{
    const adopt_lock_t adopt_lock = adopt_lock_t();
    const defer_lock_t defer_lock = defer_lock_t();
    const try_to_lock_t try_to_lock = try_to_lock_t();

    namespace detail
    {
        pthread_mutex_t* _lock_mutex_native_handle(const unique_lock<mutex>& lock)
        {
            return lock.mutex()->native_handle();
        }

        bool _lock_owns_lock(const unique_lock<mutex>& lock)
        {
            return lock.owns_lock();
        }
    } // namespace detail

} // namespace stdex
