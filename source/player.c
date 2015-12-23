#include "player.h"

//Den här funktionen returnerar spelarens namn.
const char *get_player_name(const player *p)
{
	return p->name;
}

//Den här funktionen returnerar tecknet som den angivna spelarens okrönta brickor har.
char get_uncrowned_piece_symbol(const player *p)
{
	return p->uncrowned_piece_symbol;
}

//Den här funktionen returnerar tecknet som den angivna spelarens krönta brickor har.
char get_crowned_piece_symbol(const player *p)
{
	return p->crowned_piece_symbol;
}

//Den här funktion används för att kontrollera vilken spelares tur det är.
bool player_turn(const player *p)
{
	return p->turn;
}

//Den här funktionen används för att kontrollera om angiven spelare har vunnit.
bool player_winner(const player *p)
{
	return p->winner;
}

//När en spelare har spelat klart en runda så är det nästa spelares tur. Den här funktion ändrar värdet på båda spelarnas turn-variabel.
void swap_turn(player *p1, player *p2)
{
	if(player_turn(p1)){//Är det spelare 1 som precis har spelat klart en runda? Då är det spelare 2:s tur den kommande rundan.
		p1->turn = FALSE;
		p2->turn = TRUE;
	}else{//Annars är det spelare 1:s tur.
		p2->turn = FALSE;
		p1->turn = TRUE;
	}
}

//Den här funktion sätter angiven spelare till vinnare.
void set_winner(player *p)
{
	p->winner = TRUE;
}

//Ska ett nytt spel påbörjas återställs spelarnas variabler (turn och winner).
void reset_players(player *p1, player *p2)
{
	p1->turn = TRUE;
	p2->turn = FALSE;
	p1->winner = FALSE;
	p2->winner = FALSE;
}
