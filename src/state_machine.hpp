#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <stdint.h>

class state_machine_c {

private:
	static void standby();
	static void increase_counter();
	static void decrease_counter();
	static void show_current_counter();
	static void show_changed_counter();
	static void show_achievement();

public:
	static void init();
	static void run();
};

#endif // STATE_MACHINE_HPP
