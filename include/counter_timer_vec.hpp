#ifndef COUNTER_TIMER_VEC_H_
#define COUNTER_TIMER_VEC_H_

#include <counter_timer.hpp>
#include <rcu_vector_bp.hpp>

namespace counter_timer_vec {

class TimersVector {
public:
	TimersVector(size_t num_types) : num_types_(num_types) {}
	TimersVector(const TimersVector &) = delete;
	TimersVector &operator=(const TimersVector &) = delete;
	TimersVector(TimersVector &&) = delete;
	TimersVector &operator=(TimersVector &&) = delete;
	~TimersVector() {
		size_t size = v_.size_locked();
		for (size_t i = 0; i < size; ++i) {
			delete v_.ref_locked(i);
		}
	}
	size_t len() const { return v_.size(); }
	const counter_timer::Timers &timers(size_t i) const {
		if (v_.size() <= i) {
			v_.lock();
			while (v_.size_locked() <= i) {
				v_.push_back_locked(new counter_timer::Timers(num_types_));
			}
			v_.unlock();
		}
		return *v_.read_copy(i);
	}
	const counter_timer::Timer &timer(size_t i, size_t type) const {
		return timers(i).timer(type);
	}
private:
	mutable rcu_vector_bp<counter_timer::Timers *> v_;
	static_assert(!decltype(v_)::need_register_thread());
	static_assert(!decltype(v_)::need_unregister_thread());
	const size_t num_types_;
};

template <typename Type>
class TypedTimersVector {
public:
	TypedTimersVector(size_t num_types) : v_(num_types) {}
	size_t len() const { return v_.len(); }
	const counter_timer::Timers &timers(size_t i) const { return v_.timers(i); }
	const counter_timer::Timer &timer(size_t i, Type type) const {
		return v_.timer(i, static_cast<size_t>(type));
	}
private:
	TimersVector v_;
};

}

#endif // COUNTER_TIMER_VEC_H_
