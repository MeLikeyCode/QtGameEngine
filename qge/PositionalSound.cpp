#include "PositionalSound.h"

#include "Sound.h"
#include "Map.h"
#include "Game.h"
#include "QtUtilities.h"

using namespace qge;

PositionalSound::PositionalSound(Map *inMap, std::string filePath, QPointF pos):
    map_(inMap),
    pos_(pos),
    volume_(100)
{
    sound_.reset(new Sound(filePath));
    setMap_(inMap);
}

/// Plays the PositionalSound the specified number of times (pass -1 for infinite).
/// PositionalSounds can only be heard if their Map is being visualized (but they
/// continue to seek). If play() is called while the sound is already playing,
/// it will simply restart right away.
void PositionalSound::play(int numOfTimes)
{
    sound_->play(numOfTimes);

    // if currently in a map that is in a game
    if (map_ != nullptr){
        Game* game = map_->game();
        if (game != nullptr){
            // set volume based on distance from camera
            int vol = getCalculatedVolume_();
            sound_->setVolume(vol);
            return;
        }
    }

    // if not in a map, or map not visualized by game, mute sound
    sound_->setMute(true);
}

/// Sets the overall volume of the PositionalSound.
/// Note that the actual volume will still adjust as the camera is panned around.
void PositionalSound::setVolume(int volume)
{
    volume_ = volume;
    sound_->setVolume(getCalculatedVolume_());
}

void PositionalSound::setPos(const QPointF &pos)
{
    pos_ = pos;
    sound_->setVolume(getCalculatedVolume_());
}

QPointF PositionalSound::pos()
{
    return pos_;
}

/// Executed when the camera moves on the PositionalSound's Map (I.e. when the
/// camera pans). Will adjust the volume of the sound accordingly.
void PositionalSound::onCamMoved_(QPointF newCamPos)
{
    sound_->setVolume(getCalculatedVolume_());
}

/// Executed when the Map that the PositionalSound is in is visualized.
/// Will unmute the sound.
void PositionalSound::onMapVisualized_()
{
    sound_->setMute(false);
}

/// Executed when the Map that the PositionalSound is in is no longer visualized.
/// Will mute the sound.
void PositionalSound::onMapNoLongerVisualized_()
{
    sound_->setMute(true);
}

/// Based on the volume of the PositionalSound and its current distance
/// from the camera, will return a calculated volume.
int PositionalSound::getCalculatedVolume_()
{
    assert(map_ != nullptr); // fcn cannot be called if map_ is null
    Game* game = map_->game();
    assert(game != nullptr); // fcn cannot be called if map_ is not being visualized

    const double FALLOFF_SLOPE = 0.0005; // rate at which volume falls as distance grows
    double distFromCamera = QtUtils::distance(game->centerCamPos(),pos_);
    double someFrac = -FALLOFF_SLOPE * distFromCamera + 1;
    if (someFrac < 0)
        someFrac = 0;
    if (someFrac > 1)
        someFrac = 1;
    int calculatedVolume = someFrac * volume_;
    return calculatedVolume;
}

/// Sets the Map of the PositionalSound.
void PositionalSound::setMap_(Map *map)
{
    map_ = map; // update internal variable

    // if map is currently being visualized, unmute sound
    Game* game = map_->game();
    if (game != nullptr){
        sound_->setVolume(getCalculatedVolume_());
        sound_->setMute(false); // unmute sound
    }

    // listen to when this map is visualized/unvisualized
    disconnect(map_, &Map::camMoved, this, &PositionalSound::onCamMoved_);
    disconnect(map_, &Map::setAsCurrentMap, this, &PositionalSound::onMapVisualized_);
    disconnect(map_, &Map::unsetAsCurrentMap, this, &PositionalSound::onMapNoLongerVisualized_);
    connect(map_, &Map::camMoved, this, &PositionalSound::onCamMoved_);
    connect(map_, &Map::setAsCurrentMap, this, &PositionalSound::onMapVisualized_);
    connect(map_, &Map::unsetAsCurrentMap, this, &PositionalSound::onMapNoLongerVisualized_);

}
