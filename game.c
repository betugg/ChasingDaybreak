#include <stdlib.h>
#include <stdio.h>
#include "finalProjectLib.h"
#include "mode0.h"
#include "print.h"

// Sound
#include "collectLightSound.h"
#include "dashSound.h"
#include "nextLevelSound.h"

// Collision Map Import
#include "collisionMap.h"

// Prototypes

// Entire Game
void initGame();
void updateGame();
void drawGame();

// Player
void initPlayer();
void updatePlayer();
void drawPlayer();

// BAT Enemy
void initBats();
void updateBats();
void attackBats();
void drawBats();
void animateBats();
void initBatsCol();
void updateBatsCol();
void attackBatsCol();
void drawBatsCol();
void animateBatsCol();

// SPIDER Enemy
void initSpiders();
void updateSpiders();
void attackSpiders();
void drawSpiders();
void animateSpiders();

// Objects
ANISPRITE player;
ANISPRITE light;
ANISPRITE level;
ANISPRITE monster;
ANISPRITE bat[BATCOUNT];
ANISPRITE batCol[BATCOLCOUNT];
ANISPRITE spider[SPIDERCOUNT];
ANISPRITE dashToken[DASHTOKENCOUNT];

// Variables
int gameTime = 0;
int transitionTime = 0;
int spiderTime = 0;
int batTime = 0;
int batTime2 = 0;
int lightTime = 0;
int slotOAM = 1;
int frameCounter = 4;
int batColRandomizer = 0;
int inAndOut = 0;
// int spiderSpot = 1;
// int batSpot = 5;

// Winnning Conditions
int hasWon1 = 0;
int hasWon2 = 0;
int hasWon3 = 0;

vOff = 0;
hOff= 0;

vOff1 = 0;
hOff1= 0;

// Collision Map

unsigned char* collisionmap = (unsigned char*) collisionMapBitmap;

// GAME IN GENERAL

void timeIncrament() {

    gameTime++;
    // mgba_printf("gameTime: %d", gameTime);
}

// LEVEL 1

void initGame() {

    player.col = 40;

    initPlayer();
    initLight();
    initBats();
    initBatsCol();
    initSpiders();
    initDashToken();
    initLevel();
    initMonster();

    slotOAM = 1;
}

void updateGame() {

    slotOAM = 1;

    hOff += 4;
    hOff1++;

    updatePlayer();
    updateLight();
    updateLevel();
    updateMonster();
    // updateDashToken();
    for (int i = 0; i < BATCOUNT; i++) {
		ANISPRITE* bptr = &bat[i];
		updateBats(bptr);
	}
    for (int i = 0; i < BATCOLCOUNT; i++) {
		ANISPRITE* bcptr = &batCol[i];
		updateBatsCol(bcptr);
	}
    for (int i = 0; i < SPIDERCOUNT; i++) {
		ANISPRITE* sptr = &spider[i];
		updateSpiders(sptr);
	}
    // updateDashToken();
}

void drawGame() {

    // drawPlayer();
    drawLevel();
    drawLight();
    for (int i = 0; i < BATCOUNT; i++) {
		ANISPRITE* bptr = &bat[i];
		drawBats(bptr);
	}
    for (int i = 0; i < BATCOLCOUNT; i++) {
		ANISPRITE* bcptr = &batCol[i];
		drawBatsCol(bcptr);
	}
    for (int i = 0; i < SPIDERCOUNT; i++) {
		ANISPRITE* sptr = &spider[i];
		drawSpiders(sptr);
	}
    drawDashToken();
    drawMonster();
}

void animateGame() {

    animatePlayer();
    animateLight();
    animateSpiders();
    animateBats();
    animateBatsCol();
    animateMonster();
}

// PLAYER

void initPlayer() {

    player.row = 110;
    player.col = 40;
    player.width = 32;
    player.height = 32;
    player.rdel = 3;
    player.cdel = 10;
    player.aniCounter = 0;
    player.curFrame = 0;
    player.numFrames = 3;
    player.aniState = SPRITELEFT;
    player.isJumping = 0;
    player.isPushedBack = 0;
    player.isDoubleJumping = 0;
    player.isDashing = 0;
    player.isCheating = 0;
    player.jumpFrames = 0;
    player.dashFrames = 0;
    player.pushBackFrames = 0;
    player.dashCounter = 3;
    player.livesRemaining = 3;
}

void updatePlayer() {

    // JUMPING
    if (BUTTON_PRESSED(BUTTON_UP) && (collisionCheck(collisionmap, MAPWIDTH, player.col, player.row, player.width, player.height, 0, -player.rdel) > 0) && !collisionCheck(collisionmap, MAPWIDTH, player.col, player.row, player.width, player.height, 0, player.rdel)) {
        player.isJumping = 1;
        // player.isFalling = 0;
    }

    if (player.isJumping == 1) {
        player.row -= player.rdel;
        player.jumpFrames++;
        if (player.jumpFrames > MAX_JUMP_HEIGHT) {
            player.isJumping = 0;
            // player.isFalling = 1;
            player.jumpFrames = 0;
        }
    } else if (collisionCheck(collisionmap, MAPWIDTH, player.col, player.row, player.width, player.height, 0, player.rdel) > 0){
        player.row += player.rdel;
    }

    // FAST FALL
    if (BUTTON_HELD(BUTTON_DOWN) && !player.isJumping && (collisionCheck(collisionmap, MAPWIDTH, player.col, player.row, player.width, player.height, 0, player.rdel) > 0)) {
        player.row += (player.rdel * 2);
    }

    // DASHING
    if (BUTTON_PRESSED(BUTTON_RIGHT) && collisionCheck(collisionmap, MAPWIDTH, player.col, player.row, player.width, player.height, player.cdel, 0) > 0 && !(player.dashCounter == 0)) {
        player.isDashing = 1;
        player.dashCounter--;
        playSoundB(dashSound_data, dashSound_length, 0);
    }

    if (player.isDashing == 1) {
        player.col += player.cdel;
        player.dashFrames++;
        if (player.dashFrames > MAX_DASH_DISTANCE) {
            player.isDashing = 0;
            player.dashFrames = 0;
        }
    }

    // SLIDING
    if (BUTTON_HELD(BUTTON_DOWN) && player.row >= 110 && !player.isCheating) {
        player.isSliding = 1;
        player.width = 16;
        player.height = 32;
        player.row = 126;
        drawPlayerSlide();
        frameCounter = 1;
        // if (player.aniState != 4) {
        //     player.aniState += 4;
        // }
    } else if (BUTTON_HELD(BUTTON_DOWN) && player.row >= 110 && player.isCheating) {
        player.isSliding = 1;
        player.width = 16;
        player.height = 32;
        player.row = 126;
        player.aniState = 0;
        frameCounter = 1;
        drawPlayerSlide();
    } else if (player.isSliding && !player.isCheating) {
        player.isSliding = 0;
        player.width = 32;
        player.height = 32;
        player.row = 110;
        // player.col++;
        // drawPlayer();
        frameCounter = 4;
        // if (player.aniState == 4) {
        //     player.aniState -= 4;
        // }
    } else if (player.isSliding && player.isCheating) {
        player.isSliding = 0;
        player.width = 32;
        player.height = 32;
        player.row = 110;
        // player.col++;
        // drawPlayer();
        player.aniState = 2;
        frameCounter = 4;
        // if (player.aniState == 4) {
        //     player.aniState -= 4;
        // }
    }
    if (player.isSliding) {
        drawPlayerSlide();
    } else {
        drawPlayer();
    }

    // CHEATING
    if (BUTTON_PRESSED(BUTTON_B) && !player.isCheating) {
        player.isCheating = 1;
        player.aniState = 2;
    } else if (BUTTON_PRESSED(BUTTON_B) && player.isCheating) {
        player.isCheating = 0;
        player.aniState = 0;
        player.curFrame = 0;
    }

    if (player.isCheating && BUTTON_HELD(BUTTON_A) && !player.isSliding) {
        player.aniState = 3;
        // player.curFrame = 0;
        // mgba_printf("aniState: %d", player.aniState);
        // mgba_printf("curFrame: %d", player.curFrame);
        frameCounter = 2;
    } else if (player.isCheating && !player.isSliding) {
        player.aniState = 2;
        frameCounter = 4;
    }

    // HIT BY ENEMY
    if (player.isPushedBack) {
        // playSoundB(hitSound_data, hitSound_length, 0);
        player.col -= player.cdel;
        player.pushBackFrames++;
        if (player.pushBackFrames > MAX_PUSH_BACK_DISTANCE) {
            player.isPushedBack = 0;
            player.pushBackFrames = 0;
        }
    }

    // BOUNDARY CHECK
    if (player.col >= 220) {
        player.col = 220;
    }

}

void drawPlayer() {

    shadowOAM[0].attr0 = (player.row & ROWMASK)| ATTR0_4BPP | ATTR0_SQUARE;
	shadowOAM[0].attr1 = (player.col & COLMASK) | ATTR1_MEDIUM;
	shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(player.aniState * 4, player.curFrame * 4);
}

void drawPlayerSlide() {

    shadowOAM[0].attr0 = (player.row & ROWMASK) | ATTR0_4BPP | ATTR0_WIDE;
	shadowOAM[0].attr1 = (player.col & COLMASK) | ATTR1_MEDIUM;
	shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(4 + player.aniState * 4, player.curFrame * 4);
}

void animatePlayer() {

    player.aniCounter++;
    if(player.aniCounter % 6 == 0) {
        player.curFrame++;
    }
    if (player.curFrame >= frameCounter) {
        player.curFrame = 0;
    }
}

// SPIDERS

void initSpiders(ANISPRITE* s) {

    for (int i = 0; i < SPIDERCOUNT; i++) {
        spider[i].active = 0;
        spider[i].width = 8;
        spider[i].height = 8;
        spider[i].cdel = 1;
        spider[i].rdel = 1;
        spider[i].row = 125;
        spider[i].col = ENEMY_START_DISTANCE;
        spider[i].aniState = SPRITELEFT;
    }
}

void updateSpiders(ANISPRITE* s) {

    attackSpiders();

    if (s -> active && collisionCheck(collisionmap, MAPWIDTH, s -> col, s -> row, s -> width, s -> height, -(s -> cdel), 0) > 0) {
        s -> col -= (s -> cdel * 3);
    }

    if (collision(s -> col, s -> row, s -> width, s -> height, player.col, player.row, player.width, player.height) && s -> active && !player.isDashing && !player.isCheating) {
		s -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY SPIDER");
        // s -> col = ENEMY_START_DISTANCE;
        s -> col = hOff + SCREENWIDTH;
	} else if (collision(s -> col, s -> row, s -> width, s -> height, player.col, player.row, player.width, player.height) && s -> active && !player.isDashing && player.isCheating && !BUTTON_HELD(BUTTON_A)) {
        s -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY SPIDER");
        // b -> col = ENEMY_START_DISTANCE;
        s -> col = hOff + SCREENWIDTH;
    } else if (collision(s -> col, s -> row, s -> width, s -> height, player.col, player.row, player.width, player.height) && s -> active && !player.isDashing && player.isCheating && BUTTON_HELD(BUTTON_A)) {
        s -> active = 0;
        mgba_printf("KILL SPIDER");
        // player.aniState = 1;
        // player.curFrame = 1;
        // frameCounter = 2;
    }

    if (s -> col <= 0) {
        s -> active = 0;
        // s -> col = ENEMY_START_DISTANCE;
        s -> col = hOff + SCREENWIDTH;
    }
}

void attackSpiders() {

    int skipFrames = 650;

    if (spiderTime % skipFrames == 0) {
        for (int i = 0; i < SPIDERCOUNT; i++) {
		    if (spider[i].active == 0) {
                mgba_printf("spider spawn");
                spider[i].active = 1;
                // spider[i].row = 20 + rand();
                spider[i].col = ENEMY_START_DISTANCE;
                spiderTime = 0;
			    break;
		    }
	    }
    }

    spiderTime++;
}

void drawSpiders(ANISPRITE* s) {

    if (s -> active) {
        shadowOAM[slotOAM].attr0 = (s -> row & ROWMASK) | ATTR0_4BPP | ATTR0_SQUARE;
	    shadowOAM[slotOAM].attr1 = (s -> col & COLMASK) | ATTR1_SMALL;
	    shadowOAM[slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0, 20 + s -> curFrame * 2);
    } else {
        shadowOAM[slotOAM].attr0 |= ATTR0_HIDE;
    }
    slotOAM++;
}

void animateSpiders() {

    for (int i = 0; i < SPIDERCOUNT; i++) {
        spider[i].aniCounter++;
        if(spider[i].aniCounter % 12 == 0) {
            spider[i].curFrame++;
        }
        if (spider[i].curFrame >= 2) {
            spider[i].curFrame = 0;
        }
    }
}

// BATS

void initBats() {

    for (int i = 0; i < BATCOUNT; i++) {
        bat[i].active = 0;
        bat[i].width = 8;
        bat[i].height = 8;
        bat[i].cdel = 1;
        bat[i].rdel = 1;
        bat[i].row = 100;
        bat[i].aniState = SPRITELEFT;
        bat[i].col = BAT_START_DISTANCE;
    }
}

void updateBats(ANISPRITE* b) {

    attackBats();

    // if (b -> active && collisionCheck(collisionmap, MAPWIDTH, b -> col, b -> row, b -> width, b -> height, -(b -> cdel), 0) > 0) {
    //     b -> col -= (b -> cdel * 3);
    // }

    if (b -> active) {
        b -> col -= (b -> cdel * 3);
    }

    if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && !player.isCheating) {
		b -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY BAT");
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
	} else if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && player.isCheating && !BUTTON_HELD(BUTTON_A)) {
        b -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY BAT");
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
    } else if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && player.isCheating && BUTTON_HELD(BUTTON_A)) {
        b -> active = 0;
        mgba_printf("KILL BAT");
        // player.aniState = 1;
        // player.curFrame = 1;
        // frameCounter = 2;
    }
    if (b -> col <= 2) {
        b -> active = 0;
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
    }
}

void attackBats() {

    int skipFrames = 800;

    if (batTime % skipFrames == 0) {
        mgba_printf("spawn bat");
        for (int i = 0; i < BATCOUNT; i++) {
		    if (bat[i].active == 0) {
                bat[i].active = 1;
                bat[i].row = (rand() % 110) + 20;
                bat[i].col = BAT_START_DISTANCE;
                batTime = 0;
			    break;
		    }
	    }
    }

    batTime++;
}

void drawBats(ANISPRITE* b) {

    if (b -> active) {
        shadowOAM[slotOAM].attr0 = (b -> row & ROWMASK) | ATTR0_4BPP | ATTR0_SQUARE;
	    shadowOAM[slotOAM].attr1 = (b -> col & COLMASK) | ATTR1_SMALL;
	    shadowOAM[slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0, 16 + b -> curFrame * 2);
    } else {
        shadowOAM[slotOAM].attr0 |= ATTR0_HIDE;
    }
    slotOAM++;
}

void animateBats() {

    for (int i = 0; i < BATCOUNT; i++) {
        bat[i].aniCounter++;
        if(bat[i].aniCounter % 12 == 0) {
            bat[i].curFrame++;
        }
        if (bat[i].curFrame >= 2) {
            bat[i].curFrame = 0;
        }
    }
}

// BAT COLUMN

void initBatsCol() {

    for (int i = 0; i < BATCOLCOUNT; i++) {
        batCol[i].active = 0;
        batCol[i].width = 32;
        batCol[i].height = 64;
        batCol[i].cdel = 1;
        batCol[i].rdel = 1;
        batCol[i].row = 0;
        batCol[i].col = BATCOL_START_DISTANCE;
    }
}

void updateBatsCol(ANISPRITE* b) {

    attackBatsCol();

    if (b -> active && collisionCheck(collisionmap, MAPWIDTH, b -> col, b -> row, b -> width, b -> height, -(b -> cdel), 0) > 0) {
        b -> col -= (b -> cdel * 3);
    }
    
    if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && !player.isCheating) {
		b -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY BAT COL");
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
	} else if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && player.isCheating && !BUTTON_HELD(BUTTON_A)) {
		b -> active = 0;
        player.isPushedBack = 1;
        mgba_printf("HIT BY BAT COL");
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
	} else if (collision(b -> col, b -> row, b -> width, b -> height, player.col, player.row, player.width, player.height) && b -> active && !player.isDashing && player.isCheating && BUTTON_HELD(BUTTON_A)) {
        b -> active = 0;
        mgba_printf("KILL BAT COL");
        // player.aniState = 1;
        // player.curFrame = 1;
        // frameCounter = 2;
    }

    if (b -> col <= 0) {
        b -> active = 0;
        // b -> col = ENEMY_START_DISTANCE;
        b -> col = hOff + SCREENWIDTH;
    }
}

void attackBatsCol() {

    int skipFrames = 1800;

    if (batTime2 % skipFrames == 0) {
        // mgba_printf("spawn bat col");
        for (int i = 0; i < BATCOLCOUNT; i++) {
		    if (batCol[i].active == 0) {
                batCol[i].active = 1;
                if (batColRandomizer == 0) {
                    batCol[i].row = 0;
                    batColRandomizer = 1;
                } else if (batColRandomizer == 1) {
                    batCol[i].row = 60;
                    batColRandomizer = 0;
                }
                batCol[i].col = BATCOL_START_DISTANCE;
                batTime2 = 0;
			    break;
		    }
	    }
    }

    batTime2++;
}

void drawBatsCol(ANISPRITE* b) {

    if (b -> active) {
        shadowOAM[slotOAM].attr0 = (b -> row & ROWMASK) | ATTR0_4BPP | ATTR0_TALL;
	    shadowOAM[slotOAM].attr1 = (b -> col & COLMASK) | ATTR1_LARGE;
	    shadowOAM[slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(28, 0 + b -> curFrame * 8);
    } else {
        shadowOAM[slotOAM].attr0 |= ATTR0_HIDE;
    }
    slotOAM++;
}

void animateBatsCol() {

    for (int i = 0; i < BATCOLCOUNT; i++) {
        batCol[i].aniCounter++;
        if(batCol[i].aniCounter % 12 == 0) {
            batCol[i].curFrame++;
        }
        if (batCol[i].curFrame >= 2) {
            batCol[i].curFrame = 0;
        }
    }
}

// GUIDING LIGHT

void initLight() {

    light.active = 0;
    light.row = 110;
    light.col = ENEMY_START_DISTANCE;
    light.width = 32;
    light.height = 32;
    light.rdel = 1;
    light.cdel = 1;
    light.aniCounter = 0;
    light.curFrame = 0;
    light.numFrames = 3;
    light.aniState = SPRITELEFT;
}

void updateLight() {

    guideLight();

    if (light.active) {
        light.col -= light.cdel;
    }

    if (collision(light.col, light.row, light.width, light.height, player.col, player.row, player.width, player.height) && light.active) {
		light.active = 0;
        // light.col = ENEMY_START_DISTANCE;
        light.col = hOff + SCREENWIDTH;
        playSoundB(collectLightSound_data, collectLightSound_length, 0);
        if (player.dashCounter != 3) {
            player.dashCounter++;
        }
	}

    if (light.col <= 0) {
        light.active = 0;
        // light.col = ENEMY_START_DISTANCE;
        light.col = hOff + SCREENWIDTH;
    }

}

void guideLight() {

    int skipFrames = 1200;

    if (lightTime % skipFrames == 0) {
        mgba_printf("spawn light");
		if (light.active == 0) {
            light.active = 1;
            light.row = (rand() % 110) + 20;
            light.col = ENEMY_START_DISTANCE;
            lightTime = 0;
		}
    }

    lightTime++;
}

void drawLight() {

    if (light.active) {
        shadowOAM[50].attr0 = (light.row & ROWMASK) | ATTR0_4BPP | ATTR0_SQUARE;
	    shadowOAM[50].attr1 = (light.col & COLMASK) | ATTR1_SMALL;
	    shadowOAM[50].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(0, 24 + light.curFrame * 2);
    } else {
        shadowOAM[50].attr0 |= ATTR0_HIDE;
    }
}

void animateLight() {

    light.aniCounter++;
    if(light.aniCounter % 12 == 0) {
        light.curFrame++;
    }
    if (light.curFrame >= 2) {
        light.curFrame = 0;
    }
}

// DASH TOKENS

void initDashToken() {

    int dashTokenSpace = 210;

    for (int i = 0; i < DASHTOKENCOUNT; i++) {
        dashToken[i].active = 1;
        dashToken[i].width = 8;
        dashToken[i].height = 8;
        dashToken[i].row = 10;
        dashToken[i].col = dashTokenSpace;

        dashTokenSpace += 8;
    }
}

void drawDashToken() {

    for (int i = 0; i < DASHTOKENCOUNT; i++) {
        if (i < player.dashCounter) {
            shadowOAM[60 + slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(26, 0);
        } else {
            shadowOAM[60 + slotOAM].attr2 |= ATTR0_HIDE;
        }
        shadowOAM[60 + slotOAM].attr0 = (dashToken[i].row & ROWMASK) | ATTR0_4BPP | ATTR0_SQUARE;
	    shadowOAM[60 + slotOAM].attr1 = (dashToken[i].col & COLMASK) | ATTR1_SMALL;

        slotOAM++;
    }
}

// LEVEL TRANSITIONS

void initLevel() {

    // transitionTime = 0;

    level.active = 1;
    level.width = 32;
    level.height = 8;
    level.col = 204;
    level.row = 2;
    level.curFrame = 0;
}

void updateLevel() {

    if (hasWon1 && !hasWon2 && !hasWon3) {
        level.curFrame = 1;
    } else if (hasWon1 && hasWon2 && !hasWon3) {
        level.curFrame = 2;
    }
}

void drawLevel() {
    
    shadowOAM[40 + slotOAM].attr0 = (level.row & ROWMASK) | ATTR0_4BPP | ATTR0_WIDE;
	shadowOAM[40 + slotOAM].attr1 = (level.col & COLMASK) | ATTR1_SMALL;
	shadowOAM[40 + slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(22, level.curFrame * 1);
}

// MONSTER??

void initMonster() {

    monster.col = -60;
    monster.row = player.row;
    monster.width = 64;
    monster.height = 64;
}

void updateMonster() {

    monster.row = player.row - 32;

    if (collision(monster.col, monster.row, monster.width, monster.height, player.col, player.row, player.width, player.height)) {
		goToLose();
	}

    if (hasWon2 && !hasWon3) {
        // inAndOut++;
        if (monster.col >= -10) {
            monster.col -= 0.0005;
        } else if (monster.col < 0) {
            monster.col += 0.0005;
        }
    } else if (hasWon1 && !hasWon2 && !hasWon3) {
        if (monster.col >= -30) {
            monster.col -= 0.0005;
        } else if (monster.col < 0) {
            monster.col += 0.0005;
        }
    }
}

void drawMonster() {

    shadowOAM[70 + slotOAM].attr0 = (monster.row & ROWMASK) | ATTR0_4BPP | ATTR0_SQUARE;
	shadowOAM[70 + slotOAM].attr1 = (monster.col & COLMASK) | ATTR1_LARGE;
	shadowOAM[70 + slotOAM].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(4 + (monster.aniState * 8), 16 + monster.curFrame * 8);
}

void animateMonster() {

    monster.aniCounter++;
    if(monster.aniCounter % 10 == 0) {
        monster.curFrame++;
    }
    if (monster.curFrame >= 2) {
        monster.curFrame = 0;
    }
}