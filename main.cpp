#include <QApplication>

#include "Game.h"
#include "Sprite.h"
#include "Spear.h"
#include "Axe.h"
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
#include "RandomImageEntity.h"
#include "Utilities.h"
#include "FogWeather.h"
#include <Label.h>
#include <QuestViewer.h>
#include <Quests.h>
#include <Quest.h>
#include "ScrollWindow.h"
#include "ScrollBar.h"
#include "ItemCell.h"
#include "DialogGui.h"
#include "InventoryUser.h"
#include "QuestAcceptor.h"
#include "ShopGui.h"
#include "ItemGold.h"
#include "BodyThrust.h"
#include "ECMoveInResponseToKeyboardRelativeToScreen.h"
#include "ECRotateToMouse.h"
#include "ECChaseEnemies.h"
#include "ECGrabCam.h"
#include "ECPickUpItem.h"
#include "ECBodyThruster.h"
#include "ECPathMover.h"
#include "ECFieldOfViewEmitter.h"
#include "WeaponSlot.h"
#include "ECAttackEnemiesWithWeapon.h"
#include "AbilityCell.h"
#include "SpearProjectile.h"
#include "ECMoveToNextMap.h"
#include "ECGrabCurrentMap.h"

#include <QMediaPlayer>

Spear* wSpear; // TODO: delete, test

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // register types that needed to be used in cross thread signal-slot stuff
    qRegisterMetaType<PathingMap>();
    qRegisterMetaType<std::vector<QPointF>>();

    // play sound
    Sound* s = new Sound("qrc:/resources/sounds/music2.mp3");
    s->play(-1); // play an infinite number of times

    // create a MapGrid to put some Maps inside
    MapGrid* mapGrid = new MapGrid(3,3);

    // create PathingMaps for the Maps
    PathingMap map1PathingMap(40,40,32);
    PathingMap map2PathingMap(50,50,64);

    // create the Maps
    Map* map1 = new Map(map1PathingMap);
    Map* map2 = new Map(map2PathingMap);

    // create some weather for the Maps
    RainWeather* rain1 = new RainWeather();
    SnowWeather* snow1 = new SnowWeather();
    FogWeather* fog1 = new FogWeather();

    // create some Terrains (tiles) for the Maps
    int TILE_SIZE = 256;
    TerrainLayer* dryTerrain = new TerrainLayer(256,256,
                                                map2->width()/256+1,
                                                map2->height()/256+1,
                                                QPixmap(":resources/graphics/terrain/grassstonedry.png"));
    dryTerrain->fill();

    QPixmap pm = QPixmap(":resources/graphics/terrain/flowersopacity.png");
    TerrainLayer* grassLayer = new TerrainLayer(512,512,3,3,pm);
    grassLayer->setPos(QPointF(0,0));
    grassLayer->fill();

    // set Terrain/Weather of Maps
    map2->addTerrainLayer(dryTerrain);
    map1->addTerrainLayer(grassLayer);

    //map1->setWeatherEffect(fog1);
    //map2->setWeatherEffect(snow1);

    // put Maps in MapGrid
    mapGrid->insertMap(map1,0,1);
    mapGrid->insertMap(map2,0,0);

    // create a Game
    Game* game = new Game(mapGrid,0,1);
    game->launch();

//    // test panel
//    Panel* p = new Panel();
//    p->setGuiPos(QPointF(300,300));
//    game->addGui(p);
//    //p->showBackground(false);

//    // test button
//    Button* b = new Button();
//    b->setGuiPos(QPointF(500,500));
//    game->addGui(b);

//    // test bar
//    Bar* bar = new Bar();
//    bar->setGuiPos(QPointF(500,200));
//    game->addGui(bar);

    // test label
    Label* label = new Label();
    game->addGui(label);

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

    // add some trees/rocks
//    addRandomRocks(map1,5);
//    addRandomBushes(map1,20);
//    addRandomTrees(map1,15);

////    // test QuestViewer
//    Quests* quests = new Quests();
//    Quest* q1 = new Quest("kill bears","please kill 10 bears");
//    Quest* q2 = new Quest("kill flies","please find 5 flies");
//    Quest* q3 = new Quest("kill pencils","please find 5 pencils");
//    Quest* q4 = new Quest("kill papers","please find 5 papers");
//    Quest* q5 = new Quest("kill desks","please find 5 desks");
//    Quest* q6 = new Quest("kill mice","please find 5 mice");
//    quests->addQuest(q1);
//    quests->addQuest(q2);
//    quests->addQuest(q3);
//    quests->addQuest(q4);
//    quests->addQuest(q5);
//    quests->addQuest(q6);

//    QuestViewer* questViewer = new QuestViewer(quests);
//    questViewer->setGuiPos(QPointF(0,300));
//    game->addGui(questViewer);

//    // test QuestAcceptor
//    QuestAcceptor* questAcceptor = new QuestAcceptor(game);
//    questAcceptor->setQuests(quests);
//    game->addGui(questAcceptor);

//    // test scroll bar
//    ScrollBar* sb = new ScrollBar();
//    sb->setGuiPos(QPointF(400,0));
//    sb->setBgBarLength(100);
//    sb->setFgBarLength(97.1);
//    //sb->showEvenIfFull(true);
//    sb->setFgBarLengthAsFractionOfBgBarLength(0.75);
//    game->addGui(sb);

//    // test scroll window
//    ScrollWindow* sw = new ScrollWindow(200,200);
//    sw->setGuiPos(QPointF(0,0));

//    Button* bNeg1 = new Button();
//    bNeg1->setText("0,0");
//    Button* b = new Button();
//    b->setText("(100,100)");
//    Button* b2 = new Button();
//    b2->setText("(200,200)");
//    Button* b3 = new Button();
//    b3->setText("(500,500)");

//    sw->add(bNeg1,QPointF(0,0));
//    sw->add(b,QPointF(100,100));
//    sw->add(b2,QPointF(200,200));
//    sw->add(b3,QPointF(500,500));

//    game->addGui(sw);

//    // test dialog gui
//    DialogGui* dialog = new DialogGui();

//    Response* initialResponse = new Response("Who are you?");

//    Choice* abdullah = new Choice("I'm Abdullah");
//    Choice* creep = new Choice("El Creepo");

//    // initial response/choices for it
//    dialog->addResponse(initialResponse);
//    dialog->addChoice(initialResponse,abdullah);
//    dialog->addChoice(initialResponse,creep);

//    // response for each choice
//    Response* howRude = new Response("how rude!");
//    dialog->addResponse(howRude);
//    dialog->setResponseForChoice(howRude,creep);
//    Response* nice = new Response("nice to meet ya");
//    dialog->addResponse(nice);
//    dialog->setResponseForChoice(nice,abdullah);

//    dialog->setResponse(initialResponse);

//    game->addGui(dialog);
//    dialog->setGuiPos(QPointF(450,25));

//    ShopGui* shopGui = new ShopGui(game);
//    shopGui->setGuiPos(QPointF(400,0));

//    Inventory* seller = new Inventory();

//    Bow* sBow = new Bow();
//    Spear* sSpear = new Spear();
//    Axe* sAxe = new Axe();

//    seller->addItem(sBow);
//    seller->addItem(sSpear);
//    seller->addItem(sAxe);

//    shopGui->setSeller(seller);
//    shopGui->setBuyer(player->inventory());

//    // game->addGui(shopGui);

//    // test ability cell
//    AbilityCell* abilityCell = new AbilityCell(100,100);
//    BodyThrust* bodThrustAb = new BodyThrust(player);
//    abilityCell->setAbility(bodThrustAb);
//    abilityCell->setGuiPos(QPointF(300,400));
//    game->addGui(abilityCell);

    // == new test

    // create an entity that is controlled via keyboard/mouse and picks up items
    Entity* keyMouseEntity = new Entity();

    Sprite* sprKeyMouseEntity = new Sprite();
    sprKeyMouseEntity->addFrames(":/resources/graphics/human",6,"walk");
    sprKeyMouseEntity->addFrames(":/resources/graphics/human",1,"stand");
    keyMouseEntity->setSprite(sprKeyMouseEntity);


    keyMouseEntity->setSpeed(300);

    keyMouseEntity->setPointPos(QPointF(10,200));
    map1->addEntity(keyMouseEntity);

    ECRotateToMouse* rotContr = new ECRotateToMouse(*keyMouseEntity);
    ECMoveInResponseToKeyboardRelativeToScreen* moveContr = new ECMoveInResponseToKeyboardRelativeToScreen(keyMouseEntity);
    ECGrabCam* grabCamContr = new ECGrabCam(keyMouseEntity);
    ECPickUpItem* pickUpItemContr = new ECPickUpItem(keyMouseEntity);

    // give keyboard entity a weapon slot
    WeaponSlot* wSlot = new WeaponSlot();
    wSlot->setPosition(QPointF(50,50));

    keyMouseEntity->addSlot(wSlot);

    // give a weapon (for now game has global reference to this weapon)
    wSpear = new Spear();
    keyMouseEntity->inventory()->addItem(wSpear);
    wSlot->equip(wSpear);

//    // create an entity that moves via pathfinding
//    Entity* pathMovingEntity = new Entity();
//    pathMovingEntity->setPointPos(QPointF(300,300));
//    map1->addEntity(pathMovingEntity);

//    ECPathMover* pathMoveCont = new ECPathMover(pathMovingEntity);

//    pathMoveCont->moveEntityTo(QPointF(300,300));

//    // create an entity that checks out his fov
//    Entity* fovEntity = new Entity();
//    map1->addEntity(fovEntity);

//    ECFieldOfViewEmitter* fovCont = new ECFieldOfViewEmitter(fovEntity);

//    fovEntity->setPointPos(QPointF(500,300));

//    // create an entity that  chases enemies
//    Entity* chaseEnemiesEntity = new Entity();
//    chaseEnemiesEntity->setPointPos(QPointF(200,100));

//    ECChaseEnemies* chaseEntitiesCont = new ECChaseEnemies(chaseEnemiesEntity);

//    keyMouseEntity->setGroup(1);
//    chaseEnemiesEntity->setGroup(3);
//    chaseEnemiesEntity->addEnemyGroup(1);

//    map1->addEntity(chaseEnemiesEntity);

//    // create an entity that body thrusts enemies
//    Entity* bodyThrustEntity = new Entity();
//    bodyThrustEntity->setPointPos(QPointF(300,300));

//    Sprite* sprBT = new Sprite();
//    sprBT->addFrames(":/resources/graphics/spider",7,"walk");
//    sprBT->addFrames(":/resources/graphics/spider",1,"stand");
//    bodyThrustEntity->setSprite(sprBT);

//    sprBT->play("stand",-1,10000);


//    map1->addEntity(bodyThrustEntity);
//    ECBodyThruster* bodyThrustCont = new ECBodyThruster(bodyThrustEntity);

//    keyMouseEntity->setGroup(1);
//    bodyThrustEntity->addEnemyGroup(1);

//    // create an Entity that will attack enemies with a weapon
//    Entity* weaponEntity = new Entity();
//    map1->addEntity(weaponEntity);

//    WeaponSlot* wwSlot = new WeaponSlot();
//    wwSlot->setPosition(QPointF(0,0));
//    wwSlot->setName("main weapon");

//    weaponEntity->addSlot(wwSlot);

//    wSpear = new Spear();
//    Inventory* weInv = new Inventory();
//    weaponEntity->setInventory(weInv);
//    weInv->addItem(wSpear);
//    wwSlot->equip(wSpear);

//    ECAttackEnemiesWithWeapon* weaponAttackCont = new ECAttackEnemiesWithWeapon(weaponEntity);

//    keyMouseEntity->setGroup(1);
//    weaponEntity->addEnemyGroup(1);

    return a.exec();
}
