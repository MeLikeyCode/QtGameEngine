#ifndef WEATHER_H
#define WEATHER_H

class Map;

/// Interface that represents a weather effect.
/// To add a Weather effect to a Map, first create the Weather, then use
/// Map::setWeather(Weather*).
/// @author Abdullah Aghazadah
/// @date 6/26/16
class Weather
{
    friend class Map; // Map needs to set map_ during Map::setWeather(Weather*)
public:
    Weather();

    /// Starts the weather effect.
    virtual void start() = 0;

    /// Stops the weather effect.
    virtual void stop() = 0;
protected:
    Map* map_;
};

#endif // WEATHER_H
