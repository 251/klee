//===-- Timer.h -------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_TIMER_H
#define KLEE_TIMER_H

#include "klee/Internal/System/Time.h"

#include "llvm/ADT/SmallVector.h"

#include <functional>
#include <memory>

namespace klee {

  class WallTimer {
    time::Point start;

  public:
    WallTimer();

    /// check - Return the delta since the timer was created
    time::Span check();
  };


  class Timer {
    /// Approximate delay per timer firing.
    time::Span rate;
    /// Wall time for next firing.
    time::Point nextFireTime;
  public:
    /// The event callback.
    std::function<void()> run;

    /// \param rate The time span between firings.
    /// \param callback The event callback.
    Timer(const time::Span &rate, std::function<void()> &&callback);
    ~Timer() = default;

    /// Return rate.
    time::Span getRate() const;
    /// Invoke callback if time for next firing exceeded.
    void invoke(const time::Point &currentTime);
    /// Reset timer from current time.
    void reset(const time::Point &currentTime);
  };


  class TimerGroup {
    /// Registered timers.
    llvm::SmallVector<std::unique_ptr<Timer>, 4> timers;
    /// Minimum interval between timer invocations.
    const time::Span minInterval;
    /// Wall time of last timer invocation.
    time::Point lastCheckedTime;
  public:
    explicit TimerGroup(const time::Span &minInterval);

    /// Add a timer to be executed periodically.
    ///
    /// \param timer The timer object to run on firings.
    void add(std::unique_ptr<Timer> timer);
    /// Invoke timers.
    void invoke();
    /// Reset all timers.
    void reset();
  };
} // klee

#endif /* KLEE_TIMER_H */
