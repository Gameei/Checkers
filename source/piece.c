#include "piece.h"
#include <stdlib.h>

//Den här funktionen används för att initiera en ny bricka och sedan returnera dess adress.
piece *create_piece(const player *p)
{
	piece *pic = malloc(sizeof(piece));//Allokerar minne för brickan.
	pic->symbol = get_uncrowned_piece_symbol(p);//Tilldelar brickan ett tecken.
	return pic;//Returnerar adressen.
}

//Den här funktionen returnerar en angiven brickas tecken.
char get_piece_symbol(const piece *pic)
{
	return pic->symbol;
}

//Den här funktionen används för att avgöra vilken spelare en bricka tillhör.
bool piece_belongs_to_player(const piece *pic, const player *p)
{
	return get_piece_symbol(pic) == get_uncrowned_piece_symbol(p) || get_piece_symbol(pic) == get_crowned_piece_symbol(p);
}

//Den här funktionen returnerar en angiven brickas status (om den är krönt eller inte).
bool piece_crowned(const piece *pic, const player *p)
{
	return get_piece_symbol(pic) == get_crowned_piece_symbol(p);
}

//Ska en bricka tas bort används den här funktionen.
void remove_piece(piece *pic)
{
	free(pic);//Frigör brickans minne.
}

/* Den här funktionen kröner en bricka. En brickas status ändras alltså från okrönt till krönt.
 * Mer exakt tilldelas den angivna brickan ett nytt tecken. */
void crown_piece(piece *pic, const player *p)
{
	pic->symbol = get_crowned_piece_symbol(p);
}