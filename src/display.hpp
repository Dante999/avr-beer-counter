#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <stdint.h>

class display_c {
private:
	enum class digit_e { //
		DIGIT_OFF,
		DIGIT_1,
		DIGIT_2,
		DIGIT_3,
		DIGIT_4
	};

	enum class segment_e { //
		SEGMENT_ALL_OFF,
		SEGMENT_A,
		SEGMENT_B,
		SEGMENT_C,
		SEGMENT_D,
		SEGMENT_E,
		SEGMENT_F,
		SEGMENT_G
	};

	uint16_t m_last;
	uint8_t  m_n0{0};
	uint8_t  m_n1{0};
	uint8_t  m_n2{0};
	uint8_t  m_n3{0};

private:
	void calculate_digits(uint16_t number);
	void select_digit(digit_e digit);
	void show_segment(segment_e segment);
	void display_single_number(uint8_t digit);

public:
	void init();
	void off();
	void show(uint16_t number);
	void test();
};

#endif // DISPLAY_HPP
