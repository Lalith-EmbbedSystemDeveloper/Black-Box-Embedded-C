 /* 
 * File:   car_black_box.h
 * Author: LALITH
 *
 * Created on 22 May, 2025, 8:19 AM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void display_dashboard(unsigned char event[], unsigned char speed);
void log_event(unsigned char event[], unsigned char speed);
unsigned char login_screen(unsigned char key, unsigned char reset_flag);

unsigned char menu_screen(unsigned char key, unsigned char reset_flag);

void view_log(unsigned char key, char reset_shift);
char clear_log(char reset_memory);
void download_log(void);
char change_password(unsigned char key, char reset_psswd);
char change_time(unsigned char key, unsigned char reset_time);

void clear_display();

#endif	/* CAR_BLACK_BOX_H */

