#include <display.h>

#include <avr/io.h>
#include <util/delay.h>

#define SEG_PORT PORTB
#define SEG_A    (1 << PB0)
#define SEG_B    (1 << PB1)
#define SEG_C    (1 << PB2)
#define SEG_D    (1 << PB3)
#define SEG_E    (1 << PB4)
#define SEG_F    (1 << PB5)
#define SEG_G    (1 << PB6)

#define DIGIT1_PORT PORTB
#define DIGIT2_PORT PORTD
#define DIGIT3_PORT PORTD
#define DIGIT4_PORT PORTD

#define DIGIT1_BIT (1 << PB7)
#define DIGIT2_BIT (1 << PD4)
#define DIGIT3_BIT (1 << PD5)
#define DIGIT4_BIT (1 << PD6)

enum digit_e { //
	DIGIT_OFF,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4
};

static uint16_t m_last = 0;
static int8_t   m_n0   = 0;
static int8_t   m_n1   = 0;
static int8_t   m_n2   = 0;
static int8_t   m_n3   = 0;

static inline void turn_segments_off()
{
	SEG_PORT &= ~(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);
}

static void calculate_digits(uint16_t number)
{
	m_n0 = number % 10;
	m_n1 = (number / 10) % 10;
	m_n2 = (number / 100) % 10;
	m_n3 = (number / 1000) % 10;

	m_last = number;
}

static void iterate_over_all_segments()
{
	const double delay_ms = 20;

	turn_segments_off();
	SEG_PORT |= SEG_A;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_B;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_C;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_D;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_E;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_F;
	_delay_ms(delay_ms);

	turn_segments_off();
	SEG_PORT |= SEG_G;
	_delay_ms(delay_ms);
}

static void select_digit(enum digit_e digit)
{
	DIGIT1_PORT &= ~DIGIT1_BIT;
	DIGIT2_PORT &= ~DIGIT2_BIT;
	DIGIT3_PORT &= ~DIGIT3_BIT;
	DIGIT4_PORT &= ~DIGIT4_BIT;

	switch (digit) {

	case DIGIT_OFF:
		break;

	case DIGIT_1:
		DIGIT1_PORT |= DIGIT1_BIT;
		break;

	case DIGIT_2:
		DIGIT2_PORT |= DIGIT2_BIT;
		break;

	case DIGIT_3:
		DIGIT3_PORT |= DIGIT3_BIT;
		break;

	case DIGIT_4:
		DIGIT4_PORT |= DIGIT4_BIT;
		break;
	}
}

static void show_segments_for_digit(int8_t digit)
{
	turn_segments_off();

	switch (digit) {
	case -1:
		turn_segments_off();
		break;
	case 0:
		SEG_PORT |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
		break;
	case 1:
		SEG_PORT |= SEG_B | SEG_C;
		break;
	case 2:
		SEG_PORT |= SEG_A | SEG_B | SEG_G | SEG_E | SEG_D;
		break;
	case 3:
		SEG_PORT |= SEG_A | SEG_B | SEG_G | SEG_C | SEG_D;
		break;
	case 4:
		SEG_PORT |= SEG_F | SEG_G | SEG_B | SEG_C;
		break;
	case 5:
		SEG_PORT |= SEG_A | SEG_F | SEG_G | SEG_C | SEG_D;
		break;
	case 6:
		SEG_PORT |= SEG_A | SEG_F | SEG_G | SEG_C | SEG_D | SEG_E;
		break;
	case 7:
		SEG_PORT |= SEG_A | SEG_B | SEG_C;
		break;
	case 8:
		SEG_PORT |=
		        SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
		break;
	case 9:
		SEG_PORT |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;
		break;
	default:
		SEG_PORT |= SEG_A | SEG_G | SEG_D;
		break;
	}
}

static void show_segments_for_char(char c)
{
	turn_segments_off();

	switch (c) {
	case ' ':
		turn_segments_off();
		break;
	case 'B':
		SEG_PORT |= SEG_F | SEG_E | SEG_D | SEG_C | SEG_G;
		break;
	case 'E':
		SEG_PORT |= SEG_A | SEG_F | SEG_G | SEG_E | SEG_D;
		break;
	case 'I':
		SEG_PORT |= SEG_F | SEG_E;
		break;
	case 'R':
		SEG_PORT |= SEG_E | SEG_G;
		break;
	default:
		SEG_PORT |= SEG_A | SEG_G | SEG_D;
		break;
	}
}

void display_init()
{
	DDRB = 0xFF;
	DDRD |= (1 << PD4) | (1 << PD5) | (1 << PD6);

	display_show_segment_test();
}

void display_off()
{
	turn_segments_off();
	select_digit(DIGIT_OFF);
}

void display_show_number(uint16_t number)
{

	if (number != m_last) {
		calculate_digits(number);
	}

	select_digit(DIGIT_1);
	show_segments_for_digit(m_n3);
	display_off();

	select_digit(DIGIT_2);
	show_segments_for_digit(m_n2);
	display_off();

	select_digit(DIGIT_3);
	show_segments_for_digit(m_n1);
	display_off();

	select_digit(DIGIT_4);
	show_segments_for_digit(m_n0);
	display_off();
}

void display_show_text(char a, char b, char c, char d)
{
	select_digit(DIGIT_1);
	show_segments_for_char(a);
	display_off();

	select_digit(DIGIT_2);
	show_segments_for_char(b);
	display_off();

	select_digit(DIGIT_3);
	show_segments_for_char(c);
	display_off();

	select_digit(DIGIT_4);
	show_segments_for_char(d);
	display_off();
}

void display_show_segment_test()
{

	select_digit(DIGIT_1);
	iterate_over_all_segments();

	select_digit(DIGIT_2);
	iterate_over_all_segments();

	select_digit(DIGIT_3);
	iterate_over_all_segments();

	select_digit(DIGIT_4);
	iterate_over_all_segments();

	display_off();
}
