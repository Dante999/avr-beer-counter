#include "storage.hpp"

#include <avr/eeprom.h>

namespace {
uint8_t m_is_initialized EEMEM = 0;
uint16_t m_counter1      EEMEM = 0;
uint16_t m_counter2      EEMEM = 0;
} // namespace

void storage_c::init()
{
	if (eeprom_read_byte(&m_is_initialized) != 1) {
		eeprom_write_byte(&m_is_initialized, 1);
		save_counter(0);
	}
}

uint16_t storage_c::load_counter()
{
	const auto c1 = eeprom_read_word(&m_counter1);
	const auto c2 = eeprom_read_word(&m_counter2);

	if (c1 == c2) {
		return c1;
	}
	else {
		// the chance is higher that a corrupted value is greater than the real counter
		// even if the maximum of 9999 beer is reached, there are still 2^16-9999 corrupted values left.
		return (c1 < c2) ? c1 : c2;
	}
}

void storage_c::save_counter(uint16_t value)
{
	eeprom_update_word(&m_counter1, value);
	eeprom_update_word(&m_counter2, value);
}
