/*
 * Copyright (C) 2017 https://www.brobwind.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"


//            PF7  PB15  PB14  PB11  PB12   PB2   PB1   PA6   PA7   PF0   FB9   PB8   PA5   PF1   PB4  PB13  PB10   PB0   PA4   PB6   PB5   PB7
//           [ 0]  [ 1]  [ 2]  [ 3]  [ 4]  [ 5]  [ 6]  [ 7]  [ 8]  [ 9]  [10]  [11]  [12]  [13]  [14]  [15]  [16]  [17]  [18]  [19]  [20]  [21]
// --------+-----------------------------------------------------------------------------------------------------------------------------------
// PF7 [ 0]        0, 3  0, 5  0, 7  0, 9  0,11  0,13  0,15  0,17  0,19  0,21  0,23  0,25  0,27  0,29  0,31  0,33  0,35  0,37  0,39  0,41  0,43
// PB15[ 1]  0, 1        1, 4  1, 6  1, 8  1,10  1,12  1,14  1,16  1,18  1,20  1,22  1,24  1,26  1,28  1,30  1,32  1,34  1,36  1,38  1,40  1,42
// PB14[ 2]  1, 0  1, 2        1, 7  1, 9  1,11  1,13  1,15  1,17  1,19  1,21  1,23  1,25  1,27  1,29  1,31  1,33  1,35  1,37  1,39  1,41  1,43
// PB11[ 3]  1, 1  1, 3  1, 5        2, 8  2,10  2,12  2,14  2,16  2,18  2,20  2,22  2,24  2,26  2,28  2,30  2,32  2,34  2,36  2,38  2,40  2,42
// PB12[ 4]  2, 0  2, 2  2, 4  2, 6        2,11  2,13  2,15  2,17  2,19  2,21  2,23  2,25  2,27  2,29  2,31  2,33  2,35  2,37  2,39  2,41  2,43
// PB2 [ 5]  2, 1  2, 3  2, 5  2, 7  2, 9        3,12  3,14  3,16  3,18  3,20  3,22  3,24  3,26  3,28  3,30  3,32  3,34  3,36  3,38  3,40  3,42
// PB1 [ 6]  3, 0  3, 2  3, 4  3, 6  3, 8  3,10        3,15  3,17  3,19  3,21  3,23  3,25  3,27  3,29  3,31  3,33  3,35  3,37  3,39  3,41  3,43
// PA6 [ 7]  3, 1  3, 3  3, 5  3, 7  3, 9  3,11  3,13        4,16  4,18  4,20  4,22  4,24  4,26  4,28  4,30  4,32  4,34  4,36  4,38  4,40  4,42
// PA7 [ 8]  4, 0  4, 2  4, 4  4, 6  4, 8  4,10  4,12  4,14        4,19  4,21  4,23  4,25  4,27  4,29  4,31  4,33  4,35  4,37  4,39  4,41  4,43
// PF0 [ 9]  4, 1  4, 3  4, 5  4, 7  4, 9  4,11  4,13  4,15  4,17        5,20  5,22  5,24  5,26  5,28  5,30  5,32  5,34  5,36  5,38  5,40  5,42
// FB9 [10]  5, 0  5, 2  5, 4  5, 6  5, 8  5,10  5,12  5,14  5,16  5,18        5,23  5,25  5,27  5,29  5,31  5,33  5,35  5,37  5,39  5,41  5,43
// PB8 [11]  5, 1  5, 3  5, 5  5, 7  5, 9  5,11  5,13  5,15  5,17  5,19  5,21        6,24  6,26  6,28  6,30  6,32  6,34  6,36  6,38  6,40  6,42
// PA5 [12]  6, 0  6, 2  6, 4  6, 6  6, 8  6,10  6,12  6,14  6,16  6,18  6,20  6,22        6,27  6,29  6,31  6,33  6,35  6,37  6,39  6,41  6,43
// PF1 [13]  6, 1  6, 3  6, 5  6, 7  6, 9  6,11  6,13  6,15  6,17  6,19  6,21  6,23  6,25        7,28  7,30  7,32  7,34  7,36  7,38  7,40  7,42
// PB4 [14]  7, 0  7, 2  7, 4  7, 6  7, 8  7,10  7,12  7,14  7,16  7,18  7,20  7,22  7,24  7,26        7,31  7,33  7,35  7,37  7,39  7,41  7,43
// PB13[15]  7, 1  7, 3  7, 5  7, 7  7, 9  7,11  7,13  7,15  7,17  7,19  7,21  7,23  7,25  7,27  7,29        8,32  8,34  8,36  8,38  8,40  8,42
// PB10[16]  8, 0  8, 2  8, 4  8, 6  8, 8  8,10  8,12  8,14  8,16  8,18  8,20  8,22  8,24  8,26  8,28  8,30        8,35  8,37  8,39  8,41  8,43
// PB0 [17]  8, 1  8, 3  8, 5  8, 7  8, 9  8,11  8,13  8,15  8,17  8,19  8,21  8,23  8,25  8,27  8,29  8,31  8,33        9,36  9,38  9,40  9,42
// PA4 [18]  9, 0  9, 2  9, 4  9, 6  9, 8  9,10  9,12  9,14  9,16  9,18  9,20  9,22  9,24  9,26  9,28  9,30  9,32  9,34        9,39  9,41  9,43
// PB6 [19]  9, 1  9, 3  9, 5  9, 7  9, 9  9,11  9,13  9,15  9,17  9,19  9,21  9,23  9,25  9,27  9,29  9,31  9,33  9,35  9,37       10,40 10,43
// PB5 [20] 10, 0 10, 2 10, 4 10, 6 10, 8 10,10 10,12 10,14 10,16 10,18 10,20 10,22 10,24 10,26 10,28 10,30 10,32 10,34 10,36 10,38       10,42
// PB7 [21] 10, 1 10, 3 10, 5 10, 7 10, 9 10,11 10,13 10,15 10,17 10,19 10,21 10,23 10,25 10,27 10,29 10,31 10,33 10,35 10,37 10,39 10,41
// PA10[22]  0, 0  0, 2  0, 4  0, 6  0, 8  0,10  0,12  0,14  0,16  0,18  0,20  0,22  0,24  0,26  0,28  0,30  0,32  0,34  0,36  0,38  0,40  0,42


/* For external event: key event, USB plug in/out event */
#define EVENT_EXT						1
event_source_t ext_event;

/* For LED display vsync event @60Hz */
#define EVENT_VSYNC						1
event_source_t vsync_event;

static void keyExtCallback(EXTDriver *extp, expchannel_t channel)
{
	(void)extp, (void)channel;

	chSysLockFromISR();
	chEvtBroadcastFlagsI(&ext_event, EVENT_EXT);
	chSysUnlockFromISR();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA,
			keyExtCallback},		    /* 0 */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},		/* 5 */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},		/* 10 */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},		/* 15 */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

/* 1000 @  1.3ms */
/* 2000 @  2.5ms */
/* 3000 @  3.8ms */
/* 9000 @ 11.4ms */
#define LBM									(1000)
#define LBM_M0								(1)
#define LBM_M1								(2)
#define LBM_M2								(4)
#define LBM_M3								(8)

static uint8_t lbmMu = LBM_M2;

/* Led brightness correction table */
const uint32_t LBT[22] = {
	LBM *  0 / 21, LBM *  1 / 21, LBM *  2 / 21, LBM *  3 / 21,
	LBM *  4 / 21, LBM *  5 / 21, LBM *  6 / 21, LBM *  7 / 21,
	LBM *  8 / 21, LBM *  9 / 21, LBM * 10 / 21, LBM * 11 / 21,
	LBM * 12 / 21, LBM * 13 / 21, LBM * 14 / 21, LBM * 15 / 21,
	LBM * 16 / 21, LBM * 17 / 21, LBM * 18 / 21, LBM * 19 / 21,
	LBM * 20 / 21, LBM * 21 / 21
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(384)

static const ShellCommand commands[] = {
	{NULL, NULL}
};

static const ShellConfig shellcfg = {
	(BaseSequentialStream *)&SD2,
	commands
};

/*===========================================================================*/
/* LED-44x11 animation related.                                                 */
/*===========================================================================*/

#define LED_FB_WB						6
#define LED_FB_HB						11
#define LED_FB_SIZE						(LED_FB_WB * LED_FB_HB)
#define LED_DIS_WIDTH					44
#define LED_DIS_HEIGH					11

struct AnimCtx;

typedef struct AnimCtx *(*Run)(struct AnimCtx *A, uint8_t *fb, systime_t time);

typedef struct AnimCtx {
	systime_t start;
	systime_t duration;
	uint32_t loop;
	struct 	AnimCtx *next;
	Run run;
} AnimCtx;

typedef struct NoneAnimCtx {
	AnimCtx v;
	int8_t type;
} __attribute__((packed)) NoneAnimCtx;

static AnimCtx __attribute__((unused)) * RunNone(AnimCtx *A, uint8_t *fb, systime_t time);

static NoneAnimCtx anNoneAny = {
	{ 0, TIME_INFINITE, 0, NULL, RunNone }, 0xff
};

static bool shouldTerminateAnimation(AnimCtx *A, systime_t time)
{
	return A->duration != TIME_INFINITE && A->start + A->duration < time;
}

static AnimCtx __attribute__((unused)) * RunNone(AnimCtx *A, uint8_t *fb, systime_t time)
{
	(void)time; NoneAnimCtx *nA = (NoneAnimCtx *)A;

	if (shouldTerminateAnimation(A, time)) {
		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	switch (nA->type) {
	case 0:
		memset(fb, 0x00, LED_FB_SIZE);
		break;
	case 1:
		memset(fb, 0xff, LED_FB_SIZE);
		break;
	}

	return A;
}

static AnimCtx __attribute__((unused)) * RunRand(AnimCtx *A, uint8_t *fb, systime_t time)
{
	(void)time;
	uint8_t i, j;

	if (shouldTerminateAnimation(A, time)) {
		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	memset(fb, 0x00, LED_FB_SIZE);
	for (i = 0; i < LED_DIS_HEIGH; i++) {
		for (j = 0; j < LED_DIS_WIDTH; j++) {
			uint8_t val = rand() % 2;
			fb[i * LED_FB_WB + j / 8] |= val << (7 - j % 8);
		}
	}

	return A;
}

typedef struct {
	AnimCtx v;
	const uint8_t *fb;
	struct {
		int8_t dir:1; // Left, Up: 0, Right, Down: !0
		int8_t rev:1;
	} u;
	int8_t frm;
} __attribute__((packed)) RollAnimCtx;

static AnimCtx __attribute__((unused)) * RunHr(AnimCtx *A, uint8_t *fb, systime_t time)
{
	RollAnimCtx *rA = (RollAnimCtx *)A;
	uint8_t TOTAL = !rA->u.rev ? 1 + 44 + 43 : 44 + 44;
	uint8_t frm = (time - A->start) / (A->duration / TOTAL);

	if (rA->frm == frm) return A;

	if (frm >= TOTAL) {
		if (A->loop > 1 || A->loop == 0) {
			if (A->loop > 1)  A->loop--;
			A->start = time; rA->frm = 0;

			return A;
		}

		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	memset(fb, 0x00, LED_FB_SIZE);

	if (!rA->u.rev) { // 1 + 44 + 43
		int8_t idx, row, pos, lpos, rpos;

		if (!rA->u.dir) { // Roll left <-
			pos = frm > LED_DIS_WIDTH ? frm - LED_DIS_WIDTH : (LED_DIS_WIDTH - frm);
			for (idx = pos; idx < LED_DIS_WIDTH; idx += 2) { // pos: 0 ~ LED_DIS_WIDTH - 1
				lpos = idx;
				if (pos == LED_DIS_WIDTH - frm) {
					rpos = (idx - pos) / 2;
				} else {
					rpos = (idx - pos) / 2 + pos;
				}

				for (row = 0; row < LED_DIS_HEIGH; row++) {
					if (rA->fb[row * LED_FB_WB + rpos / 8] & (1 << (7 - rpos % 8))) {
						fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
					}
				}
			}
			for (idx = 0; idx < pos && pos != LED_DIS_WIDTH - frm; idx++) {
				for (row = 0; row < LED_DIS_HEIGH; row++) {
					fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
				}
			}
		} else { // Roll right ->
			pos = frm > LED_DIS_WIDTH ? (LED_DIS_WIDTH - 1) - (frm - LED_DIS_WIDTH) : frm - 1;
			for (idx = pos; idx >= 0 && idx < LED_DIS_WIDTH; idx -= 2) {
				lpos = idx;
				if (pos == frm - 1) {
					rpos = LED_DIS_WIDTH - 1 - (pos - idx) / 2;
				} else {
					rpos = pos - (pos - idx) / 2;
				}
				for (row = 0; row < LED_DIS_HEIGH; row++) {
					if (rA->fb[row * LED_FB_WB + rpos / 8] & (1 << (7 - rpos % 8))) {
						fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
					}
				}
			}
			for (idx = pos + 1; idx < LED_DIS_WIDTH && pos != frm - 1; idx++) {
				for (row = 0; row < LED_DIS_HEIGH; row++) {
					fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
				}
			}
		}
	} else if (!!rA->u.rev) { // 44 + 44
		int8_t idx, row, pos, lpos, rpos;

		if (!rA->u.dir) { // Roll left <-
			pos = frm >= LED_DIS_WIDTH ? LED_DIS_WIDTH - 2 - (frm - LED_DIS_WIDTH) : frm;
			for (idx = pos; idx >= 0; idx -= 2) {
				lpos = idx;
				if (pos == frm) {
					rpos = pos - (pos - idx) / 2;
				} else {
					rpos = LED_DIS_WIDTH - 1 - (pos - idx) / 2;
				}

				for (row = 0; row < LED_DIS_HEIGH; row++) {
					if (rA->fb[row * LED_FB_WB + rpos / 8] & (1 << (7 - rpos % 8))) {
						fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
					}
				}
			}
			for (idx = pos + 1; idx < LED_DIS_WIDTH && pos == frm; idx++) {
				for (row = 0; row < LED_DIS_HEIGH; row++) {
					fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
				}
			}
		} else { // Roll right ->
			pos = frm >= LED_DIS_WIDTH ? frm - LED_DIS_WIDTH + 1: LED_DIS_WIDTH - 1 - frm;
			for (idx = pos; idx < LED_DIS_WIDTH; idx += 2) {
				lpos = idx;
				if (pos != frm - LED_DIS_WIDTH + 1) {
					rpos = pos + (idx - pos) / 2;
				} else {
					rpos = (idx - pos) / 2;
				}

				for (row = 0; row < LED_DIS_HEIGH; row++) {
					if (rA->fb[row * LED_FB_WB + rpos / 8] & (1 << (7 - rpos % 8))) {
						fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
					}
				}
			}

			for (idx = 0; idx < pos && pos != frm - LED_DIS_WIDTH + 1; idx++) {
				for (row = 0; row < LED_DIS_HEIGH; row++) {
					fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
				}
			}
		}
	}

	rA->frm = frm; return A;
}

static AnimCtx __attribute__((unused)) * RunVr(AnimCtx *A, uint8_t *fb, systime_t time)
{
	RollAnimCtx *rA = (RollAnimCtx *)A;
	const uint8_t TOTAL = !rA->u.rev ? 1 + 11 + 10 : 11 + 11;
	uint8_t frm = (time - A->start) / (A->duration / TOTAL);

	if (rA->frm == frm) return A;

	if (frm >= TOTAL) {
		if (A->loop > 1 || A->loop == 0) {
			if (A->loop > 1)  A->loop--;
			A->start = time; rA->frm = 0;

			return A;
		}

		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	memset(fb, 0x00, LED_FB_SIZE);

	if (!rA->u.rev) {
		int8_t idx, pos, cpos;

		if (!rA->u.dir) { // Roll up
			pos = frm > LED_DIS_HEIGH ? frm - LED_DIS_HEIGH : (LED_DIS_HEIGH - frm);
			for (idx = pos; idx < LED_DIS_HEIGH; idx += 2) { // pos: (LED_DIS_HEIGH - 1) ~ 0
				if (pos == LED_DIS_HEIGH - frm) {
					cpos = (idx - pos) / 2;
				} else {
					cpos = (idx - pos) / 2 + pos;
				}

				memcpy(&fb[idx * LED_FB_WB], &rA->fb[cpos * LED_FB_WB], LED_FB_WB);
			}
			for (idx = 0; idx < pos && pos != LED_DIS_HEIGH - frm; idx++) {
				memcpy(&fb[idx * LED_FB_WB], &rA->fb[idx * LED_FB_WB], LED_FB_WB);
			}
		} else { // Roll down
			pos = frm > LED_DIS_HEIGH ? (LED_DIS_HEIGH - 1) - (frm - LED_DIS_HEIGH) : frm - 1;
			for (idx = pos; idx >= 0 && idx < LED_DIS_HEIGH ; idx -= 2) {
				if (pos == frm - 1) {
					cpos = LED_DIS_HEIGH - 1 - (pos - idx) / 2;
				} else {
					cpos = pos - (pos - idx) / 2;
				}
				memcpy(&fb[idx * LED_FB_WB], &rA->fb[cpos * LED_FB_WB], LED_FB_WB);
			}
			for (idx = pos + 1; idx < LED_DIS_HEIGH && pos != frm - 1; idx++) {
				memcpy(&fb[idx * LED_FB_WB], &rA->fb[idx * LED_FB_WB], LED_FB_WB);
			}
		}
	} else if (!!rA->u.rev) {
		int8_t idx, pos, cpos;

		if (!rA->u.dir) { // Roll up
			pos = frm >= LED_DIS_HEIGH ? LED_DIS_HEIGH - 2 - (frm - LED_DIS_HEIGH) : frm;
			for (idx = pos; idx >= 0; idx -= 2) {
				if (pos == frm) {
					cpos = pos - (pos - idx) / 2;
				} else {
					cpos = LED_DIS_HEIGH - 1 - (pos - idx) / 2;
				}

				memcpy(&fb[idx * LED_FB_WB], &rA->fb[cpos * LED_FB_WB], LED_FB_WB);
			}
			for (idx = pos + 1; idx < LED_DIS_HEIGH && pos == frm; idx++) {
				memcpy(&fb[idx * LED_FB_WB], &rA->fb[idx * LED_FB_WB], LED_FB_WB);
			}
		} else { // Roll down
			pos = frm >= LED_DIS_HEIGH ? frm - LED_DIS_HEIGH + 1: LED_DIS_HEIGH - 1 - frm;
			for (idx = pos; idx < LED_DIS_HEIGH; idx += 2) {
				if (pos != frm - LED_DIS_HEIGH + 1) {
					cpos = pos + (idx - pos) / 2;
				} else {
					cpos = (idx - pos) / 2;
				}

				memcpy(&fb[idx * LED_FB_WB], &rA->fb[cpos * LED_FB_WB], LED_FB_WB);
			}

			for (idx = 0; idx < pos && pos != frm - LED_DIS_HEIGH + 1; idx++) {
				memcpy(&fb[idx * LED_FB_WB], &rA->fb[idx * LED_FB_WB], LED_FB_WB);
			}
		}
	}

	rA->frm = frm; return A;
}

typedef struct {
	AnimCtx v;
	const uint8_t *fb;
	uint8_t rev;
	uint8_t frm;
} __attribute__((packed)) RevealAnimCtx;

static AnimCtx __attribute__((unused)) * RunReveal(AnimCtx *A, uint8_t *fb, systime_t time)
{
	RevealAnimCtx *rA = (RevealAnimCtx *)A;
	const uint8_t TOTAL = LED_DIS_WIDTH / 2 + LED_DIS_WIDTH % 2 + 1;
	uint8_t frm = (time - A->start) / (A->duration / TOTAL);

	if (rA->frm == frm) return A;

	if (frm >= TOTAL) {
		if (A->loop > 1 || A->loop == 0) {
			if (A->loop > 1) A->loop--;
			A->start = time; rA->frm = 0;

			return A;
		}

		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	memset(fb, 0x00, LED_FB_SIZE);

	if (!rA->rev) {
		int8_t idx, row, lpos, rpos;

		lpos = LED_DIS_WIDTH / 2 + (LED_DIS_WIDTH % 2 ? 1 : -1) - frm;
		rpos = LED_DIS_WIDTH / 2 + (LED_DIS_WIDTH % 2 ? 1 :  0) + frm;
		for (idx = lpos + 1; idx < rpos; idx++) {
			for (row = 0; row < LED_DIS_HEIGH; row++) {
				fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
			}
		}

		for (row = 0; row < LED_DIS_HEIGH && lpos >= 0; row++) {
			fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
			if (lpos == rpos) continue;

			fb[row * LED_FB_WB + rpos / 8] |= 1 << (7 - rpos % 8);
		}
	} else {
		int8_t idx, row, lpos, rpos;

		lpos = LED_DIS_WIDTH / 2 + (LED_DIS_WIDTH % 2 ? 1 : -1) - frm;
		rpos = LED_DIS_WIDTH / 2 + (LED_DIS_WIDTH % 2 ? 1 :  0) + frm;
		for (idx = 0; idx < lpos; idx++) {
			for (row = 0; row < LED_DIS_HEIGH; row++) {
				fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
			}
		}

		for (row = 0; row < LED_DIS_HEIGH && lpos >= 0; row++) {
			fb[row * LED_FB_WB + lpos / 8] |= 1 << (7 - lpos % 8);
			if (lpos == rpos) continue;

			fb[row * LED_FB_WB + rpos / 8] |= 1 << (7 - rpos % 8);
		}

		for (idx = rpos + 1; idx < LED_DIS_WIDTH; idx++) {
			for (row = 0; row < LED_DIS_HEIGH; row++) {
				fb[row * LED_FB_WB + idx / 8] |= rA->fb[row * LED_FB_WB + idx / 8] & (1 << (7 - idx % 8));
			}
		}
	}

	rA->frm = frm; return A;
}

typedef struct {
	AnimCtx v;
	const uint8_t *fb;
	uint16_t len;
	uint16_t frm;
} __attribute__((packed)) LessAnimCtx;

static AnimCtx __attribute__((unused)) * RunLess(AnimCtx *A, uint8_t *fb, systime_t time)
{
	LessAnimCtx *lA = (LessAnimCtx *)A;
	uint16_t TOTAL = LED_DIS_WIDTH + lA->len;
	uint16_t frm = (time - A->start) / (A->duration / TOTAL);

	if (lA->frm == frm) return A;

	if (frm >= TOTAL) {
		if (A->loop > 1 || A->loop == 0) {
			if (A->loop > 1) A->loop--;
			A->start = time; lA->frm = 0;

			return A;
		}

		return A->next != NULL ? (A->next->start = time, A->next) : (AnimCtx *)&anNoneAny;
	}

	memset(fb, 0x00, LED_FB_SIZE);

	int16_t idx, row, pos, rpos;

	pos = frm >= LED_DIS_WIDTH ? 0 : LED_DIS_WIDTH - 1 - frm;
	for (idx = pos; idx < LED_DIS_WIDTH - 1; idx++) {
		if (pos == LED_DIS_WIDTH - 1 - frm) {
			rpos = idx - pos;
		} else {
			rpos = frm + (idx - pos) - (LED_DIS_WIDTH - 1);
		}
		for (row = 0; row < LED_DIS_HEIGH && rpos < lA->len; row++) {
			if (lA->fb[row * ((lA->len + 7) / 8) + rpos / 8] & (1 << (7 - rpos % 8))) {
				fb[row * LED_FB_WB + idx / 8] |= 1 << (7 - idx % 8);
			}
		}
	}

	lA->frm = frm; return A;
}

static const uint8_t cWelcome[LED_FB_SIZE] = {
	0x04, 0x10, 0xbc, 0x10, 0x12, 0x00,
	0xf7, 0xeb, 0x25, 0x11, 0x12, 0x00,
	0x14, 0x22, 0x24, 0x91, 0x53, 0xf0,
	0x99, 0x22, 0x24, 0x92, 0x54, 0x80,
	0x91, 0x1a, 0x24, 0x10, 0x54, 0x40,
	0x51, 0x0a, 0x27, 0xff, 0xd0, 0x00,
	0x51, 0x0a, 0xa4, 0x44, 0x57, 0xf0,
	0x22, 0x8f, 0x2c, 0x44, 0x54, 0x90,
	0x22, 0x88, 0x20, 0x44, 0x94, 0x90,
	0x54, 0x48, 0x20, 0x84, 0x94, 0x90,
	0x88, 0x37, 0xff, 0x07, 0x97, 0xf0,
};

static const uint8_t cHappyNy[LED_FB_SIZE] = {
	0x20, 0x68, 0x00, 0x88, 0x00, 0xe0,
	0xfb, 0x8f, 0xfc, 0xbf, 0x3f, 0x00,
	0x52, 0x08, 0x42, 0x89, 0x20, 0x00,
	0x52, 0x10, 0x42, 0xc9, 0x22, 0x00,
	0xfb, 0xe7, 0xfa, 0x89, 0x22, 0x00,
	0x22, 0x44, 0x42, 0x89, 0x7f, 0xf0,
	0xfa, 0x44, 0x42, 0xbf, 0x82, 0x00,
	0x22, 0x5f, 0xfc, 0x88, 0x02, 0x00,
	0x72, 0x40, 0x40, 0x94, 0x12, 0x40,
	0xaa, 0x40, 0x40, 0xa2, 0x22, 0x20,
	0x24, 0x40, 0x40, 0xc1, 0xc6, 0x10,
};

static const uint8_t cLessDemo[6 * 11] = {
	0x20, 0x68, 0x00, 0x88, 0x00, 0xe0,
	0xfb, 0x8f, 0xfc, 0xbf, 0x3f, 0x00,
	0x52, 0x08, 0x42, 0x89, 0x20, 0x00,
	0x52, 0x10, 0x42, 0xc9, 0x22, 0x00,
	0xfb, 0xe7, 0xfa, 0x89, 0x22, 0x00,
	0x22, 0x44, 0x42, 0x89, 0x7f, 0xf0,
	0xfa, 0x44, 0x42, 0xbf, 0x82, 0x00,
	0x22, 0x5f, 0xfc, 0x88, 0x02, 0x00,
	0x72, 0x40, 0x40, 0x94, 0x12, 0x40,
	0xaa, 0x40, 0x40, 0xa2, 0x22, 0x20,
	0x24, 0x40, 0x40, 0xc1, 0xc6, 0x10,
};

static AnimCtx *A = (AnimCtx *)&anNoneAny;
static mutex_t anMtx;

/*
 * LED-44x11 worker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waLedAnim, 320);
static __attribute__((noreturn)) THD_FUNCTION(LedAnim, arg) {
	(void)arg;
	chRegSetThreadName("LED-44x11-AN");

	RollAnimCtx A_now_B = {
		{ 0, MS2ST(2000), 1, NULL, RunVr }, cWelcome, { 1, 0 }, -1
	};

	NoneAnimCtx A_now_M = {
		{ 0, MS2ST(3000), 1, NULL, RunNone }, 0xff
	};

	RollAnimCtx A_now_E = {
		{ 0, MS2ST(2000), 1, NULL, RunVr }, cWelcome, { 1, 1 }, -1
	};

	NoneAnimCtx A_now_P = {
		{ 0, MS2ST(2000), 1, NULL, RunNone }, 0xff
	};

	RevealAnimCtx B_rev_B = {
		{ 0, MS2ST(2000), 1, NULL, RunReveal }, cHappyNy, 0, -1
	};

	NoneAnimCtx B_rev_M = {
		{ 0, MS2ST(3000), 1, NULL, RunNone }, 0xff
	};

	RevealAnimCtx B_rev_E = {
		{ 0, MS2ST(2000), 1, NULL, RunReveal }, cHappyNy, 1, -1
	};

	NoneAnimCtx B_rev_P = {
		{ 0, MS2ST(2000), 1, NULL, RunNone }, 0xff
	};

	LessAnimCtx C_les_B = {
		{ 0, MS2ST(8000), 1, NULL, RunLess }, cLessDemo, 44, -1
	};

	AnimCtx D_rnd_A = {
		0, MS2ST(2000), 1, NULL, RunRand
	};

	A_now_B.v.start = chVTGetSystemTime();
	A_now_B.v.next = (AnimCtx *)&A_now_M;
	A_now_M.v.next = (AnimCtx *)&A_now_E;
	A_now_E.v.next = (AnimCtx *)&A_now_P;
	A_now_P.v.next = (AnimCtx *)&B_rev_B;

	B_rev_B.v.next = (AnimCtx *)&B_rev_M;
	B_rev_M.v.next = (AnimCtx *)&B_rev_E;
	B_rev_E.v.next = (AnimCtx *)&B_rev_P;
	B_rev_P.v.next = (AnimCtx *)&C_les_B;

	C_les_B.v.next = (AnimCtx *)&D_rnd_A;

	D_rnd_A.next = (AnimCtx *)&A_now_B;

	chMtxLock(&anMtx);
	A = (AnimCtx *)&A_now_B;
	chMtxUnlock(&anMtx);

	event_listener_t el;
	chEvtRegister(&ext_event, &el, EVENT_EXT);

	while (TRUE) {
		chEvtWaitAnyTimeout(ALL_EVENTS, TIME_INFINITE);
		switch (lbmMu) {
			case LBM_M0: lbmMu = LBM_M3; break;
			case LBM_M1: lbmMu = LBM_M0; break;
			case LBM_M2: lbmMu = LBM_M1; break;
			case LBM_M3: lbmMu = LBM_M2; break;
		}
	}

	chEvtUnregister(&ext_event, &el);
}

/*===========================================================================*/
/* LED-44x11 worker related.                                                 */
/*===========================================================================*/

#define L44X11_FB_NUM					2

#define L44X11_FB_IDLE					0
#define L44X11_FB_PROD					1
#define L44X11_FB_CONS					2
#define L44X11_FB_DONE					3

// GPIOA: 4, 5, 6, 7, 10
#define L44X11_GA_OTMASK	(\
	(0x01UL <<  4) | (0x01UL <<  5) | (0x01UL <<  6) | (0x01UL <<  7) | \
	(0x01UL << 10))

// GPIOB: 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#define L44X11_GB_OTMASK	(\
	(0x01UL <<  0) | (0x01UL <<  1) | (0x03UL <<  2) | (0x01UL <<  4) | \
	(0x01UL <<  5) | (0x01UL <<  6) | (0x03UL <<  7) | (0x01UL <<  8) | \
	(0x01UL <<  9) | (0x01UL << 10) | (0x03UL << 11) | (0x01UL << 12) | \
	(0x01UL << 13) | (0x01UL << 14) | (0x03UL << 15))

// GPIOF: 0, 1, 7
#define L44X11_GF_OTMASK	(\
	(0x01UL <<  0) | (0x01UL <<  1) | (0x01UL <<  7))


static const struct LedPinCfg {
	Gd32_gpio_t *port;
	uint8_t pin;
} __attribute__((packed)) ledPinCfg[] = {
	{ GPIOF,  7 }, { GPIOB, 15 }, { GPIOB, 14 }, { GPIOB, 11 }, { GPIOB, 12 },
	{ GPIOB,  2 }, { GPIOB,  1 }, { GPIOA,  6 }, { GPIOA,  7 }, { GPIOF,  0 },
	{ GPIOB,  9 }, { GPIOB,  8 }, { GPIOA,  5 }, { GPIOF,  1 }, { GPIOB,  4 },
	{ GPIOB, 13 }, { GPIOB, 10 }, { GPIOB,  0 }, { GPIOA,  4 }, { GPIOB,  6 },
	{ GPIOB,  5 }, { GPIOB,  7 }, { GPIOA, 10 }
};

struct LedFbWo {
	uint16_t Ga;
	uint16_t Gb;
	uint16_t Gf;
	uint16_t Co;
};

static struct LedFbCtx {
	mutex_t mtx;
	uint8_t state[L44X11_FB_NUM];
	struct LedFbWo fb[L44X11_FB_NUM][23];
} ledFbCtx;

static void fbToPinCtl(const uint8_t *fb, struct LedFbWo *fbWo) {
	uint8_t i, j, jmp;

	for (i = 0; i < 23; i++) {
		fbWo[i].Ga = fbWo[i].Gb = fbWo[i].Gf = fbWo[i].Co = 0;

		for (j = 0, jmp = 0; j < 22; j++) {
			uint8_t row, col;
			uint16_t *p;

			if (ledPinCfg[j].port == GPIOA) {
				p = &fbWo[i].Ga;
			} else if (ledPinCfg[j].port == GPIOB) {
				p = &fbWo[i].Gb;
			} else {
				p = &fbWo[i].Gf;
			}

			if (i == j) {
				jmp = 1;
				*p |= 0x01 << ledPinCfg[j].pin; // Output open-drain mode
				continue;
			}

			row = i % 2 ? (i / 2 + jmp) : (i / 2);
			col = i % 2 ? (j * 2 + 1 - jmp) : (j * 2 + jmp);

			row = row == 11 ? 0 : row;

			if (i == 19 && j == 21) col = 43;
			if (i == 20 && j == 21) col = 42;

			if (fb[row * 6 + col / 8] & (1 << (7 - col % 8))) {
				fbWo[i].Co++;
				continue;
			}

			*p |= 0x01 << ledPinCfg[j].pin; // Output open-drain mode
		}

		fbWo[i].Ga |= 0x01 << ledPinCfg[22].pin; // output open-drain mode
	}
}

/*
 * LED-44x11 worker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waLedWorker, 128);
static __attribute__((noreturn)) THD_FUNCTION(LedWorker, arg) {
	(void)arg;
	uint8_t idx;
	uint8_t ledFb[LED_FB_SIZE];

	chRegSetThreadName("LED-44x11-WO");

	event_listener_t vsync_el;
	chEvtRegister(&vsync_event, &vsync_el, EVENT_VSYNC);

	while (TRUE) {
		uint8_t woIdx = L44X11_FB_NUM;

		chEvtWaitAnyTimeout(ALL_EVENTS, TIME_INFINITE);
retry:
		chMtxLock(&ledFbCtx.mtx);
		for (idx = 0; idx < L44X11_FB_NUM; idx++) {
			if (ledFbCtx.state[idx] == L44X11_FB_IDLE) {
				ledFbCtx.state[idx] = L44X11_FB_PROD;
				woIdx = idx;
				break;
			}
		}
		chMtxUnlock(&ledFbCtx.mtx);

		if (woIdx == L44X11_FB_NUM) { chThdYield(); goto retry; }

		chMtxLock(&anMtx);
		A = A->run(A, ledFb, chVTGetSystemTime());
		chMtxUnlock(&anMtx);

		fbToPinCtl(ledFb, ledFbCtx.fb[woIdx]);

		ledFbCtx.state[woIdx] = L44X11_FB_CONS;
	}

	chEvtUnregister(&vsync_event, &vsync_el);
}

/*===========================================================================*/
/* LED-44x11 display related.                                                */
/*===========================================================================*/

/**
 * @brief   GPT3 callback.
 */
void irq_gpt3_cb(GPTDriver *gptp) {
	(void)gptp;
	chSysLockFromISR();
	chEvtBroadcastFlagsI(&vsync_event, EVENT_VSYNC);
	chSysUnlockFromISR();
}

/*
 * GPT3 configuration.
 */
static const GPTConfig gpt3cfg = {
	10000,                /* 10KHz timer clock.*/
	irq_gpt3_cb,    /* Timer callback.*/
	0,
	0
};

static void led_44x11_delay_any(uint32_t count) {
	while (count--) __NOP();
}

/*
 * LED-44x11 display thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waLedDisplay, 256);
static __attribute__((noreturn)) THD_FUNCTION(LedDisplay, arg) {
	(void)arg;
	chRegSetThreadName("LED-44x11-DP");

	event_listener_t vsync_el;
	chEvtRegister(&vsync_event, &vsync_el, EVENT_VSYNC);

	gptStart(&GPTD3, &gpt3cfg);
	gptStartContinuous(&GPTD3, 167); // @60Hz

	while (TRUE) {
		uint8_t idx, woIdx = L44X11_FB_NUM, doIdx = L44X11_FB_NUM;

		chEvtWaitAnyTimeout(ALL_EVENTS, TIME_INFINITE);
retry:
		chMtxLock(&ledFbCtx.mtx);
		for (idx = 0; idx < L44X11_FB_NUM; idx++) {
			if (ledFbCtx.state[idx] == L44X11_FB_CONS) {
				woIdx = idx;
			} else if (ledFbCtx.state[idx] == L44X11_FB_DONE) {
				doIdx = idx;
			}
		}
		if (woIdx != L44X11_FB_NUM && doIdx != L44X11_FB_NUM) {
			ledFbCtx.state[doIdx] = L44X11_FB_IDLE;
		} else {
			woIdx = (woIdx == L44X11_FB_NUM) ? doIdx : woIdx;
		}
		chMtxUnlock(&ledFbCtx.mtx);

		if (woIdx == L44X11_FB_NUM) { chThdSleepMilliseconds(1); goto retry; }

		const struct LedFbWo *fbWo = ledFbCtx.fb[woIdx];

		chSysLock();
		uint8_t lbmLe = lbmMu;
		uint16_t Ma = GPIOA->OTYPER & ~L44X11_GA_OTMASK;
		uint16_t Mb = GPIOB->OTYPER & ~L44X11_GB_OTMASK;
		uint16_t Mf = GPIOF->OTYPER & ~L44X11_GF_OTMASK;
		for (idx = 0; idx < sizeof(ledPinCfg) / sizeof(ledPinCfg[0]); idx++) {
			Gd32_gpio_t *port = ledPinCfg[idx].port;
			uint8_t pin = ledPinCfg[idx].pin;

			GPIOA->OTYPER = Ma | fbWo[idx].Ga;
			GPIOB->OTYPER = Mb | fbWo[idx].Gb;
			GPIOF->OTYPER = Mf | fbWo[idx].Gf;

			port->BSRR.H.clear = 1 << pin;
			led_44x11_delay_any(LBT[fbWo[idx].Co] * lbmLe);
			port->BSRR.H.set = 1 << pin;
		}
		chSysUnlock();

		ledFbCtx.state[woIdx] = L44X11_FB_DONE;
	}

	gptStopTimer(&GPTD3);
	chEvtUnregister(&vsync_event, &vsync_el);
}

/*
 * Application entry point.
 */
int __attribute__((noreturn)) main(void) {
	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	chSysInit();

	/*
	 * Activates the serial driver 2 using the driver default configuration.
	 * PA2(TX) and PA3(RX) are routed to USART2.
	 */
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(1) | PAL_GD32_OTYPE_PUSHPULL |
			PAL_GD32_OSPEED_HIGHEST | PAL_GD32_PUPDR_FLOATING);
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(1) | PAL_GD32_OTYPE_PUSHPULL |
			PAL_GD32_OSPEED_HIGHEST | PAL_GD32_PUPDR_FLOATING);

	/*
	 * Activates the serial driver 1 and SDC driver 1 using default
	 * configuration.
	 */
	sdStart(&SD2, NULL);

	/*
	 * Shell manager initialization.
	 */
	shellInit();

	chEvtObjectInit(&ext_event);
	chEvtObjectInit(&vsync_event);
	chMtxObjectInit(&anMtx);

	chMtxObjectInit(&ledFbCtx.mtx);
	{
		uint8_t idx;

		for (idx = 0; idx < sizeof(ledPinCfg) / sizeof(ledPinCfg[0]); idx++) {
			Gd32_gpio_t *port = ledPinCfg[idx].port;
			uint8_t pin = ledPinCfg[idx].pin;

			palSetPadMode(port, pin, PAL_MODE_OUTPUT_OPENDRAIN | PAL_GD32_OSPEED_HIGHEST);
		}
	}

    /*
     * Activates the EXT driver 1.
     */
    extStart(&EXTD1, &extcfg);

	/*
	 * Creates the LedAnmi thread.
	 */
	chThdCreateStatic(waLedAnim, sizeof(waLedAnim), NORMALPRIO - 2, LedAnim, NULL);

	/*
	 * Creates the LedWorker thread.
	 */
	chThdCreateStatic(waLedWorker, sizeof(waLedWorker), NORMALPRIO - 1, LedWorker, NULL);

	/*
	 * Creates the LedDisplay thread.
	 */
	chThdCreateStatic(waLedDisplay, sizeof(waLedDisplay), NORMALPRIO, LedDisplay, NULL);

	/*
	 * Normal main() thread activity, in this demo it does nothing except
	 * sleeping in a loop and check the button state.
	 */
	while (TRUE) {
		thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
				"shell", NORMALPRIO + 1, shellThread, (void *)&shellcfg);
		chThdWait(shelltp);	/* Waiting termination. */

		chThdSleepMilliseconds(500);
	}
}
