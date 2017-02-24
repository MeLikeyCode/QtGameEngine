#include "WeatherEffect.h"
#include <cassert>

WeatherEffect::WeatherEffect():
    map_(nullptr),
    started_(false)
{
    // empty
}

/// Starts the WeatherEffect. Does nothing if the WeatherEffect
/// was already started. Throws if the WeatherEffect is not in a Map.
void WeatherEffect::start()
{
    assert(!map_.isNull()); // error: WeatherEffect must be in a Map before starting

    if (started_)   // do nothing if already started
        return;

    // otherwise, delegate to concrete WeatherEffect
    started_ = true;
    start_();
}

/// Stops the WeatherEffect. Does nothing if the WeatherEffect
/// is already stopped. Throws if the WeatherEffect is not in a Map.
void WeatherEffect::stop()
{
    assert(!map_.isNull()); // error: WeatherEffect must be in a Map before stopping

    if (!started_) // do nothing if already stopped
        return;

    // otherwise, delegate to concrete WeatherEffect
    started_ = false;
    stop_();
}
