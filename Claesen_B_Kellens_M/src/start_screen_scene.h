//
// Created by Maikel on 8/12/2019.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_START_SCREEN_SCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_START_SCREEN_SCENE_H



#include <libgba-sprite-engine/scene.h>


class SampleStartScene : public Scene {
private:
    std::unique_ptr<Sprite> animation;
    bool pressingAorB = false;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    SampleStartScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;

};

#endif //GBA_SPRITE_ENGINE_SAMPLE_START_SCENE_H
