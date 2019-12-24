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

#include <libgba-sprite-engine/gba/tonc_core.h>

#include "character_onbewerkt_transparant_16.h"
#include "kul.h"
#include "backgroundblocks.h"

int __qran_seed= 42;     // Seed / rnd holder


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
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(pal_bg_mother, sizeof(pal_bg_mother)));

    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(character_onbewerkt_transparant_16Tiles, sizeof(character_onbewerkt_transparant_16Tiles))
            .withSize(SIZE_32_32)
            .withLocation(120, 80)
            .buildPtr();
    seedRandomMap(MAP_WIDTH * MAP_HEIGHT);
    bg = std::unique_ptr<Background>(new Background(1, dirt__4_Tiles, sizeof(dirt__4_Tiles), map, sizeof(map)));
    bg.get()->useMapScreenBlock(16);
}

/*
// HIER GEKOPIEERD VAN CONWAY
void MotherloadScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_pal_mother, sizeof(bg_pal_mother)));

    seedRandomMap(((32 * 64) / 100) * 0.25);

    bg = std::unique_ptr<Background>(new Background(1, motherload_data, sizeof(motherload_data), map, sizeof(map)));
    bg.get()->useMapScreenBlock(16);

   // postload();
}
 */

void MotherloadScene::seedRandomMap(int seedcount) {
    /*
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            //int x = qran_range(0, 32);
            //int y = qran_range(0, 64);
            //map[y * 32 + x] = BROWNBGTILE;
            int i = qran_range(0, 3);
            if(i == 0) {
                map[y * MAP_WIDTH + x] = DIRT;
            }
            else if(i == 1){
                map[y * MAP_WIDTH + x] = BROWNBGTILE;
            }
            else {
                map[y * MAP_WIDTH + x] = AIR;
            }
        }
    }
     */
    int x = 29;
    int y = 0;
    map[y*MAP_WIDTH+x] = DIRT;
}


///
void MotherloadScene::tick(u16 keys) {

    TextStream::instance().setText(std::to_string(-scrollY), 0, 0);

    //bg.get()->scroll(scrollX, scrollY);

    scrollY += 1;
    if(keys & KEY_LEFT) {
        scrollX -= 2;
    }
    if(keys & KEY_RIGHT) {
        scrollX += 2;
    }
    if(keys & KEY_UP) {
        scrollY -= 3;
    }
    if(keys & KEY_DOWN) {
        scrollY += 2;
    }
}
