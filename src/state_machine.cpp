#include "state_machine.hpp"

#include <avr/sleep.h>

#include "buttons.hpp"
#include "display.hpp"

namespace {
using cycles_t = uint16_t;

constexpr cycles_t MAX_CYCLES_SHOW_COUNTER     = 10000;
constexpr cycles_t MAX_CYCLES_INCREASE_DECRASE = 1000;

constexpr cycles_t MAX_CYCLES_BLINK = 2000;

enum state_e {
	STATE_STANDBY,
	STATE_INCREASE,
	STATE_SHOW_CHANGED_COUNTER,
	STATE_DECREASE,
	STATE_SHOW_CURRENT_COUNTER,
	STATE_SHOW_ACHIEVEMENT
};

display_c m_display;
buttons_c m_buttons;
uint16_t  m_counter;
state_e   m_state;

} // namespace

void state_machine_c::blink_counter_value()
{
	m_display.show(m_counter);

	//	static uint16_t temp = 0;
	//	static bool     show = true;

	//	if (temp < MAX_CYCLES_BLINK) {

	//		if (show) {
	//			m_display.show(m_counter);
	//		}
	//		else {
	//			m_display.off();
	//		}

	//		++temp;
	//	}
	//	else {
	//		show = !show;
	//		temp = 0;
	//	}
}

void state_machine_c::handle_state_standby()
{
	if (m_buttons.is_user_button_pressed()) {
		m_state = state_e::STATE_SHOW_CURRENT_COUNTER;
	}
	else if (m_buttons.is_bottle_button_pressed()) {
		m_state = state_e::STATE_INCREASE;
	}

	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode(); // in den Schlafmodus wechseln
}

void state_machine_c::handle_state_show_current()
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

void state_machine_c::handle_state_show_changed()
{
	static cycles_t cycles = 0;

	if (m_buttons.is_bottle_button_pressed()) {
		m_state = state_e::STATE_INCREASE;
		cycles  = 0;
	}
	else if (m_buttons.is_user_button_pressed()) {
		m_state = state_e::STATE_DECREASE;
		cycles  = 0;
	}
	else if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		blink_counter_value();
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = state_e::STATE_STANDBY;
	}
}

void state_machine_c::handle_state_show_achievement()
{
	static cycles_t cycles = 0;

	if (cycles < MAX_CYCLES_SHOW_COUNTER) {
		m_display.show('B', 'E', 'E', 'R');
		++cycles;
	}
	else {
		cycles  = 0;
		m_state = state_e::STATE_SHOW_CHANGED_COUNTER;
	}
}

void state_machine_c::handle_state_increase()
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

void state_machine_c::handle_state_decrease()
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
		--m_counter;
		m_state = state_e::STATE_SHOW_CHANGED_COUNTER;
	}
}

void state_machine_c::init()
{
	m_display.init();
	m_buttons.init();
	m_state   = state_e::STATE_STANDBY;
	m_counter = 0;
}

void state_machine_c::run()
{
	switch (m_state) {

	case state_e::STATE_STANDBY:
		handle_state_standby();
		break;
	case state_e::STATE_INCREASE:
		handle_state_increase();
		break;
	case state_e::STATE_SHOW_CHANGED_COUNTER:
		handle_state_show_changed();
		break;
	case state_e::STATE_SHOW_ACHIEVEMENT:
		handle_state_show_achievement();
		break;
	case state_e::STATE_DECREASE:
		handle_state_decrease();
		break;
	case state_e::STATE_SHOW_CURRENT_COUNTER:
		handle_state_show_current();
		break;
	}
}
