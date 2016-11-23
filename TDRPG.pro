#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T22:11:25
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TDRPG
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    Map.cpp \
    Game.cpp \
    Grid.cpp \
    Node.cpp \
    Sprite.cpp \
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
    AIEntity.cpp \
    SpearProjectile.cpp \
    ProjectileMoveBehaviorSine.cpp \
    ProjectileMoveBehaviorSpinning.cpp \
    InventoryViewer.cpp \
    ItemRainOfSpears.cpp \
    ItemTeleport.cpp \
    ItemPushback.cpp \
    RainWeather.cpp \
    MapGrid.cpp \
    SnowWeather.cpp \
    WeatherEffect.cpp \
    Gui.cpp \
    Panel.cpp \
    Button.cpp \
    Bar.cpp \
    TerrainLayer.cpp \
    RandomImageEntity.cpp \
    Utilities.cpp \
    FogWeather.cpp \
    Quest.cpp \
    Quests.cpp \
    QuestViewer.cpp \
    Label.cpp \
    ScrollWindow.cpp \
    ScrollBar.cpp \
    QtUtilities.cpp \
    DialogGui.cpp \
    ItemCell.cpp \
    InventoryUser.cpp \
    QuestAcceptor.cpp \
    ShopGui.cpp \
    ItemGold.cpp \
    BodyThrust.cpp \
    Ability.cpp \
    NoTargetAbility.cpp \
    PointTargetAbility.cpp \
    EntityTargetAbility.cpp \
    BodyThrustEntity.cpp \
    ECRotateToMouse.cpp \
    ECMoveInResponseToKeyboardRelativeToScreen.cpp \
    ECRotater.cpp

HEADERS  += \
    Map.h \
    Game.h \
    Grid.h \
    Node.h \
    Sprite.h \
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
    AIEntity.h \
    SpearProjectile.h \
    ProjectileMoveBehaviorSine.h \
    ProjectileMoveBehaviorSpinning.h \
    InventoryViewer.h \
    ItemRainOfSpears.h \
    ItemTeleport.h \
    ItemPushback.h \
    RainWeather.h \
    MapGrid.h \
    SnowWeather.h \
    WeatherEffect.h \
    Gui.h \
    Panel.h \
    Button.h \
    Bar.h \
    TerrainLayer.h \
    RandomImageEntity.h \
    Utilities.h \
    FogWeather.h \
    Quest.h \
    Quests.h \
    QuestViewer.h \
    Label.h \
    ScrollWindow.h \
    ScrollBar.h \
    QtUtilities.h \
    DialogGui.h \
    ItemCell.h \
    InventoryUser.h \
    QuestAcceptor.h \
    ShopGui.h \
    ItemGold.h \
    BodyThrust.h \
    Ability.h \
    NoTargetAbility.h \
    PointTargetAbility.h \
    EntityTargetAbility.h \
    BodyThrustEntity.h \
    ECRotateToMouse.h \
    ECMoveInResponseToKeyboardRelativeToScreen.h \
    ECRotater.h

RESOURCES += \
    res.qrc
