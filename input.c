// input.c

#include "input.h"
// Stores current and previous state of key register
void key_poll()
{
	__key_prev = __key_curr;
	__key_curr = ~REG_KEYINPUT & KEY_MASK;	
}

// Functions for getting states of curr/previous keys
u32 key_curr_state()		{ return __key_curr; }
u32 key_prev_state()		{ return __key_prev; }
u32 key_is_down(u32 key)	{ return __key_curr & key; }
u32 key_is_up(u32 key)		{ return ~__key_curr & key; }
u32 key_was_down(u32 key)	{ return __key_prev & key; }
u32 key_was_up(u32 key)		{ return ~__key_curr & key; }

u32 key_transit(u32 key)	{ return (__key_curr ^ __key_prev) & key; }
u32 key_held(u32 key)		{ return (__key_curr & __key_prev) & key; }
u32 key_hit(u32 key)		{ return (__key_curr & ~__key_prev) & key; }
u32 key_released(u32 key)	{ return (~__key_curr & __key_prev) & key; }
void reset()                { __key_prev = 0;}
