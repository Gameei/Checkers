#include "square.h"
#include <stdio.h>

//Den här funktionen används för att initiera en ny bricka.
square new_square(piece *pic)
{
	square s;
	s.pic = pic;//Rutan tilldelas en bricka eller bara NULL. 
	return s;
}

//Den här funktionen returnerar en rutas bricka.
const piece *get_square_piece(const square *s)
{
	return s->pic;
}

//Ska en rutas bricka tas bort körs den här funktionen.
void remove_square_piece(square *s)
{
	remove_piece(s->pic);//Tar bort rutans bricka.
	s->pic = NULL;//Markerar att rutan är tom.
}

//Denna funktion används för att flytta en rutas bricka till en annan ruta.
void move_square_piece(square *ori_s, square *dest_s)
{
	dest_s->pic = ori_s->pic;//Flyttar brickan.
	ori_s->pic = NULL;//Rutan som brickan flyttades från är nu tom.
}

//Denna funktion returnerar true om rutan är tom.
bool square_empty(const square *s)
{
	return get_square_piece(s) == NULL;
}

//Denna funktion kontrollerar om en ruta innehåller en bricka som tillhör en viss spelare.
bool square_contains_player_piece(const square *s, const player *p)
{
	//Kontrollerar först om en ruta på angiven rad är tom. Är den inte tom kontrolleras sedan om brickan i rutan tillhör angiven spelare.
	return square_empty(s) == FALSE && piece_belongs_to_player(get_square_piece(s), p);
}

//Den här funktionen skriver ut en ruta (inklusive en eventuell bricka).
void print_square(const square *s)
{
	//Är rutan inte tom skrivs brickans symbol ut. Annars skrivs ett mellanrum ut.
	printf("| %c ", square_empty(s) == FALSE ? get_piece_symbol(get_square_piece(s)) : EMPTY);
}
