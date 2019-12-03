//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include "flying_stuff_scene.h"
#include "kul.h"

std::vector<Sprite *> FlyingStuffScene::sprites() {
    return {
        player.get()
    };
}

std::vector<Background *> FlyingStuffScene::backgrounds() {
    return {
        bg.get()
    };
}

void FlyingStuffScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(sharedPal, sizeof(sharedPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_palette, sizeof(bg_palette)));

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(piskel2Tiles, sizeof(piskel2Tiles))
            .withSize(SIZE_16_16)
            .withLocation(120, 80)
            .buildPtr();
    bg = std::unique_ptr<Background>(new Background(1, background_data, sizeof(background_data), map, sizeof(map)));
    bg.get()->useMapScreenBlock(16);
}

void FlyingStuffScene::tick(u16 keys) {

    TextStream::instance().setText(std::to_string(-scrollY), 0, 0);

    bg.get()->scroll(scrollX, scrollY);

    if(keys & KEY_LEFT) {
        scrollX -= 2;
    } else if(keys & KEY_RIGHT) {
        scrollX += 2;
    } else if(keys & KEY_UP) {
        scrollY -= 2;
    } else if(keys & KEY_DOWN) {
        scrollY += 2;
    } else {
        player->setVelocity(0, 0);
    }
}
