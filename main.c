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
char mmddyyyy[11] = {'0','1','/','0','1','/','0','0','0','0','\0'};
char hhmmss[9] = {'0','0',':','0','0',':','0','0','\0'};
	
struct Clock {
	unsigned int month;
	unsigned int day;
	unsigned int year;
	
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	unsigned int flag;				//for hour, if it is AM then 
	unsigned int militaryTime;		//originally 0
};
/*
struct Clock update_clock(struct Clock c)
{
	char month[3];
	month[0] = mmddyyyy[0];
	month[1] = mmddyyyy[1];
	month[2] = '\0';
	c.month = atoi(month);
	
	char day[3];
	day[0] = mmddyyyy[3];
	day[1] = mmddyyyy[4];
	day[2] = '\0';
	c.day = atoi(day);
	
	char year[5];
	year[0] = mmddyyyy[6];
	year[1] = mmddyyyy[7];
	year[2] = mmddyyyy[8];
	year[3] = mmddyyyy[9];
	year[4] = '\0';
	c.year = atoi(year);
	
	char hour[3];
	hour[0] = hhmmss[0];
	hour[1] = hhmmss[1];
	hour[2] = '\0';
	c.hour = atoi(hour);
	
	char minute[3];
	minute[0] = hhmmss[3];
	minute[1] = hhmmss[4];
	minute[2] = '\0';
	c.minute = atoi(minute);
	
	char sec[3];
	sec[0] = hhmmss[6];
	sec[1] = hhmmss[7];
	sec[2] = '\0';
	c.year = atoi(sec);
	
	return c;
}*/

int numAssignedToKey(int k)
{
	if (k >= 1 && k <= 3) { return k; }				//123
	else if (k >= 5 && k <= 7 ) {return (k-1); }	//456
	else if (k >= 9 && k <= 11) { return (k-2); }	//789
	else {return 0;} //k = 15
}
/*
struct Clock validate_date(int k, struct Clock c) //mm/dd/yyyy
{
	int num = numAssignedToKey(k);
	char number[3];
	if (y_cursor == 0){ //validate the month
		if (num == 1 && (mmddyyyy[1] == '1' || mmddyyyy[1] == '2')) {
			number[0] = (char)num;
			number[1] = mmddyyyy[1];
			number[2] = '\0';
			c.month = atoi(number);
			return c;
		} else if (num == 0 && mmddyyyy[1] != '0') {
			number[0] = (char)num; 
			number[1] = mmddyyyy[1];
			number[2] = '\0';
			c.month = atoi(number);
			return c;
		}
		
	} else if (y_cursor == 1) {
		if (mmddyyyy[0] == '0') {
			number[0] = mmddyyyy[0];
			number[1] = (char)num; 
			number[2] = '\0';
			c.month = atoi(number);
			return c;
			
		} else if (mmddyyyy[0] == '1') {
			if (num == 1 || num == 2) {
				number[0] = mmddyyyy[0];
				number[1] = (char)num;
				number[2] = '\0';
				c.month = atoi(number);
				return c;
			}
		}
		
	} else if (y_cursor == 3 || y_cursor == 4) {
		char currentMonth[2];
		currentMonth[0] = mmddyyyy[0];
		currentMonth[1] = mmddyyyy[1];
		int month = atoi(currentMonth);
		
		char day[2];
		if (y_cursor == 3){
			if ( num >= 0 && num <= 3) {	//change d1
				day[0] = (char)num;
				day[1] = mmddyyyy[4];			//change date but it might not be valid, will check at end
			}
		} else if (y_cursor == 4) {
			if (num >= 0 && num <= 9) { //change d2, check later too 
				day[0] = mmddyyyy[3];
				day[1] = (char)num;	
			}
		}
		
		if ((atoi(day) <= months[month]) && (atoi(day) > 0)) {
			c.day = atoi(day);		//mmddyyyy[3] = day[0];	//mmddyyyy[4] = day[1];
			return c;
		}
	} else if (x_cursor >= 6 && x_cursor <= 9){		//x_cursor is in year
		mmddyyyy[x_cursor] = num;		 
		char year[5] = {mmddyyyy[6], mmddyyyy[7], mmddyyyy[8], mmddyyyy[9], '\0'};		
		c.year = atoi(year);
		return c;
	}
	return c;
}

struct Clock validate_time(int k, struct Clock c) //hh:mm:ss
{
	int num = numAssignedToKey(k);
	char time[3];
	if (x_cursor == 0){
		if (num == 1 && atoi(hhmmss[1]) <= 2 && atoi(hhmmss[1]) >= 0 ) {
			time[0] = (char)num;
			time[1] = hhmmss[1];
			c.hour = atoi(time);
			return c;
		} else if (num == 0 && atoi(hhmmss[1]) <= 2 && atoi(hhmmss[1]) >= 0 ){
			time[0] = (char)num;
			time[1] = hhmmss[1];
			c.hour = atoi(time);
			return c;
		} else { 
			return c;
		}
	} else if (x_cursor == 1) { //biggest is 12
		
		hhmmss[x_cursor] = (char)num;
		return num;
				
	} else if (x_cursor == 3 || x_cursor == 6){ //Highest is 59
		if ((num >= 0 && num <= 5)) { //if the second number is a nine we can change the 1st number to numbers 0-5
			time[0] = (char)num; //hhmmss[x_cursor] = (char)num;
			time[1] = hhmmss[x_cursor + 1];
			time[2] = '\0';
			if (x_cursor == 3){
				c.minute = atoi(time);
			} else {
				c.second = atoi(time);
			}
			return c;
		} else { 
			return c; 
		}
	} else if (x_cursor == 4 || x_cursor == 7){
		if (num >= 0 && num <= 9){
			time[0] = hhmmss[x_cursor-1]; //hhmmss[x_cursor] 
			time[1] = (char)num;
			time[2] = '\0';
			if (x_cursor == 4){
				c.minute = atoi(time);
			} else {
				c.second = atoi(time);
			}
			return c;
		}
	}
	return c;
}*/
//CHANGES THE BUFFER OR MOVES CURSOR
struct Clock get_input(struct Clock c)
{
	int k = get_key(); //	if ((k >= 1 && k <= 3) || (k >= 5 && k <= 7) || (k >= 9 && k <= 11) || (k == 14)) {	//input is in specific range
	if (k == 1) {
		c.second -= 2;
		if (c.second < 0){
			c.second = 59;
			return c;
		}
		return c;
	} else if (k == 2) {
		c.second += 1;
		if (c.second >= 60) {
			c.second = 0;
			return c;
		}
		return c;
	} else if (k == 5) {
		if (c.minute <= 0) {
			c.minute = 59;
		} else {
			c.minute -= 1;
		}
		return c;
	} else if (k == 6) {
		c.minute += 1;
		if (c.minute >= 60) {
			c.minute = 0;
			return c;
		}
		return c;
	} else if (k == 9) {
		/*if (c.hour == 1) {
			c.hour = 12;	//MILITARY TIME, SWITCH FLAG?
		}  else {
			c.hour -= 1;
		}*/
		if (c.hour == 0){
			c.hour = 23;
		} else {
			c.hour -= 1;
		}
		return c;
	} else if (k == 10) { //taken care of??
		if (c.hour == 23){
			c.hour = 0;
		} else {
			c.hour += 1;
		}
		return c;
	} else if (k == 3) {
		c.day -= 1;
		if (c.day <= 0){
			c.day = months[c.month-1]; 
			return c;
		}
		return c;
	} else if (k== 4) {
		c.day += 1;
		if (c.day > months[c.month-1]){
			c.day = 1;
			return c;
		}
		return c;
	} else if (k == 7) {
		if (c.month == 1) {
			c.month = 12;
		} else {
			c.month -= 1;
		}
		if (c.day > months[c.month-1]){
			c.day = 1;
		}
		return c;
	} else if (k == 8) {
		c.month += 1;
		if (c.month > 12){
			c.month = 1;
		}
		if (c.day > months[c.month-1]){
			c.day = 1;
		}
		return c;
	} else if (k == 11 && c.year != 0) {
		c.year -= 1;
		return c;
	} else if (k == 12) {
		c.year += 1;
		return c;
	} else if (k == 16) {																	//military time??
		if (c.militaryTime == 1) {
			c.militaryTime = 0;
		} else {
			c.militaryTime = 1;
		}
	} //else if (k == 13) {//15 16
		//c.flag = 1;			//make the hour PM
	return c;
}

struct Clock run_clock(struct Clock c)
{
	c.second += 1;
	if (c.second == 60){
		c.second = 0;
		c.minute += 1;
		if (c.minute == 60){
			c.minute = 0;
			c.hour += 1;
			if (c.hour == 24){
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
	return c;
}


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

int get_hour(struct Clock c)
{
	if (c.hour == 0){
		return 12;
	} else if (c.hour == 12) {
		return 12;
	} else if (c.hour >= 1 && c.hour <= 11) {
		return c.hour;
	} else if (c.hour > 12 && c.hour <= 23) {
		return c.hour - 12;
	}
}

struct Clock set_flag(struct Clock c)
{
	if (c.hour == 0){
		c.flag = 0;		//12 AM
	} else if (c.hour == 12) {
		c.flag = 1;		//12 PM
	} else if (c.hour >= 1 && c.hour <= 11) {
		c.flag = 0;
	} else if (c.hour > 12 && c.hour <= 23) {
		c.flag = 1;
	}
	return c;
}	

int main()
{
	struct Clock c = {1, 1, 0, 0, 0, 0, 0, 0};//flag 0 = AM, 1 = PM
	ini_avr();
	ini_lcd();
	clr_lcd();

	while(1){
		sprintf(mmddyyyy, "%02i/%02i/%04i", c.month, c.day, c.year);
		pos_lcd(0,0);
		puts_lcd2(mmddyyyy);
		if (c.militaryTime == 1) {	//on
			sprintf(hhmmss, "%02i:%02i:%02i %c", c.hour, c.minute, c.second, 'M');
		} else {
			int hr = get_hour(c);
			c = set_flag(c);
			char amPm;
			if (c.flag == 0) {
				amPm = 'A';
			} else {
				amPm = 'P';
			}
			sprintf(hhmmss, "%02i:%02i:%02i %c", hr, c.minute, c.second, amPm);
		}
		pos_lcd(1,0);
		puts_lcd2(hhmmss);

		c = get_input(c);				//check for input & update buffer/move cursor if needed
		wait_avr(1000);
		c = run_clock(c); //advance time/update clock
	}
	return 0;
}
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