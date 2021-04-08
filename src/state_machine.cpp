#include "state_machine.hpp"

#include <avr/sleep.h>

#include "buttons.hpp"
#include "display.hpp"
#include "storage.hpp"

namespace {
using cycles_t = uint16_t;

constexpr cycles_t MAX_CYCLES_SHOW_COUNTER     = 10000;
constexpr cycles_t MAX_CYCLES_INCREASE_DECRASE = 1000;

enum state_e {
	STATE_STANDBY,
	STATE_INCREASE_COUNTER,
	STATE_DECREASE_COUNTER,
	STATE_SHOW_CHANGED_COUNTER,
	STATE_SHOW_CURRENT_COUNTER,
	STATE_SHOW_ACHIEVEMENT
};

display_c m_display;
buttons_c m_buttons;
uint16_t  m_counter;
state_e   m_state;

} // namespace

void state_machine_c::standby()
{
	if (m_buttons.is_user_button_pressed()) {
		m_state   = state_e::STATE_SHOW_CURRENT_COUNTER;
		m_counter = storage_c::load_counter();
	}
	else if (m_buttons.is_bottle_button_pressed()) {
		m_state   = state_e::STATE_INCREASE_COUNTER;
		m_counter = storage_c::load_counter();
	}

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
}

void state_machine_c::show_current_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		m_display.show(m_counter);
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = state_e::STATE_STANDBY;
	}
}

void state_machine_c::show_changed_counter()
{
	static cycles_t cycles = 0;

	if (m_buttons.is_bottle_button_pressed()) {
		m_state = state_e::STATE_INCREASE_COUNTER;
		cycles  = 0;
	}
	else if (m_buttons.is_user_button_pressed()) {
		m_state = state_e::STATE_DECREASE_COUNTER;
		cycles  = 0;
	}
	else if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		m_display.show(m_counter);
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = state_e::STATE_STANDBY;
		storage_c::save_counter(m_counter);
	}
}

void state_machine_c::show_achievement()
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
		m_display.show('B', 'E', 'E', 'R');
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = state_e::STATE_SHOW_CHANGED_COUNTER;

		if (m_counter > 9999) {
			m_counter = 0;
		}
	}
}

void state_machine_c::increase_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_INCREASE_DECRASE) {

		if (!m_buttons.is_bottle_button_pressed()) {
			++cycles;
		}

		m_display.off();
	}
	else {
		cycles = 0;
		++m_counter;

		if (m_counter % 100 == 0) {
			m_state = state_e::STATE_SHOW_ACHIEVEMENT;
		}
		else {
			m_state = state_e::STATE_SHOW_CHANGED_COUNTER;
		}
	}
}

void state_machine_c::decrease_counter()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_INCREASE_DECRASE) {

		if (!m_buttons.is_user_button_pressed()) {
			++cycles;
		}

		m_display.off();
	}
	else {
		cycles = 0;

		if (m_counter > 0) {
			--m_counter;
		}

		m_state = state_e::STATE_SHOW_CHANGED_COUNTER;
	}
}

void state_machine_c::init()
{
	storage_c::init();
	m_display.init();
	m_buttons.init();
	m_state   = state_e::STATE_STANDBY;
	m_counter = 0;
}

void state_machine_c::run()
{
	switch (m_state) {

	case state_e::STATE_STANDBY:
		standby();
		break;
	case state_e::STATE_INCREASE_COUNTER:
		increase_counter();
		break;
	case state_e::STATE_SHOW_CHANGED_COUNTER:
		show_changed_counter();
		break;
	case state_e::STATE_SHOW_ACHIEVEMENT:
		show_achievement();
		break;
	case state_e::STATE_DECREASE_COUNTER:
		decrease_counter();
		break;
	case state_e::STATE_SHOW_CURRENT_COUNTER:
		show_current_counter();
		break;
	}
}
