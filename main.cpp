#include <QApplication>
#include "MapGrid.h"
#include "PathingMap.h"
#include "Map.h"
#include "Entity.h"
#include "ECKeyboardMover8Directional.h"
#include "ECMouseFacer.h"
#include "ECCameraFollower.h"
#include "TerrainLayer.h"
#include "WeaponSlot.h"
#include "Spear.h"
#include "Inventory.h"
#include "ECBodyThruster.h"
#include "ECEnemyAttacker.h"
#include "Axe.h"
#include "ItemRainOfSpears.h"
#include "ItemPushback.h"
#include "ECItemPickerUpper.h"
#include "InventoryViewer.h"
#include "InventoryUser.h"
#include "Sound.h"
#include "Bow.h"
#include "RangedWeaponSlot.h"
#include "ECPathMover.h"
#include "ECMapMover.h"
#include "ECCurrentMapGrabber.h"
#include "PositionalSound.h"
#include "FogWeather.h"
#include "RainWeather.h"
#include "SnowWeather.h"
#include "Utilities.h"
#include "ECFieldOfViewEmitter.h"
#include "ECChaser.h"
#include "SpriteSheet.h"
#include "ECKeyboardMover4Directional.h"
#include "ECKeyboardMoverPerspective.h"
#include "Animation.h"
#include "TopDownSprite.h"
#include "AngledSprite.h"
#include "Sprite.h"
#include "FireballLauncher.h"
#include "AnimationAttack.h"

Entity* player;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // play music
    Sound* bgMusic = new Sound("qrc:/resources/sounds/music.mp3");
    bgMusic->play(-1);

    // create a MapGrid to put some Maps inside
    MapGrid* mapGrid = new MapGrid(3,3);

    // create the Maps
    Map* map1 = new Map();
    Map* map2 = new Map(PathingMap(50,50,64));

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

    // create a Game to visualize the Maps
    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create an entity that is controlled via keyboard/mouse
    player = new Entity();
    player->setHealth(500);

    // create an EntitySprite for the entity
    SpriteSheet skeletonSpriteSheet(":/resources/graphics/human3/skeleton_0.png",32,8,128,128);
    AngledSprite* sprplayer = new AngledSprite();
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        sprplayer->addAnimation((180+45*i) % 360,"stand",skeletonSpriteSheet,Node(0,0+i),Node(3,0+i));
        for (int j = 2; j > 0; --j){
            sprplayer->addFrame(skeletonSpriteSheet.tileAt(Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        sprplayer->addAnimation((180+45*i) % 360,"walk",skeletonSpriteSheet,Node(4,0+i),Node(11,0+i));

        // attack
        sprplayer->addAnimation((180+45*i) % 360,"attack",skeletonSpriteSheet,Node(12,0+i),Node(16,0+i));
    }
    player->setSprite(sprplayer);

    // add entity to a map
    map1->addEntity(player);
    player->setPos(QPointF(200,200));

    // add controllers to control the behavior of the entity
    ECMouseFacer* rotContr = new ECMouseFacer(player);
    ECKeyboardMoverPerspective* moveContr = new ECKeyboardMoverPerspective(player);
    ECCameraFollower* grabCamContr = new ECCameraFollower(player);
    ECMapMover* moveToNMC = new ECMapMover(player);
    ECCurrentMapGrabber* grabCM = new ECCurrentMapGrabber(player);
    ECItemPickerUpper* pickUpItemContr = new ECItemPickerUpper(player);

    // create some slots for the entity
    WeaponSlot* rightHandMelee = new WeaponSlot();
    rightHandMelee->setName("melee");
    rightHandMelee->setPosition(QPointF(25,50));
    player->addSlot(rightHandMelee);

    RangedWeaponSlot* rangedSlot = new RangedWeaponSlot();
    rangedSlot->setName("ranged");
    rangedSlot->setPosition(QPointF(64,45));
    player->addSlot(rangedSlot);

    // add spear to entity's inventory
    Spear* spear = new Spear();
    player->inventory()->addItem(spear);

//    // add bow to entitys inventory and equip it
//    Bow* bow = new Bow();
//    player->inventory()->addItem(bow);
//    rangedSlot->equip(bow);

    // add fireball to entitys inventory and equip it
//    FireballLauncher* fireballOrb = new FireballLauncher();
//    player->inventory()->addItem(fireballOrb);
//    rangedSlot->equip(fireballOrb);

    AnimationAttack* animAttack = new AnimationAttack("attack",10,75,45);
    player->inventory()->addItem(animAttack);
    rightHandMelee->equip(animAttack);

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



//    // create an inventory viewer to visuallize the inventory of player
//    InventoryViewer* invViewer = new InventoryViewer();
//    invViewer->setInventory(player->inventory());
//    game->addGui(invViewer);

    // create a gui that allows visualizing/using of inventory of player
    InventoryUser* invUser = new InventoryUser(game,player->inventory());
    game->addGui(invUser);

    // test weather effects
//    RainWeather* rain = new RainWeather();
//    map1->addWeatherEffect(*rain);
//    FogWeather* fog = new FogWeather();
//    map1->addWeatherEffect(*fog);
//    SnowWeather* snow = new SnowWeather();
//    map1->addWeatherEffect(*snow);

    // test random image entites
    //addRandomTrees(map1,50);

//    // create a test chaser enemy
//    Entity* testFOVEntity = new Entity();
//    testFOVEntity->setGroup(2);
//    testFOVEntity->addEnemyGroup(0);
//    testFOVEntity->setPos(QPoint(50,700));
//    map1->addEntity(testFOVEntity);
//    ECEnemyChaser* ecChase = new ECEnemyChaser(testFOVEntity);

    // create a test goblin
    Entity* goblinEntity = new Entity();
    AngledSprite* goblinSprite = new AngledSprite();
    goblinEntity->setSprite(goblinSprite);
    SpriteSheet goblinSpriteSheet(":/resources/graphics/characterSpritesheets/goblin.png",48,8,128,128);
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        goblinSprite->addAnimation((180+45*i) % 360,"stand",goblinSpriteSheet,Node(0,0+i),Node(3,0+i));
        for (int j = 2; j > 0; --j){
            goblinSprite->addFrame(goblinSpriteSheet.tileAt(Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        goblinSprite->addAnimation((180+45*i) % 360,"walk",goblinSpriteSheet,Node(12,0+i),Node(19,0+i));

        // attack
        goblinSprite->addAnimation((180+45*i) % 360,"attack",goblinSpriteSheet,Node(20,0+i),Node(23,0+i));
    }
    goblinSprite->play("stand",-1,10,0);
    goblinSprite->setPos(QPointF(-64,-64));
    goblinEntity->setPos(QPointF(500,500));
    map1->addEntity(goblinEntity);

    ECAttacker* enemyAttacker = new ECAttacker(goblinEntity);
    enemyAttacker->addAttackee(player);

    AnimationAttack* animAtt = new AnimationAttack("attack",10,200,45);
    animAtt->setCastRange(200);
    goblinEntity->inventory()->addItem(animAtt);
    WeaponSlot* goblinSlot = new WeaponSlot();
    goblinEntity->addSlot(goblinSlot);
    goblinSlot->equip(animAtt);

//    // test EntitySprite positioning relative to Entity
//    Entity* testEntity = new Entity();
//    testEntity->sprite()->setPos(QPointF(500,500));
//    map1->addEntity(testEntity);
//    testEntity->setPos(QPointF(0,0));

    return a.exec();
}
