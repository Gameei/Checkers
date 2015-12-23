#include "utility.h"

//Denna datapost ska representerar en spelare.
typedef struct{
	char name[16];//Varje spelare har ett namn.
	char uncrowned_piece_symbol, crowned_piece_symbol;//Dessa tecken anger hur spelaren brickor kommer att se ut.
	bool turn, winner;//Är det den här spelarens tur? Har den här spelaren vunnit? Det håller dessa variabler reda på.
}player;

//Funktionsprototyper.
const char *get_player_name(const player *p);
char get_uncrowned_piece_symbol(const player *p);
char get_crowned_piece_symbol(const player *p);
bool player_turn(const player *p);
bool player_winner(const player *p);
void swap_turn(player *p1, player *p2);
void set_winner(player *p);
void reset_players(player *p1, player *p2);
