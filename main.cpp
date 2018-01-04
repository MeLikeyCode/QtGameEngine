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
#include "MCSpawner.h"
#include "CItemDropper.h"
#include "CHealthShower.h"
#include "DialogGui.h"
#include "ECGuiShower.h"
#include "ShopGui.h"
#include "ECMerchant.h"
#include "ItemHealthPotion.h"
#include "ItemTeleport.h"
#include "RandomImageEntity.h"

Entity* player;
CItemDropper* itemDropper;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // play music
    Sound* bgMusic = new Sound("qrc:/resources/sounds/music3.mp3");
    bgMusic->setVolume(50);
    bgMusic->play(-1);

    // create a MapGrid to put some Maps inside
    MapGrid* mapGrid = new MapGrid(3,3);

    // create the Maps
    Map* map1 = new Map(new PathingMap(100,100,16));
    Map* map2 = new Map(new PathingMap(50,50,64));

    // put Maps in MapGrid
    mapGrid->setMapAtPos(map1,0,1);
    mapGrid->setMapAtPos(map2,0,0);

    // create a Game to visualize the Maps
    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create an entity that is controlled via keyboard/mouse
    player = new Entity();
    player->setGroup(1);
    player->setMaxHealth(100);
    player->setHealth(100);
    player->pathingMap().fill();

    // create an EntitySprite for the entity
    SpriteSheet minitaurSpriteSheet(":/resources/graphics/characterSpritesheets/minotaur_alpha.png",24,8,128,128);
    AngledSprite* sprplayer = new AngledSprite();
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        sprplayer->addFrames((180+45*i) % 360,"stand",minitaurSpriteSheet,Node(0,0+i),Node(3,0+i));
        for (int j = 2; j > 0; --j){
            sprplayer->addFrame(minitaurSpriteSheet.tileAt(Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        sprplayer->addFrames((180+45*i) % 360,"walk",minitaurSpriteSheet,Node(4,0+i),Node(11,0+i));

        // attack
        sprplayer->addFrames((180+45*i) % 360,"attack",minitaurSpriteSheet,Node(12,0+i),Node(18,0+i));
    }
    player->setOrigin(QPointF(64,64));
    player->setSprite(sprplayer);
    player->setPathingMapPos(QPointF(64,64));

    // add entity to a map
    map1->addEntity(player);
    player->setPos(QPointF(200,650));

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

    AnimationAttack* animAttack = new AnimationAttack("attack","qrc:/resources/sounds/axe.wav",10,85,90);
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
    ros->setNumOfCharges(2);
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

////    // add a weather effect (can add multiple at the same time)
//    RainWeather* rain = new RainWeather();
//    map1->addWeatherEffect(*rain);
//    FogWeather* fog = new FogWeather();
//    map1->addWeatherEffect(*fog);



//    // create a test chaser enemy
//    Entity* testFOVEntity = new Entity();
//    testFOVEntity->setGroup(2);
//    testFOVEntity->addEnemyGroup(0);
//    testFOVEntity->setPos(QPoint(50,700));
//    map1->addEntity(testFOVEntity);
//    ECEnemyChaser* ecChase = new ECEnemyChaser(testFOVEntity);

//    // test ECFieldOfViewEmitter
//    Entity* e = new Entity();
//    e->sprite()->setOrigin({32,32});
//    map1->addEntity(e);
//    ECFieldOfViewEmitter* fovEmitter = new ECFieldOfViewEmitter(e);
//    e->setPos(QPointF(100,400));

//    // test ECChaser
//    Entity* e = new Entity();
//    e->sprite()->setOrigin({32,32});
//    map1->addEntity(e);
//    ECChaser* chaser = new ECChaser(e);
//    chaser->addChasee(player);
//    e->setPos({100,600});

    // create a test spider
    Entity* spiderEntity = new Entity();
    spiderEntity->pathingMap().fill();
    AngledSprite* spiderSprite = new AngledSprite();
    spiderEntity->setSprite(spiderSprite);
    SpriteSheet spiderSpriteSheet(":/resources/graphics/characterSpritesheets/spider.png",48,8,128,128);
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        spiderSprite->addFrames((180+45*i) % 360,"stand",spiderSpriteSheet,Node(0,0+i),Node(3,0+i));
        for (int j = 2; j > 0; --j){
            spiderSprite->addFrame(spiderSpriteSheet.tileAt(Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        spiderSprite->addFrames((180+45*i) % 360,"walk",spiderSpriteSheet,Node(4,0+i),Node(11,0+i));

        // attack
        spiderSprite->addFrames((180+45*i) % 360,"attack",spiderSpriteSheet,Node(12,0+i),Node(15,0+i));

        // hit
        spiderSprite->addFrames((180+45*i) % 360,"hit",spiderSpriteSheet,Node(18,0+i),Node(20,0+i));

        // die
        spiderSprite->addFrames((180+45*i) % 360,"die",spiderSpriteSheet,Node(18,0+i),Node(23,0+i));
    }
    spiderSprite->play("stand",-1,10,0);
    spiderEntity->setOrigin(QPointF(64,64));
    spiderEntity->setPos(QPointF(500,500));
    //map1->addEntity(spiderEntity);
    spiderEntity->setPathingMapPos(QPointF(64,64));

    ECBodyThruster* bt = new ECBodyThruster(spiderEntity);
    bt->setThrustDistance(100);
    bt->addTargetEntity(player);
    spiderEntity->addSound("die","qrc:/resources/sounds/spiderDie.mp3");

    // create an item dropper
    itemDropper = new CItemDropper();
    itemDropper->addEntity(spiderEntity);

    // create health shower
    CHealthShower* hs = new CHealthShower();
    hs->addEntity(player);
    hs->addEntity(spiderEntity);

    // create a dialog gui (to test gui shower)
    DialogGui* dg = new DialogGui();
    Response* startingText = new Response("Hello stranger, what brings thee here?");
    Choice* startTextChoice1 = new Choice("Screw you!");
    Choice* startTextChoice2 = new Choice("I'm looking for adventure!");

    dg->addResponse(startingText);
    dg->addChoice(startingText,startTextChoice1);
    dg->addChoice(startingText,startTextChoice2);
    dg->setResponse(startingText); // set initial text

    Response* screwYouResponse = new Response("You're manners are not very good...you won't last long here, kid.");
    Response* adventureResponse = new Response("I salute thee! You should talk to our village elder for some quests.");
    dg->addResponse(screwYouResponse);
    dg->addResponse(adventureResponse);
    dg->setResponseForChoice(screwYouResponse,startTextChoice1);
    dg->setResponseForChoice(adventureResponse,startTextChoice2);

    Choice* backChoice = new Choice("<back>");
    dg->addChoice(screwYouResponse, backChoice);
    dg->setResponseForChoice(startingText, backChoice);

    double guix = game->width() - dg->getGuiBoundingBox().width();
    dg->setGuiPos(QPointF(guix,0));

    // create gui shower
    ECGuiShower* ds = new ECGuiShower(spiderEntity,dg);
    ds->addEntityOfInterest(player);

//    // create a merchant
//    Axe* iaxe = new Axe();
//    iaxe->setDescription("A trusty axe.");

//    spiderEntity->inventory()->addItem(iaxe);
//    spiderEntity->inventory()->addItem(new FireballLauncher());
//    spiderEntity->inventory()->addItem(new Bow());
//    spiderEntity->inventory()->addItem(new ItemHealthPotion(10));
//    spiderEntity->inventory()->addItem(new ItemTeleport());

//    ECMerchant* merchant = new ECMerchant(spiderEntity);
//    merchant->addEntityOfInterest(player);

    // give player 8 health potions
    ItemHealthPotion* hpLotsOfCharges = new ItemHealthPotion(10);
    hpLotsOfCharges->setNumOfCharges(8);
    player->inventory()->addItem(hpLotsOfCharges);

    // test new way to specify pathing map of an entity
    Entity* bEntity = new Entity();

    QPixmap bPixmap(":/resources/graphics/buildings/bldg2.png");
    bPixmap = bPixmap.scaled(0.85*bPixmap.width(),0.85*bPixmap.height());
    TopDownSprite* bSprite = new TopDownSprite(bPixmap);
    bEntity->setSprite(bSprite);

    QPixmap bPMPixmap(":/resources/graphics/buildings/bldg2pathing.png");
    bPMPixmap = bPMPixmap.scaled(0.85*bPMPixmap.width(),0.85*bPMPixmap.height());
    PathingMap* bPM = new PathingMap(bPMPixmap,32);
    bEntity->setPathingMap(*bPM);

    bEntity->setPos(QPointF(600,900));
    map1->addEntity(bEntity);
    QPixmap grassPatch (":/resources/graphics/terrain/grasspatch.png");
    map1->addTerrainDecoration(grassPatch,bEntity->pos() - QPointF(grassPatch.width()/2,0));

    // add well
    Entity* well = new Entity();

    QPixmap wellPixmap(":/resources/graphics/buildings/well.png");
    wellPixmap = wellPixmap.scaledToWidth(wellPixmap.width()*0.5);
    QPixmap wellPMPixmap(":/resources/graphics/buildings/wellpathing.png");
    wellPMPixmap = wellPMPixmap.scaledToWidth(wellPMPixmap.width()*0.5);

    well->setSprite(new TopDownSprite(wellPixmap));
    well->setPathingMap(*(new PathingMap(wellPMPixmap,32)));

    well->setPos(QPointF(1300,900));
    map1->addEntity(well);

    TerrainLayer* flowers = new TerrainLayer(2,2,QPixmap(":resources/graphics/terrain/flowersopacity.png"));
    flowers->fill();
    map1->addTerrainLayer(flowers);
    flowers->setPos(QPointF(750,500));

    bEntity->setInvulnerable(true);
    well->setInvulnerable(true);

    // add random trees in map 2
    addRandomTrees(map2,15,"Two",8);
//    addRandomTrees(map2,15,"Three",8);
//    addRandomTrees(map2,15,"Four",8);

    // add weather effects in map 2
//    map2->addWeatherEffect(*(new FogWeather()));
//    map2->addWeatherEffect(*(new RainWeather()));

    // create a spanwer in map 2
    MCSpawner* spawner = new MCSpawner(map2,QRectF(0,0,map2->width(),map2->height()),MCSpawner::SpawnType::Spider,10,0.3);

    // create villager sprite
    SpriteSheet villagerSS(":/resources/graphics/characterSpritesheets/villager.png",17,8,91,163);
    AngledSprite* sprVillager = new AngledSprite();
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        sprVillager->addFrames((90+45*i) % 360,"stand",villagerSS,Node(16,0+i),Node(16,0+i));

        // walk
        sprVillager->addFrames((90+45*i) % 360,"walk",villagerSS,Node(0,0+i),Node(14,0+i));
    }

    // create villager
    sprVillager->play("stand",-1,1,0);
    Entity* villager = new Entity();
    villager->setOrigin(QPointF(91.0/2,163.0/2));
    villager->setSprite(sprVillager);
    villager->setPathingMapPos(QPointF(91.0/2,163.0/2));
    villager->setPos(bEntity->botRight() - QPointF(150,150));
    map1->addEntity(villager);
    sprVillager->scale(0.65);
    villager->setFacingAngle(135);

    PathingMap treePM(QPixmap(":/resources/graphics/tree/treeOnepathing.png"),32);

    Entity* animatedTree = new Entity();
    TopDownSprite* animatedTreeSpr = new TopDownSprite();
    animatedTreeSpr->addFrames(":/resources/graphics/tree/animTree",30,"animTree");
    for (int i = 28, n = 1; i >= n; --i){
        std::string fullPath = ":/resources/graphics/tree/animTree/animTree" + std::to_string(i) + ".png";
        animatedTreeSpr->addFrame(QPixmap(fullPath.c_str()),"animTree");
    }
    animatedTree->setSprite(animatedTreeSpr);
    animatedTree->setPathingMap(*(new PathingMap(QPixmap(":/resources/graphics/tree/animTree/animTreepathing.png"),32)));
    animatedTree->setInvulnerable(true);
    map1->addEntity(animatedTree);
    animatedTree->sprite()->play("animTree",-1,8,0);
    animatedTree->setPos(bEntity->pos() + QPointF(300,0));

    RandomImageEntity* tree2 = new RandomImageEntity(":/resources/graphics/tree", "treeOne" , 5, *(new PathingMap(treePM)));
    RandomImageEntity* tree3 = new RandomImageEntity(":/resources/graphics/tree", "treeOne" , 5, *(new PathingMap(treePM)));
    RandomImageEntity* tree4 = new RandomImageEntity(":/resources/graphics/tree", "treeOne" , 5, *(new PathingMap(treePM)));
    RandomImageEntity* tree5 = new RandomImageEntity(":/resources/graphics/tree", "treeOne" , 5, *(new PathingMap(treePM)));
    RandomImageEntity* tree6 = new RandomImageEntity(":/resources/graphics/tree", "treeOne" , 5, *(new PathingMap(treePM)));

    map1->addEntity(tree2);
    map1->addEntity(tree3);
    map1->addEntity(tree4);
    map1->addEntity(tree5);
    map1->addEntity(tree6);

    tree2->setPos(QPointF(300,300));
    tree3->setPos(QPointF(800,375));
    tree4->setPos(QPointF(1200,1400));
    tree5->setPos(QPointF(200,1400));
    tree6->setPos(QPointF(1400,200));

    return a.exec();
}
