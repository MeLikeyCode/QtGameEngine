#include "PositionalSound.h"
#include "Sound.h"
#include "Utilities.h"
#include "Map.h"
#include "Game.h"
#include <cassert>

PositionalSound::PositionalSound(std::string filePath, QPointF pos):
    pos_(pos),
    volume_(100)
{
    sound_.reset(new Sound(filePath));
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
            sound->setVolume(getCalculatedVolume_());
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
    sound->setVolume(getCalculatedVolume_());
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
    int distFromCamera = distance(game->centerCamPos(),pos_);
    if (distance < 1) // clamp to 1 or greater
        distFromCamera = 1;
    int calculatedVolume = 1/distFromCamera * volume_;
    return calculatedVolume;
}
