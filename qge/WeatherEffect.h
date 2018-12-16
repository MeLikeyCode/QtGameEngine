#pragma once

#include "Vendor.h"

#include "Map.h"

namespace qge{

/// Abstract class that represents a weather effect for a Map. To add a
/// WeatherEffect effect to a Map, first create an instance of the
/// WeatherEffect, then use Map::addWeatherEffect(WeatherEffect*). You can
/// remove a WeatherEffect from a map by calling Map::removeWeatherEffect().
///
/// To create your own weather effect, extend this class and define what
/// happens to the Map when the weather effect is started (start_()),
/// stopped (stop_()), paused (pause_()), and resumed (resume_()).
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// WeatherEffect* w = new SomeConcreteWeatherEffect();
/// Map* map; // assume valid Map
/// map->addWeatherEffect(w);       // add the WeatherEffect to the Map, automatically start()s it
/// map->removeWeatherEffect(w);    // remove WeatherEffect from Map, automatically stops it
/// w->stop();                      // stop WeatherEffect
/// w->start();                     // start WeatherEffect
/// w->pause();                     // "freeze" the WeatherEffect in its current "frame"
/// w->start();                     // resumes the WeatherEffect from its current frame
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// When a WeatherEffect is stopped, it will:
/// - show itself slowly going away
/// - clean up its graphics
///
/// When a WeatherEffect is paused, it will simply "freeze" itself in its current "frame."
/// It will continue playing when its resumed.
///
/// A paused WeatherEffect should consume very little, if any, resource
/// (concrete classes - please adhere).
///
/// @author Abdullah Aghazadah
/// @date 6/26/16
class WeatherEffect: public QObject // so that we can use QPointer<WeatherEffect>
{
    Q_OBJECT
    friend class Map; // Map needs to set map_ during Map::addWeatherEffect(WeatherEffect*)
                      // (because a WeatherEffect operates on a Map)
public:
    enum State { STOPPED, STARTED, PAUSED };

    WeatherEffect();

    // these functions use template method pattern to do some "skeletal work"
    // then delegate to concrete class. They are virtual in case the skeletal work
    // does not suit you. If you do decide to override them, please ensure that you
    // abide by the contract it sets forth in its api doc. Clients depend on this contract.
    virtual void start();
    virtual void stop();
    virtual void resume();
    virtual void pause();

public slots:
    void onMapVisualized_();
    void onMapUnvisualized_();

protected:

    // the implementation of these functions should start/stop the
    // WeatherEffect respectively. the template "skeletal" methods above
    // already handle a few things for you (look at them to find out what)
    virtual void start_() = 0;
    virtual void stop_() = 0;
    virtual void resume_() = 0;
    virtual void pause_() = 0;

    QPointer<Map> map_;
    State state_;

private:
    void setMap_(Map &map);
};

}
