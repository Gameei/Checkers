#include "table.h"
#include <stdio.h>

/* Denna funktion används för att initiera en ny ruta på ett angivet spelbräde. Rutan kan
 * antingen initieras med en bricka eller utan en bricka. För att rutan ska kunna initieras
 * med en bricka får inte player-pekaren peka på NULL. 
 * För övrigt är funktionen deklaread som static då jag vill att den här funktionen bara ska vara 
 * tillgänglig inom den här källkodfilen (samma sak med de andra funktionerna nedan som har deklarerats med static). */
static void init_table_square(table *t, const u_short row, const u_short column, const player *p)
{
	t->squares[row][column] = new_square(p != NULL ? create_piece(p) : NULL);
}

//Den här funktionen initierar ett spelbräde.
static void init_table(table *t, const player *p1, const player *p2)
{
	u_short row, column;
	const char P1_SYMBOL = get_uncrowned_piece_symbol(p1), P2_SYMBOL = get_uncrowned_piece_symbol(p2);

	//Denna array innehåller endast tecken och anger bara hur ett spelbräde ska se ut när ett nytt spel påbörjas.
	const char DEFAULT_SQUARES[TABLE_ROW_SIZE][TABLE_COLUMN_SIZE] =
	{{P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY},
	{EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL},
	{P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY, P1_SYMBOL, EMPTY},
	{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
	{EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL},
	{P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY},
	{EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL, EMPTY, P2_SYMBOL}};

	//Initierar alla rutor på spelbrädet med hjälp av arrayen ovan som läses av.
	for(row = 0; row < TABLE_ROW_SIZE; row++)
		for(column = 0; column < TABLE_COLUMN_SIZE; column++)
			if(DEFAULT_SQUARES[row][column] == P1_SYMBOL)//Matchar tecknet spelare 1:s tecken? Då initieras denna ruta med en bricka som tillhör spelare 1.
				init_table_square(t, row, column, p1);
			else if(DEFAULT_SQUARES[row][column] == P2_SYMBOL)//Matchar tecknet spelare 2:s tecken? Då initieras denna ruta med en bricka som tillhör spelare 2.
				init_table_square(t, row, column, p2);
			else//Matchar tecknet varken spelare 1:s eller spelare 2:s tecken? Då initieras denna rutan som en tom ruta.
				init_table_square(t, row, column, NULL);
}

//Denna funktion tar bort alla brickor från ett spelbräde.
static void clean_table(table *t)
{
	u_short row, column;
	for(row = 0; row < TABLE_ROW_SIZE; row++)
		for(column = 0; column < TABLE_COLUMN_SIZE; column++)
			if(square_empty(get_table_square(t, row, column)))//Är rutan inte tom? Då ska dess bricka tas bort.
				remove_square_piece(&t->squares[row][column]);//Tar bort rutans bricka.
}

//Denna funktion skriver ut en horisontell linje.
static void print_horizontal_line()
{
	u_short column;
	for(column = 0; column < TABLE_COLUMN_SIZE; column++)
		printf("%s", column == 0 ? "  +---" : "+---");//Är det den första delen av linjen som skrivs ut? Då ska två mellanrum först skrivas ut för att ge plats åt radnumret.
	printf("+\n");
}

//Denna funktion skriver ut ett radnummer.
static void print_row_number(const u_short row)
{
	printf("%hu ", row + 1);
}

//Den här funktionen skriver ut en rads alla rutor.
static void print_squares(const table *t, const u_short row)
{
	u_short column;
	for(column = 0; column < TABLE_COLUMN_SIZE; column++)
		print_square(get_table_square(t, row, column));//Skriver ut ruta efter ruta.
	printf("|\n");
}

//Denna funktion skriver ut kolumnnummret för varje kolumn.
static void print_column_number()
{
	u_short column;
	for(column = 0; column < TABLE_COLUMN_SIZE; column++)
		printf("%s%hu%c", column == 0 ? "    " : "  ", column + 1, ' ');//Är det det första kolumnnumret som skrivs ut ska två extra mellanrum först skrivas ut.
	printf("\n\n");
}

//Den här funktionen returnerar ett initierat spelbräde.
table new_table(const player *p1, const player *p2)
{
	table t;
 	init_table(&t, p1, p2);
	return t;
}

//Den här funktionen returnerar en angiven ruta.
const square *get_table_square(const table *t, const u_short row, const u_short column)
{
	return &t->squares[row][column];
}

//Vill man påbörja ett nytt spel efter att en spelare har vunnit så är det den här funktionen som man ska kalla på.
void reset_table(table *t, const player *p1, const player *p2)
{
	clean_table(t);//Rensar först spelbrädet.
 	init_table(t, p1, p2);//Initierar det sedan med nya brickor.
}

//Den här funktionen skriver ut hela spelbrädet (inklusive rad- och kolumnnumrering).
void print_table(const table *t)
{
	u_short row;
	for(row = 0; row < TABLE_ROW_SIZE; row++){//Skriver ut rad efter rad. Varje rad består av en horisontell linje, ett radnummer och ett visst antal rutor.
		print_horizontal_line();//Skriver först ut radens horisontella linje.
		print_row_number(row);//Sedan dess radnummer.
		print_squares(t, row);//Sist dess rutor.
	}
	//Skriver allra sist ut en extra horisontell linje och numreringen för alla kolumner.
	print_horizontal_line();
	print_column_number();
}

//Denna funktion används för att kontrollera om en bricka som tillhör en viss spelare finns på angiven rad.
bool row_contains_player_piece(const table *t, const player *p, const u_short row)
{
	u_short column;
	for(column = 0; column < TABLE_COLUMN_SIZE; column++)//Kontrollerar ruta efter ruta på en angiven rad fram till att en bricka som tillhör angiven spelare hittas.
		if(square_contains_player_piece(get_table_square(t, row, column), p))
			return TRUE;
	return FALSE;
}

//Denna funktion används för att kontrollera om en spelare har slut på brickor.
bool table_contains_player_piece(const table *t, const player *p)
{
	u_short row, column;
	for(row = 0; row < TABLE_ROW_SIZE; row++)
		for(column = 0; column < TABLE_COLUMN_SIZE; column++)
			if(square_contains_player_piece(get_table_square(t, row, column), p))
				return TRUE;
	return FALSE;
}
//Den här funktionen kontrollerar om en ruta sydöst om en annan ruta är ledig.
bool se_square_vacant(const table *t, const u_short row, const u_short column)
{
	return square_empty(get_table_square(t, row + 1, column + 1));
}

//Den här funktionen kontrollerar om det går att hoppa över en bricka som befinner sig sydöst om en annan bricka.
bool se_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	/* Kontrollerar först så att den sydöstra rutan inte är tom. Kontrollerar sedan om den sydöstra rutan innehåller en bricka som tillhör motståndarspelaren. Sen kontrolleras det om en
	 * ruta existerar sydöst om den sydöstra rutan som man kan hoppa till med sin bricka. Gör det det så kontrolleras det sist om denna ruta är ledig. Är rutan ledig kan ett hopp genomföras. */
	return se_square_vacant(t, row, column) == FALSE &&
			piece_belongs_to_player(get_square_piece(get_table_square(t, row + 1, column + 1)), player_turn(p1) == FALSE ? p1 : p2) &&
			row + 2 < TABLE_ROW_SIZE && column + 2 < TABLE_COLUMN_SIZE &&
			se_square_vacant(t, row + 1, column + 1);
}

//Denna funktion kontrollerar om en angiven bricka kan flyttas åt sydöst.
bool piece_movable_se(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	if(row + 1 < TABLE_ROW_SIZE && column + 1 < TABLE_COLUMN_SIZE)//Kontrollerar först om en ruta ett steg åt sydöst överhuvudtaget existerar.
		if(se_square_vacant(t, row, column) || se_square_jumpable(t, row, column, p1, p2))//Existerar den kontrolleras det sedan om den är tom eller om det går att hoppa över den.
			return TRUE;//Uppfylls något av kraven går brickan att flytta.
	return FALSE;
}

//Den här funktionen kontrollerar om en ruta sydväst om en annan ruta är ledig.
bool sw_square_vacant(const table *t, const u_short row, const u_short column)
{
	return square_empty(get_table_square(t, row + 1, column - 1));
}

/* Den här funktionen kontrollerar om det går att hoppa över en bricka som befinner sig sydväst om en annan bricka.
 * Valde att inte kommentera innehållet i den här funktionen och de två andra nedan då dessa är så lika funktionen ovan. */
bool sw_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	return sw_square_vacant(t, row, column) == FALSE &&
			piece_belongs_to_player(get_square_piece(get_table_square(t, row + 1, column - 1)), player_turn(p1) == FALSE ? p1 : p2) &&
			row + 2 < TABLE_ROW_SIZE && column - 2 >= 0 &&
			sw_square_vacant(t, row + 1, column - 1);
}

//Denna funktion kontrollerar om en angiven bricka kan flyttas åt sydväst. Valde att inte kommentera innehållet i den här funktionen och de två andra nedan då dessa är så lika funktionen ovan.
bool piece_movable_sw(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	if(row + 1 < TABLE_ROW_SIZE && column - 1 >= 0)
		if(sw_square_vacant(t, row, column) || sw_square_jumpable(t, row, column, p1, p2))
			return TRUE;
	return FALSE;
}

//Den här funktionen kontrollerar om en ruta nordöst om en annan ruta är ledig.
bool ne_square_vacant(const table *t, const u_short row, const u_short column)
{
	return square_empty(get_table_square(t, row - 1, column + 1));
}

//Den här funktionen kontrollerar om det går att hoppa över en bricka som befinner sig nordöst om en annan bricka.
bool ne_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	return ne_square_vacant(t, row, column) == FALSE &&
			piece_belongs_to_player(get_square_piece(get_table_square(t, row - 1, column + 1)), player_turn(p1) == FALSE ? p1 : p2) &&
			row - 2 >= 0 && column + 2 < TABLE_COLUMN_SIZE &&
			ne_square_vacant(t, row - 1, column + 1);
}

//Denna funktion kontrollerar om en angiven bricka kan flyttas åt nordöst.
bool piece_movable_ne(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	if(row - 1 >= 0 && column + 1 < TABLE_COLUMN_SIZE)
		if(ne_square_vacant(t, row , column) || ne_square_jumpable(t, row, column, p1, p2))
			return TRUE;
	return FALSE;
}

//Den här funktionen kontrollerar om en ruta nordväst om en annan ruta är ledig.
bool nw_square_vacant(const table *t, const u_short row, const u_short column)
{
	return square_empty(get_table_square(t, row - 1, column - 1));
}

//Denna funktion kontrollerar om en angiven bricka kan flyttas åt nordväst.
bool nw_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	return nw_square_vacant(t, row, column) == FALSE &&
			piece_belongs_to_player(get_square_piece(get_table_square(t, row - 1, column - 1)), player_turn(p1) == FALSE ? p1 : p2) &&
			row - 2 >= 0 && column - 2 >= 0 &&
			nw_square_vacant(t, row - 1, column - 1);
}

//Denna funktion kontrollerar om en angiven bricka kan flyttas åt nordväst.
bool piece_movable_nw(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	if(row - 1 >= 0 && column - 1 >= 0)
		if(nw_square_vacant(t, row, column) || nw_square_jumpable(t, row, column, p1, p2))
			return TRUE;
	return FALSE;
}

//Den här funktionen kontrollerar om en angiven bricka är flyttbar.
bool piece_movable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2)
{
	if(player_turn(p1)){//Är det spelare 1:s tur?
		if(piece_movable_se(t, row, column, p1, p2) || piece_movable_sw(t, row, column, p1, p2))//Kan brickan flyttas åt sydöst eller sydväst?
			return TRUE;//Kan den det går den att flytta.
		else if(piece_crowned(get_square_piece(get_table_square(t, row, column)), p1))//Är brickan krönt kan övriga riktningar kontrolleras.
			if(piece_movable_ne(t, row, column, p1, p2) || piece_movable_nw(t, row, column, p1, p2))
				return TRUE;
	}else{//Är det spelare 2:s tur?
		if(piece_movable_ne(t, row, column, p1, p2) || piece_movable_nw(t, row, column, p1, p2))//Kan brickan flyttas åt nordöst eller nordväst?
			return TRUE;//Kan den det går den att flytta.
		else if(piece_crowned(get_square_piece(get_table_square(t, row, column)), p2))//Är brickan krönt kan övriga riktningar kontrolleras.
			if(piece_movable_se(t, row, column, p1, p2) || piece_movable_sw(t, row, column, p1, p2))
				return TRUE;
	}
	return FALSE;
}

//Den här funktionen kontrollerar om det finns någon flyttbar bricka (tillhörande en viss spelare) på en angiven rad.
bool row_contains_movable_piece(const table *t, const u_short row, const player *p1, const player *p2)
{
	u_short column;
	for(column = 0; column < TABLE_COLUMN_SIZE; column++)
		if(square_empty(get_table_square(t, row, column)) == FALSE && piece_movable(t, row, column, p1, p2))
			return TRUE;//Sökningen avbryts om en bricka som går att flytta hittas.
	return FALSE;
}

//Den här funktionen kontrollerar om det finns någon flyttbar bricka (tillhörande en viss spelare) någonstans på ett helt spelbräde.
bool table_contains_movable_piece(const table *t, const player *p1, const player *p2)
{
	u_short row, column;
	for(row = 0; row < TABLE_ROW_SIZE; row++)
		for(column = 0; column < TABLE_COLUMN_SIZE; column++)
			if(square_empty(get_table_square(t, row, column)) == FALSE && square_contains_player_piece(get_table_square(t, row, column), player_turn(p1) ? p1 : p2) && piece_movable(t, row, column, p1, p2))
				return TRUE;//Sökningen avbryts om en bricka som går att flytta hittas.
	return FALSE;
}