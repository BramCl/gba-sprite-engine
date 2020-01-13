//
// Created by Maikel on 27/12/2019.
//
//sram_mem is standaard aangemaakt in het memory van de engine, in dit stuk code wordt wat met de pointers gespeeld om het geheugen juist
//te gebruiken zonder dat er dingen fout lopen zoals te kleine geheugens of overlappende date. (Dit werkt zolang de highscore niet te hoog is)
#include <libgba-sprite-engine/sprites/sprite_builder.h>
#include <libgba-sprite-engine/background/text_stream.h>
#include <libgba-sprite-engine/gba_engine.h>
#include <libgba-sprite-engine/effects/fade_out_scene.h>
#include "gameover_scene.h"
#include "motherload_scene.h"
#include "digger.h"
#include "backgroundblocks.h"

std::vector<Background *> GameOverScene::backgrounds() {
    return {};
}
std::vector<Sprite *> GameOverScene::sprites() {
    return {  animation.get() };
}
void GameOverScene::load() {
    backgroundPalette = std::unique_ptr<BackgroundPaletteManager>(new BackgroundPaletteManager());

    //standaard zit er 255 in de geheugens, dit naar 0 veranderen zodat er geen fake highscore is.
    for(int i = 0; i < 22; i++){
        if(*(sram_mem+i) == 255){
            *(sram_mem+i) = 0;
        }
    }

    SpriteBuilder<Sprite> builder;
    score = *sram_mem+ *(sram_mem+1)*255;
    animation = builder.withData(0, 0).buildPtr();

    TextStream::instance().setText("GAME OVER", 2, 8);
    TextStream::instance().setText("Uw score is:", 4, 8);
    TextStream::instance().setText(std::to_string(score), 5, 8);
    calculateHighScores();
    printHighScores();
}
void GameOverScene::calculateHighScores(){
    //de behaalde score wordt in de lijst met highscores geplaatst op de juiste plaats
    for(int j = 2; j <22;j += 2) {
        if (score > (*(sram_mem+j) + *(sram_mem + j + 1) * 255)) {
            for (int i = 22; i > j; i--) {
                *(sram_mem + i) = *(sram_mem + i - 2);
            }
            *(sram_mem + j) = *sram_mem;
            *(sram_mem + j + 1) = *(sram_mem+1);
            break;
        }
    }
}
void GameOverScene::printHighScores(){
    //De highscores worden op het scherm gezet
    TextStream::instance().setText("HighScores:", 6, 8);
    for(int i = 2; i < 22; i +=2){
        TextStream::instance().setText(std::to_string(*(sram_mem + i) + *(sram_mem + i + 1)*255), 7+(i/2), 8);
    }
}
void GameOverScene::tick(u16 keys) {
    if(keys) {//Als er op een toets wordt gedrukt, wordt het spel herstart
        if(!engine->isTransitioning()) {
            engine->transitionIntoScene(new MotherloadScene(engine), new FadeOutScene(2));
        }
    }
}