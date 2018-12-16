#pragma once

#include "Vendor.h"

namespace qge{

/// Represents a sound that can be played a certain number of times (including
/// an infinite number of times). The sound will emit a signal when its done
/// playing however many times it has been asked to play.
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Sound* sound = new Sound("qrc:/resources/sounds/music.mp3");
/// sound->play(-1); // play the Sound an infinite number of times (loop infinitely)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @author Abdullah Aghazadah
/// @date 2/9/2016
class Sound: public QObject
{
    Q_OBJECT
public:
    Sound(std::string filePath, QObject* parent = nullptr);

    void play(int numOfTimes);
    void stop();

    void setVolume(int volume);
    void setMute(bool tf);

    QMediaPlayer::State state();
    int volume();

signals:
    /// Emitted when the Sound is finished playing the number of times that it
    /// has been asked to play.
    /// @note This signal will never emit if the sound has been asked to play
    /// an infinite number of times (i.e. -1 is passed in play()).
    void finished(Sound* sound);

public slots:
    void stateChanged_();
private:
    QMediaPlayer* mediaPlayer_;
    int numTimesPlayed_;
    int numTimesToPlay_;
};

}
