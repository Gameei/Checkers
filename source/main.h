#include "table.h"

#define GAME_NAME "Checkers"

//Denna datapost används till att hålla reda på koordinater som matas in av en användare.
typedef struct{
	u_short row;
	u_short column;
}input_coordinates;