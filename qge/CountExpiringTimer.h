#pragma once

#include "Vendor.h"

namespace qge{

/// Represents a timer that will fire a specific number of times and then
/// deletes itself.
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

}
