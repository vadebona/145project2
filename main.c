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

int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // stores how many days are in each month in an array


struct Clock {
	int year;
	int month;
	int day;
	
	int hour;
	int minute;
	int second;

};

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
}

 struct Clock update_date(struct Clock c, int d) {
	c.day += 1;
	
	// increment the month:
	if(c.day > months[c.month-1]) {
		c.month += 1;
		c.day = 1;
	}
	
	// Increment the year:
	if(c.month > 12) {
		c.month = 1;
		c.day = 1;
		c.year += 1;
		
		if(c.year % 4 == 0) {
			months[1] = 29;
		} else {
			months[1] = 28;
		}
	}
	
} 

int is_pressed(int r, int c) {
	// Set all R's and all C's to N/C
	DDRC = 0;
	PORTC = 0;
	
	// Set r to Strong 0
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	// Set c to Weak 1
	SET_BIT(DDRC, c+4); // Add 4 to reach pins 4-7 on keypad
	CLR_BIT(PORTC, c+4);
	wait_avr(5);
	
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
	/* struct Clock c = {0, 0, 0, 0, 0, 0};
	int total_seconds = 0;
	//int total_days = 0;
	
	while(1) {
		wait_avr(1000);
		total_seconds += 1;
		c = update_time(c, total_seconds);
		//if(total_seconds % 8640 == 0) { c = update_date(c, total_days); }
	} */
	
    // TESTING THE KEYPAD:
	/*while (1) {
		int k = get_key();
		
		// blink led k times
		for(int i = 0; i < k; i++) {
			SET_BIT(PORTB, 0);
			wait_avr(400);
			CLR_BIT(PORTB, 0);
			wait_avr(400);
		}
		
		// wait 2 seconds
		wait_avr(2000);
    } */
	
	// TESTING THE LCD SCREEN:
	int k;
	char buf[17];
	ini_avr();
	
	while(1) {
		k = get_key();
		pos_lcd(0,0);
		sprintf(buf, "%02i", k);
		puts_lcd2(buf);
	}
	
	return 0;
}

