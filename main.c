/*
 * Project2.c
 *
 * Created: 4/23/2018 5:40:03 PM
 * Author : Vicky Adebona
 */ 

#include <avr/io.h>
#include "avr.c"
#include "lcd.c"
#include <stdio.h>

int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // stores how many days are in each month in an array
int x_cursor, y_cursor;
char mmddyyyy[10] = {'0','1','/','0','1','/','0','0','0','0'};//,'\0'};
char hhmmss[8] = {'0','0',':','0','0',':','0','0'};//, '\0'};
	
struct Clock {
	int month;
	int day;
	int year;
	
	int hour;
	int minute;
	int second;

	int flag; //for hour, if it is AM then 
};

void update_clock(struct Clock c)
{
	char month[2];
	month[0] = mmddyyyy[0];
	month[1] = mmddyyyy[1];
	c.month = atoi(month);
	
	char day[2];
	day[0] = mmddyyyy[3];
	day[1] = mmddyyyy[4];
	c.day = atoi(day);
	
	char year[4];
	year[0] = mmddyyyy[6];
	year[1] = mmddyyyy[7];
	year[2] = mmddyyyy[8];
	year[3] = mmddyyyy[9];
	c.year = atoi(year);
	
	char hour[2];
	hour[0] = hhmmss[0];
	hour[1] = hhmmss[1];
	c.hour = atoi(hour);
	
	char minute[2];
	minute[0] = hhmmss[3];
	minute[1] = hhmmss[4];
	c.minute = atoi(minute);
	
	char sec[2];
	sec[0] = hhmmss[6];
	sec[1] = hhmmss[7];
	c.year = atoi(sec);
}

int numAssignedToKey(int k)
{
	if (k >= 1 && k <= 3) { return k; }				//123
	else if (k >= 5 && k <= 7 ) {return (k-1); }	//456
	else if (k >= 9 && k <= 11) { return (k-2); }	//789
	else {return 0;} //k = 15
}

int validate_date(int k) //mm/dd/yyyy
{
	int num = numAssignedToKey(k);
	if (y_cursor == 0){ //validate the month
		if (num == 1 && (mmddyyyy[1] == '1' || mmddyyyy[1] == '2')) {
			mmddyyyy[0] = (char)num;
			return num;
		} else if (num == 0 && mmddyyyy[1] != '0') {
			mmddyyyy[0] = (char)num; 
			return num;
		}
	} else if (y_cursor == 1) {
		if (num == 0 && mmddyyyy[0] != '0') {
			mmddyyyy[1] = (char)num; 
			return num;
		}
	} else if (y_cursor == 3 || y_cursor == 4) {
		char currentMonth[2];
		currentMonth[0] = mmddyyyy[0];
		currentMonth[1] = mmddyyyy[1];
		int month = atoi(currentMonth);
		//int d = atoi();
		char day[2];
		if (y_cursor == 3 && num >= 0 && num <= 3) {//change d1
			day[0] = (char)num;
			day[1] = mmddyyyy[4]; //change date but it might not be valid, will check at end
		} else if (y_cursor == 2 && num >= 0 && num <= 9) { //change d2, check later too 
			day[0] = mmddyyyy[3];
			day[1] = (char)num;	
		}
		//int d = atoi(day);
		if (atoi(day) <= months[month] && atoi(day) > 0) {
			mmddyyyy[3] = day[0];
			mmddyyyy[4] = day[1];
			return num;
		}
	} else if (x_cursor >= 6){		//x_cursor is in year
		mmddyyyy[y_cursor] = num;		 //char year[4] = mmddyyyy[6] + mmddyyyy[7] + mmddyyyy[8] + mmddyyyy[9];		//int yr = atoi(year);
		return num;
	}
	return -1;
}

int validate_time(int k) //hh:mm:ss
{
	int num = numAssignedToKey(k);
	if (y_cursor == 0 || y_cursor == 1) {
		hhmmss[y_cursor] = (char)num;
		return num;
	} else if (y_cursor == 3 || y_cursor == 6){ //Highest is 59
		if ((num >= 0 && num <= 5)) { //if the second number is a nine we can change the 1st number to numbers 0-5
			hhmmss[y_cursor] = (char)num;
			return num;
		} else { 
			return -1; 
		}
	} else if (y_cursor == 4 || y_cursor == 7){
		if (num >= 0 && num <= 9){
			hhmmss[y_cursor] = (char)num;
			return num;
		}
	}
	return -1;
}
//CHANGES THE BUFFER OR MOVES CURSOR
void get_input(struct Clock c)
{
	int k = get_key();
	if ((k >= 1 && k <= 3) || (k >= 5 && k <= 7) || (k >= 9 && k <= 11) || (k == 15)) {	//input is in specific range
		int valid = -1;
		if (x_cursor == 0 && y_cursor != 2 && y_cursor != 5)	{						//check mmddyyyy
				valid = validate_date(k);
		} else {	
			if (y_cursor != 2 && y_cursor != 5){										//check hhmmss
				valid = validate_time(k);
			}
		}
		
		if (valid != -1) {
			//change the LCD, input the num/valid in current position
			update_clock(c);
		}
		
	} else if (k == 12 && x_cursor != 0) {												//move cursor LEFT
		x_cursor -= 1;
		
	} else if (k == 14){																//move cursor RIGHT
		if (y_cursor == 0 && x_cursor != 9){
			x_cursor += 1;
			
		} else if (y_cursor == 1 && x_cursor != 7){	//y_cursor == 1, bottom row
			x_cursor += 1;
			
		}
	} else if (k == 16 && y_cursor != 1) { 												//move cursor DOWN
		y_cursor = 1;
		
	} else if (k == 12 && y_cursor != 0) {												//move cursor UP
		y_cursor = 0;
		
	} else if (k==4) {																	//reset clock OR military time??
		//c = {1, 1, 0, 0, 0, 0, 0, 0};
		//hhmmss = {'0','0',':','0','0',':','0','0'};
		//mmddyyyy = {'0','1','/','0','1','/','0','0','0','0'};
	} else if (k==8) {
		c.flag = 1;			//make the hour PM
	} //else k == 0 and no key was pressed
		
}

void run_clock(struct Clock c)
{
	c.second += 1;
	if (c.second == 60){
		c.second = 0;
		c.minute += 1;
		if (c.minute == 60){
			c.minute = 0;
			c.hour += 1;
			if (c.hour == 13 && c.flag == 0){
				c.flag = 1; //now it is in PM and must go through the 12 hours to change the day
				c.hour = 0;
			} else if (c.hour == 13 && c.flag == 1 ){
				c.flag = 0;
				c.hour = 0;
				c.day += 1;
				
				int days_in_month = months[c.month-1];
				if (c.day % days_in_month  == 0) {
					c.day = 1;
					c.month += 1;
					if (c.month == 13){
						c.month = 1;
						c.year += 1;
					}	
				}
			}
		}
	}
}

/*
struct Clock update_time(struct Clock c, int s) {
	c.second += 1;
	
	if(s % 60 == 0) {
		c.minute += 1;
	}
	
	if (s % 360 == 0) {
		c.hour += 1;
	}
	
	if (c.second == 60) {
		c.second = 0;
	}
	
	if(c.minute == 60) {
		c.minute = 0;
	}
	
	if(c.hour > 12) {
		c.hour = 1;
	}
	
	return c;	
}*/


void display_clock(struct Clock c)
{
}


/*
void create_mmddyy_buffer(struct Clock c)
{
	char buf_month[2];
	/*if (c.month <= 9 && c.month >= 0){
		buf_month[0] = '0';
		buf_month[1] = c.month;
	} else {*//*
	sprintf(buf_month, "%02d", c.month);
	//}
	
	char buf_day[2];
	sprintf(buf_day, "%02d", c.day);
	
	char buf_year[4];
	sprintf(buf_year, "%04d", c.year);
	
	//sprintf(mmddyyyy, "%c/%c/%c", buf_month, buf_day, buf_month);
}

void create_hhmmss_buf(struct Clock c)
{
	char buf_hour[2];
	sprintf(buf_hour, "%02i", c.hour);
	
	char buf_min[2];
	sprintf(buf_min, "%02i", c.minute);
	
	char buf_sec[2];
	sprintf(buf_sec, "%02i", c.second);
	
	sprintf(hhmmss, "%02i:%02i:%02i", c.hour, c.minute, c.second);//buf_hour, buf_min, buf_sec);
}*/

int is_pressed(int r, int c) {
	// Set all R's and all C's to N/C
	DDRC = 0;
	PORTC = 0;
	
	// Set r to Strong 0
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	// Set c to Weak 1
	CLR_BIT(DDRC, c+4); // Add 4 to reach pins 4-7 on keypad
	SET_BIT(PORTC, c+4);
	wait_avr(1);
	
	// Change state of button
	if(GET_BIT(PINC, c+4)) {
		return 0; // button is not pressed
	} else {
		return 1; // button is pressed
	}
}

int get_key() {
	int r, c;
	
	for(r = 0; r < 4; ++r) {
		for(c = 0; c < 4; ++c) {
			if(is_pressed(r, c)) {
				return r * 4 + c + 1;
			}
		}
	}
	return 0;
}

int main()
{
	struct Clock c = {1, 1, 0, 0, 0, 0, 0};
	int total_seconds, x_cursor, y_cursor = 0;
	ini_avr();
	ini_lcd();
	clr_lcd();

	while(1){
		sprintf(mmddyyyy, "%02i/%02i/%04i", c.month, c.day, c.year);//create_mmddyy_buffer(c);
		sprintf(hhmmss, "%02i:%02i:%02i", c.hour, c.minute, c.second);//create_hhmmss_buf(c);
		pos_lcd(0,0);
		puts_lcd2(mmddyyyy);
		pos_lcd(1,0);
		puts_lcd2(hhmmss);
		get_input(c);				//check for input & update buffer/move cursor if needed
		wait_avr(1000);
		run_clock(c); //advance time/update clock
	}
	/*char buf[17];
	while(1) {
		int k =  get_key();// c = get_time();
		pos_lcd(0,0);
		sprintf(buf, "%02i", k);
		puts_lcd2(buf);
		/*wait_avr(1000);
		total_seconds += 1;
		c = update_time(c, total_seconds);*//*
	}*/
	/*
	while (1) {
		int k = get_key();
		pos_lcd(0,0);
		put_lcd(k+'0');
		for(int i = 0; i < k; i++) {	//blink led k times
			SET_BIT(PORTB, 0);
			wait_avr(400);
			CLR_BIT(PORTB, 0);
			wait_avr(400);
		}
		wait_avr(2000);			//wait 2 seconds
    }*/
	return 0;
}

