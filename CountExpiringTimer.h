#ifndef COUNTEXPIRINGTIMER_H
#define COUNTEXPIRINGTIMER_H

#include <QObject>
#include <QTimer>

/// Represents a timer that will fire a specific number of times and then
/// disconnects itself.
/// @author Abdullah Aghazadah
/// @date 6-8-15
class CountExpiringTimer: public QObject {
    Q_OBJECT
public:
    // constructor
    CountExpiringTimer(QObject* parent=nullptr);

    void start(int freqInMs, int numTimesToFire);

    bool started() const;
    void disconnect();

public slots:
    void fired_();

signals:
    void timeout();

private:
    int numTimesToFire_;
    int numTimesFired_;
    bool started_;
    QTimer* timer_;
};

#endif // COUNTEXPIRINGTIMER_H
