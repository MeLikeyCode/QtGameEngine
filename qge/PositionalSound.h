#pragma once

#include "Vendor.h"

#include "Sound.h"

namespace qge{

class Map;
class Sound;

/// Represents a sound object you can put in a Map. The furter the camera of the Map is from the
/// sound object, the lower its volume. When the PositionalSound's Map is not being visualized
/// (i.e. its Map is not the one Game is currently rendering) it is muted. When its Map is being
/// visualized, it is unmuted. So, in summary, PositionalSounds can only be heard when their Map is
/// being visualized, and their volume depends on how far the camera is from them.
///
/// Exmaple usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// PositionalSound* ps = new PositionalSound(soundFile,pos);
/// Map* map; // assume map exists
/// map->addPositionalSound(ps);
/// ps->play(3) // play the sound 3 times (pass -1 to play an infinite num times)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @author Abdullah Aghazadah
/// @date 2/22/17
class PositionalSound: public QObject
{
    Q_OBJECT
    friend class Map;
public:
    PositionalSound(Map* inMap, std::string filePath, QPointF pos);

    void play(int numOfTimes);
    void stop();
    void setVolume(int volume);
    void setPos(const QPointF& pos);
    QPointF pos();

public slots:
    void onCamMoved_(QPointF newCamPos);
    void onMapVisualized_();
    void onMapNoLongerVisualized_();
private:
    Map* map_; // the map the sound is in
    QPointF pos_; // the position of the sound
    int volume_;
    bool shouldBePlaying_;

    std::unique_ptr<Sound> sound_; // internal sound (used to actually play the sound)

    // helper fcn
    int getCalculatedVolume_();
    void setMap_(Map* map);
};

}
