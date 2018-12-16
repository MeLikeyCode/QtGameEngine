#include "ShardsOfFireAbility.h"

#include "Sound.h"
#include "SpearProjectile.h"
#include "Map.h"
#include "TopDownSprite.h"
#include "ECSineMover.h"

qge::ShardsOfFireAbility::ShardsOfFireAbility(int numShards, double shardDistance, qge::Entity *owner):
    NoTargetAbility(owner),
    numShards_(numShards),
    shardDistance_(shardDistance)
{
    setIcon(QPixmap(":/resources/graphics/effects/fireRain.png"));
    setDescription("Expells shards of fire from the owner.");
    soundEffect_ = new Sound("qrc:/resources/sounds/fireball.wav");
}

void qge::ShardsOfFireAbility::useImplementation()
{
    // play sound
    soundEffect_->play(1);

    // constants
    const double angleStep = 360.0 / numShards_;

    // create projectiles
    std::vector<SpearProjectile*> projectiles;
    for (int i = 0, n = numShards_; i < n; ++i){
        SpearProjectile* projectile = new SpearProjectile(shardDistance_,10);
        projectile->setSprite(new TopDownSprite(QPixmap(":/resources/graphics/effects/fireball.png")));
        owner()->map()->addEntity(projectile);
        projectiles.push_back(projectile);
    }

    // make sure projectiles don't damage each other (or owner)
    for (SpearProjectile* p : projectiles){
        p->addEntityToNotCollideWith(owner());
        for (SpearProjectile* op : projectiles){
            p->addEntityToNotCollideWith(op);
        }
    }

    // launch projectiles
    for (int i = 0, n = numShards_; i < n; ++i){
        double angle = i * angleStep;
        QLineF line(owner()->pos(),QPoint(owner()->pos().x()+1,owner()->pos().y()));
        line.setAngle(angle);
        line.setLength(shardDistance_);

        SpearProjectile* projectile = projectiles[i];
        projectile->setPos(owner()->pos());
        projectile->setFacingAngle(line.angle());
        projectile->shootTowards(line.p2());
    }
}
