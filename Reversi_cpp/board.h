#include <vector>
#ifndef BOARD_H
#define BOARD_H
enum Disc {BLACK, WHITE, EMPTY};
class Board {
	public:	
		Board();
		bool game_alive();
		bool has_move();
		bool has_move(Disc disc);
		bool put_disc(const int& row, const int& col);
		Disc get_turn() const {return turn;}
		void print_board();
		bool move_is_ok(const int& row, const int& col);
		bool apply_move(const int& row, const int& col);
		int utility(Disc me) const;
		std::vector<std::vector<int>> get_moves();
		void clone_board(Disc clone[8][8]);
		void set_state(Disc new_board[8][8], Disc new_turn);
	private:
		bool apply_flipp(const int& row, const int& col, const int& dr, const int& dc);
		int count(Disc disc) const;
		Disc turn;
		Disc board[8][8];
};
#endif
