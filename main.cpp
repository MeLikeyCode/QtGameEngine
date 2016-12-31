#include <QApplication>
#include "MapGrid.h"
#include "PathingMap.h"
#include "Map.h"
#include "Entity.h"
#include "ECMoveInResponseToKeyboardRelativeToScreen.h"
#include "ECRotateToMouse.h"
#include "ECGrabCam.h"
#include "ECPickUpItem.h"
#include "TerrainLayer.h"
#include "Sprite.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create a MapGrid to put some Maps inside
    MapGrid* mapGrid = new MapGrid(3,3);

    // create PathingMaps for the Maps
    PathingMap map1PathingMap(40,40,32);
    PathingMap map2PathingMap(50,50,64);

    // create the Maps
    Map* map1 = new Map(map1PathingMap);
    Map* map2 = new Map(map2PathingMap);

    // create some Terrains (tiles) for the Maps
    TerrainLayer* dryTerrain = new TerrainLayer(map2->width()/256+1,
                                                map2->height()/256+1,
                                                QPixmap(":resources/graphics/terrain/grassstonedry.png"));
    dryTerrain->fill();

    TerrainLayer* grassLayer = new TerrainLayer(3,3,QPixmap(":resources/graphics/terrain/flowersopacity.png"));
    grassLayer->fill();

    // set Maps' terrain
    map2->addTerrainLayer(dryTerrain);
    map1->addTerrainLayer(grassLayer);

    // put Maps in MapGrid
    mapGrid->insertMap(map1,0,1);
    mapGrid->insertMap(map2,0,0);

    // create a Game
    Game* game = new Game(mapGrid,0,1);
    game->launch();

    // create an entity that is controlled via keyboard/mouse and picks up items
    Entity* keyMouseEntity = new Entity();

    Sprite* sprKeyMouseEntity = new Sprite();
    sprKeyMouseEntity->addFrames(":/resources/graphics/human",6,"walk");
    sprKeyMouseEntity->addFrames(":/resources/graphics/human",1,"stand");
    keyMouseEntity->setSprite(sprKeyMouseEntity);

    map1->addEntity(keyMouseEntity);

    ECRotateToMouse* rotContr = new ECRotateToMouse(*keyMouseEntity);
    ECMoveInResponseToKeyboardRelativeToScreen* moveContr = new ECMoveInResponseToKeyboardRelativeToScreen(keyMouseEntity);
    ECGrabCam* grabCamContr = new ECGrabCam(keyMouseEntity);
    ECPickUpItem* pickUpItemContr = new ECPickUpItem(keyMouseEntity);

    keyMouseEntity->setPointPos(QPointF(200,200));

    return a.exec();
}
