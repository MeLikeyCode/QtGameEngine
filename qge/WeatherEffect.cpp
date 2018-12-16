#include "WeatherEffect.h"

#include "Map.h"

using namespace qge;

WeatherEffect::WeatherEffect():
    map_(nullptr),
    state_(STOPPED)
{
    // empty
}

/// Starts the WeatherEffect. If it has already been started, does nothing.
/// Please use resume() if the WeatherEffect is paused and you want it to
/// continue again.
void WeatherEffect::start()
{
    // error: a WeatherEffect cannot be started if its not in a map
    assert(map_ != nullptr);

    // error: a WeatherEffect cannot be started unless its map is being visualized
    assert(map_->game() != nullptr);

    // error: use resume() to continue playing a paused WeatherEffect, not start()
    assert(state_ != PAUSED);

    // do nothing if already started
    if (state_ == STARTED)
        return;

    state_ = STARTED; // remember that it has started
    start_();         // delegate actual starting to concrete WeatherEffect
}

/// Stops the WeatherEffect. Does nothing if the WeatherEffect
/// is already stopped.
void WeatherEffect::stop()
{
    assert(!map_.isNull()); // error: WeatherEffect must be in a Map before stopping

    if (state_ == STOPPED) // do nothing if already stopped
        return;

    state_ = STOPPED;   // remember that it has already stopped
    stop_();            // delegate actual stopping to concrete WeatherEffect
}

/// Resumes the WeatherEffect (i.e. picks it up right where it was paused).
/// Does nothing if the WeatherEffect is already playing ("started"). Throws if
/// the WeatherEffect is stopped (must call start() to start a stopped
/// WeatherEffect).
void WeatherEffect::resume()
{
    assert(map_ != nullptr); // error: WeatherEffect must be in a Map to be resumed

    assert(state_ != STOPPED); // error: a stopped WeatherEffect cannot be "resumed", call start()

    if (state_ == STARTED) // if the WeatherEffect is already playing, do nothing
        return;

    state_ = STARTED; // remember that we've started
    resume_();        // let concrete WeatherEffect do the actual resuming
}

/// Pauses the WeatherEffect.
/// Does nothing if the WeatherEffect is already paused, or stopped.
/// A paused WeatherEffect "freezes" in its current "frame".
/// When it is resumed via resume(), it will pick up right from where it was paused.
void WeatherEffect::pause()
{
    assert(map_ != nullptr); // error: WeatherEffect must be in a Map to be paused

    if (state_ == PAUSED || state_ == STOPPED) // if already paused or stopped, do nothing
        return;

    state_ = PAUSED; // remember that we're paused
    pause_();        // let concrete class pause the WeatherEffect ("freeze") it
}

/// Executed when the Map of the WeatherEffect is visualized.
/// If the WeatherEffect is paused, will be asked to resume.
void WeatherEffect::onMapVisualized_()
{
    if (state_ == PAUSED)
        resume();
}

/// Executed when the Map of the WeatherEffect is unvisualized.
/// If the WeatherEffect is playing ("started"), will be asked to pause.
void WeatherEffect::onMapUnvisualized_()
{
    if (state_ == STARTED)
        pause();
}

/// Sets the Map that the WeatherEffect should be in.
/// This function is called by Map::addWeatherEffect().
void WeatherEffect::setMap_(Map& map)
{
    // error: a WeatherEffect that is already added to a Map cannot be added to
    // another Map (or even moved from one to another)
    // instead of moving it, simply delete it and create another one for the new map
    assert(map_ == nullptr);

    map_ = &map; // update map internal variable

    // listen to when the Map is visualized/unvisualized
    connect(map_, &Map::setAsCurrentMap, this, &WeatherEffect::onMapVisualized_);
    connect(map_, &Map::unsetAsCurrentMap, this, &WeatherEffect::onMapUnvisualized_);
}
