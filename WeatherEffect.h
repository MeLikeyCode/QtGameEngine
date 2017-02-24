#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include "Map.h"
#include <QPointer>

/// Abstract class that represents a weather effect for a Map. To add a
/// WeatherEffect effect to a Map, first create an instance of the
/// WeatherEffect, then use Map::addWeatherEffect(WeatherEffect*). You can
/// remove a WeatherEffect from a map by calling Map::removeWeatherEffect().
///
/// To create your own weather effect, extend this class and define what
/// happens to the Map when the weather effect is started (start_()) and
/// stopped (stop_()).
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// WeatherEffect* w = new SomeConcreteWeatherEffect();
/// Map* map; // assume valid Map
/// map->addWeatherEffect(w);       // add WeatherEffect to Map, automatically starts it
/// map->removeWeatherEffect(w);    // remove WeatherEffect from Map, automatically stops it
/// w->start();                     // manually start WeatherEffect
/// w->stop();                      // manually stop WeatherEffect
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Abdullah Aghazadah
/// @date 6/26/16
class WeatherEffect: public QObject // so that we can use QPointer<WeatherEffect>
{
    Q_OBJECT
    friend class Map; // Map needs to set map_ during Map::addWeatherEffect(WeatherEffect*)
                      // (because a WeatherEffect operates on a Map)
public:
    WeatherEffect();

    // these functions use template method pattern to do some "skeletal work"
    // then delegate to concrete class. They are virtual in case the skeletal work
    // does not suit you. If you do decide to override them, please ensure that you
    // abide by the contract it sets forth in its api doc. Clients depend on this contract.
    virtual void start();
    virtual void stop();

protected:
    virtual void start_() = 0;
    virtual void stop_() = 0;

    QPointer<Map> map_;
    bool started_;
};

#endif // WEATHER_H
