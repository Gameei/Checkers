#include "player.h"

//Den här dataposten ska föreställa en spelbricka.
typedef struct{
	//Varje bricka har ett tecken som används när brickan ska skrivas ut. Tecknet används även för att avgöra vilken spelare brickan tillhör.
	char symbol;
}piece;

//Funktionsprototyper.
piece *create_piece(const player *p);
char get_piece_symbol(const piece *pic);
bool piece_belongs_to_player(const piece *pic, const player *p);
bool piece_crowned(const piece *pic, const player *p);
void remove_piece(piece *pic);
void crown_piece(piece *pic, const player *p);
