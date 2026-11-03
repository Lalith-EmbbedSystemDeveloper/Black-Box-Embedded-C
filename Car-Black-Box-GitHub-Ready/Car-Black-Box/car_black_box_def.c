/*
 File    : car_black_box_def.c
 Author  : Lalith
 created : 22/5/25
 */
//function definition for car black box
#include "main.h"

unsigned char clock_reg[3];
char time[7]; //HHMMSS
unsigned char log[11];
char log_pos = 0, sec;
unsigned char return_time;
char *menu[] = {"View log", "Clear log", "Download log", "Change passwd", "Set time"};
char menu_pos;  
int clear_flag = 0;


void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD
    
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0'; // 2 -> bit 4, 5
    time[1] = (clock_reg[0] & 0x0F) + '0'; //3 -> bit 0 - 3
    
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    
    time[6] = '\0';
}


void display_time(void)
{
    get_time();
    //HH:MM:SS
    clcd_putch(time[0], LINE2(2)); 
    clcd_putch(time[1], LINE2(3)); 
    clcd_putch(':', LINE2(4)); 
    clcd_putch(time[2], LINE2(5)); 
    clcd_putch(time[3], LINE2(6)); 
    clcd_putch(':', LINE2(7)); 
    clcd_putch(time[4], LINE2(8)); 
    clcd_putch(time[5], LINE2(9)); 
   
}

void display_dashboard(unsigned char event[], unsigned char speed)
{
    //display title meassage on clcd line1
    clcd_print("TIME     E  SP", LINE1(2));
    
    //DISPLAY TIME
    display_time();
    
    //DISPLAY EVENT
    clcd_print(event, LINE2(11));
    
    
    //DISPLAY SPEED
    //int to ascii speed => 45
    clcd_putch((speed / 10) + '0', LINE2(14));
    clcd_putch((speed % 10) + '0', LINE2(15));
    
}

void store_event()
{
    char addr;
    if(log_pos == 10)
    {
        log_pos = 0;
    }
    addr = 0x05 + log_pos * 10;
    ext_eeprom_24C02_str_write(addr, log);
    log_pos++;
    if(clear_flag < 9)
        clear_flag++;
    
}
void log_event(unsigned char event[], unsigned char speed)
{
    get_time();
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    log[8] = speed / 10 + '0';
    log[9] = speed % 10 + '0';
    log[10] = '\0';
    
    store_event();
}
unsigned char  login_screen(unsigned char key, unsigned char reset_flag)
{
    static char user_passwd[4];
    static unsigned char i;
    static unsigned char attempt_left;
    
    if(reset_flag == RESET_PASSWORD)
    {
        i = 0;
        attempt_left = 3;
        user_passwd[0] = '\0';
        user_passwd[1] = '\0';
        user_passwd[2] = '\0';
        user_passwd[3] = '\0';
        key = 0xFF;
        return_time = 5;
    }
    if(return_time == 0)
    {
        //switch to dashboard screen
        return RETURN_BACK;
    }
    __delay_ms(50);
    //sw4 = 1,sw5 = 0
    if(key == SW4 && i < 4)
    {
        clcd_putch('*', LINE2(4+i));
        user_passwd[i] = '1';
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        clcd_putch('*', LINE2(4+i));
        user_passwd[i] = '0';
        i++;
        return_time = 5;
    }
    
    if (i == 4)
    {
        char s_passwd[4];
        for(int j = 0; j < 4; j++)
        {
            s_passwd[j] = ext_eeprom_24C02_read(j);
        }
        
        //compare stored password with userpassowrd
        if(strncmp(user_passwd, s_passwd, 4) == 0)
        {
            clear_display();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("Login Success", LINE1(1));
            __delay_ms(3000);
            
            //change to the main menu screen
            return LOGIN_SUCCESS;
        }
        else
        {
            attempt_left--;
            if(attempt_left == 0)
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("You are Blocked", LINE1(0));
                clcd_print("wait for ", LINE2(0));
                clcd_print("secs", LINE2(12));
                //wait until sec reaches 0
                sec = 60;
                while(sec)
                {
                    clcd_putch(sec / 10 + '0', LINE2(9));
                    clcd_putch(sec % 10 + '0', LINE2(10));
                }
                
                attempt_left = 3;
            }
            else
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("Wrong password", LINE1(0));
                clcd_print("attempts left", LINE2(2));
                clcd_putch(attempt_left + '0', LINE2(0));
                __delay_ms(3000);
            }
            clear_display();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i = 0;
            return_time = 5;
        }
    }
}

unsigned char menu_screen(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_MENU)
    {
        menu_pos = 0;
        
    }
    if(key == SW4 && menu_pos > 0)
    {
        menu_pos--;
        clear_display();
    }
    else if(key == SW5 && menu_pos < 4)
    {
        menu_pos++;
        clear_display();
    }
    if(menu_pos == 4)
    {
        clcd_putch('*',LINE2(0));
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
    }
    else
    {
        clcd_putch('*',LINE1(0));
        clcd_print(menu[menu_pos], LINE1(2));
        clcd_print(menu[menu_pos + 1], LINE2(2));
    }
    return menu_pos;
}
void view_log(unsigned char key, char reset_index)
{
    static unsigned char index;
    char log[11];
    unsigned char add;
    
    if(clear_flag == 0)
    {
        clcd_print("No Logs Found",LINE2(0));
    }
    else
    {
        if(reset_index == RESET_VIEW_LOG_POS)
        {
            index = 0;
        }
        if(key == SW4 && index <(clear_flag -1))
        {
            index++;
        }
        else if(key == SW5 && index > 0)
        {
            index--;
        }
       
        for(int i = 0; i < 10; i++)
        {
            add = index * 10 + 5;//updating the address
            log[i] = ext_eeprom_24C02_read(add + i);
        }
        /*displaying index*/
        clcd_putch(index % 10 + '0', LINE2(0));
        
        //displaying hours value
        clcd_putch(log[0], LINE2(2));
        clcd_putch(log[1], LINE2(3));
        clcd_putch(':', LINE2(4));
        
        //displaying minutes value
        clcd_putch(log[2], LINE2(5));
        clcd_putch(log[3], LINE2(6));
        clcd_putch(':',LINE2(7));
        
        //displaying seconds value
        clcd_putch(log[4], LINE2(8));
        clcd_putch(log[5], LINE2(9));
        
        //displaying event character
        clcd_putch(log[6], LINE2(11));
        clcd_putch(log[7], LINE2(12));
        
        //displaying speed value
        clcd_putch(log[8], LINE2(14));
        clcd_putch(log[9], LINE2(15));
        
        
    }
}

char clear_log(char reset_memory)
{
    clcd_print("Logs cleared", LINE1(0));
    clcd_print("Successfully", LINE2(0));
    
    if(reset_memory == RESET_MEMORY)
    {
        
        log_pos = 0;
        clear_flag = 0;
        return TASK_SUCCESS;
    }
    return TASK_FAILURE;

    
}
void download_log(void)
{
    char log[11];
    log[10] = 0;
    unsigned char add;
    int position = 0;
    int index = 0;
    if(clear_flag == 0)
    {
        puts("No logs Founds\r\n");
        
    }
    else
    {
        puts("Logs:\r\n");
        puts("#   Time     Event  Speed\r\n");
        
        while(index < clear_flag)
        {
            position = index + 1;
            index++;
            for(int i = 0; i < 10; i++)
            {
                add = position * 10 + 5 +i;
                log[i] = ext_eeprom_24C02_read(add);
            }
            putchar(index + '0');
            puts("   ");
            
            //time
            putchar(log[0]);//hours
            putchar(log[1]);
            putchar(':');
            
            
            putchar(log[2]);//mins
            putchar(log[3]);
            putchar(':');
            
            putchar(log[4]);//sec
            putchar(log[5]);
            puts("   ");
            
            //event
            putchar(log[6]);
            putchar(log[7]);
            puts("   ");
            
            //speeed
            putchar(log[8]);
            putchar(log[9]);
            puts("\r\n");
            
        }
    }
}
char change_password(unsigned char key, char reset_passwd)
{
    static char new_psswd[9];
    static int psswd_pos = 0;
    static char psswd_changed = 0;
    static unsigned char toggle_cursor = 0;
    static unsigned int blink_delay= 0;
    
    if(blink_delay++ == 5)
    {
        blink_delay = 0;
        toggle_cursor = !toggle_cursor;
        
    }
    if(reset_passwd == RESET_PASSWORD)
    {
        strncpy(new_psswd, "    ", 4);
        psswd_pos = 0;
        psswd_changed = 0;
        return_time = 5;
    }
    if(!return_time)
    {
        return RETURN_BACK;
    }
    if(psswd_changed)
        return TASK_FAILURE;
    
    if(psswd_pos < 4)
    {
        clcd_print("Enter new passwd: ", LINE1(0));
        
        if(toggle_cursor == 0)
        {
            clcd_putch((unsigned char) 0xFF, LINE2(psswd_pos));
        }
        else
        {
            clcd_putch(' ', LINE2(psswd_pos));
        }
    }
    else if(psswd_pos > 3 && psswd_pos < 8)
    {
        clcd_print("Re-enter new psswd", LINE1(0));
        
        if(toggle_cursor == 0)
        {
            clcd_putch((unsigned char)0xFF, LINE2(psswd_pos));
        }
        else
        {
            clcd_putch(' ', LINE2(psswd_pos));
        }
    }
    switch(key)
    {
        case SW4:
            new_psswd[psswd_pos] = '1';
            clcd_putch('*', LINE2(psswd_pos));
            psswd_pos++;
            return_time = 5;
            if(psswd_pos == 4)
                clcd_print("                 ", LINE2(0));
            
            break;
        case SW5:
            new_psswd[psswd_pos] = '0';
            clcd_putch('*', LINE2(psswd_pos));
            psswd_pos++;
            return_time = 5;
            if(psswd_pos == 4)
                clcd_print("                 ",LINE2(0));
            break;
    }
    if(psswd_pos == 8)
    {
        if(strncmp(new_psswd, &new_psswd[4], 4) == 0)//if both entered passwds are matched
        {
            new_psswd[8] = 0;
            ext_eeprom_24C02_str_write(0x00, &new_psswd[4]);
            psswd_pos++;
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            clcd_print("Password changed", LINE1(0));
            clcd_print("Successfully", LINE2(0));
            psswd_changed = 1;
            __delay_ms(1000);
            return TASK_SUCCESS;
        }
        else
        {
            clcd_write(CLEAR_DISP_SCREEN,INST_MODE);
            clcd_print("Password Mismatched", LINE1(0));
            psswd_changed = 1;
            __delay_ms(1000);
            return TASK_SUCCESS;
        }
    }
    return TASK_FAILURE;
}
char change_time(unsigned char key, unsigned char reset_time)
{
    static unsigned int new_time[3];
    static unsigned int blink_pos;
    static unsigned char wait;
    static unsigned char blink;
    static char t_done = 0;
    char buffer;
    if(reset_time == RESET_TIME)
    {
        get_time();
        
        new_time[0] = (time[0] & 0x0F) * 10 + (time[1] & 0x0F);
        
        new_time[1] = (time[2] & 0x0F) * 10 + (time[3] & 0x0F);
        
        new_time[2] = (time[4] & 0x0F) * 10 + (time[5] & 0x0F);
        
        clcd_print("Time (HH:MM:SS)", LINE1(0));
        
        //update the flags
        blink_pos = 2;
        wait = 0;
        blink = 0;
        t_done = 0;
        key = ALL_RELEASED;
    }
     if(t_done)
            return TASK_FAILURE;
        
        switch(key)
        {
            case SW4://increment the value
                new_time[blink_pos]++;
                break;
            case SW5://change the field
                blink_pos = (blink_pos + 1)  % 3;
                break;
                
            case SW4_LP:
                get_time();
                
                
                buffer = ((new_time[0] / 10) << 4) | new_time[0] % 10; 
                clock_reg[0] = (clock_reg[0] & 0xC0) | buffer; 
                write_ds1307(HOUR_ADDR, clock_reg[0]); 


                buffer = ((new_time[1] / 10) << 4) | new_time[1] % 10; 
                clock_reg[1] = (clock_reg[1] & 0x80) | buffer; 
                write_ds1307(MIN_ADDR, clock_reg[1]); 


                buffer = ((new_time[2] / 10) << 4) | new_time[2] % 10; 
                clock_reg[2] = (clock_reg[2] & 0x80) | buffer; 
                write_ds1307(SEC_ADDR, clock_reg[2]); 


               clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
               clcd_print("Time changed", LINE1(2));
               clcd_print("Successfully", LINE2(2));
               
               t_done = 1;
               __delay_ms(1000);
               return TASK_SUCCESS;
        }
        
        if(new_time[0] > 23)
            new_time[0] = 0;
        if(new_time[1] > 59)
            new_time[1] = 0;
        if(new_time[2] > 59)
            new_time[2] = 0;
        
        if(wait++ == 1)
        {
            wait = 0;
            blink = !blink;
            
            if(blink)
            {
                switch(blink_pos)
                {
                    case 0:
                        clcd_print("  ", LINE2(0));
                        __delay_ms(160);
                        break;
                    case 1:
                        clcd_print("  ", LINE2(3));
                        __delay_ms(160);
                        break;
                    case 2:
                        clcd_print("  ", LINE2(6));
                        __delay_ms(160);
                        break;
                }
            }
        }
    
    
    /* Displaying hours field */
    clcd_putch(new_time[0] / 10 + '0', LINE2(0));
    clcd_putch(new_time[0] % 10 + '0', LINE2(1));
    clcd_putch(':', LINE2(2));

    /* Displaying mins field */
    clcd_putch(new_time[1] / 10 + '0', LINE2(3));
    clcd_putch(new_time[1] % 10 + '0', LINE2(4));
    clcd_putch(':', LINE2(5));

    /* Displaying secs field */
    clcd_putch(new_time[2] / 10 + '0', LINE2(6));
    clcd_putch(new_time[2] % 10 + '0', LINE2(7));
    
    return TASK_FAILURE;
    
      
}
void clear_display()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}