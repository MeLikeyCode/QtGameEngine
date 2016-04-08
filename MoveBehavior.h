#ifndef MOVEBEHAVIOR_H
#define MOVEBEHAVIOR_H

#include <QObject>

class DynamicEntity;

/// Represents a movement behavior that DynamicEntities can use to dictate
/// how (the algorithm used) they move.
/// @author Abdullah Aghazadah
/// @date 4/5/16
class MoveBehavior : public QObject
{
    Q_OBJECT
public:
    explicit MoveBehavior(DynamicEntity* entity);

    DynamicEntity* getEntity();

public slots:
    virtual void moveStep() = 0; // pure virtual -> makes class abstract

private:
    DynamicEntity* entity_;
};

#endif // MOVEBEHAVIOR_H
