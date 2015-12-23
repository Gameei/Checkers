#include "square.h"

#define TABLE_ROW_SIZE 8
#define TABLE_COLUMN_SIZE 8

//Den här dataposten ska representera ett spelbräde.
typedef struct{
	square squares[TABLE_ROW_SIZE][TABLE_COLUMN_SIZE];//Varje spelbräde består av ett visst antal rutor.
}table;

//Funktionsprototyper.
table new_table(const player *p1, const player *p2);
const square *get_table_square(const table *t, const u_short row, const u_short column);
void reset_table(table *t, const player *p1, const player *p2);
void print_table(const table *t);
bool row_contains_player_piece(const table *t, const player *p, const u_short row);
bool table_contains_player_piece(const table *t, const player *p);
bool se_square_vacant(const table *t, const u_short row, const u_short column);
bool se_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool piece_movable_se(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool sw_square_vacant(const table *t, const u_short row, const u_short column);
bool sw_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool piece_movable_sw(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool ne_square_vacant(const table *t, const u_short row, const u_short column);
bool ne_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool piece_movable_ne(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool piece_movable_nw(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool nw_square_vacant(const table *t, const u_short row, const u_short column);
bool nw_square_jumpable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool piece_movable(const table *t, const u_short row, const u_short column, const player *p1, const player *p2);
bool row_contains_movable_piece(const table *t, const u_short row, const player *p1, const player *p2);
bool table_contains_movable_piece(const table *t, const player *p1, const player *p2);
