#include "state_machine.h"

#include <avr/sleep.h>

#include "buttons.h"
#include "display.h"
#include "storage.h"

typedef uint16_t cycles_t;

#define MAX_CYCLES_SHOW_COUNTER ((cycles_t)10000)
#define MAX_CYCLES_INCREASE     ((cycles_t)2000)
#define MAX_CYCLES_DECREASE     ((cycles_t)1000)

enum state_e {
	STATE_STANDBY,
	STATE_INCREASE_COUNTER,
	STATE_DECREASE_COUNTER,
	STATE_SHOW_CHANGED_COUNTER,
	STATE_SHOW_CURRENT_COUNTER,
	STATE_SHOW_ACHIEVEMENT
};

uint16_t     m_counter;
enum state_e m_state;
int8_t       m_diff_since_wakeup;

static void standby()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();

	m_diff_since_wakeup = 0;

	if (buttons_is_user_button_pressed()) {
		m_state   = STATE_SHOW_CURRENT_COUNTER;
		m_counter = storage_load_counter();
	}
	else if (buttons_is_bottle_button_pressed()) {
		m_state   = STATE_INCREASE_COUNTER;
		m_counter = storage_load_counter();
	}
}

static void show_current_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		display_show_number(m_counter);
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = STATE_STANDBY;
	}
}

static void show_changed_counter()
{
	static cycles_t cycles = 0;

	if (buttons_is_bottle_button_pressed()) {
		m_state = STATE_INCREASE_COUNTER;
		cycles  = 0;
	}
	else if (buttons_is_user_button_pressed()) {
		m_state = STATE_DECREASE_COUNTER;
		cycles  = 0;
	}
	else if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		display_show_number(m_counter);
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = STATE_STANDBY;
		storage_save_counter(m_counter);
	}
}

static void show_achievement()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		/*
		 * Ideas:
		 * 10'000: WHAT THE FUCK (slideshow)
		 * 09'000: GOGO
		 * 08'000: NICE
		 * 07'000: MORE
		 */
		display_show_text('B', 'E', 'E', 'R');
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = STATE_SHOW_CHANGED_COUNTER;

		if (m_counter > 9999) {
			m_counter = 0;
		}
	}
}

static void increase_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_INCREASE) {

		if (!buttons_is_bottle_button_pressed()) {
			++cycles;
		}

		display_show_number(m_counter);
	}
	else {
		cycles = 0;
		++m_counter;
		++m_diff_since_wakeup;

		if (m_counter % 100 == 0) {
			m_state = STATE_SHOW_ACHIEVEMENT;
		}
		else {
			m_state = STATE_SHOW_CHANGED_COUNTER;
		}
	}
}

static void decrease_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_DECREASE) {

		if (!buttons_is_user_button_pressed()) {
			++cycles;
		}

		display_show_number(m_counter);
	}
	else {
		cycles = 0;

		if (m_counter > 0 && m_diff_since_wakeup > 0) {
			--m_counter;
			--m_diff_since_wakeup;
		}

		m_state = STATE_SHOW_CHANGED_COUNTER;
	}
}

void state_machine_init()
{
	storage_init();
	display_init();
	buttons_init();

	m_state   = STATE_STANDBY;
	m_counter = 0;
}

void state_machine_run()
{
	switch (m_state) {

	case STATE_STANDBY:
		standby();
		break;
	case STATE_INCREASE_COUNTER:
		increase_counter();
		break;
	case STATE_SHOW_CHANGED_COUNTER:
		show_changed_counter();
		break;
	case STATE_SHOW_ACHIEVEMENT:
		show_achievement();
		break;
	case STATE_DECREASE_COUNTER:
		decrease_counter();
		break;
	case STATE_SHOW_CURRENT_COUNTER:
		show_current_counter();
		break;
	}
}
