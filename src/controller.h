#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct Controller
{
    unsigned short button; /* A,B,C,Z,L,R,START,PAD ,button 	*/
    char stick_x;          /* Actual used range -61 <= stick X <= 61*/
    char stick_y;          /* Actual used range -63 <= stick Y <= 63*/
    // unsigned char errno;    /* Error				*/
    unsigned short trigger; /* Button's trigger data		*/
} Controller;

extern Controller controller[1];

void Controller_update();

#ifdef _ULTRA64
#define CONT_BUTTON_A A_BUTTON
#define CONT_BUTTON_B B_BUTTON
#define CONT_BUTTON_Z Z_TRIG
#define CONT_BUTTON_START START_BUTTON
#define CONT_PAD_UP U_JPAD
#define CONT_PAD_DOWN D_JPAD
#define CONT_PAD_LEFT L_JPAD
#define CONT_PAD_RIGHT R_JPAD
#define CONT_BUTTON_L L_TRIG
#define CONT_BUTTON_R R_TRIG
#define CONT_C_UP U_CBUTTONS
#define CONT_C_DOWN D_CBUTTONS
#define CONT_C_LEFT L_CBUTTONS
#define CONT_C_RIGHT R_CBUTTONS

#else
#define CONT_BUTTON_A (1 << 0)
#define CONT_BUTTON_B (1 << 1)
#define CONT_BUTTON_Z (1 << 2)
#define CONT_BUTTON_START (1 << 3)
#define CONT_PAD_UP (1 << 4)
#define CONT_PAD_DOWN (1 << 5)
#define CONT_PAD_LEFT (1 << 6)
#define CONT_PAD_RIGHT (1 << 7)
#define CONT_BUTTON_L (1 << 8)
#define CONT_BUTTON_R (1 << 9)
#define CONT_C_UP (1 << 10)
#define CONT_C_DOWN (1 << 11)
#define CONT_C_LEFT (1 << 12)
#define CONT_C_RIGHT (1 << 13)
#endif

void Controller_print(Controller *cont);

#endif
