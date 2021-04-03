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

	uint16_t m_last{0};
	uint8_t  m_n0{0};
	uint8_t  m_n1{0};
	uint8_t  m_n2{0};
	uint8_t  m_n3{0};

private:
	inline void turn_segments_off();

	void calculate_digits(uint16_t number);
	void select_digit(digit_e digit);
	void show_segments_for_digit(uint8_t digit);
	void show_segments_for_char(char c);

public:
	void init();
	void off();
	void show(uint16_t number);
	void show(char a, char b, char c, char d);
	void test();
};

#endif // DISPLAY_HPP
