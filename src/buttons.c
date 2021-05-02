#include "buttons.h"

#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/io.h>

void buttons_init()
{
	DDRD &= ~((1 << PD2) | (1 << PD3));
	PORTD |= (1 << PD2) | (1 << PD3);

	MCUCR |= (1 << ISC11); // falling edge on INT1 generates interrupt
	MCUCR |= (1 < ISC01);  // falling edge on INT0 generates interrupt

	GIMSK |= (1 << INT1);
	GIMSK |= (1 << INT0);
}

ISR(INT1_vect)
{
	// only used for waking up the controller...
}

ISR(INT0_vect)
{
	// only used for waking up the controller...
}

bool buttons_is_user_button_pressed()
{
	return !(PIND & (1 << PD3)) ? true : false;
}

bool buttons_is_bottle_button_pressed()
{
	return !(PIND & (1 << PD2)) ? true : false;
}
