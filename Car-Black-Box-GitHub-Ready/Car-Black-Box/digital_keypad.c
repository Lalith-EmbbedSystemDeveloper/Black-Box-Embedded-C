/*
 File    : digital_keypad.c
 Author  : Lalith
 */
#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad()
{
    static unsigned char prev_key;
    static char once;
    static int long_pressed;
    unsigned char key = KEYPAD_PORT & INPUT_LINES;
    
    if(key != ALL_RELEASED && once == 0)
    {
        once = 1;
        long_pressed = 0;
        prev_key = key;
    }
    else if(key == ALL_RELEASED && once == 1)
    {
        once = 0;
        if(long_pressed < 15)
        {
            return prev_key;
        }
    }
    
    if(once == 1 && long_pressed < 16)
    {
        long_pressed++;
    }
    else if(once == 1 && long_pressed == 16 && key == SW4)
    {
        long_pressed++;
        return SW4_LP;
    }
    else if(once ==1 && long_pressed == 16 && key == SW5)
    {
        long_pressed++;
        return SW5_LP;
    }
    
    return ALL_RELEASED;
}
