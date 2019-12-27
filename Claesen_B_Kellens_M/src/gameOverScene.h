//
// Created by maike on 27/12/2019.
//

#ifndef GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H
#define GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H

#include <libgba-sprite-engine/scene.h>


class GameOverScene : public Scene {
private:
    std::unique_ptr<Sprite> animation;
    bool pressingAorB = false;

public:
    std::vector<Sprite *> sprites() override;
    std::vector<Background *> backgrounds() override;

    GameOverScene(std::shared_ptr<GBAEngine> engine) : Scene(engine) {}

    void load() override;
    void tick(u16 keys) override;

};


#endif //GBA_SPRITE_ENGINE_PROJECT_GAMEOVERSCENE_H
