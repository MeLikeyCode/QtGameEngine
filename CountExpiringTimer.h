#ifndef COUNTEXPIRINGTIMER_H
#define COUNTEXPIRINGTIMER_H

// inherits
#include <QObject>

// pointer member
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
    void fired();

signals:
    void timeout();

private:
    int numTimesToFire_;
    int numTimesFired_;
    bool started_;

    // internal
    QTimer* timer_;
};

#endif // COUNTEXPIRINGTIMER_H
