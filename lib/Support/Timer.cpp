//===-- Timer.cpp ---------------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "klee/Internal/Support/ErrorHandling.h"
#include "klee/Internal/Support/Timer.h"
#include "klee/Internal/System/Time.h"


using namespace klee;


// WallTimer

WallTimer::WallTimer() {
  start = time::getWallTime();
}

time::Span WallTimer::check() {
  return {time::getWallTime() - start};
}


// Timer

Timer::Timer(const time::Span &rate, std::function<void()> &&callback) :
  rate{rate}, nextFireTime{time::getWallTime() + rate}, run{std::move(callback)} {};

time::Span Timer::getRate() const {
  return rate;
};

void Timer::invoke(const time::Point &currentTime) {
  if (currentTime < nextFireTime) return;

  run();
  nextFireTime = currentTime + rate;
};

void Timer::reset(const time::Point &currentTime) {
  nextFireTime = currentTime + rate;
};


// TimerGroup

TimerGroup::TimerGroup(const time::Span &minInterval) : minInterval{minInterval}, lastCheckedTime{time::getWallTime()} {};

void TimerGroup::add(std::unique_ptr<klee::Timer> timer) {
  const auto &rate = timer->getRate();
  if (rate < minInterval)
    klee_warning("Timer rate below minimum timer interval (-timer-interval)");
  if (rate.toMicroseconds() % minInterval.toMicroseconds())
    klee_warning("Timer rate not a multiple of timer interval (-timer-interval)");

  timers.emplace_back(std::move(timer));
}

void TimerGroup::invoke() {
  const auto currentTime = time::getWallTime();
  const auto currentInterval = currentTime - lastCheckedTime;

  // short circuit if below TimerInterval
  if (currentInterval < minInterval)
    return;

  // invoke timers
  for (auto &timer : timers)
    timer->invoke(currentTime);

  lastCheckedTime = currentTime;
}

void TimerGroup::reset() {
  const auto currentTime = time::getWallTime();
  for (auto &timer : timers)
    timer->reset(currentTime);
  lastCheckedTime = currentTime;
}
