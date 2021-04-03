#include "display.hpp"

#include <avr/io.h>
#include <util/delay.h>

#define DISPLAY_PORTX PORTB

namespace {
constexpr uint8_t SEG_A(1 << PB0);
constexpr uint8_t SEG_B(1 << PB1);
constexpr uint8_t SEG_C(1 << PB2);
constexpr uint8_t SEG_D(1 << PB3);
constexpr uint8_t SEG_E(1 << PB4);
constexpr uint8_t SEG_F(1 << PB5);
constexpr uint8_t SEG_G(1 << PB6);
} // namespace

void display_c::init()
{
	DDRB = 0xFF;
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6);

	test();
}

void display_c::off()
{
	turn_segments_off();
	select_digit(digit_e::DIGIT_OFF);
}

void display_c::turn_segments_off()
{
	DISPLAY_PORTX &= ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);
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

void display_c::show_segments_for_digit(uint8_t digit)
{

	DISPLAY_PORTX &= ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);

	switch (digit) {
	case 0:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
		break;
	case 1:
		DISPLAY_PORTX |= SEG_B | SEG_C;
		break;
	case 2:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_G | SEG_E | SEG_D;
		break;
	case 3:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_G | SEG_C | SEG_D;
		break;
	case 4:
		DISPLAY_PORTX |= SEG_F | SEG_G | SEG_B | SEG_C;
		break;
	case 5:
		DISPLAY_PORTX |= SEG_A | SEG_F | SEG_G | SEG_C | SEG_D;
		break;
	case 6:
		DISPLAY_PORTX |= SEG_A | SEG_F | SEG_G | SEG_C | SEG_D | SEG_E;
		break;
	case 7:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_C;
		break;
	case 8:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
		break;
	case 9:
		DISPLAY_PORTX |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;
		break;
	default:
		DISPLAY_PORTX |= SEG_A | SEG_G | SEG_D;
		break;
	}
}

void display_c::show(uint16_t number)
{

	if (number != m_last) {
		calculate_digits(number);
	}

	select_digit(digit_e::DIGIT_1);
	show_segments_for_digit(m_n3);
	off();

	select_digit(digit_e::DIGIT_2);
	show_segments_for_digit(m_n2);
	off();

	select_digit(digit_e::DIGIT_3);
	show_segments_for_digit(m_n1);
	off();

	select_digit(digit_e::DIGIT_4);
	show_segments_for_digit(m_n0);
	off();
}

void display_c::test()
{
	auto fnc_iterate_segments = [&] {
		constexpr double delay_ms = 20;

		turn_segments_off();
		DISPLAY_PORTX |= SEG_A;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_B;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_C;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_D;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_E;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_F;
		_delay_ms(delay_ms);

		turn_segments_off();
		DISPLAY_PORTX |= SEG_G;
		_delay_ms(delay_ms);
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
