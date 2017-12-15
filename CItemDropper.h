#ifndef CITEMDROPPER_H
#define CITEMDROPPER_H

#include <QObject>
#include <unordered_set>

class Entity;

class CItemDropper : public QObject // inherits from QObject for signal/slot mechanism
{
    Q_OBJECT
public:
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);

signals:

public slots:
    void onEntityDies_(Entity* entity);

private:
    std::unordered_set<Entity*> entities_;

};

#endif // CITEMDROPPER_H
