#include "display.hpp"

#include <avr/io.h>
#include <util/delay.h>

void display_c::init()
{
	DDRB = 0xFF;
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6);

	test();
}

void display_c::off()
{
	show_segment(segment_e::SEGMENT_ALL_OFF);
	select_digit(digit_e::DIGIT_OFF);
}

void display_c::calculate_digits(uint16_t number)
{
	m_n0 = number % 10;
	m_n1 = (number / 10) % 10;
	m_n2 = (number / 100) % 10;
	m_n3 = (number / 1000) % 10;

	m_last = number;
}

void display_c::select_digit(display_c::digit_e digit)
{
	PORTB &= ~(1 << PB7);
	PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6));

	switch (digit) {
	case digit_e::DIGIT_OFF:
		break;

	case digit_e::DIGIT_1:
		PORTB |= (1 << PB7);
		break;

	case digit_e::DIGIT_2:
		PORTD |= (1 << PD4);
		break;

	case digit_e::DIGIT_3:
		PORTD |= (1 << PD5);
		break;

	case digit_e::DIGIT_4:
		PORTD |= (1 << PD6);
		break;
	}
}

void display_c::show_segment(display_c::segment_e segment)
{

	switch (segment) {
	case segment_e::SEGMENT_ALL_OFF:
		PORTB &= 0x80;
		break;
	case segment_e::SEGMENT_A:
		PORTB |= (1 << PB0);
		break;
	case segment_e::SEGMENT_B:
		PORTB |= (1 << PB1);
		break;
	case segment_e::SEGMENT_C:
		PORTB |= (1 << PB2);
		break;
	case segment_e::SEGMENT_D:
		PORTB |= (1 << PB3);
		break;
	case segment_e::SEGMENT_E:
		PORTB |= (1 << PB4);
		break;
	case segment_e::SEGMENT_F:
		PORTB |= (1 << PB5);
		break;
	case segment_e::SEGMENT_G:
		PORTB |= (1 << PB6);
		break;
	}
}

void display_c::display_single_number(uint8_t digit)
{

	show_segment(segment_e::SEGMENT_ALL_OFF);

	switch (digit) {
	case 0:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		show_segment(segment_e::SEGMENT_E);
		show_segment(segment_e::SEGMENT_F);
		break;
	case 1:
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		break;
	case 2:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_G);
		show_segment(segment_e::SEGMENT_E);
		show_segment(segment_e::SEGMENT_D);
		break;
	case 3:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_G);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		break;
	case 4:
		show_segment(segment_e::SEGMENT_F);
		show_segment(segment_e::SEGMENT_G);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		break;
	case 5:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_F);
		show_segment(segment_e::SEGMENT_G);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		break;
	case 6:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_F);
		show_segment(segment_e::SEGMENT_G);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		show_segment(segment_e::SEGMENT_E);
		break;
	case 7:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		break;
	case 8:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		show_segment(segment_e::SEGMENT_E);
		show_segment(segment_e::SEGMENT_F);
		show_segment(segment_e::SEGMENT_G);
		break;
	case 9:
		show_segment(segment_e::SEGMENT_A);
		show_segment(segment_e::SEGMENT_B);
		show_segment(segment_e::SEGMENT_C);
		show_segment(segment_e::SEGMENT_D);
		show_segment(segment_e::SEGMENT_F);
		show_segment(segment_e::SEGMENT_G);
		break;
	}
}

void display_c::show(uint16_t number)
{

	if (number != m_last) {
		calculate_digits(number);
	}

	select_digit(digit_e::DIGIT_1);
	display_single_number(m_n3);
	off();

	select_digit(digit_e::DIGIT_2);
	display_single_number(m_n2);
	off();

	select_digit(digit_e::DIGIT_3);
	display_single_number(m_n1);
	off();

	select_digit(digit_e::DIGIT_4);
	display_single_number(m_n0);
	off();
}

void display_c::test()
{
	auto fnc_iterate_segments = [&] {
		constexpr double delay_ms = 20;

		show_segment(segment_e::SEGMENT_A);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_B);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_C);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_D);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_E);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_F);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_G);
		_delay_ms(delay_ms);

		show_segment(segment_e::SEGMENT_ALL_OFF);
	};

	select_digit(digit_e::DIGIT_1);
	fnc_iterate_segments();

	select_digit(digit_e::DIGIT_2);
	fnc_iterate_segments();

	select_digit(digit_e::DIGIT_3);
	fnc_iterate_segments();

	select_digit(digit_e::DIGIT_4);
	fnc_iterate_segments();
}
