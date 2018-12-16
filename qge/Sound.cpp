#include "Sound.h"

using namespace qge;

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
/// number of times. If the Sound is already playing, it will simply *restart*
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

/// Mutes/unmutes the Sound.
void Sound::setMute(bool tf)
{
    mediaPlayer_->setMuted(tf);
}

/// Returns the state of the Sound (i.e. playing, stopped, etc...)
QMediaPlayer::State Sound::state()
{
    return mediaPlayer_->state();
}

/// Returns the volume of the Sound. 0-100.
int Sound::volume()
{
    return mediaPlayer_->volume();
}

/// Executed each time the state of the internal MediaPlayer changes.
void Sound::stateChanged_()
{
    // approach:
    // - check if it stopped
    // - if so, check to see if we should play it again
    // - if so, play it again
    // - otherwise, emit finished() signal

    if (mediaPlayer_->state() == QMediaPlayer::StoppedState){
        if (numTimesPlayed_ < numTimesToPlay_ || numTimesToPlay_ == -1){
            mediaPlayer_->setPosition(0);
            mediaPlayer_->play();
            numTimesPlayed_++;
        }
        else{
            emit finished(this);
        }
    }
}
