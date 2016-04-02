#include <QApplication>

#include "Game.h"
#include "Sprite.h"
#include "Spear.h"
#include "Axe.h"
#include "DynamicEntity.h"
#include "Sound.h"
#include "MeleeWeaponSlot.h"
#include "Bow.h"
#include "RangedWeaponSlot.h"
#include "Map.h"
#include "AsyncShortestPathFinder.h"

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
    player->setStepSize(25);
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
    player->addNamedPoint(QPointF(50,30),"center");

    // add some equipment slots for the player
    MeleeWeaponSlot* leftHand = new MeleeWeaponSlot();
    leftHand->setName("leftHand");
    leftHand->setPosition(player->namedPoint("left shoulder"));
    player->addSlot(leftHand);
    MeleeWeaponSlot* rightHand = new MeleeWeaponSlot();
    rightHand->setName("rightHand");
    rightHand->setPosition(player->namedPoint("right shoulder"));
    player->addSlot(rightHand);
    RangedWeaponSlot* leftHandRanged = new RangedWeaponSlot();
    leftHandRanged->setName("leftHandRanged");
    leftHandRanged->setPosition(player->namedPoint("center"));
    player->addSlot(leftHandRanged);

//    // create some weapons (MeleeWeapons)
//    Axe* axe = new Axe();
//    player->addItemToInventory(axe);

//    Spear* spear = new Spear();
//    player->addItemToInventory(spear);

    Bow* bow = new Bow();
    player->addItemToInventory(bow);

    // equip weapons to slots
    //player->equipItem(axe,leftHand);
    //player->equipItem(spear,rightHand);
    player->equipItem(bow,leftHandRanged);

    // play a sound
    Sound* sound = new Sound("qrc:/resources/sounds/music.mp3");
    sound->play(10);

    // test asyn
    AsyncShortestPathFinder* apf = new AsyncShortestPathFinder(map->pathingMap(),Node(0,0),Node(0,0));

    return a.exec();
}
