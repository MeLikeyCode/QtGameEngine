#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T22:11:25
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TDRPG
TEMPLATE = app


SOURCES += main.cpp \
    Map.cpp \
    Game.cpp \
    Grid.cpp \
    Node.cpp \
    Sprite.cpp \
    Terrain.cpp \
    Edge.cpp \
    Graph.cpp \
    PathGrid.cpp \
    PathingMap.cpp \
    Tree.cpp \
    Tests/TestPathGrid.cpp \
    Tests/TestPathingMap.cpp \
    CountExpiringTimer.cpp \
    MeleeWeapon.cpp \
    Spear.cpp \
    PlayerControlledMoveBehavior.cpp \
    MoveRelativeToSelf.cpp \
    MoveRelativeToScreen.cpp \
    DynamicEntity.cpp \
    Entity.cpp \
    Axe.cpp \
    Sound.cpp \
    Item.cpp \
    Inventory.cpp \
    EquipableItem.cpp \
    EntityTargetItem.cpp \
    NoTargetItem.cpp \
    PointTargetItem.cpp \
    Slot.cpp \
    MeleeWeaponSlot.cpp \
    Projectile.cpp \
    RangedWeapon.cpp \
    Bow.cpp \
    RangedWeaponSlot.cpp \
    ProjectileMoveBehavior.cpp \
    ProjectileCollisionBehavior.cpp \
    ProjectileMoveBehaviorStraight.cpp \
    ProjectileCollisionBehaviorDamage.cpp \
    ProjectileMoveBehaviorHoming.cpp \
    Weapon.cpp \
    WeaponSlot.cpp \
    AsyncShortestPathFinder.cpp \
    AIEntity.cpp

HEADERS  += \
    Map.h \
    Game.h \
    Grid.h \
    Node.h \
    Sprite.h \
    Terrain.h \
    Edge.h \
    Graph.h \
    PathGrid.h \
    PathingMap.h \
    Tree.h \
    Tests/TestPathGrid.h \
    Tests/TestPathingMap.h \
    CountExpiringTimer.h \
    MeleeWeapon.h \
    Spear.h \
    PlayerControlledMoveBehavior.h \
    MoveRelativeToSelf.h \
    MoveRelativeToScreen.h \
    DynamicEntity.h \
    Entity.h \
    Axe.h \
    Sound.h \
    Item.h \
    Inventory.h \
    EquipableItem.h \
    EntityTargetItem.h \
    NoTargetItem.h \
    PointTargetItem.h \
    Slot.h \
    MeleeWeaponSlot.h \
    Projectile.h \
    RangedWeapon.h \
    Bow.h \
    RangedWeaponSlot.h \
    ProjectileMoveBehavior.h \
    ProjectileCollisionBehavior.h \
    ProjectileMoveBehaviorStraight.h \
    ProjectileCollisionBehaviorDamage.h \
    ProjectileMoveBehaviorHoming.h \
    Weapon.h \
    WeaponSlot.h \
    AsyncShortestPathFinder.h \
    AIEntity.h

RESOURCES += \
    res.qrc
