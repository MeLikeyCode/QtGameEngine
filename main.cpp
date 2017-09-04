#include <QApplication>
#include "MapGrid.h"
#include "PathingMap.h"
#include "Map.h"
#include "Entity.h"
#include "ECMoveByKeyboardEightDirectional.h"
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
#include "ECFieldOfViewEmitter.h"
#include "ECChaseEnemies.h"
#include "SpriteSheet.h"
#include "ECMoveByKeyboardFourDirectional.h"
#include "Animation.h"

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
    SpriteSheet characterSpriteSheet(":/resources/graphics/human/character.png",13,21,64,64);
    Sprite* sprplayer = new Sprite();
    sprplayer->addFrames(Node(0,8),Node(8,8),characterSpriteSheet,"walkUp");
    sprplayer->addFrame(Node(0,8),characterSpriteSheet,"standUp");
    sprplayer->addFrames(Node(0,9),Node(8,9),characterSpriteSheet,"walkLeft");
    sprplayer->addFrame(Node(0,9),characterSpriteSheet,"standLeft");
    sprplayer->addFrames(Node(0,10),Node(8,10),characterSpriteSheet,"walkDown");
    sprplayer->addFrame(Node(0,10),characterSpriteSheet,"standDown");
    sprplayer->addFrames(Node(0,11),Node(8,11),characterSpriteSheet,"walkRight");
    sprplayer->addFrame(Node(0,11),characterSpriteSheet,"standRight");
    player->setSprite(sprplayer);

    map1->addEntity(player);

    // add controllers to control the entity
    //ECRotateToMouse* rotContr = new ECRotateToMouse(*player);
    ECMoveByKeyboardFourDirectional* moveContr = new ECMoveByKeyboardFourDirectional(player);
    ECGrabCam* grabCamContr = new ECGrabCam(player);
    ECMoveToNextMap* moveToNMC = new ECMoveToNextMap(player);
    ECGrabCurrentMap* grabCM = new ECGrabCurrentMap(player);

    player->setPos(QPointF(200,200));

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
    axeItem->setPos(QPointF(100,300));
    map1->addEntity(axeItem);

    ItemRainOfSpears* ros = new ItemRainOfSpears();
    ros->setPos(QPointF(200,200));
    map1->addEntity(ros);

    ItemPushback* pushBackItem = new ItemPushback();
    pushBackItem->setPos(QPointF(300,300));
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

    // create a test chaser enemy
    Entity* testFOVEntity = new Entity();
    testFOVEntity->setGroup(2);
    testFOVEntity->addEnemyGroup(0);
    testFOVEntity->setPos(QPoint(50,700));
    map1->addEntity(testFOVEntity);
    ECChaseEnemies* ecChase = new ECChaseEnemies(*testFOVEntity);

    // test Animation
    SpriteSheet spriteSheet(":/resources/graphics/human/character.png",13,21,64,64);
    Animation* anim = new Animation();
    anim->addFrames(":/resources/graphics/human2",6,"walk",".png");
    map1->scene()->addItem(anim);
    anim->setPos(300,300);
    anim->play(-1,15);

    return a.exec();
}
