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

#define DIRT_LB 0x0030
#define DIRT_RB 0x0031
#define DIRT_LO 0x0038
#define DIRT_RO 0x0039
#define BROWNBGTILE 0x0032
#define AIR 0x0000

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>

class MotherloadScene : public Scene {
private:

    std::unique_ptr<Sprite> player;
    std::unique_ptr<Background> bg;
    u16 map[MAP_SIZE] = {};
    u16 fullMap[FULL_MAP_SIZE] = {};

    int scrollX, scrollY, rotateA, lastUpdate;
    bool update;
public:
    MotherloadScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), rotateA(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;
    void seedRandomMap();
    bool blockIsClear(int upperLeftX, int upperLeftY);
    void updateMap();
    void load() override;
    void tick(u16 keys) override;
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
