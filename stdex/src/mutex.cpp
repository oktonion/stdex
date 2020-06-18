// stdex includes
#include "../include/mutex.hpp"

// POSIX includes
/*none*/

// std includes
/*none*/

using namespace stdex;

const adopt_lock_t adopt_lock = adopt_lock_t();
const defer_lock_t defer_lock = defer_lock_t();
const try_to_lock_t try_to_lock = try_to_lock_t();

namespace stdex
{
    namespace detail
    {
        pthread_mutex_t* _get_mutex_native_handle(const unique_lock<mutex>& lock)
        {
            return lock.mutex()->native_handle();
        }
    } // namespace detail

} // namespace stdex