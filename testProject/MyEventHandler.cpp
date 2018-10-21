#include "MyEventHandler.h"

#include "qge/Game.h"
#include "qge/MeleeWeaponSlot.h"
#include "qge/TopDownSprite.h"
#include "qge/Entity.h"
#include "qge/Map.h"
#include "qge/ECBodyThruster.h"

extern qge::Entity* player;

EventHandler::EventHandler(qge::Game *forGame): game_(forGame)
{
    QObject::connect(forGame, &qge::Game::mousePressed,this,&EventHandler::onMousePressed);
}

void EventHandler::onMousePressed(QPoint pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
        ((qge::MeleeWeaponSlot*)player->slot("melee"))->use(); // use whatever item is equipped in the players "melee" slot

    if (button == Qt::RightButton){
        // create an enemy for the player
        qge::Entity* enemy = new qge::Entity();

        qge::TopDownSprite* spr = new qge::TopDownSprite();
        spr->addFrames(":/resources/graphics/spider",7,"walk");
        spr->addFrames(":/resources/graphics/spider",1,"stand");
        spr->play("stand",-1,1,0);
        enemy->setSprite(spr);

        player->map()->addEntity(enemy);
        enemy->setPos(game_->mapToMap(pos));
        qge::ECBodyThruster* bodyThrustContr = new qge::ECBodyThruster(enemy);
        bodyThrustContr->addTargetEntity(player);
    }
}
