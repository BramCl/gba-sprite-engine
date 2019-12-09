//
// Created by maike on 8/12/2019.
//

//
// Created by Wouter Groeneveld on 28/07/18.
//

#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include "motherload_scene.h"

#include "character_onbewerkt_transparant_16.h"
#include "kul.h"

std::vector<Sprite *> MotherloadScene::sprites() {
    return {
            player.get()
    };
}

std::vector<Background *> MotherloadScene::backgrounds() {
    return {
            bg.get()
    };
}
// HIER HET ROOD DE KARAKTERS NOG
void MotherloadScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(character_onbewerkt_transparant_16Pal, sizeof(character_onbewerkt_transparant_16Pal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_palette, sizeof(bg_palette)));

    SpriteBuilder<Sprite> builder;
    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(character_onbewerkt_transparant_16Tiles, sizeof(character_onbewerkt_transparant_16Tiles))
            .withSize(SIZE_32_32)
            .withLocation(120, 80)
            .buildPtr();
    bg = std::unique_ptr<Background>(new Background(1, background_data, sizeof(background_data), map, sizeof(map)));
    bg.get()->useMapScreenBlock(16);
}

void MotherloadScene::tick(u16 keys) {

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
        scrollY += 1;
    }
}
