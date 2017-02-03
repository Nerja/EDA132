#include "board.h"
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

Board::Board() {
	turn = BLACK;
	for(int i = 0; i != 8; ++i)
		for(int j = 0; j != 8; ++j)
			board[i][j] = EMPTY;
	board[3][3] = BLACK;
	board[4][4] = BLACK;

	board[3][4] = WHITE;
	board[4][3] = WHITE;
}

bool Board::game_alive() {
	return has_move(BLACK) || has_move(WHITE);
}

bool Board::has_move() {
	return has_move(turn);
}

bool Board::has_move(Disc disc) {
	bool move = false;
	Disc old_turn = turn;
	turn = disc;
	for(int i = 0; i != 8; ++i)
		for(int j = 0; j != 8; ++j)
			if(move_is_ok(i, j))
				move = true;		
	turn = old_turn;
	return move;
}

bool Board::apply_flipp(const int& row, const int& col,const int& dr, const int& dc) {
	Disc board_clone[8][8];
	memcpy(board_clone, board, sizeof(Disc)*64);
	int r = row + dr;
	int c = col + dc;
	Disc other = (turn == BLACK) ? WHITE : BLACK;
	if(r < 0 || r > 7 || c < 0 || c > 7 || (board[r][c] != other)) {
		return false;
	}
	board[r][c] = turn;
	for(int s = 1; s != 6; ++s) {
		r += dr;
		c += dc;
		if(r < 0 || r > 7 || c < 0 || c > 7 || board[r][c] == EMPTY) {
			memcpy(board, board_clone, sizeof(Disc)*64);
			return false;
		} else if(board[r][c] == turn)
			return true;
		else if(board[r][c] == other)
			board[r][c] = turn; 
	}
	memcpy(board, board_clone, sizeof(Disc)*64);
	return false;
}

bool Board::move_is_ok(const int& row, const int& col) {
	Disc board_clone[8][8];
	memcpy(board_clone, board, sizeof(Disc)*64);
	bool move_ok = apply_move(row, col);
	memcpy(board, board_clone, sizeof(Disc)*64);
	return move_ok;
}

bool Board::apply_move(const int& row, const int& col) {
	bool does_flipp = false;
	for(int dr = -1; dr != 2; ++dr) 
		for(int dc = -1; dc != 2; ++dc) 
			if((dc != 0 || dr != 0) && apply_flipp(row, col, dr, dc)) 
				does_flipp = true;
	return does_flipp;
}

bool Board::put_disc(const int& row, const int& col) {
	int r = row;
	int c = col;
	Disc old_disc = board[r][c];
	board[r][c] = turn; 
	if(old_disc == EMPTY && apply_move(r, c)) {		
		Disc next_turn = (turn == BLACK) ? WHITE : BLACK;
		if(has_move(next_turn))
			turn = next_turn;
		return true;
	} else {
		board[r][c] = old_disc;
		return false;
	}
}

int Board::count(Disc disc) const {
	int count = 0;
	for(int i = 0; i != 8; ++i)
		for(int j = 0; j != 8; ++j)
			if(board[i][j] == disc)
				count++;
	return count;
}

void Board::clone_board(Disc clone[8][8]) {
	memcpy(clone, board, sizeof(Disc)*64);
}

int Board::utility(Disc me) const {
	Disc other = (me == BLACK ? WHITE : BLACK);
	return count(me) - count(other);
}

vector<vector<int>> Board::get_moves() {
	vector<vector<int>> moves;
	for(int i = 0; i != 8; ++i)
		for(int j = 0; j != 8; ++j)
			if(board[i][j] == EMPTY && move_is_ok(i,j))
				moves.push_back({i,j});
	return moves;
}

void Board::set_state(Disc new_board[8][8], Disc new_turn) {
	memcpy(board, new_board, sizeof(Disc)*64);
	turn = new_turn;
}

void Board::print_board() {
	cout << "Black: " << to_string(count(BLACK)) << " White: " << to_string(count(WHITE)) << endl;
	for(int i = 0; i != 8; ++i) {
		cout << to_string(i+1) << "\t";		
		for(int j = 0; j != 8; ++j) {
			switch(board[i][j]) {
				case EMPTY:
					cout << (move_is_ok(i,j) ? "?" : "_");
					break;
				case WHITE:
					cout << "O";
					break;
				default: 
					cout << "X";
			}
			cout << "   ";
		}
		cout << endl << endl;
	}
	cout << endl << endl << "\tA   B   C   D   E   F   G   H" << endl;
}
