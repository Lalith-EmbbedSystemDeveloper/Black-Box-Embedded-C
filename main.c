/*
 File          : main.c
 Author's Name : Lalith Singarapu
 Date          : 22/5/25
 Description   : Car Black Box Project
 */

#include "main.h"
#pragma config WDTE = OFF          //watching timer enable bit (WDT disabled)

void init_config()
{
    //configure i2c
    init_i2c(100000);
    
    //configure rtc
    init_ds1307();
            
    //configure clcd
    init_clcd();
    
    //configure dkp
    init_digital_keypad();
    
    //configure adc
    init_adc();
    
    //configure timer2
    init_timer2();
    
    //configure uart
    init_uart(9600);
    
    PEIE = 1;
    GIE = 1;
   
}
void main()
{
    init_config();
    
    unsigned char control_flag = DASHBOARD_SCREEN, key, key_LP;
    unsigned char reset_flag;
    unsigned char event[3] = "ON";
    unsigned char speed = 0;
    unsigned char *gear[] = { "GN", "GR", "G1", "G2", "G3", "G4" };
                             //0      1     2     3    4       5
    unsigned char gr = 0, menu_pos;
    log_event(event, speed);
    ext_eeprom_24C02_str_write(0x00,"1010");
    while(1)
    {
        speed = read_adc() / 10.3;//0 - 1023 => 0 - 99
        key = read_digital_keypad();
       
        for(unsigned int i = 300; i--;);//to avoid bouncing effect
        if(key == SW1)
        {
            //display collision
            strcpy(event, "CO");
            log_event(event, speed);
        }
        else if(key == SW2 && gr < 6)
        {
            //increment the gear
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
        }
        else if(key == SW3 && gr > 0)
        {
            //decrement the gear
            gr--;
            strcpy(event, gear[gr]);
            log_event(event, speed);
        }
        else if((key == SW4 || key == SW5) && control_flag == DASHBOARD_SCREEN)
        {
            control_flag = LOGIN_SCREEN;
            clear_display();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            reset_flag = RESET_PASSWORD;
            TMR2ON = 1;
        }
        else if(key == SW4_LP && control_flag == MAIN_MENU_SCREEN)
        {
            
            switch(menu_pos)
            {
                case 0: //view log
                    clear_display();
                    clcd_print("# TIME     E  SP", LINE1(0));
                    control_flag = VIEW_LOG_SCREEN;
                    reset_flag = RESET_VIEW_LOG_POS;
                    break;
                case 1://clear log
                    clear_display();
                    control_flag = CLEAR_LOG_SCREEN;
                    reset_flag = RESET_MEMORY;
                    break;
                case 2://download
                    clear_display();
                    control_flag = MAIN_MENU_SCREEN;
                    reset_flag = RESET_MAIN_MENU_SCREEN;
                    log_event("DL", speed);
                    clcd_print("Open", LINE1(0));
                    __delay_ms(2000);
                    control_flag = MAIN_MENU_SCREEN;
                    reset_flag = RESET_MAIN_MENU_SCREEN;
                      
                    break;
                case 3://change password
                    control_flag = CHANGE_PASSWORD_SCREEN;
                    clear_display();
                    reset_flag = RESET_PASSWORD;
                    TMR2ON = 1;
                    break;
                case 4:
                    clear_display();
                    control_flag = SET_TIME_SCREEN;
                    reset_flag = RESET_TIME;
                    log_event("ST", speed);
                    break;
            }
        }
        else if(key == SW4_LP && control_flag == VIEW_LOG_SCREEN)
        {
            control_flag = MAIN_MENU_SCREEN;
            clear_display();
        }
        else if(key == SW4_LP && control_flag == CLEAR_LOG_SCREEN)
        {
            control_flag = MAIN_MENU_SCREEN;
            clear_display();
        }
        else if(key == SW4_LP && control_flag == CHANGE_PASSWORD_SCREEN)
        {
            control_flag = MAIN_MENU_SCREEN;
            clear_display();
        }
        else if(key == SW5_LP && control_flag == MAIN_MENU_SCREEN)
        {
            control_flag = DASHBOARD_SCREEN;
            clear_display();
        }
        switch(control_flag)
        {
            case DASHBOARD_SCREEN:
                display_dashboard(event, speed);
                break;
            case LOGIN_SCREEN:
                switch(login_screen(key, reset_flag))
                {
                    case RETURN_BACK:
                        control_flag = DASHBOARD_SCREEN;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag = MAIN_MENU_SCREEN;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        clear_display();
                        reset_flag = RESET_MENU;
                        continue;
                }
                break;
            case MAIN_MENU_SCREEN:
                menu_pos = menu_screen(key, reset_flag);
                break;
            case VIEW_LOG_SCREEN:
                view_log(key, reset_flag);
                break;
            case CLEAR_LOG_SCREEN:
                if (clear_log(reset_flag) == TASK_SUCCESS)
                    __delay_ms(1000);

                if (reset_flag == RESET_MEMORY)
                    log_event("CL", speed);

                control_flag = MAIN_MENU_SCREEN; /* go back to login menu */
                reset_flag = RESET_MAIN_MENU_SCREEN;
                clear_display();
                break;
            case CHANGE_PASSWORD_SCREEN:
                switch(change_password(key, reset_flag))
                {
                    case TASK_SUCCESS:
                        __delay_ms(1000);
                        log_event("CP", speed);
                        control_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MAIN_MENU_SCREEN;
                        clear_display();
                        break;
                    case RETURN_BACK:
                        control_flag = DASHBOARD_SCREEN;
                        reset_flag = RESET_MAIN_MENU_SCREEN;
                        break;
                }
                break;
            case SET_TIME_SCREEN:
                if(change_time(key, reset_flag) == TASK_SUCCESS)
                {
                    control_flag = MAIN_MENU_SCREEN;
                    reset_flag = RESET_MAIN_MENU_SCREEN;
                    clear_display();
                    continue;
                }
                break;
        }
        reset_flag = RESET_NOTHING;
    }
    return;
}
