#include "finalProjectLib.h"
#include "mode0.h"
#include "sound.h"

// Time Variables
int time_s;
int time_cs;

void setupInterrupts() {

	REG_IME = 0;

    REG_IE |= INT_VBLANK;
    // REG_IE |= INT_TM2;

    REG_DISPSTAT |= INT_VBLANK_ENABLE;

    // REG_TM2CNT = 0;
    // REG_TM2D = 65536 - 60;
    // REG_TM2CNT = TM_FREQ_1024 | TIMER_ON | TM_IRQ;

    REG_INTERRUPT = interruptHandler;
	REG_IME = 1;
}

void setupInterruptsWithTimers() {

	REG_IME = 0;

    REG_IE |= INT_VBLANK;
    REG_IE |= INT_TM2;

    REG_DISPSTAT |= INT_VBLANK_ENABLE;

    REG_TM2CNT = 0;
    REG_TM2D = 65536 - 60;
    REG_TM2CNT = TM_FREQ_1024 | TIMER_ON | TM_IRQ;

    REG_INTERRUPT = interruptHandler;
	REG_IME = 1;
}

void interruptHandler() {

	REG_IME = 0;

	if(REG_IF & INT_VBLANK) {
        if (soundA.isPlaying) {

            soundA.vBlankCount = soundA.vBlankCount + 1;
            if (soundA.vBlankCount > soundA.duration) {
                if (soundA.loops) {
                    playSoundA(soundA.data, soundA.length, soundA.loops);
                } else {
                    soundA.isPlaying = 0;
                    dma[1].cnt = 0;
                    REG_TM0CNT = TIMER_OFF;
                }
            }
        }

        if (soundB.isPlaying) {

            soundB.vBlankCount = soundB.vBlankCount + 1;
            if (soundB.vBlankCount > soundB.duration) {
                if (soundB.loops) {
                    playSoundB(soundB.data, soundB.length, soundB.loops);
                } else {
                    soundB.isPlaying = 0;
                    dma[2].cnt = 0;
                    REG_TM1CNT = TIMER_OFF;
                }
            }
		}

		REG_IF = INT_VBLANK;

	} else if (REG_IF & INT_TM2) {
        time_cs++;
        // mgba_printf("time_cs: %d", time_cs);
        if (time_cs > 100) {
            time_cs = 0;
            time_s++;
            // mgba_printf("time_s: %d", time_s);
        }
    }

    REG_IF = REG_IF;

	REG_IME = 1;
}