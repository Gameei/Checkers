#include "piece.h"

#define EMPTY ' '

/* Den här dataposten agerar som en ruta och varje ruta har möjligheten att innehålla en bricka.
 * Finns ingen bricka i en ruta är den tom (dess pekare pekar på NULL). */
typedef struct{
	piece *pic; 
}square;

//Funktionsprototyper.
square new_square(piece *pic);
const piece *get_square_piece(const square *s);
void remove_square_piece(square *s);
void move_square_piece(square *ori_s, square *dest_s);
bool square_empty(const square *s);
bool square_contains_player_piece(const square *s, const player *p);
void print_square(const square *s);