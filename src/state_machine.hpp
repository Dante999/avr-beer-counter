#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <stdint.h>

class state_machine_c {

private:
	static void blink_counter_value();

	static void handle_state_standby();
	static void handle_state_increase();
	static void handle_state_decrease();
	static void handle_state_show_current();
	static void handle_state_show_changed();
	static void handle_state_show_achievement();

public:
	static void init();
	static void run();
	static void on_user_button_pressed();
	static void on_bottle_button_pressed();
};

#endif // STATE_MACHINE_HPP
