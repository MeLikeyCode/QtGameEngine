#include <QApplication>

#include "Game.h"
#include "Sprite.h"
#include "Spear.h"
#include "Axe.h"
#include "DynamicEntity.h"
#include "Sound.h"

#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create a Map and a Game
    Map* map = new Map(15,15,64);
    Game* game = new Game(map);
    game->launch();

    // create a DynamicEntity (an Entity that can move around)
    DynamicEntity* player = new DynamicEntity();
    map->addEntity(player);
    player->setCellPos(Node(2,2));
    player->setPlayerControlled(true);
    game->setPlayer(player); // game knows about this entity (for testing)

    // give the entity a sprite (overrides default one)
    Sprite* spr = new Sprite();
    player->setSprite(spr);
    spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
    spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
    spr->play("stand",1,1); // play stand anim

    // add some attachment points for the player
    player->addNamedPoint(QPointF(24,5),"left shoulder");
    player->addNamedPoint(QPointF(24,58),"right shoulder");

    Axe* axe = new Axe();
    player->w1_ = axe;
    player->addItemToInventory(axe);
    player->equipItem(axe, player->namedPoint("left shoulder"));

    Spear* spear = new Spear();
    player->w2_ = spear;
    player->addItemToInventory(spear);
    player->equipItem(spear, player->namedPoint("right shoulder"));

    // play a sound
    Sound* sound = new Sound("qrc:/resources/sounds/music.mp3");
    sound->play(10);

    return a.exec();
}
