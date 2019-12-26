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
#include <libgba-sprite-engine/gba_engine.h>
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
    engine->getTimer()->start();
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
bool MotherloadScene::blockIsClear(int x, int y) {
    if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE) {
        return true;
    } else
        return fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == AIR;
}
bool MotherloadScene::blockIsMineable(int x, int y){
    if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_LB) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_LO) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_RB) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_RO) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == AIR) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == STONE_LB) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == STONE_LO) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == STONE_RB) {
        return false;
    } else return fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] != STONE_RO;
}

void MotherloadScene::seedRandomMap() {

    for (int x = 0; x < MAP_WIDTH; x += 2) {
        for (int y = 6; y < FULL_MAP_HEIGHT; y += 2) {
            if(x==0){
                fullMap[y * MAP_WIDTH + x] = STONE_LB;
                fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
            }
            else if(x==(MAP_WIDTH-2)){
                fullMap[y * MAP_WIDTH + x] = STONE_LB;
                fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
            }
            else {
                int i = rand() % 100;
                if (i <= 4) { //5% is empty space
                    fullMap[y * MAP_WIDTH + x] = BROWNBGTILE;
                    fullMap[y * MAP_WIDTH + (x + 1)] = BROWNBGTILE;
                    fullMap[(y + 1) * MAP_WIDTH + x] = BROWNBGTILE;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = BROWNBGTILE;
                } else if (i > 4 && i <= 7) { //3% is stone
                    fullMap[y * MAP_WIDTH + x] = STONE_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
                } else if (i > 7 && i <= 14) { //7% is iron
                    fullMap[y * MAP_WIDTH + x] = IRON_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = IRON_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = IRON_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = IRON_RO;
                } else if (i > 14 && i <= 19) { //5% is copper
                    fullMap[y * MAP_WIDTH + x] = COPPER_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = COPPER_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = COPPER_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = COPPER_RO;
                } else if (i > 19 && i <= 21) { //2% is gold
                    fullMap[y * MAP_WIDTH + x] = GOLD_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = GOLD_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = GOLD_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = GOLD_RO;
                } else if (i > 21 && i <= 22) { //1% is emerald
                    fullMap[y * MAP_WIDTH + x] = DIAMOND_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = DIAMOND_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = DIAMOND_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = DIAMOND_RO;
                } else if (i > 22 && i <= 23) { //1% is diamond
                    fullMap[y * MAP_WIDTH + x] = EMERALD_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = EMERALD_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = EMERALD_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = EMERALD_RO;
                } else if (i > 23 && i <= 25) { //2% is lava
                    fullMap[y * MAP_WIDTH + x] = LAVA_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = LAVA_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = LAVA_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = LAVA_RO;
                } else {
                    fullMap[y * MAP_WIDTH + x] = DIRT_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = DIRT_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = DIRT_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = DIRT_RO;
                }
            }
        }
        for (int y = 0; y < 6; y+=2)
            if(x==0){
                fullMap[y * MAP_WIDTH + x] = STONE_LB;
                fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
            }
            else if(x==(MAP_WIDTH-2)){
                fullMap[y * MAP_WIDTH + x] = STONE_LB;
                fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
            }
            else {
                fullMap[y * MAP_WIDTH + x] = AIR;
                fullMap[(y+1) * MAP_WIDTH + x] = AIR;
            }
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
    TextStream::instance().setText(std::to_string(engine->getTimer()->getTotalMsecs()), 0, 1);
    TextStream::instance().setText(std::to_string(-scrollY), 1, 1);

    bg.get()->scroll(8, 0);
    player->moveTo(104+scrollX,18);
    bg.get()->updateMap(map);

    if (keys & KEY_LEFT) {
        if (blockIsClear(14, 4) && blockIsClear(14,5)) {
            scrollX -= 2;
            player->animateToFrame(0);
        }
        else if (blockIsMineable(14,4) && blockIsMineable(13,4) && blockIsMineable(14,5) && blockIsMineable(13,5) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
            fullMap[(14 + scrollX / 8) + ((4 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(13 + scrollX / 8) + ((4 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(14 + scrollX / 8) + ((5 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(13 + scrollX / 8) + ((5 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            updateMap();
        }
    }
    if (keys & KEY_RIGHT) {
        if (blockIsClear(17, 4) && blockIsClear(17, 5)) {
            scrollX += 2;
            player->animateToFrame(2);
        }
        else if (blockIsMineable(17,4) && blockIsMineable(18,4) && blockIsMineable(17,5) && blockIsMineable(18,5) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
            fullMap[(17 + scrollX / 8) + ((4 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(18 + scrollX / 8) + ((4 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(17 + scrollX / 8) + ((5 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            fullMap[(18 + scrollX / 8) + ((5 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
            updateMap();
        }
    }
    if (keys & KEY_UP) {
        if (blockIsClear(15, 3) && blockIsClear(16,3)) {
            scrollY -= 2;
            player->animateToFrame(1);
        }
    }
    if(blockIsClear(15,6) && blockIsClear(16,6)) {
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

        if (blockIsMineable(15,6) && blockIsMineable(16,6) && blockIsMineable(15,7) && blockIsMineable(16,7) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
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
