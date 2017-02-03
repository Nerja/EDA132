#include <iostream>
#include "board.h"
#include <unistd.h>
#include <string>
#include <algorithm>
#include "ai.h"
#include <vector>
#include <unistd.h>

using namespace std;

void clear() {
	for(int i = 0; i != 10; ++i)
		cout << endl;
}

void makeMove(Board& b) {
	Disc turn = b.get_turn();
	while(b.has_move() && turn == b.get_turn()) {
		if(turn == WHITE) { //Ai do your magic	
			vector<int> a = action(b, turn);
			b.put_disc(a[0], a[1]);
		} else {
			cout << "Enter move: " << endl;
			string move;
			cin >> move;
			clear();
			transform(move.begin(), move.end(), move.begin(), ::tolower);
			int row = move[0] - '0' - 1;
			int col = move[1] - 'a';
			b.put_disc(row, col);
		}
		b.print_board();
	}
}

int main() {
	Board b;
	b.print_board();
	while(b.game_alive())
		makeMove(b);		
	return 0;
}
