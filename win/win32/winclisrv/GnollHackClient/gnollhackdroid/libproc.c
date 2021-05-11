

#include "hack.h"

struct window_procs lib_procs = {
	"LIB" };


#ifdef USER_SOUNDS

void play_usersound(const char* filename, int volume)
{
	return;
}

#endif

void set_username()
{
	strcpy(plname, "Adventurer");
}

