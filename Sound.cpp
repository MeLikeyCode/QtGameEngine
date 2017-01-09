#include "Sound.h"

#include <QDebug> // TODO: remove

Sound::Sound(std::string filePath, QObject *parent):
    QObject(parent)
{
    // initialize
    mediaPlayer_ = new QMediaPlayer(this);
    mediaPlayer_->setMedia(QUrl(filePath.c_str()));

    numTimesPlayed_ = 0;
    numTimesToPlay_ = 0;

    connect(mediaPlayer_,&QMediaPlayer::stateChanged,this,&Sound::stateChanged_);
}

/// Plays the sound the specified number of times. Pass -1 to play an infinite
/// number of times. If the Sound is already playing, it will simply restart
/// playing right away.
void Sound::play(int numOfTimes)
{
    numTimesToPlay_ = numOfTimes;
    numTimesPlayed_ = 1;

    if (mediaPlayer_->state() == QMediaPlayer::PlayingState){
        mediaPlayer_->stop();
    }

    mediaPlayer_->play();
}

/// Stops playing the sound.
void Sound::stop()
{
    numTimesPlayed_ = 0;
    numTimesToPlay_ = 0;
    mediaPlayer_->stop();
}

/// Sets the volume of the sound, from 0-100.
void Sound::setVolume(int volume)
{
    mediaPlayer_->setVolume(volume);
}

/// Executed each time the state of the internal MediaPlayer changes.
void Sound::stateChanged_()
{
    // approach:
    // - check if it stopped
    // - if so, check to see if we should play it again
    // - if so, play it again

    if (mediaPlayer_->state() == QMediaPlayer::StoppedState){
        if (numTimesPlayed_ < numTimesToPlay_ || numTimesToPlay_ == -1){
            mediaPlayer_->setPosition(0);
            mediaPlayer_->play();
            numTimesPlayed_++;
        }
    }
}
