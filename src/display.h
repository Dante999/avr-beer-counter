#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();
void display_off();
void display_show_number(uint16_t number);
void display_show_text(char a, char b, char c, char d);
void display_show_segment_test();

#endif // DISPLAY_H
