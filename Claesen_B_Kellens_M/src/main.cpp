//
// Created by Maikel on 8/12/2019.
//
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/allocator.h>

#include "start_screen_scene.h"

int main() {
    std::shared_ptr<GBAEngine> engine(new GBAEngine());
    SampleStartScene* startScene = new SampleStartScene(engine);
    engine->setScene(startScene);

    while (true) {
        engine->update();
        engine->delay(1000);
    }
    return 0;
}