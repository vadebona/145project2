/*
 * Project2.c
 *
 * Created: 4/23/2018 5:40:03 PM
 * Author : Vicky Adebona
 */ 

#include <avr/io.h>
#include "avr.c"
#include "lcd.c"


struct Clock {
	int year;
	int month;
	int day;
	
	int hour;
	int minute;
	int second;

};

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
}

int main()
{
    while (1) {
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
    }
	
	return 0;
}

