#ifndef WEATHER_H
#define WEATHER_H

class Game;

/// Interface that represents a weather effect.
/// @author Abdullah Aghazadah
/// @date 6/26/16
class Weather
{
public:
    /// Starts the weather effect.
    virtual void start() = 0;

    /// Stops the weather effect.
    virtual void stop() = 0;
};

#endif // WEATHER_H
