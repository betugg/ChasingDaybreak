// Winning Variables
extern int hasWon1;
extern int hasWon2;
extern int hasWon3;

// ANISPRITE
typedef struct {
    int row;
    int col;
    int rdel;
    int cdel;
    int oldRow;
    int oldCol;
    int width;
    int height;
    int aniCounter;
    int aniState;
    int prevAniState;
    int curFrame;
    int numFrames;
    int isJumping;
    int isFalling;
    int isDoubleJumping;
    int isDashing;
    int isPushedBack;
    int isSliding;
    int isCheating;
    int jumpFrames;
    int doubleJumpFrames;
    int dashFrames;
    int pushBackFrames;
    int dashCounter;
    int livesRemaining;
    int active;
} ANISPRITE;

// GAME MACROS
#define MAX_DASH_COUNT 3
#define MAX_JUMP_HEIGHT 20
#define MAX_DASH_DISTANCE 8
#define MAX_PUSH_BACK_DISTANCE 4
#define BATCOUNT 5
#define BATCOLCOUNT 2
#define SPIDERCOUNT 3
#define DASHTOKENCOUNT 3
#define ENEMY_START_DISTANCE 250
#define BAT_START_DISTANCE 280
#define BATCOL_START_DISTANCE 290
#define MAPHEIGHT 160
#define MAPWIDTH 240

// GAME OBJECTS
ANISPRITE player;
ANISPRITE light;
ANISPRITE level;
ANISPRITE monster;
ANISPRITE bat[BATCOUNT];
ANISPRITE batCol[BATCOLCOUNT];
ANISPRITE spider[SPIDERCOUNT];
ANISPRITE dashToken[DASHTOKENCOUNT];

// VARIABLES
extern int gameTime;
extern int transitioning;
extern int vOff1;
extern int hOff1;
extern int vOff;
extern int hOff;
extern int time_s;
extern int time_cs;

// Sprite positions
enum {SPRITELEFT, SPRITECROSS1, SPRITERIGHT, SPRITECROSS2};

// TILE MAP STUFF

#define UNDERLINE_LENGTH 5