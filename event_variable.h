/*
 * Event variable implements in C++11.
 *
 * file:   event_variable.h
 * date:   2016-01-03
 * author: chenhaotian93@gmail.com
 */

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <mutex>
#include <ratio>

class EventVariable {
public:
    explicit EventVariable(): m_is_set(false) {}

    void clear() { m_is_set = false; }

    bool is_set() const { return m_is_set; }

    // Notify all waiters.
    void set() {
        m_is_set = true;

        m_condition.notify_all();
    }

    // Wait synchronized.
    void wait() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_condition.wait(lock);
    }

    // Wait with timeout.
    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_is_set) {
            return true;
        }

        return std::cv_status::no_timeout ==
            m_condition.wait_for(lock, timeout);
    }

private:
    std::atomic<bool>       m_is_set;

    std::mutex              m_mutex;

    std::condition_variable m_condition;
};

