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
#include "RandomImageEntity.h"
#include "Utilities.h"
#include "FogWeather.h"
#include <Label.h>
#include <QuestViewer.h>
#include <Quests.h>
#include <Quest.h>
#include "ScrollWindow.h"

#include <QMediaPlayer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // register types that needed to be used in cross thread signal-slot stuff
    qRegisterMetaType<PathingMap>();
    qRegisterMetaType<std::vector<QPointF>>();

    // play sound
    Sound* s = new Sound("qrc:/resources/sounds/music2.mp3");
    s->setVolume(20);
    s->play(-1); // play an infinite number of times

    // create MapGrid/Maps
    MapGrid* mapGrid = new MapGrid(3,3);

    PathingMap map1PathingMap(40,40,32);
    PathingMap map2PathingMap(50,50,64);

    Map* map1 = new Map(map1PathingMap);
    Map* map2 = new Map(map2PathingMap);

    // create some weathers
    RainWeather* rain1 = new RainWeather();
    SnowWeather* snow1 = new SnowWeather();
    FogWeather* fog1 = new FogWeather();

    // create some Terrains (tiles)
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

    // add some equipment slots for the player dynamic entity
    MeleeWeaponSlot* leftHand = new MeleeWeaponSlot();
    leftHand->setName("leftHand");
    leftHand->setPosition(player->namedPoint("left shoulder"));
    player->addSlot(leftHand);

    RangedWeaponSlot* leftHandRanged = new RangedWeaponSlot();
    leftHandRanged->setName("leftHandRanged");
    leftHandRanged->setPosition(player->namedPoint("center"));
    player->addSlot(leftHandRanged);

//    // test inventoryviewoer
//    InventoryViewer* v = new InventoryViewer(game,player->inventory());
//    v->setNumCellsHorizontally(3);
//    v->setNumCellsVertically(6);
//    game->addGui(v);

//    // test panel
//    Panel* p = new Panel();
//    p->setGuiPos(QPointF(300,300));
//    game->addGui(p);

//    // test button
//    Button* b = new Button();
//    b->setGuiPos(QPointF(500,500));
//    game->addGui(b);

//    // test bar
//    Bar* bar = new Bar();
//    bar->setGuiPos(QPointF(500,200));
//    game->addGui(bar);

    // add bow to players inventory
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

    // add some trees/rocks
    addRandomRocks(map1,5);
    addRandomBushes(map1,20);
    addRandomTrees(map1,15);

//    // test QuestViewer
//    Quests* quests = new Quests();
//    Quest* q1 = new Quest("kill bears","please kill 10 bears");
//    Quest* q2 = new Quest("kill flies","please find 5 flies");
//    quests->addQuest(q1);
//    quests->addQuest(q2);

//    QuestViewer* questViewer = new QuestViewer(quests);
//    game->addGui(questViewer);

    // test ScrollWindow
    ScrollWindow* sw = new ScrollWindow();
    for (int i = 0, n = 25; i < n; i++){
        if (i % 2 == 0){
            Button* b = new Button();
            sw->add(b);
        } else{
            Label* l = new Label();
            sw->add(l);
        }
    }

    game->addGui(sw);

    return a.exec();
}
