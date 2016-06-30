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
#include "InventoryViewer.h"
#include "Inventory.h"
#include "ItemRainOfSpears.h"
#include "ItemTeleport.h"
#include "ItemPushback.h"
#include "MapGrid.h"
#include "Terrain.h"

#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // register types that needed to be used in cross thread signal-slot stuff
     qRegisterMetaType<PathingMap>();
     qRegisterMetaType<std::vector<QPointF>>();

     Sound* s = new Sound("qrc:/resources/sounds/music2.mp3");
     s->play(-1);

    // create a Map and a Game
    MapGrid* mapGrid = new MapGrid(3,3);
    Map* mainMap = new Map(30,30,64);
    mapGrid->insertMap(mainMap,0,1);

    Map* secondaryMap = new Map(20,20,64);
    int TILE_SIZE = 256;
    Terrain* terrain_ = new Terrain(20*64/TILE_SIZE+1,20*64/TILE_SIZE+1,
                           TILE_SIZE,TILE_SIZE);
    terrain_->fill(QPixmap(":resources/graphics/terrain/grassstonedry.png"));
    secondaryMap->setTerrain(terrain_);

    mapGrid->insertMap(secondaryMap,0,0);

    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create a DynamicEntity (an Entity that can move around)
    DynamicEntity* player = new DynamicEntity();
    mainMap->addEntity(player);
    player->setCellPos(Node(4,4));
    player->setPlayerControlled(true);
    player->setStepSize(10);
    game->setPlayer(player); // game knows about this entity (for testing)
    player->setFollowedByCam(true);
    player->setGroupID(0);

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
//    MeleeWeaponSlot* rightHand = new MeleeWeaponSlot();
//    rightHand->setName("rightHand");
//    rightHand->setPosition(player->namedPoint("right shoulder"));
//    player->addSlot(rightHand);
    RangedWeaponSlot* leftHandRanged = new RangedWeaponSlot();
    leftHandRanged->setName("leftHandRanged");
    leftHandRanged->setPosition(player->namedPoint("center"));
    player->addSlot(leftHandRanged);

//    // create some weapons (MeleeWeapons)
//    Axe* axe = new Axe();
//    player->addItemToInventory(axe);

//    Spear* spear = new Spear();
//    player->addItemToInventory(spear);

//    Bow* bow = new Bow();
//    player->addItemToInventory(bow);

    // equip weapons to slots
    // player->equipItem(axe,leftHand);
    //player->equipItem(spear,rightHand);
    // player->equipItem(bow,leftHandRanged);

//    // play a sound
//    Sound* sound = new Sound("qrc:/resources/sounds/music.mp3");
//    sound->play(10);

    // test inventoryviewoer
    InventoryViewer* v = new InventoryViewer(game,500,128,player->inventory());
    game->addInventoryViewer(v);

    Bow* bow = new Bow();
    player->addItemToInventory(bow);

    // add some items to ground
    Spear* spear = new Spear();
    spear->setPointPos(QPointF(300,300));
    mainMap->addEntity(spear);

    Axe* axe = new Axe();
    axe->setPointPos(QPointF(400,300));
    mainMap->addEntity(axe);

    ItemRainOfSpears* ros = new ItemRainOfSpears();
    ros->setPointPos(QPointF(500,500));
    mainMap->addEntity(ros);

    ItemTeleport* tel = new ItemTeleport();
    tel->setPointPos(QPointF(600,600));
    mainMap->addEntity(tel);

    ItemPushback* pb = new ItemPushback();
    pb->setPointPos(QPointF(700,700));
    mainMap->addEntity(pb);

    return a.exec();
}
