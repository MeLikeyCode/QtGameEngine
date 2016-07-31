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
#include "TerrainLayer.h"
#include "RainWeather.h"
#include "SnowWeather.h"
#include "Panel.h"
#include "Button.h"
#include "Bar.h"

#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // register types that needed to be used in cross thread signal-slot stuff
     qRegisterMetaType<PathingMap>();
     qRegisterMetaType<std::vector<QPointF>>();

     Sound* s = new Sound("qrc:/resources/sounds/music2.mp3");
     s->setVolume(20);
     //s->play(-1);

    // create a Map and a Game
    MapGrid* mapGrid = new MapGrid(3,3);

    PathingMap map1PathingMap(20,20,64);
    PathingMap map2PathingMap(100,100,64);

    Map* map1 = new Map(map1PathingMap);
    Map* map2 = new Map(map2PathingMap);

    RainWeather* rain1 = new RainWeather();
    SnowWeather* snow1 = new SnowWeather();

    int TILE_SIZE = 256;
    TerrainLayer* dryTerrain = new TerrainLayer(TILE_SIZE,TILE_SIZE,
                           map2->width()/TILE_SIZE+1,map2->height()/TILE_SIZE+1,
                                                QPixmap(":resources/graphics/terrain/grassstonedry.png"));

    QImage img(128,128,QImage::Format_RGB32);
    img.fill(Qt::red);
    QPixmap pm = QPixmap::fromImage(img);
    TerrainLayer* grassLayer = new TerrainLayer(180,180,2,2,
                                                QPixmap(":resources/graphics/terrain/grass.png"));
    grassLayer->setPos(QPointF(300,300));
    grassLayer->fill();

    dryTerrain->fill();
    map2->addTerrainLayer(dryTerrain);
    map1->addTerrainLayer(grassLayer);

    //map1->setWeatherEffect(rain1);
    //map2->setWeatherEffect(snow1);

    mapGrid->insertMap(map1,0,1);
    mapGrid->insertMap(map2,0,0);

    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create a DynamicEntity (an Entity that can move around)
    DynamicEntity* player = new DynamicEntity();
    map1->addEntity(player);
    player->setCellPos(Node(4,4));
    player->setPlayerControlled(true);
    player->setStepSize(10);
    game->setPlayer(player); // game knows about this entity (for testing)
    player->setFollowedByCam(true);
    player->setGroupID(0);
    //player->setPointZ(50);
    player->setHeight(64);

    // give the entity a sprite (overrides default one)
    Sprite* spr = new Sprite();
    spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
    spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
    player->setSprite(spr);
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
    InventoryViewer* v = new InventoryViewer(game,player->inventory());
    v->setNumCellsHorizontally(3);
    v->setNumCellsVertically(6);
    game->addGui(v);

//    // test panel
//    Panel* p = new Panel();
//    p->setViewPos(QPointF(300,300));
//    game->addGui(p);

//    // test button
//    Button* b = new Button();
//    b->setViewPos(QPointF(500,500));
//    game->addGui(b);

//    // test bar
//    Bar* bar = new Bar();
//    bar->setViewPos(QPointF(500,200));
//    game->addGui(bar);

    Bow* bow = new Bow();
    player->inventory()->addItem(bow);

    // add some items to ground
    Spear* spear = new Spear();
    spear->setPointPos(QPointF(300,300));
    map1->addEntity(spear);

    Axe* axe = new Axe();
    axe->setPointPos(QPointF(400,300));
    map1->addEntity(axe);

    ItemRainOfSpears* ros = new ItemRainOfSpears();
    ros->setPointPos(QPointF(500,500));
    map1->addEntity(ros);

    ItemTeleport* tel = new ItemTeleport();
    tel->setPointPos(QPointF(600,600));
    map1->addEntity(tel);

    ItemPushback* pb = new ItemPushback();
    pb->setPointPos(QPointF(700,700));
    map1->addEntity(pb);

    return a.exec();
}
