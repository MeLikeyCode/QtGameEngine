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

/// Plays the PositionalSound.
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

    // otherwise, mute the sound
    sound_.
}

/// Sets the overall volume of the PositionalSound.
/// Note that the volume will still adjust as the camera is panned around.
void PositionalSound::setVolume(int volume)
{
    volume_ = volume;
    sound->setVolume(getCalculatedVolume_());
}

/// Executed when the Map that that positional sound is in is visualized.
/// If were supposed to be playing, will calculate volume and play.
void PositionalSound::onMapVisualized_()
{
    if (shouldBePlaying_){
        sound_->setVolume(getCalculatedVolume_());
        play()
    }
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

/// Sets the Map that the positional sound is in.
void PositionalSound::setMap_(Map *map)
{
    // if had an old map, stop listening to it


    map_ = map;
}
