//
// Created by maike on 8/12/2019.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define FULL_MAP_HEIGHT (MAP_HEIGHT*8)
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define FULL_MAP_SIZE (MAP_WIDTH * FULL_MAP_HEIGHT)

#define LAVA_LB 0x0010
#define LAVA_RB 0x0011
#define LAVA_LO 0x0018
#define LAVA_RO 0x0019

#define DIRT_LB 0x0030
#define DIRT_RB 0x0031
#define DIRT_LO 0x0038
#define DIRT_RO 0x0039

#define GOLD_LB 0x0020
#define GOLD_RB 0x0021
#define GOLD_LO 0x0028
#define GOLD_RO 0x0029

#define IRON_LB 0x0022
#define IRON_RB 0x0023
#define IRON_LO 0x002A
#define IRON_RO 0x002B

#define COPPER_LB 0x0024
#define COPPER_RB 0x0025
#define COPPER_LO 0x002C
#define COPPER_RO 0x002D

#define EMERALD_LB 0x0026
#define EMERALD_RB 0x0027
#define EMERALD_LO 0x002E
#define EMERALD_RO 0x002F

#define STONE_LB 0x0034
#define STONE_RB 0x0035
#define STONE_LO 0x003C
#define STONE_RO 0x003D

#define DIAMOND_LB 0x0036
#define DIAMOND_RB 0x0037
#define DIAMOND_LO 0x003E
#define DIAMOND_RO 0x003F

#define BROWNBGTILE 0x0032
#define AIR 0x0000

#define DIRT_MINE_TIME 700
#define IRON_MINE_TIME 1000
#define COPPER_MINE_TIME 1200
#define GOLD_MINE_TIME 1400
#define EMERALD_MINE_TIME 1700
#define DIAMOND_MINE_TIME 2000

#define IRON_MONEY 1
#define COPPER_MONEY 2
#define GOLD_MONEY 4
#define EMERALD_MONEY 7
#define DIAMOND_MONEY 12

#define FUEL_DRAIN_SPEED_DIRT 0.0001
#define FUEL_DRAIN_SPEED_IRON 0.0005
#define FUEL_DRAIN_SPEED_COPPER 0.0008
#define FUEL_DRAIN_SPEED_GOLD 0.001
#define FUEL_DRAIN_SPEED_EMERALD 0.002
#define FUEL_DRAIN_SPEED_DIAMOND 0.005
#define FUEL_DRAIN_SPEED_FLYING 0.05

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>


class MotherloadScene : public Scene {
private:

    std::unique_ptr<Sprite> player;
    std::unique_ptr<Sprite> batterySprite;
    std::unique_ptr<Background> bg;
    u16 map[MAP_SIZE] = {};
    u16 fullMap[FULL_MAP_SIZE] = {};

    int scrollX, scrollY, rotateA, lastUpdate, startMiningScrollX, startMiningScrollY, startMiningTimer, score, level, scoreMultiplier, levelCost, checker1, levelCheck,fuelCost;
    float money, fuel, fuelDrainSpeed;
    bool update, dead;
public:
    MotherloadScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), rotateA(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;
    void seedRandomMap();
    bool blockIsClear(int x, int y);
    bool blockIsMineable(int x, int y);
    void updateMap();
    void mineBlock(int x, int y, u16 keys);
    void load() override;
    void tick(u16 keys) override;
    void addScore(int points);
    void addMoney(float money);
    void refuel();
    void batteryUpdate();
    void upgradeByScore();
    void levelUp();
    void levelChecker();
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
