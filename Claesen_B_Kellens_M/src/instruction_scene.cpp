//
// Created by Maikel on 8/12/2019.
//

#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "instruction_scene.h"
#include "motherload_scene.h"
#include "digger.h"

std::vector<Background *> InstructionScene::backgrounds() {
    return {};
}
std::vector<Sprite *> InstructionScene::sprites() {
    return {  animation.get() };
}
void InstructionScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager());

    SpriteBuilder<Sprite> builder;

    animation = builder
            .withData(0, 0)
            .buildPtr();


    TextStream::instance().setText("Instructions: ", 1, 0);
    TextStream::instance().setText("- Use arrows to move around",3,2);
    TextStream::instance().setText("- A/X to refuel at sea level",5,2);
    TextStream::instance().setText("- Lava kills", 7, 2);
    TextStream::instance().setText("- Rocks aren't drillable", 9, 2);
    TextStream::instance().setText("- Level up at certain scores", 11, 2);
    TextStream::instance().setText("- Fuel and level price rise", 13, 2);
    TextStream::instance().setText("- Enjoy!", 15, 2);

    engine->getTimer()->start();
}
void InstructionScene::tick(u16 keys) {
    if(keys) {//Als er op een toets wordt gedrukt, begint het spel
        if(!engine->isTransitioning()) {
            engine->transitionIntoScene(new MotherloadScene(engine), new FadeOutScene(2));
        }
    }

}
