#include "MyEventHandler.h"

#include "qge/Game.h"
#include "qge/MeleeWeaponSlot.h"
#include "qge/TopDownSprite.h"

using namespace qge;

EventHandler::EventHandler(Game *forGame): game_(forGame)
{
    QObject::connect(forGame, &Game::mousePressed,this,&EventHandler::onMousePress);
}

void EventHandler::onMousePress(QPointF pos, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
        ((MeleeWeaponSlot*)player->slot("melee"))->use(); // use whatever item is equipped in the players "melee" slot

    if (button == Qt::RightButton){
        // create an enemy for the player
        Entity* enemy = new Entity();

        TopDownSprite* spr = new TopDownSprite();
        spr->addFrames(":/resources/graphics/spider",7,"walk");
        spr->addFrames(":/resources/graphics/spider",1,"stand");
        spr->play("stand",-1,1,0);
        enemy->setSprite(spr);

        player->map()->addEntity(enemy);
        enemy->setPos(game_->mapToMap(pos.toPoint()));
        ECBodyThruster* bodyThrustContr = new ECBodyThruster(enemy);
        bodyThrustContr->addTargetEntity(player);
    }
}
