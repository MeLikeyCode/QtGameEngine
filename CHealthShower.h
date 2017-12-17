#ifndef CHEALTHSHOWER_H
#define CHEALTHSHOWER_H

#include <QObject>
#include <unordered_set>
#include <unordered_map>
#include <QPoint>

class Entity;
class Map;
class Bar;

class CHealthShower : public QObject
{
    Q_OBJECT // inherits from QObject for signals/slots
public:
    CHealthShower();

    void addEntity(Entity* entity);

public slots:
    void onEntityMoved_(Entity* sender, QPointF fromPos, QPointF toPos);
    void onEntityHealthChanged_(Entity* sender);
    void onEntityEntersMap_(Entity* sender, Map* mapEntered, Map* oldMap);
    void onEntityLeavesMap_(Entity* sender, Map* mapLeft);
    void onEntitysMapVisualized_(Map* mapVisualized);
    void onEntitysMapUnVisualized_(Map* mapUnVisualized);

private:
    std::unordered_set<Entity*> entities_;
    std::unordered_map<Entity*,Bar*> entityToBar_;
};

#endif // CHEALTHSHOWER_H
