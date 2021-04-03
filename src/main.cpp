

#include <avr/io.h>
#include <util/delay.h>

#include "display.hpp"

void loop_segments() {}

int main(void)
{

	display_c d;

	d.init();

	while (1) {

		for (uint16_t i = 0; i < 999; ++i) {

			for (uint8_t j = 0; j < 255; ++j) {
				d.show(i);
			}
		}

		//_delay_ms(100);
	}

	//	DDRB |= 0xFF;
	//	DDRD |= (1 << PD6);
	//	PORTD |= (1 << PD6);

	//	//	PORTB |= (1<<PB7);

	//	while (1) {

	//		for (uint8_t i = 0; i < 7; ++i) {
	//			PORTB = 0x00;
	//			PORTB |= (1 << i);
	//			_delay_ms(100);
	//		}
	//	}
}
