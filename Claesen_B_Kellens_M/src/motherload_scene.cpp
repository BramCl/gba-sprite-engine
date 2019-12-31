//
// Created by maike on 8/12/2019.
//
#include <libgba-sprite-engine/sprites/affine_sprite.h>
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/gba/tonc_memmap.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba/tonc_memdef.h>
#include <libgba-sprite-engine/gba_engine.h>
#include "motherload_scene.h"

#include <libgba-sprite-engine/gba/tonc_core.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>

#include "character_onbewerkt_transparant_16.h"
#include "kul.h"
#include "backgroundblocks.h"
#include "digger.h"
#include "gameOverScene.h"
#include "drill_sound.h"

std::vector<Sprite *> MotherloadScene::sprites() {
    return {
            player.get(),
            batterySprite.get()
    };
}
std::vector<Background *> MotherloadScene::backgrounds() {
    return {
            bg.get()
    };
}

void MotherloadScene::load() {
    foregroundPalette = std::unique_ptr<ForegroundPaletteManager>(new ForegroundPaletteManager(diggerPal, sizeof(diggerPal)));
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(bg_big_Pal, sizeof(bg_big_Pal)));
   // splashPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager(dirtSplash_Left_Pal, sizeof(dirtSplash_Left_Pal)));
    update = true;
    startMiningScrollX = 0;
    startMiningScrollY = 0;
    startMiningTimer = 0;
    dead = false;
    fuel = 50;
    fuelDrainSpeed = 10;
    score = 0;
    money = 0;
    SpriteBuilder<AffineSprite> affineBuilder;

    player = affineBuilder
            .withData(digger, sizeof(digger))
            .withSize(SIZE_32_32)
            .withLocation(112, 18)
            .buildPtr();

    batterySprite = affineBuilder
            .withData(battery, sizeof(battery))
            .withSize(SIZE_16_8)
            .withLocation(10, 0)
            .buildPtr();
    seedRandomMap();
    bg = std::unique_ptr<Background>(new Background(1, dirt_bigTiles, sizeof(dirt_bigTiles), map, sizeof(map)));

    bg.get()->useMapScreenBlock(16);
}
bool MotherloadScene::blockIsClear(int x, int y) {
    if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == BROWNBGTILE) {
        return true;
    } else
        return fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == AIR;
}
bool MotherloadScene::blockIsMineable(int x, int y){
    /*
    if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_LB) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_LO) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_RB) {
        return false;
    } else if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == LAVA_RO) {
        return false;
    } else */if (fullMap[(x + scrollX / 8) + (y + scrollY / 8) * MAP_WIDTH] == AIR) {
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
            if (y == 254) {
                fullMap[y * MAP_WIDTH + x] = STONE_LB;
                fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
            } else {
                if (x == 0) {
                    fullMap[y * MAP_WIDTH + x] = STONE_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
                } else if (x == (MAP_WIDTH - 2)) {
                    fullMap[y * MAP_WIDTH + x] = STONE_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
                } else {
                    int i = rand() % (90+((engine->getTimer()->getTotalMsecs())%20));
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
            for (int y = 0; y < 6; y += 2)
                if (x == 0) {
                    fullMap[y * MAP_WIDTH + x] = STONE_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
                } else if (x == (MAP_WIDTH - 2)) {
                    fullMap[y * MAP_WIDTH + x] = STONE_LB;
                    fullMap[y * MAP_WIDTH + (x + 1)] = STONE_RB;
                    fullMap[(y + 1) * MAP_WIDTH + x] = STONE_LO;
                    fullMap[(y + 1) * MAP_WIDTH + (x + 1)] = STONE_RO;
                } else {
                    fullMap[y * MAP_WIDTH + x] = AIR;
                    fullMap[(y + 1) * MAP_WIDTH + x] = AIR;
                }
        }
    }


}
void MotherloadScene::updateMap(){
    for(int i = 0; i <MAP_SIZE; i++){
        map[i]= fullMap[i+(scrollY/8)*MAP_WIDTH];
    }
}
void MotherloadScene::mineBlock(int x, int y, u16 keys) {
    if(!keys){
        startMiningTimer = engine->getTimer()->getTotalMsecs();
        return;
    }
    if(scrollX != startMiningScrollX || scrollY != startMiningScrollY) {
        startMiningScrollX = scrollX;
        startMiningScrollY = scrollY;
        startMiningTimer = engine->getTimer()->getTotalMsecs();
    }
    else{
    auto frame = player->getCurrentFrame();
        if(frame == 0 || frame == 4 ||frame == 5) {
            player->makeAnimated(4, 2, 5);
        }
        else if(frame == 3 || frame == 8 ||frame == 9) {
            player->makeAnimated(8, 2, 5);
        }
        else if(frame == 2 || frame == 6 ||frame == 7) {
            player->makeAnimated(6, 2, 5);
        }


        if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == DIRT_LB &&
                (startMiningTimer + DIRT_MINE_TIME) >= (engine->getTimer()->getTotalMsecs())){
            fuel = fuel - FUEL_DRAIN_SPEED_DIRT*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == IRON_LB &&
                (startMiningTimer + IRON_MINE_TIME) >= engine->getTimer()->getTotalMsecs()){
            fuel = fuel - FUEL_DRAIN_SPEED_IRON*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == COPPER_LB &&
                (startMiningTimer + COPPER_MINE_TIME) >= engine->getTimer()->getTotalMsecs()){
            fuel = fuel - FUEL_DRAIN_SPEED_COPPER*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == GOLD_LB &&
                (startMiningTimer+GOLD_MINE_TIME) >= engine->getTimer()->getTotalMsecs()){
            fuel = fuel - FUEL_DRAIN_SPEED_GOLD*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == EMERALD_LB &&
                (startMiningTimer+EMERALD_MINE_TIME) >= engine->getTimer()->getTotalMsecs()){
            fuel = fuel - FUEL_DRAIN_SPEED_EMERALD*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == DIAMOND_LB &&
                (startMiningTimer +DIAMOND_MINE_TIME) >= engine->getTimer()->getTotalMsecs()){
            fuel = fuel - FUEL_DRAIN_SPEED_DIAMOND*fuelDrainSpeed;
            engine.get()->enqueueSound(drill, drill_bytes, 44100);
            return;
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == LAVA_LB){
            dead = true;
            return;
        }
        if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == IRON_LB){
            addMoney(IRON_MONEY);
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == COPPER_LB){
            addMoney(COPPER_MONEY);
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == GOLD_LB){
            addMoney(GOLD_MONEY);
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == EMERALD_LB){
            addMoney(EMERALD_MONEY);
        }
        else if(fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] == DIAMOND_LB){
            addMoney(DIAMOND_MONEY);
        }
        fullMap[(x + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
        fullMap[(x+1 + scrollX / 8) + ((y + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
        fullMap[(x + scrollX / 8) + ((y+1 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
        fullMap[(x+1 + scrollX / 8) + ((y+1 + scrollY / 8) * MAP_WIDTH)] = BROWNBGTILE;
        addScore(1);
        updateMap();
    }
}
void MotherloadScene::tick(u16 keys) {
    player -> stopAnimating();
    if((scrollY % 8) == 0 && lastUpdate != scrollY){
        update = true;
        lastUpdate = scrollY;
    }
    if(update) {
        updateMap();
        update = !update;
    }
    fuel = fuel -0.003*fuelDrainSpeed;
    if(fuel < 0){
        dead = true;

    }
    if (dead){
        if(score > 254){
            *(sram_mem+1) = score / 255;
        }
        else{
            *(sram_mem+1) = 0;
        }
        *sram_mem = score;
        if(!engine->isTransitioning()) {
            engine->transitionIntoScene(new GameOverScene(engine), new FadeOutScene(2));
        }
    }
    TextStream::instance().setText("Score: " + std::to_string(score), 0, 15);
    TextStream::instance().setText("Money: " + std::to_string((int) money), 1, 15);

    batteryUpdate();
    upgradeByScore();
    bg.get()->scroll(8, 0);
    player->moveTo(104+scrollX,18);
    bg.get()->updateMap(map);

    if(0< player ->getX() <20 && scrollY ==0){ // toets X op toetsenbor
        TextStream::instance().setText("press x to refuel", 2, 2);
        if(keys & KEY_A){
            if(money == 0){
                TextStream::instance().setText("not enough money to refuel", 2, 2);
            }
            refuel();
        }
    }
    else{
        TextStream::instance().setText("", 2, 2);
    }
    if (keys & KEY_LEFT) {
        if (blockIsClear(14, 4) && blockIsClear(14,5)) {
            scrollX -= 2;
            player->animateToFrame(0);
        }
        else if (blockIsMineable(14,4) && blockIsMineable(13,4) && blockIsMineable(14,5) && blockIsMineable(13,5) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
            mineBlock(13,4, keys);
        }
    }
    if (keys & KEY_RIGHT) {
        if (blockIsClear(17, 4) && blockIsClear(17, 5)) {
            scrollX += 2;
            player->animateToFrame(2);
        }
        else if (blockIsMineable(17,4) && blockIsMineable(18,4) && blockIsMineable(17,5) && blockIsMineable(18,5) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
            mineBlock(17,4, keys);
        }
    }
    if (keys & KEY_UP) {
        if (blockIsClear(15, 3) && blockIsClear(16,3)) {
            scrollY -= 2;
            player->animateToFrame(1);
            fuel = fuel - FUEL_DRAIN_SPEED_FLYING;
            auto frame = player->getCurrentFrame();
            if(frame == 1 || frame == 10 ||frame == 11) {
                player->makeAnimated(10, 2, 5);

            }
        }
    }
    if(!keys){ //wanneer er geen knop is ingedrukt, moet dit geforward worden naar de mineBlock functie, zo kan deze z'n timer resetten.
        mineBlock(15,6,keys);
    }
    if((blockIsClear(15,6) && blockIsClear(16,6))) {
        if (keys & KEY_DOWN) {
            scrollY += 1;
            player->animateToFrame(3);
        }
        scrollY += 1;
    }
    if(fullMap[(15 + scrollX / 8) + ((6 + scrollY / 8) * MAP_WIDTH)] == LAVA_LB && fullMap[(16 + scrollX / 8) + ((6 + scrollY / 8) * MAP_WIDTH)] == LAVA_RB){
        dead = true;
    }
    if(keys & KEY_DOWN){
        while((15 + scrollX / 8)%2 != 0){
            scrollX--;
        }

        if (blockIsMineable(15,6) && blockIsMineable(16,6) && blockIsMineable(15,7) && blockIsMineable(16,7) && !blockIsClear(15,6) && !blockIsClear(16,6)) {
            mineBlock(15,6, keys);
        }
    }
    if(scrollY < 0){
        scrollY = 0;
    }
}
void MotherloadScene::addScore(int points) {
    score = score + points;
}
void MotherloadScene::addMoney(float money){
    this -> money  = this->money + money;
}
void MotherloadScene::refuel(){
    if(fuel < 100 && money > 0){
        fuel = fuel  + 0.5;
        money = money -0.025;
    }
}
void MotherloadScene::batteryUpdate(){
    if(fuel >95) batterySprite-> animateToFrame(0);
    else if(90< fuel && fuel < 95) batterySprite -> animateToFrame(1);
    else if(80< fuel && fuel < 90) batterySprite -> animateToFrame(2);
    else if(70< fuel && fuel < 80) batterySprite -> animateToFrame(3);
    else if(60< fuel && fuel < 70) batterySprite -> animateToFrame(4);
    else if(50< fuel && fuel < 60) batterySprite -> animateToFrame(5);
    else if(40< fuel && fuel < 50) batterySprite -> animateToFrame(6);
    else if(30< fuel && fuel < 40) batterySprite -> animateToFrame(7);
    else if(20< fuel && fuel < 30) batterySprite -> animateToFrame(8);
    else if(10< fuel && fuel < 20) batterySprite -> animateToFrame(9);
    else if(0< fuel && fuel < 10) batterySprite -> animateToFrame(10);
}
void MotherloadScene::upgradeByScore(){
    if(score == 10){
        fuelDrainSpeed = 8;
    }
    else if(score == 50){
        fuelDrainSpeed = 6;
    }
    else if(score == 100){
        fuelDrainSpeed = 4;
    }
    else if(score == 200){
        fuelDrainSpeed = 2;
    }
    else if(score == 500){
        fuelDrainSpeed = 1;
    }
}