#ifndef STORAGE_C_HPP
#define STORAGE_C_HPP

#include <stdint.h>

class storage_c {

public:
	storage_c() = delete;

	static void     init();
	static uint16_t load_counter();
	static void     save_counter(uint16_t value);
};

#endif // STORAGE_C_HPP
