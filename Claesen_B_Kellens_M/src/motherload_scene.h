//
// Created by maike on 8/12/2019.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H


#include <libgba-sprite-engine/sprites/sprite.h>
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/scene.h>
#include <libgba-sprite-engine/background/background.h>
#include <libgba-sprite-engine/background/text_stream.h>

class MotherloadScene : public Scene {
private:

    std::unique_ptr<Sprite> player;
    std::unique_ptr<Background> bg;

    int scrollX, scrollY, rotateA;
public:
    MotherloadScene(std::shared_ptr<GBAEngine> engine) : Scene(engine), scrollX(0), scrollY(0), rotateA(0) {}

    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    void load() override;
    void tick(u16 keys) override;
};

#endif //GBA_SPRITE_ENGINE_PROJECT_MOTHERLOAD_SCENE_H
