#ifndef POSITIONALSOUND_H
#define POSITIONALSOUND_H

#include <QPointF>
#include <string>
#include <memory>
#include <QObject>

class Map;
class Sound;

/// Represents a sound object you can put in a Map. The furter the camera of
/// the map is from the sound object, the lower its volume.
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
    PositionalSound(std::string filePath, QPointF pos);

    void play(int numOfTimes);
    void stop();
    void setVolume(int volume);

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

#endif // POSITIONALSOUND_H
