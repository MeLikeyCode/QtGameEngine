#include "CountExpiringTimer.h"

using namespace qge;

CountExpiringTimer::CountExpiringTimer(QObject *parent):
    QObject(parent),
    numTimesFired_(0),
    numTimesToFire_(0),
    started_(false)
{
    // constructor body

    timer_ = new QTimer(this);
    connect(timer_,SIGNAL(timeout()),this,SLOT(fired_()));
}

/// Starts the timer, which will fire with the given frequency, the given
/// number of times.
void CountExpiringTimer::start(int freqInMs, int numTimesToFire){
    // make sure its not already started
    assert(!started());

    // start it
    started_ =  true;
    numTimesFired_ = 0;
    numTimesToFire_ = numTimesToFire;
    timer_->start(freqInMs);

}

/// Returns true if the timer has been started.
bool CountExpiringTimer::started() const{
    return started_;
}

/// Disconnects the internal timer.
void CountExpiringTimer::disconnect()
{
    timer_->disconnect();
}

/// This function is called whenever the internal timer fires.
///
/// If it has fired enough times, it will disconnect and delete itself.
void CountExpiringTimer::fired_(){
    emit timeout();

    ++numTimesFired_;
    if (numTimesFired_ > numTimesToFire_){
        deleteLater();
    }
}
