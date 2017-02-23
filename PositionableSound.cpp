#include "PositionableSound.h"
#include "Sound.h"
#include "Utilities.h"
#include "Map.h"
#include "Game.h"
#include <cassert>

PositionableSound::PositionableSound(std::string filePath, QPointF pos):
    pos_(pos),
    volume_(100)
{
    sound_.reset(new Sound(filePath));
}

/// Plays the PositionableSound.
void PositionableSound::play(int numOfTimes)
{
    assert(map_ != nullptr);
    Game* game = map_->game();
    assert(game != nullptr);

    // set volume based on distance from camera
    sound->setVolume(getCalculatedVolume_());

    sound_->play(numOfTimes);
}

/// Sets the overall volume of the PositionableSound.
/// Note that the volume will still adjust as the camera is panned around.
void PositionableSound::setVolume(int volume)
{
    volume_ = volume;
    sound->setVolume(getCalculatedVolume_());
}

/// Based on the volume of the PositionableSound and its current distance
/// from the camera, will return a calculated volume.
int PositionableSound::getCalculatedVolume_()
{
    int distFromCamera = distance(game->centerCamPos(),pos_);
    if (distance < 1) // clamp to 1 or greater
        distFromCamera = 1;
    int calculatedVolume = 1/distFromCamera * volume_;
    return calculatedVolume;
}
