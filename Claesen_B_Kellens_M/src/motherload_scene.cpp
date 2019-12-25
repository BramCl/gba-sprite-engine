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
#include "digger.h"

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
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(diggerPal, sizeof(diggerPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_big_Pal, sizeof(bg_big_Pal)));
    update = true;
    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(digger, sizeof(digger))
            .withSize(SIZE_32_32)
            .withLocation(112, 18)
            .buildPtr();
    seedRandomMap();
    bg = std::unique_ptr<Background>(new Background(1, dirt_bigTiles, sizeof(dirt_bigTiles), map, sizeof(map)));

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
bool MotherloadScene::blockIsClear(int upperLeftX, int upperLeftY) {
    if((2 + scrollY / 8)%2 != 0){
        scrollY++;
    }
    if (fullMap[(upperLeftX + scrollX / 8) + (upperLeftY + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE &&
        fullMap[(upperLeftX + 1 + scrollX / 8) + (upperLeftY + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE &&
        fullMap[(upperLeftX + scrollX / 8) + (upperLeftY + 1 + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE &&
        fullMap[(upperLeftX + 1 + scrollX / 8) + (upperLeftY + 1 + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE) {
        return true;
    } else
        return fullMap[(upperLeftX + scrollX / 8) + (upperLeftY + scrollY / 8) * MAP_WIDTH] == AIR &&
               fullMap[(upperLeftX + 1 + scrollX / 8) + (upperLeftY + scrollY / 8) * MAP_WIDTH] == AIR &&
               fullMap[(upperLeftX + scrollX / 8) + (upperLeftY + 1 + scrollY / 8) * MAP_WIDTH] == AIR &&
               fullMap[(upperLeftX + 1 + scrollX / 8) + (upperLeftY + 1 + scrollY / 8) * MAP_WIDTH] == AIR;

}
void MotherloadScene::seedRandomMap() {

    for (int x = 0; x < MAP_WIDTH; x += 2) {
        for (int y = 6; y < FULL_MAP_HEIGHT; y += 2) {
            int i = qran_range(0, 6);
            if(i == 0){
                fullMap[y * MAP_WIDTH + x] = BROWNBGTILE;
                fullMap[y * MAP_WIDTH + (x+1)] = BROWNBGTILE;
                fullMap[(y+1) * MAP_WIDTH + x] = BROWNBGTILE;
                fullMap[(y+1) * MAP_WIDTH + (x+1)] = BROWNBGTILE;
            }
            else {
                fullMap[y * MAP_WIDTH + x] = DIRT_LB;
                fullMap[y * MAP_WIDTH + (x+1)] = DIRT_RB;
                fullMap[(y+1) * MAP_WIDTH + x] = DIRT_LO;
                fullMap[(y+1) * MAP_WIDTH + (x+1)] = DIRT_RO;
            }
        }
        for (int y = 0; y < 6; y++)
            fullMap[y* MAP_WIDTH + x] = AIR;
    }


}


void MotherloadScene::updateMap(){
    for(int i = 0; i <MAP_SIZE; i++){
        map[i]= fullMap[i+(scrollY/8)*MAP_WIDTH];
    }
}
void MotherloadScene::tick(u16 keys) {
    if((scrollY % 8) == 0 && lastUpdate != scrollY){
        update = true;
        lastUpdate = scrollY;
    }
    if(update) {
        updateMap();
        update = !update;
    }
    TextStream::instance().setText(std::to_string(-scrollY), 0, 0);

    bg.get()->scroll(scrollX, 0);
    bg.get()->updateMap(map);

    if (keys & KEY_LEFT) {
        while((13 + scrollX / 8)%2 != 0){
            scrollX--;
        }
        if (blockIsClear(13, 4)) {
            scrollX -= 2;
            player->animateToFrame(0);
        }
    }
    if (keys & KEY_RIGHT) {
        while((17 + scrollX / 8)%2 != 0){
            scrollX++;
        }
        if (blockIsClear(17, 4)) {
            scrollX += 2;
            player->animateToFrame(2);
        }
    }
    if (keys & KEY_UP) {
        while((2 + scrollY / 8)%2 != 0){
            scrollY--;
        }
        if (blockIsClear(15, 2)) {
            scrollY -= 2;
            player->animateToFrame(1);
        }
    }
    if(blockIsClear(15,6)) {
        if (keys & KEY_DOWN) {
            scrollY += 1;
            player->animateToFrame(3);
        }
        scrollY += 1;
    }
    if(keys & KEY_DOWN){
        while((15 + scrollX / 8)%2 != 0){
            scrollX--;
        }

        if (!blockIsClear(15,6)) {
            fullMap[(15 + scrollX / 8) + ((6 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(16 + scrollX / 8) + ((6 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(15 + scrollX / 8) + ((7 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(16 + scrollX / 8) + ((7 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            updateMap();
        }
    }

    if(scrollY < 0){
        scrollY = 0;
    }
}
