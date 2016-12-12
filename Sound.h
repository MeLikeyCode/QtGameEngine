#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <QMediaPlayer>
#include <QObject>
#include <QTimer>

/// Represents a sound that can be played a certain number of times (including
/// an infinite number of times).
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

public slots:
    void stateChanged();
private:
    QMediaPlayer* mediaPlayer_;
    int numTimesPlayed_;
    int numTimesToPlay_;
};

#endif // SOUND_H
