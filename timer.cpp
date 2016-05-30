#include "timer.h"
#include <algorithm>
extern "C" {
	#include <libavutil/time.h>
}

Timer::Timer() {
	update();
}

void Timer::wait(const int64_t period) {
	target_time_ += period;

	const int64_t lag = target_time_ - av_gettime() + adjust();

	if (lag > 0) {
		av_usleep(static_cast<unsigned>(lag));
	}

	const int64_t error = av_gettime() - target_time_;
	derivative_ = error - proportional_;
	integral_ += error;
	proportional_ = error;

}

void Timer::update() {
	target_time_ = av_gettime();
}

int64_t Timer::adjust() const {
	return P_ * proportional_ + I_ * integral_ + D_ * derivative_;
}
