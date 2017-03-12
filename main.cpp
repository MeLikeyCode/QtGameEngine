#include <QApplication>
#include "MapGrid.h"
#include "PathingMap.h"
#include "Map.h"
#include "Entity.h"
#include "ECMoveInResponseToKeyboardRelativeToScreen.h"
#include "ECRotateToMouse.h"
#include "ECGrabCam.h"
#include "TerrainLayer.h"
#include "Sprite.h"
#include "WeaponSlot.h"
#include "Spear.h"
#include "Inventory.h"
#include "ECBodyThruster.h"
#include "ECAttackEnemiesWithWeapon.h"
#include "Axe.h"
#include "ItemRainOfSpears.h"
#include "ItemPushback.h"
#include "ECPickUpItem.h"
#include "InventoryViewer.h"
#include "InventoryUser.h"
#include "Sound.h"
#include "Bow.h"
#include "RangedWeaponSlot.h"
#include "PathMover.h"
#include "ECMoveToNextMap.h"
#include "ECGrabCurrentMap.h"
#include "PositionalSound.h"
#include "FogWeather.h"
#include "RainWeather.h"
#include "SnowWeather.h"
#include "Utilities.h"

Entity* player;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // play music
    Sound* bgMusic = new Sound("qrc:/resources/sounds/music.mp3");
    bgMusic->play(-1);

    // create a MapGrid to put some Maps inside
    MapGrid* mapGrid = new MapGrid(3,3);

    // create
    PathingMap map2PathingMap(50,50,64);

    // create the Maps
    Map* map1 = new Map();
    Map* map2 = new Map(map2PathingMap);

    // create a TerrainLayer (tiles) that can go in a map
    TerrainLayer* dryTerrain = new TerrainLayer(map2->width()/256+1,
                                                map2->height()/256+1,
                                                QPixmap(":resources/graphics/terrain/grassstonedry.png"));
    dryTerrain->fill();

    // add a terrain layer into map
    map2->addTerrainLayer(dryTerrain);

    // put Maps in MapGrid
    mapGrid->setMapAtPos(map1,0,1);
    mapGrid->setMapAtPos(map2,0,0);

    // create a Game
    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create an entity that is controlled via keyboard/mouse
    player = new Entity();
    player->setHealth(500);

    // create a sprite for the entity
    Sprite* sprplayer = new Sprite();
    sprplayer->addFrames(":/resources/graphics/human",6,"walk");
    sprplayer->addFrames(":/resources/graphics/human",1,"stand");
    player->setSprite(sprplayer);

    map1->addEntity(player);

    // add controllers to control the entity
    ECRotateToMouse* rotContr = new ECRotateToMouse(*player);
    ECMoveInResponseToKeyboardRelativeToScreen* moveContr = new ECMoveInResponseToKeyboardRelativeToScreen(player);
    ECGrabCam* grabCamContr = new ECGrabCam(player);
    ECMoveToNextMap* moveToNMC = new ECMoveToNextMap(player);
    ECGrabCurrentMap* grabCM = new ECGrabCurrentMap(player);

    player->setPointPos(QPointF(200,200));

    // create some slots for the entity
    WeaponSlot* rightHandMelee = new WeaponSlot();
    rightHandMelee->setName("right hand melee");
    rightHandMelee->setPosition(QPointF(25,50));
    player->addSlot(rightHandMelee);

    RangedWeaponSlot* rangedSlot = new RangedWeaponSlot();
    rangedSlot->setName("ranged");
    rangedSlot->setPosition(QPointF(50,25));
    player->addSlot(rangedSlot);

    // add spear to entity's inventory
    Spear* spear = new Spear();
    player->inventory()->addItem(spear);

    // add bow to entitys inventory
    Bow* bow = new Bow();
    player->inventory()->addItem(bow);
    rangedSlot->equip(bow);


//    // create an entity that will thrust nearby enemy entities
//    Entity* enemy = new Entity();
//    map1->addEntity(enemy);
//    ECBodyThruster* bodyThrustContr = new ECBodyThruster(*enemy);
//    enemy->addEnemyGroup(0);

//    // create an entity that attacks nearby enemy entites with weapon
//    Entity* enemy2 = new Entity();
//    map1->addEntity(enemy2);

//    Spear* enemySpear = new Spear();
//    enemy2->inventory()->addItem(enemySpear);
//    WeaponSlot* enemyWS = new WeaponSlot();
//    enemy2->addSlot(enemyWS);
//    enemyWS->equip(enemySpear);
//    enemy2->addEnemyGroup(0);

//    ECAttackEnemiesWithWeapon* attackContr = new ECAttackEnemiesWithWeapon(*enemy2);

    // drop some items on the ground
    Axe* axeItem = new Axe();
    axeItem->setPointPos(QPointF(100,300));
    map1->addEntity(axeItem);

    ItemRainOfSpears* ros = new ItemRainOfSpears();
    ros->setPointPos(QPointF(200,200));
    map1->addEntity(ros);

    ItemPushback* pushBackItem = new ItemPushback();
    pushBackItem->setPointPos(QPointF(300,300));
    map1->addEntity(pushBackItem);

    ECPickUpItem* pickUpItemContr = new ECPickUpItem(player);

//    // create an inventory viewer to visuallize the inventory of player
//    InventoryViewer* invViewer = new InventoryViewer();
//    invViewer->setInventory(player->inventory());
//    game->addGui(invViewer);

    // create a gui that allows visualizing/using of inventory of player
    InventoryUser* invUser = new InventoryUser(game,player->inventory());
    game->addGui(invUser);

//    // test positional sound
//    PositionalSound* ps = new PositionalSound("qrc:/resources/sounds/axe.wav",QPointF(0,0));
//    ps->play(-1);
//    map1->addPositionalSound(ps);

//    RainWeather* rain = new RainWeather();
//    map1->addWeatherEffect(*rain);
//    FogWeather* fog = new FogWeather();
//    map1->addWeatherEffect(*fog);
//    SnowWeather* snow = new SnowWeather();
//    map1->addWeatherEffect(*snow);

    //addRandomTrees(map1,50);

    return a.exec();
}
