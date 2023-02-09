// How to Play
// See instructions sheet at beginning of game
// Survive long enough to win.
// (Press B and then A for a SURPRISE!)

// Basics
#include <stdlib.h>
#include <stdio.h>
#include "finalProjectLib.h"
#include "mode0.h"
#include "print.h"

// Backgrounds
#include "startBG.h"
#include "instructions.h"
#include "instructions2.h"
#include "pauseBG.h"
#include "winBG.h"
#include "loseBG.h"
#include "level1BG.h"
#include "level1BGOverlay.h"
#include "level2BG.h"
#include "level3BG.h"
#include "cutscene1BG.h"
#include "cutscene2BG.h"
#include "cutscene3BG.h"
#include "cutscene4BG.h"
#include "cutscene5BG.h"
#include "cutscene6BG.h"
#include "cutscene7BG.h"
#include "cutscene2BG1.h"
#include "cutscene2BG2.h"
#include "cutscene2BG3.h"
#include "cutscene2BG4.h"
#include "cutscene2BG5.h"
#include "cutscene2BG6.h"
#include "cutscene2BG7.h"
#include "cutscene2BG8.h"
#include "cutscene2BG9.h"
#include "cutscene2BG10.h"
#include "cutscene2BG11.h"
#include "cutscene2BG12.h"

// Songs
#include "loseSong.h"
#include "startSong.h"
#include "pauseSong.h"
#include "gameSong.h"
#include "instructionsSong.h"
#include "winSong.h"
#include "cutscene1Song.h"
#include "cutscene2Song.h"
#include "cutscene3Song.h"
#include "nextLevelSound.h"

// Spritesheet
#include "spritesheet.h"

// Prototypes.
void initialize();

// State Prototypes.
void goToStart();
void start();
void goToInstructions();
void instructions();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();

// States.
enum {
    START,
    CUTSCENE1,
    INSTRUCTIONS,
    INSTRUCTIONS2,
    GAME,
    CUTSCENE2,
    PAUSE,
    WIN,
    LOSE
};

// Variables
int state;

int seed;

int vOff;
int hOff;
int vOff1;
int hOff1;

int transitioning = 0;
int transition1 = 0;
int transition2 = 0;

int option1 = 1;
int option2 = 0;

int time_cs;
int time_s;

int fromPause = 0;

// Button Variables.
unsigned short buttons;
unsigned short oldButtons;

// Shadow OAM.
OBJ_ATTR shadowOAM[128];
// OBJ_AFFINE *shadowOAM_Affine = (OBJ_AFFINE*)shadowOAM;

int main() {

    mgba_open();
    mgba_printf("Debugging Started");

    initialize();

    while (1) {

        // Update button variables.
        oldButtons = buttons;
        buttons = BUTTONS;

        // State Machine.
        switch (state) {
        case START:
            start();
            break;
        case CUTSCENE1:
            cutscene1();
            break;
        case INSTRUCTIONS:
            instructions();
            break;
        case INSTRUCTIONS2:
            instructions2();
            break;
        case GAME:
            game();
            break;
        case CUTSCENE2:
            cutscene2();
            break;
        case PAUSE:
            pause();
            break;
        case WIN:
            win();
            break;
        case LOSE:
            lose();
            break;
        }

        waitForVBlank();

        DMANow(3, shadowOAM, OAM, 512);

        REG_BG0HOFF = hOff;
        REG_BG0VOFF = vOff;

        REG_BG1HOFF = hOff1;
        REG_BG1VOFF = vOff1;
    }
}

// Sets up GBA.
void initialize() {

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | BG0_ENABLE;

    REG_BG0CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(28) | BG_4BPP | BG_SIZE_SMALL;
    REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(16) | BG_4BPP | BG_SIZE_SMALL;

    DMANow(3, spritesheetPal, SPRITEPALETTE, 256);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);

    hideSprites();

    buttons = BUTTONS;
    oldButtons = 0;

    setupSounds();
    setupInterrupts();

    goToStart();
}

// Sets up the start state.
void goToStart() {

    hasWon1 = 0;
    hasWon2 = 0;
    hasWon3 = 0;

    hideSprites();

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE;

    DMANow(3, startBGPal, PALETTE, 256);
    DMANow(3, startBGTiles, &CHARBLOCK[0], startBGTilesLen / 2);
    DMANow(3, startBGMap, &SCREENBLOCK[16], startBGMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    stopSound();
    playSoundA(startSong_data, startSong_length, 1);

    state = START;
    seed = 0;
}

// Runs every frame of the start state.
void start() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    seed++;

    if (BUTTON_PRESSED(BUTTON_A)) {
        // initGame();
        // goToGame();
        // hasWon1 = 0;
        // goToInstructions();
        goToCutscene1();
    }
}

void goToCutscene1() {

    DMANow(3, cutscene1BGPal, PALETTE, 256);
    DMANow(3, cutscene1BGTiles, &CHARBLOCK[0], cutscene1BGTilesLen / 2);
    DMANow(3, cutscene1BGMap, &SCREENBLOCK[16], cutscene1BGMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    setupInterruptsWithTimers();

    stopSound();
    playSoundA(cutscene1Song_data, cutscene1Song_length, 1);

    state = CUTSCENE1;
    // mgba_printf("state: %d", state);
}

void cutscene1() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    // mgba_printf("HERE");

    if (time_s == 10) {
        // mgba_printf("2!");
        DMANow(3, cutscene2BGPal, PALETTE, 256);
        DMANow(3, cutscene2BGTiles, &CHARBLOCK[0], cutscene2BGTilesLen / 2);
        DMANow(3, cutscene2BGMap, &SCREENBLOCK[16], cutscene2BGMapLen / 2);
        stopSound();
        playSoundA(cutscene2Song_data, cutscene2Song_length, 1);
    }
    if (time_s == 16) {
        // mgba_printf("3!");
        DMANow(3, cutscene3BGPal, PALETTE, 256);
        DMANow(3, cutscene3BGTiles, &CHARBLOCK[0], cutscene3BGTilesLen / 2);
        DMANow(3, cutscene3BGMap, &SCREENBLOCK[16], cutscene3BGMapLen / 2);
    } 
    if (time_s == 20) {
        // mgba_printf("4!");
        DMANow(3, cutscene4BGPal, PALETTE, 256);
        DMANow(3, cutscene4BGTiles, &CHARBLOCK[0], cutscene4BGTilesLen / 2);
        DMANow(3, cutscene4BGMap, &SCREENBLOCK[16], cutscene4BGMapLen / 2);
    } 
    if (time_s == 22) {
        // mgba_printf("5!");
        DMANow(3, cutscene5BGPal, PALETTE, 256);
        DMANow(3, cutscene5BGTiles, &CHARBLOCK[0], cutscene5BGTilesLen / 2);
        DMANow(3, cutscene5BGMap, &SCREENBLOCK[16], cutscene5BGMapLen / 2);
    } 
    if (time_s == 24) {
        // mgba_printf("6!");
        DMANow(3, cutscene6BGPal, PALETTE, 256);
        DMANow(3, cutscene6BGTiles, &CHARBLOCK[0], cutscene6BGTilesLen / 2);
        DMANow(3, cutscene6BGMap, &SCREENBLOCK[16], cutscene6BGMapLen / 2);
    } 
    if (time_s == 26) {
        // mgba_printf("7!");
        DMANow(3, cutscene7BGPal, PALETTE, 256);
        DMANow(3, cutscene7BGTiles, &CHARBLOCK[0], cutscene7BGTilesLen / 2);
        DMANow(3, cutscene7BGMap, &SCREENBLOCK[16], cutscene7BGMapLen / 2);
    } 
    if (time_s == 40) {
        goToInstructions();
    }

    if (BUTTON_PRESSED(BUTTON_B)) {
        goToInstructions();
    }
}

void goToInstructions() {

    REG_TM2CNT = TIMER_OFF;

    time_s = 0;
    time_cs = 0;

    setupInterrupts();

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    DMANow(3, instructionsPal, PALETTE, 256);
    DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen / 2);
    DMANow(3, instructionsMap, &SCREENBLOCK[16], instructionsMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    stopSound();
    playSoundA(instructionsSong_data, instructionsSong_length, 1);

    state = INSTRUCTIONS;
}

void instructions() {

    transition1 = 0;
    transition2 = 0;

    hasWon1 = 0;
    hasWon2 = 0;
    hasWon3 = 0;

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    if (BUTTON_PRESSED(BUTTON_A)) {
        goToInstructions2();
    }
}

void goToInstructions2() {

    // setupInterrupts();

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    DMANow(3, instructions2Pal, PALETTE, 256);
    DMANow(3, instructions2Tiles, &CHARBLOCK[0], instructions2TilesLen / 2);
    DMANow(3, instructions2Map, &SCREENBLOCK[16], instructions2MapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    state = INSTRUCTIONS2;
}

void instructions2() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    if (BUTTON_PRESSED(BUTTON_A)) {
        srand(seed);
        player.col = 40;
        initGame();
        goToGame();
        hasWon1 = 0;
        hasWon2 = 0;
        hasWon3 = 0;
        gameTime = 0;
    }
}

// Sets up the game state.
void goToGame() {

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | BG0_ENABLE;

    hideSprites();

    // DMANow(3, gameBGPal, PALETTE, 256);
    // DMANow(3, gameBGTiles, &CHARBLOCK[0], gameBGTilesLen / 2);
    // DMANow(3, gameBGMap, &SCREENBLOCK[28], 1024 * 4);

    DMANow(3, level1BGPal, PALETTE, 256);
    DMANow(3, level1BGTiles, &CHARBLOCK[0], level1BGTilesLen / 2);
    DMANow(3, level1BGMap, &SCREENBLOCK[16], level1BGMapLen / 2);

    DMANow(3, level1BGOverlayTiles, &CHARBLOCK[1], level1BGOverlayTilesLen / 2);
    DMANow(3, level1BGOverlayMap, &SCREENBLOCK[28], level1BGOverlayMapLen / 2);

    state = GAME;

    REG_BG0VOFF = vOff;
    REG_BG0HOFF = hOff;

    REG_BG1VOFF = vOff1;
    REG_BG1HOFF = hOff1;

    if (!fromPause) {
        stopSound();
        playSoundA(gameSong_data, gameSong_length, 1);
    } else {
        fromPause = 0;
    }
}

// Runs every frame of the game state.
void game() {

    // mgba_printf("player Col: %d", player.col);

    updateGame();
    drawGame();
    animateGame();
    timeIncrament();

    waitForVBlank();

    // Pause
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        // mgba_printf("PAUSE");
        goToPause();
    }

    // Game Time Limit
    if (gameTime == 2500) { // 2500 // 5000
        playSoundB(nextLevelSound_data, nextLevelSound_length, 0);
        hasWon1 = 1;
    } else if (gameTime == 5000) { // 5000 // 10000
        playSoundB(nextLevelSound_data, nextLevelSound_length, 0);
        hasWon2 = 1;
    } else if (gameTime == 7500) { // 7500 // 15000
        playSoundB(nextLevelSound_data, nextLevelSound_length, 0);
        hasWon3 = 1;
    }

    // Moving to Next Level
    if (hasWon1 && !hasWon2 && !hasWon3 && !transition1) {
        player.dashCounter = 3;
        // transitioning = 1;
        goToGame2();
    } else if (hasWon1 && hasWon2 && !hasWon3 && !transition2) {
        // goToWin();
        player.dashCounter = 3;
        // transitioning = 1;
        goToGame3();
    } else if (hasWon1 && hasWon2 && hasWon3) {
        goToCutscene2();
    }

    // Losing
    if (player.col <= 0) {
        // REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;
        goToLose();
    }
}

void goToGame2() {

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | BG0_ENABLE;

    DMANow(3, level2BGPal, PALETTE, 256);
    DMANow(3, level2BGTiles, &CHARBLOCK[0], level2BGTilesLen / 2);
    DMANow(3, level2BGMap, &SCREENBLOCK[16], level2BGMapLen / 2);

    transition1 = 1;

    state = GAME;
}

void goToGame3() {

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | BG0_ENABLE;

    DMANow(3, level3BGPal, PALETTE, 256);
    DMANow(3, level3BGTiles, &CHARBLOCK[0], level3BGTilesLen / 2);
    DMANow(3, level3BGMap, &SCREENBLOCK[16], level3BGMapLen / 2);

    transition2 = 1;

    state = GAME;
}

void goToCutscene2() {

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    hideSprites();

    DMANow(3, cutscene2BG1Pal, PALETTE, 256);
    DMANow(3, cutscene2BG1Tiles, &CHARBLOCK[0], cutscene2BG1TilesLen / 2);
    DMANow(3, cutscene2BG1Map, &SCREENBLOCK[16], cutscene2BG1MapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    setupInterruptsWithTimers();

    stopSound();
    playSoundA(cutscene3Song_data, cutscene3Song_length, 0);

    state = CUTSCENE2;
    // mgba_printf("state: %d", state);
}

void cutscene2() {

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    hideSprites();

    // mgba_printf("HERE");

    if (time_s == 10) {
        // mgba_printf("2!");
        DMANow(3, cutscene2BG2Pal, PALETTE, 256);
        DMANow(3, cutscene2BG2Tiles, &CHARBLOCK[0], cutscene2BG2TilesLen / 2);
        DMANow(3, cutscene2BG2Map, &SCREENBLOCK[16], cutscene2BG2MapLen / 2);
    }
    if (time_s == 12) {
        // mgba_printf("3!");
        DMANow(3, cutscene2BG3Pal, PALETTE, 256);
        DMANow(3, cutscene2BG3Tiles, &CHARBLOCK[0], cutscene2BG3TilesLen / 2);
        DMANow(3, cutscene2BG3Map, &SCREENBLOCK[16], cutscene2BG3MapLen / 2);
    } 
    if (time_s == 14) {
        // mgba_printf("4!");
        DMANow(3, cutscene2BG4Pal, PALETTE, 256);
        DMANow(3, cutscene2BG4Tiles, &CHARBLOCK[0], cutscene2BG4TilesLen / 2);
        DMANow(3, cutscene2BG4Map, &SCREENBLOCK[16], cutscene2BG4MapLen / 2);
    } 
    if (time_s == 18) {
        // mgba_printf("5!");
        DMANow(3, cutscene2BG5Pal, PALETTE, 256);
        DMANow(3, cutscene2BG5Tiles, &CHARBLOCK[0], cutscene2BG5TilesLen / 2);
        DMANow(3, cutscene2BG5Map, &SCREENBLOCK[16], cutscene2BG5MapLen / 2);
    } 
    if (time_s == 22) {
        // mgba_printf("6!");
        DMANow(3, cutscene2BG6Pal, PALETTE, 256);
        DMANow(3, cutscene2BG6Tiles, &CHARBLOCK[0], cutscene2BG6TilesLen / 2);
        DMANow(3, cutscene2BG6Map, &SCREENBLOCK[16], cutscene2BG6MapLen / 2);
    } 
    if (time_s == 24) {
        // mgba_printf("7!");
        DMANow(3, cutscene2BG7Pal, PALETTE, 256);
        DMANow(3, cutscene2BG7Tiles, &CHARBLOCK[0], cutscene2BG7TilesLen / 2);
        DMANow(3, cutscene2BG7Map, &SCREENBLOCK[16], cutscene2BG7MapLen / 2);
    } 
    if (time_s == 26) {
        DMANow(3, cutscene2BG8Pal, PALETTE, 256);
        DMANow(3, cutscene2BG8Tiles, &CHARBLOCK[0], cutscene2BG8TilesLen / 2);
        DMANow(3, cutscene2BG8Map, &SCREENBLOCK[16], cutscene2BG8MapLen / 2);
    }
    if (time_s == 27) {
        DMANow(3, cutscene2BG9Pal, PALETTE, 256);
        DMANow(3, cutscene2BG9Tiles, &CHARBLOCK[0], cutscene2BG9TilesLen / 2);
        DMANow(3, cutscene2BG9Map, &SCREENBLOCK[16], cutscene2BG9MapLen / 2);
    }
    if (time_s == 28) {
        DMANow(3, cutscene2BG10Pal, PALETTE, 256);
        DMANow(3, cutscene2BG10Tiles, &CHARBLOCK[0], cutscene2BG10TilesLen / 2);
        DMANow(3, cutscene2BG10Map, &SCREENBLOCK[16], cutscene2BG10MapLen / 2);
    }
    if (time_s == 29) {
        DMANow(3, cutscene2BG11Pal, PALETTE, 256);
        DMANow(3, cutscene2BG11Tiles, &CHARBLOCK[0], cutscene2BG11TilesLen / 2);
        DMANow(3, cutscene2BG11Map, &SCREENBLOCK[16], cutscene2BG11MapLen / 2);
    }
    if (time_s == 30) {
        DMANow(3, cutscene2BG12Pal, PALETTE, 256);
        DMANow(3, cutscene2BG12Tiles, &CHARBLOCK[0], cutscene2BG12TilesLen / 2);
        DMANow(3, cutscene2BG12Map, &SCREENBLOCK[16], cutscene2BG12MapLen / 2);
    }

    if (time_s == 35) {
        goToWin();
    }

    if (BUTTON_PRESSED(BUTTON_B)) {
        goToWin();
    }
}

// Sets up the pause state.
void goToPause() {

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    hideSprites();

    pauseSoundA();

    DMANow(3, pauseBGPal, PALETTE, 256);
    DMANow(3, pauseBGTiles, &CHARBLOCK[0], pauseBGTilesLen / 2);
    DMANow(3, pauseBGMap, &SCREENBLOCK[16], pauseBGMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    option1 = 1;
    option2 = 0;

    // stopSound();
    playSoundB(pauseSong_data, pauseSong_length, 1);

    state = PAUSE;
}

// Runs every frame of the pause state.
void pause() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    if (BUTTON_PRESSED(BUTTON_UP) && !option1) {
        moveUnderline(11);
        mgba_printf("Option 1");
        option1 = 1;
        option2 = 0;
    } else if (BUTTON_PRESSED(BUTTON_DOWN) && !option2) {
        moveUnderline(15);
        mgba_printf("Option 2");
        option1 = 0;
        option2 = 1;
    }

    if (BUTTON_PRESSED(BUTTON_A) && option1) {
        if (!hasWon1 && !hasWon2 && !hasWon3) {
            fromPause = 1;
            stopSoundB();
            unpauseSoundA();
            goToGame();
        } else if (hasWon1 && !hasWon2 && !hasWon3) {
            stopSoundB();
            // playSoundA(gameSong_data, gameSong_length, 1);
            unpauseSoundA();
            goToGame2();
        } else if (hasWon1 && hasWon2 && !hasWon3) {
            stopSoundB();
            // playSoundA(gameSong_data, gameSong_length, 1);
            unpauseSoundA();
            goToGame3();
        }
    } else if (BUTTON_PRESSED(BUTTON_A) && option2) {
        goToStart();
    }
}

// Sets up the win state.
void goToWin() {

    REG_TM2CNT = TIMER_OFF;

    time_s = 0;
    time_cs = 0;

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    hideSprites();

    DMANow(3, winBGPal, PALETTE, 256);
    DMANow(3, winBGTiles, &CHARBLOCK[0], winBGTilesLen / 2);
    DMANow(3, winBGMap, &SCREENBLOCK[16], winBGMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    stopSound();
    playSoundA(winSong_data, winSong_length, 1);

    state = WIN;
}

// Runs every frame of the win state.
void win() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    if (BUTTON_PRESSED(BUTTON_A)) {
        mgba_printf("BACK TO START");
        goToStart();
    }
}

// Sets up the lose state.
void goToLose() {

    hideSprites();

    REG_DISPCTL = MODE0 | BG1_ENABLE;

    hideSprites();

    DMANow(3, loseBGPal, PALETTE, 256);
    DMANow(3, loseBGTiles, &CHARBLOCK[0], loseBGTilesLen / 2);
    DMANow(3, loseBGMap, &SCREENBLOCK[16], loseBGMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    option1 = 1;
    option2 = 0;

    stopSound();
    playSoundA(loseSong_data, loseSong_length, 1);

    state = LOSE;
}

// Runs every frame of the lose state.
void lose() {

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    if (BUTTON_PRESSED(BUTTON_UP) && !option1) {
        moveUnderline(8);
        mgba_printf("Option 1");
        option1 = 1;
        option2 = 0;
    } else if (BUTTON_PRESSED(BUTTON_DOWN) && !option2) {
        moveUnderline(14);
        mgba_printf("Option 2");
        option1 = 0;
        option2 = 1;
    }

    if (BUTTON_PRESSED(BUTTON_A) && option1) {
        srand(seed);
        player.col = 40;
        initGame();
        hasWon1 = 0;
        hasWon2 = 0;
        hasWon3 = 0;
        gameTime = 0;
        goToInstructions();
    } else if (BUTTON_PRESSED(BUTTON_A) && option2) {
        goToStart();
    }
}