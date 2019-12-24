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
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_Tiles, sizeof(bg_Tiles)));

    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(character_onbewerkt_transparant_16Tiles, sizeof(character_onbewerkt_transparant_16Tiles))
            .withSize(SIZE_32_32)
            .withLocation(112, 8)
            .buildPtr();
    seedRandomMap();
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

void MotherloadScene::seedRandomMap() {

    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 5; y < FULL_MAP_HEIGHT; y++) {
            //int x = qran_range(0, 32);
            //int y = qran_range(0, 64);
            //map[y * 32 + x] = BROWNBGTILE;
            int i = qran_range(0, 5);
            if(i == 0){
                fullMap[y * MAP_WIDTH + x] = BROWNBGTILE;
            }
            else {
                fullMap[y * MAP_WIDTH + x] = DIRT;
            }
        }
        for (int y = 0; y < 5; y++)
            fullMap[y* MAP_WIDTH + x] = AIR;
    }


}


///
void MotherloadScene::tick(u16 keys) {
    for(int i = 0; i <MAP_SIZE; i++){
        map[i]= fullMap[i+scrollY*MAP_WIDTH/8];
    }
    TextStream::instance().setText(std::to_string(-scrollY), 0, 0);

    bg.get()->scroll(scrollX, scrollY);
    bg.get()->updateMap(map);

    scrollY += 0;
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
