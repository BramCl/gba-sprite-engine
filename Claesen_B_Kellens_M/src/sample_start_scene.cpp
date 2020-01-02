//
// Created by maike on 8/12/2019.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "sample_start_scene.h"
#include "motherload_scene.h"
#include "gameOverScene.h"
#include "digger.h"


std::vector<Background *> SampleStartScene::backgrounds() {
    return {};
}

std::vector<Sprite *> SampleStartScene::sprites() {
    return {  animation.get() };
}

void SampleStartScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager());

    SpriteBuilder<Sprite> builder;

    animation = builder
            .withData(0, 0)
            .buildPtr();


    TextStream::instance().setText("Press any key to start", 8, 4);
    TextStream::instance().setText("Frankenload", 6, 10);


    engine->getTimer()->start();
}

void SampleStartScene::tick(u16 keys) {
    if(keys) {
        if(!engine->isTransitioning()) {
            engine->transitionIntoScene(new MotherloadScene(engine), new FadeOutScene(2));
        }
    }

}
