#include <QApplication>

#include "Game.h"
#include "Sprite.h"
#include "Spear.h"

Game* game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create a Map and a Game
    Map* map = new Map(15,15,64);
    game = new Game(map);
    game->launch();

    // create an Entity
    Entity* player = new Entity();
    map->addEntity(player);
    player->setCellPos(Node(2,2));

    // give the entity a sprite (overrides default one)
    Sprite* spr = new Sprite();
    player->setSprite(spr);
    spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
    spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
    spr->play("stand",1,1); // play stand anim

    // allow this Entity to be accessed by Entity* Game::player()
    // allow Entity to move in response to keyboard/mouse
    game->setPlayer(player);

    // add some attachment points for the player
    player->addNamedPoint(QPointF(-20,0),"left shoulder");
    player->addNamedPoint(QPointF(-20,50),"right shoulder");
    player->addNamedPoint(QPointF(25,25),"head");

    Spear* spear = new Spear();
    map->addEntity(spear);
    player->spear_ = spear;
    spear->setPointPos(player->namedPoint("left shoulder"));
    spear->setParentEntity(player);
    spear->setOwner(player);

//    Spear* spear2 = new Spear();
//    map->addEntity(spear2);
//    player->spear2_ = spear2;
//    spear2->setPointPos(player->namedPoint("right shoulder"));
//    spear2->setParentEntity(player);
//    spear2->setOwner(player);

    // enemies
    Entity* e1 = new Entity();
    Sprite* e1s = new Sprite();
    e1s->addFrames(":resources/graphics/human",1,"stand");
    e1s->addFrames(":resources/graphics/human",6,"walk");
    map->addEntity(e1);
    e1->setSprite(e1s);
    e1->setCellPos(Node(8,8));

    Entity* e2 = new Entity();
    Sprite* e2s = new Sprite();
    e2s->addFrames(":resources/graphics/human",1,"stand");
    e2s->addFrames(":resources/graphics/human",6,"walk");
    map->addEntity(e2);
    e2->setSprite(e2s);
    e2->setCellPos(Node(3,8));

//    Entity* e2 = new Entity();
//    map->addEntity(e2);
//    e2->setCellPos(Node(6,8));

//    Entity* e3 = new Entity();
//    map->addEntity(e3);
//    e3->setCellPos(Node(6,5));

//    Entity* e4 = new Entity();
//    map->addEntity(e4);
//    e4->setCellPos(Node(8,5));

    game->enemies_.push_back(e1);
    game->enemies_.push_back(e2);



    return a.exec();
}
