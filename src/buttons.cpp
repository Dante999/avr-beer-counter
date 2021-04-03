#include "buttons.hpp"

#include <avr/io.h>

void buttons_c::init()
{
	DDRD &= ~((1 << PD2) | (1 << PD3));
	PORTD |= (1 << PD2) | (1 << PD3);
}

bool buttons_c::is_user_button_pressed()
{
	return !(PIND & (1 << PD3)) ? true : false;
}

bool buttons_c::is_bottle_button_pressed()
{
	return !(PIND & (1 << PD2)) ? true : false;
}
