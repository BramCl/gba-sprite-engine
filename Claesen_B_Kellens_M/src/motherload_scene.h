//
// Created by maike on 8/12/2019.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
#define MAP_WIDTH 32
#define MAP_HEIGHT 32
#define MAP_SIZE MAP_WIDTH * MAP_HEIGHT

#define DIRT 0x000C
#define BROWNBGTILE  0x000E
#define AIR 0x000D

#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>

class MotherloadScene : public Scene {
private:

    std::unique_ptr<Sprite> player;
    std::unique_ptr<Background> bg;
    u16 map[MAP_SIZE] = {DIRT}, buffer[MAP_SIZE] = {DIRT};

    int scrollX, scrollY, rotateA;
public:
    MotherloadScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), rotateA(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;
    void seedRandomMap(int seedcount);
    void load() override;
    void tick(u16 keys) override;
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
