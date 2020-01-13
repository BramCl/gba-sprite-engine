//
// Created by Maikel on 8/12/2019.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "start_screen_scene.h"
#include "instruction_scene.h"
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
    TextStream::instance().setText("Motherload", 6, 10);

    engine->getTimer()->start();
}
void SampleStartScene::tick(u16 keys) {
    if(keys) {//Start de instructies als er op een toets wordt gedrukt
        if(!engine->isTransitioning()) {
            engine->transitionIntoScene(new InstructionScene(engine), new FadeOutScene(4));
        }
    }

}
