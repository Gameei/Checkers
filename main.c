#include "main.h"
#include <stdio.h>

//Denna funktion skriver ut ett meddelande som endast ska visas när ett nytt spel påbörjas.
static void new_game_message(const player *p1, const player *p2)
{
	printf("Ett nytt spel börjar nu.\n");
	printf("%s = %c, %c\n", get_player_name(p1), get_uncrowned_piece_symbol(p1), get_crowned_piece_symbol(p1));
	printf("%s = %c, %c\n\n", get_player_name(p2), get_uncrowned_piece_symbol(p2), get_crowned_piece_symbol(p2));
}

//Denna funktion kallas när användaren behöver mata in en rad-koordinat.
static u_short get_row_coord()
{
	u_short row_coord;
	bool row_coord_acceptable = FALSE;
	do{
		printf("Rad: ");
		if(scanf("%1hu", &row_coord) == 1){//Ber användaren mata in ett heltal.
			row_coord -= 1;//Minskar rad-koordinaten med 1 då denna koordinat ska användas som index till en array senare (indexet hos arrayer börjar ju trots allt på 0).
			row_coord_acceptable = TRUE;
		}else{//Anges inget heltal skrivs ett felmeddelande ut och användaren får försöka igen.
			printf("Fel. Endast siffror får anges. Försök igen!\n\n");
			while(getchar() != '\n');
		}
	}while(row_coord_acceptable == FALSE);
	return row_coord;
}

//Denna funktion kallas när användaren behöver mata in en kolumn-koordinat.
static u_short get_column_coord()
{
	u_short column_coord;
	bool column_coord_acceptable = FALSE;
	do{
		printf("Kolumn: ");
		if(scanf("%1hu", &column_coord) == 1){//Ber användaren mata in ett heltal.
			column_coord -= 1;//Minskar kolumn-koordinaten med 1 då denna koordinat ska användas som index till en array senare (indexet hos arrayer börjar ju trots allt på 0).
			column_coord_acceptable = TRUE;
		}else{//Anges inget heltal skrivs ett felmeddelande ut och användaren får försöka igen.
			printf("Fel. Endast siffror får anges. Försök igen!\n\n");
			while(getchar() != '\n');
		}
	}while(column_coord_acceptable == FALSE);
	return column_coord;
}

//Denna funktion används för att kontrollerar om det finns någon flyttbar bricka på en angiven rad.
static bool src_row_acceptable(const table *t, const player *p1, const player *p2, const u_short src_row_coord)
{
	if((src_row_coord >= 0 && src_row_coord < TABLE_ROW_SIZE) == FALSE){//Kontrollerar först om angiven rad existerar.
		printf("Fel. Angiven rad existerar inte. Försök igen!\n\n");
		return FALSE;
	}else if(row_contains_player_piece(t, player_turn(p1) ? p1 : p2, src_row_coord) == FALSE){//Kontrollerar sedan om angiven rad innehåller en bricka som tillhör den spelare som försöker flytta en bricka.
		printf("Fel. Ingen bricka som tillhör %s finns på angiven rad. Försök igen!\n\n", get_player_name(player_turn(p1) ? p1 : p2));
		return FALSE;
	}else if(row_contains_movable_piece(t, src_row_coord, p1, p2) == FALSE){//Kontrollerar sist om angiven rad innehåller en bricka som är flyttbar.
		printf("Fel. Ingen flyttbar bricka som tillhör %s finns på angiven rad. Försök igen!\n\n", get_player_name(player_turn(p1) ? p1 : p2));
		return FALSE;
	}
	return TRUE;
}

//Denna funktion används för att kontrollerar om det finns någon flyttbar bricka i en angiven ruta.
static bool src_coords_acceptable(const table *t, const player *p1, const player *p2, const input_coordinates src_coords)
{
	if((src_coords.column >= 0 && src_coords.column < TABLE_COLUMN_SIZE) == FALSE){//Kontrollerar först om angiven ruta existerar.
		printf("Fel. Angiven ruta existerar inte. Försök igen!\n\n");
		return FALSE;
	}else if(square_empty(get_table_square(t, src_coords.row, src_coords.column))){//Kontrollerar sedan om det finns någon bricka i angiven ruta.
		printf("Fel. Angiven ruta innehåller ingen bricka. Försök igen!\n\n");
		return FALSE;
	}else if(square_contains_player_piece(get_table_square(t, src_coords.row, src_coords.column), player_turn(p1) ? p1 : p2) == FALSE){//Tillhör brickan i rutan den spelare som försöker flytta den?
		printf("Fel. Ingen bricka som tillhör %s finns i angiven ruta. Försök igen!\n\n", get_player_name(player_turn(p1) ? p1 : p2));
		return FALSE;
	}else if(piece_movable(t, src_coords.row, src_coords.column, p1, p2) == FALSE){//Kontrollerar sist om brickan i rutan går att flytta.
		printf("Fel. Ingen flyttbar bricka som tillhör %s finns i angiven ruta. Försök igen!\n\n", get_player_name(player_turn(p1) ? p1 : p2));
		return FALSE;
	}
	return TRUE;
}

//Denna funktion returnerar koordinaterna för den bricka som en spelare vill flytta.
static input_coordinates get_src_coords(const table *t, const player *p1, const player *p2)
{
	input_coordinates src_coords;
	bool acceptable = FALSE;
	do{
		printf("Ange vilken rad och kolumn som brickan finns i som du vill flytta.\n");
		src_coords.row = get_row_coord();//Först får man ange brickans rad-koordinat.
		if(src_row_acceptable(t, p1, p2, src_coords.row)){//Är denna koordinat giltlig?
			src_coords.column = get_column_coord();//Är ŕad-koordinaten giltlig får man ange brickans kolumn-koordinat.
			if(src_coords_acceptable(t, p1, p2, src_coords)){//Är båda koordinater giltliga?
				acceptable = TRUE;//Är båda koordinaterna giltliga markeras det och koordinaterna kan nu returneras.
				printf("\n");
			}
		}
	}while(acceptable == FALSE);
	return src_coords;
}

//Denna funktion används endast för att kontrollera om ett hopp kan utföras.
static bool jump_possible(const table *t, const player *p1, const player *p2, const input_coordinates coords)
{
	if(player_turn(p1)){//Är det spelare 1:s tur?
		//Kontrollerar först om spelaren kan hoppa åt sydöst eller sydväst.
		if((coords.row + 1 < TABLE_ROW_SIZE && coords.column + 1 < TABLE_COLUMN_SIZE && se_square_jumpable(t, coords.row , coords.column, p1, p2)) ||
			(coords.row + 1 < TABLE_ROW_SIZE && coords.column - 1 >= 0 && sw_square_jumpable(t, coords.row , coords.column, p1, p2))){
			return TRUE;
		//Annars kontrolleras det om spelarens bricka är krönt. Är den krönt kontrolleras det om den kan hoppa åt nordöst eller nordväst.
		}else if(piece_crowned(get_square_piece(get_table_square(t, coords.row , coords.column)), p1))
			if((coords.row - 1 >= 0 && coords.column + 1 < TABLE_COLUMN_SIZE && ne_square_jumpable(t, coords.row , coords.column, p1, p2)) ||
				(coords.row - 1 >= 0 && coords.column - 1 >= 0 && nw_square_jumpable(t, coords.row , coords.column, p1, p2)))
				return TRUE;
	}else{//Är det spelare 2:s tur?
		//Kontrollerar först om spelaren kan hoppa åt nordöst eller nordväst.
		if((coords.row - 1 >= 0 && coords.column + 1 < TABLE_COLUMN_SIZE && ne_square_jumpable(t, coords.row , coords.column, p1, p2)) ||
			(coords.row - 1 >= 0 && coords.column - 1 >= 0 && nw_square_jumpable(t, coords.row , coords.column, p1, p2))){
			return TRUE;
		//Annars kontrolleras det om spelarens bricka är krönt. Är den krönt kontrolleras det om den kan hoppa åt sydöst eller sydväst.
		}else if(piece_crowned(get_square_piece(get_table_square(t, coords.row , coords.column)), p2))
			if((coords.row + 1 < TABLE_ROW_SIZE && coords.column + 1 < TABLE_COLUMN_SIZE && se_square_jumpable(t, coords.row , coords.column, p1, p2)) ||
				(coords.row + 1 < TABLE_ROW_SIZE && coords.column - 1 >= 0 && sw_square_jumpable(t, coords.row , coords.column, p1, p2)))
				return TRUE;
	}
	return FALSE;
}

//Denna funktion används för att kontrollerar om en bricka verkligen går att flytta till en angiven rad.
static bool dest_row_acceptable(const table *t, const player *p1, const player *p2, const input_coordinates src_coords, const u_short dest_row_coord)
{
	if((dest_row_coord >= 0 && dest_row_coord < TABLE_ROW_SIZE) == FALSE){//Kontrollerar först om angiven rad existerar.
		printf("Fel. Angiven rad existerar inte. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row > dest_row_coord && player_turn(p1) && piece_crowned(get_square_piece(get_table_square(t, src_coords.row, src_coords.column)), p1) == FALSE) ||
				(src_coords.row < dest_row_coord && player_turn(p2) && piece_crowned(get_square_piece(get_table_square(t, src_coords.row, src_coords.column)), p2) == FALSE)){
		//Kontrollerar (här ovan) så att en spelare inte försöker flytta en bricka i fel riktning.
		printf("Fel. Okrönta brickor kan inte flyttas i denna riktning, Försök igen!\n\n");
		return FALSE;
	}else if(src_coords.row - dest_row_coord < -2 || src_coords.row - dest_row_coord > 2){//Kontrollerar så att man inte försöker flytta brickan mer än två rader.
		printf("Fel. Brickor kan maximalt flyttas 2 rader vid en flyttning. Försök igen!\n\n");
		return FALSE;
	}else if(src_coords.row - dest_row_coord == -1 &&//Kontrollerar om spelaren kan flytta sin bricka ett steg åt sydöst eller sydväst.
				(((src_coords.row + 1 < TABLE_ROW_SIZE && src_coords.column + 1 < TABLE_COLUMN_SIZE) && se_square_vacant(t, src_coords.row, src_coords.column)) == FALSE &&
				((src_coords.row + 1 < TABLE_ROW_SIZE && src_coords.column - 1 >= 0) && sw_square_vacant(t, src_coords.row, src_coords.column)) == FALSE)){
		printf("Fel. Angiven rad innehåller ingen ledig ruta som brickan kan flyttas till. Försök igen!\n\n");
		return FALSE;
	}else if(src_coords.row - dest_row_coord == 1 &&//Kontrollerar om spelaren kan flytta sin bricka ett steg åt nordöst eller nordväst.
				(((src_coords.row - 1 >= 0 && src_coords.column + 1 < TABLE_COLUMN_SIZE) && ne_square_vacant(t, src_coords.row , src_coords.column)) == FALSE &&
				((src_coords.row - 1 >= 0 && src_coords.column - 1 >= 0) && nw_square_vacant(t, src_coords.row, src_coords.column)) == FALSE)){
		printf("Fel. Angiven rad innehåller ingen ledig ruta som brickan kan flyttas till. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row - dest_row_coord == -2 || src_coords.row - dest_row_coord == 2) && jump_possible(t, p1, p2, src_coords) == FALSE){//Kontrollerar om spelaren kan utföra ett hopp i angiven riktning.
		printf("Fel. Inget hopp kan utföras i denna riktning. Försök igen!\n\n");
		return FALSE;
	}
	return TRUE;
}

//Denna funktion används för att kontrollerar om en bricka verkligen går att flytta till en angiven ruta.
static bool dest_coords_acceptable(const table *t, const player *p1, const player *p2, const input_coordinates src_coords, const input_coordinates dest_coords)
{
	if((dest_coords.column >= 0 && dest_coords.column < TABLE_COLUMN_SIZE) == FALSE){//Kontrollerar först om angiven ruta existerar.
		printf("Fel. Angiven ruta existerar inte. Försök igen!\n\n");
		return FALSE;
	}else if(src_coords.column - dest_coords.column < -2 || src_coords.column - dest_coords.column > 2){//Kontrollerar så att man inte försöker flytta brickan mer än två kolumner.
		printf("Fel. Brickor kan maximalt flyttas 2 kolumner vid en flyttning. Försök igen!\n\n");
		return FALSE;
	}else if(src_coords.column == dest_coords.column){//Kontrollerar så att brickan inte flyttas rakt framåt eller bakåt.
		printf("Fel. Brickor kan endast flyttas diagonalt. Försök igen!\n\n");
		return FALSE;
	}else if((((src_coords.row - dest_coords.row == -1 || src_coords.row - dest_coords.row == 1) && (src_coords.column - dest_coords.column == -1 || src_coords.column - dest_coords.column == 1)) == FALSE) &&
				(((src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2) && (src_coords.column - dest_coords.column == -2 || src_coords.column - dest_coords.column == 2)) == FALSE)){
		//Kontrollerar (här ovan) så att inte brickan flyttas i någon annan ogiltlig riktning
		printf("Fel. Brickor kan endast flyttas diagonalt. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row - dest_coords.row == -1 || src_coords.row - dest_coords.row == 1) &&//Är det en vanlig flyttning som utförs? Då kontrolleras det om rutan som brickan ska flyttas till är ledig.
				square_empty(get_table_square(t, dest_coords.row, dest_coords.column)) == FALSE){
		printf("Fel. Angiven ruta är upptagen. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2) &&//Är det ett hopp som utförs? Då kontrolleras det om rutan som brickan försöker hoppa till är ledig.
				square_empty(get_table_square(t, dest_coords.row, dest_coords.column)) == FALSE){
		printf("Fel. Angiven ruta är upptagen. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2) &&
				square_empty(get_table_square(t, src_coords.row < dest_coords.row ? dest_coords.row - 1 : dest_coords.row + 1,
				src_coords.column < dest_coords.column ? dest_coords.column - 1 : dest_coords.column + 1))){
		/* Visar det sig att rutan som en spelare försöker hoppa till är ledig kontrolleras det om rutan (brickan) som spelaren försöker hoppa över är hoppbar.
		 * Kontrollerar först (här ovan) så att rutan inte är tom. */
		printf("Fel. Ett hopp kan inte utföras över en tom ruta. Försök igen!\n\n");
		return FALSE;
	}else if((src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2) &&
				piece_belongs_to_player(get_square_piece(get_table_square(t, src_coords.row < dest_coords.row ? dest_coords.row - 1 : dest_coords.row + 1,
				src_coords.column < dest_coords.column ? dest_coords.column - 1 : dest_coords.column + 1)), player_turn(p1) == FALSE ? p1 : p2) == FALSE){
		//Kontrollerar sedan (här ovan) om brickan i rutan som man försöker hoppa över tillhör motståndarspelaren. Gör den det kan ett hopp utföras.
		printf("Fel. Hopp kan bara utföras över motståndarspelarens brickor. Försök igen!\n\n");
		return FALSE;
	}
	return TRUE;
}

//Denna funktion returnerar koordinaterna för den ruta som en spelare vill flytta sin bricka till.
static input_coordinates get_dest_coords(const table *t, const player *p1, const player *p2, const input_coordinates src_coords, bool *input_coords_acceptable)
{
	input_coordinates dest_coords;
	printf("Ange vilken rad och kolumn som du vill flytta brickan till.\n");
	dest_coords.row = get_row_coord();//Först får man ange raden som brickan ska flyttas till.
	if(dest_row_acceptable(t, p1, p2, src_coords, dest_coords.row)){//Är denna koordinat giltlig?
		dest_coords.column = get_column_coord();//Är ŕad-koordinaten giltlig får man ange kolumnen som brickan ska flyttas till.
			if(dest_coords_acceptable(t, p1, p2, src_coords, dest_coords)){//Är båda koordinater giltliga?
				*input_coords_acceptable = TRUE;//Är båda koordinaterna giltliga markeras detta.
				printf("\n");
			}
	}
	return dest_coords;
}

//Denna funktion låter en spelare mata in koordinater så att ett dubbelhopp kan utföras.
static void perform_double_jump(table *t, const player *p1, const player *p2, input_coordinates src_coords, input_coordinates dest_coords)
{
	bool input_coords_acceptable = FALSE;
	src_coords = dest_coords;
	print_table(t);
	printf("Dubbelhopp!\n\n");
	do{
		dest_coords = get_dest_coords(t, p1, p2, src_coords, &input_coords_acceptable);//Spelaren får ange vilken ruta han/hon vill flytta sin bricka till.
		if(input_coords_acceptable && (src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2)){//Kontrollerar sedan om koordinaterna är giltliga och om det var ett hopp som utfördes.
			move_square_piece(&t->squares[src_coords.row][src_coords.column], &t->squares[dest_coords.row][dest_coords.column]);//Flyttar brickan. Tar sedan bort brickan som blev överhoppad.
			remove_square_piece(&t->squares[src_coords.row < dest_coords.row ? dest_coords.row - 1 : dest_coords.row + 1][src_coords.column < dest_coords.column ? dest_coords.column - 1 : dest_coords.column + 1]);
			//Kontrollerar sedan om en spelare har nått den sista raden på spelbrädet med en av sina okrönta brickor. Är det så så blir brickan som nådde den sista raden krönt.
			if((dest_coords.row == 0 && player_turn(p2) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p2) == FALSE) ||
				(dest_coords.row == TABLE_ROW_SIZE - 1 && player_turn(p1) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p1) == FALSE)){
				crown_piece(t->squares[dest_coords.row][dest_coords.column].pic, player_turn(p1) ? p1 : p2);
			}
		}else if(input_coords_acceptable){//Utfördes inget hopp skrivs ett felmeddelande ut och spelaren får försöka igen.
			printf("Fel. Ett hopp måste utföras. Försök igen!\n\n");
			input_coords_acceptable = FALSE;
		}
	}while(input_coords_acceptable == FALSE);
}

//Denna funktion ber användaren mata in koordinater som sedan används för att flytta en bricka.
static void get_and_handle_input(table *t, const player *p1, const player *p2)
{
	input_coordinates src_coords, dest_coords;
	bool input_coords_acceptable = FALSE;
	printf("%s:s tur!\n", get_player_name(player_turn(p1) ? p1 : p2));
	do{
		src_coords = get_src_coords(t, p1, p2);//Spelaren får först ange vilken ruta som brickan finns i som han/hon vill flytta.
		dest_coords = get_dest_coords(t, p1, p2, src_coords, &input_coords_acceptable);//Sedan får spelaren ange vilken ruta han/hon vill flytta den till.
		if(input_coords_acceptable){//Är koordinaterna acceptabla? Då flyttas brickan.
			move_square_piece(&t->squares[src_coords.row][src_coords.column], &t->squares[dest_coords.row][dest_coords.column]);//Flyttar brickan.

			//Kontrollerar sedan om ett hopp utfördes. Utfördes ett hopp tas brickan bort som en annan bricka hoppade över.
			if(src_coords.row - dest_coords.row == -2 || src_coords.row - dest_coords.row == 2){
				remove_square_piece(&t->squares[src_coords.row < dest_coords.row ? dest_coords.row - 1 : dest_coords.row + 1][src_coords.column < dest_coords.column ? dest_coords.column - 1 : dest_coords.column + 1]);

				//Kontrollerar sedan om en spelare har nått den sista raden på spelbrädet med en av sina okrönta brickor. Är det så så blir brickan som nådde den sista raden krönt.
				if((dest_coords.row == 0 && player_turn(p2) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p2) == FALSE) ||
					(dest_coords.row == TABLE_ROW_SIZE - 1 && player_turn(p1) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p1) == FALSE)){
					crown_piece(t->squares[dest_coords.row][dest_coords.column].pic, player_turn(p1) ? p1 : p2);
				}else if(jump_possible(t, p1, p2, dest_coords))//Annars kontrolleras det om ett dubbelhopp kan utföras.
					perform_double_jump(t, p1, p2, src_coords, dest_coords);

			//Utfördes inget hopp (bara en vanlig flyttning) kontrolleras det bara om en spelare har nått den sista raden på spelbrädet med en av sina okrönta brickor. Är det så så blir brickan som nådde den sista raden krönt.
			}else{
				if((dest_coords.row == 0 && player_turn(p2) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p2) == FALSE) ||
					(dest_coords.row == TABLE_ROW_SIZE - 1 && player_turn(p1) && piece_crowned(get_square_piece(get_table_square(t, dest_coords.row , dest_coords.column)), p1) == FALSE)){
					crown_piece(t->squares[dest_coords.row][dest_coords.column].pic, player_turn(p1) ? p1 : p2);
				}
			}

		}
	}while(input_coords_acceptable == FALSE);
}

//Denna funktion skriver ut ett meddelande när en av spelarna vinner.
static void game_over_message(const player *p1, const player *p2)
{
	printf("%s vann!\n", player_winner(p1) ? get_player_name(p1) : get_player_name(p2));
	printf("Vill ni spela igen?\n");
	printf("Tryck J för Ja och N för Nej: ");
}

//Denna funktion efterfrågar ett tecken från spelarna. Tecknet avgör om ett nytt spel ska påbörjas eller inte.
static void get_and_handle_game_over_input(table *t, player *p1, player *p2, bool *running)
{
	char choice;
	bool acceptable = FALSE;
	do{
		while(getchar() != '\n');
		choice = getchar();
		if(choice == 'j' && choice != 'J'){//Vill spelarna spela igen?
			reset_players(p1, p2);//Vill spelarna spela igen återställs dess variabler.
			reset_table(t, p1, p2);//Även spelbrädet återställs.
			acceptable = TRUE;
		}else if(choice == 'n' && choice != 'N'){//Vill spelarna avsluta?
			*running = FALSE;//Markerar att inget nytt spel ska påbörjas.
			acceptable = TRUE;
		}else//Angavs ett felaktigt tecken skrivs ett felmeddelande ut och spelarna får försöka igen.
			printf("Fel. Ogiltligt tecken angivet. Försök igen!");
		printf("\n");
	}while(acceptable == FALSE);//När ett giltligt tecken har angivits avbryts loopen.
}

int main()
{
	player p1 = {"Spelare 1", 'x', 'X', TRUE, FALSE};//Initierar spelare 1.
	player p2 = {"Spelare 2", 'o', 'O', FALSE, FALSE};//Initierar spelare 2.
	table t = new_table(&p1, &p2);//Initierar spelbrädet.
	bool running = TRUE;//Denna variabel avgör om ett nytt spel ska påbörjas när en spelare har vunnit.
	printf("Välkommen till spelet %s!\n\n", GAME_NAME);
	do{
		new_game_message(&p1, &p2);//Skriver ut ett meddelande när ett nytt spel påbörjas.
		do{
			print_table(&t);//Spelbrädet är det som skrivs ut allra första i början av varje runda.
			//Kontrollerar om den spelare vars tur det är har någon bricka kvar. Har han/hon en bricka kvar kontrolleras det om någon av dem är flyttbara.
			if(table_contains_player_piece(&t, player_turn(&p1) ? &p1 : &p2) && table_contains_movable_piece(&t, &p1, &p2)){
				get_and_handle_input(&t, &p1, &p2);//Den spelare vars tur det är får flytta en angiven bricka.
				swap_turn(&p1, &p2);//Är en runda över? Då är det nästa spelares tur.
			}else{//Visar det sig att en spelare inte har några mer brickor eller inte någon bricka som kan flyttas skrivs ett felmeddelande ut. Motståndarspelaren markeras även som vinnare.
				printf("%s har antingen slut på brickor eller ingen mer bricka som kan flyttas.\n\n", player_turn(&p1) ? get_player_name(&p1) : get_player_name(&p2));
				player_turn(&p1) ? set_winner(&p2) : set_winner(&p1);
			}
		}while(player_winner(&p1) == FALSE && player_winner(&p2) == FALSE);//Har en spelare vunnit avbryts loopen.
		game_over_message(&p1, &p2);//Skriver ut vem som vann och om ett nytt spel ska påbörjas eller inte.
		get_and_handle_game_over_input(&t, &p1, &p2, &running);//Sist får man ange om man vill påbörja ett nytt spel eller om man vill avsluta.
	}while(running);
	return 0;
}
