#include "ai.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

/**vector<int> action(Board& b, Disc me) {
	vector<vector<int>> action_space = b.get_moves();
	int move = rand()%(action_space.size());
	cout << to_string(action_space.size()) << " pick: " << to_string(move) << endl;
	return action_space[move];
}**/

int max_value(Board& b, Disc me, int alpha, int beta, int d) {
	Disc turn = b.get_turn();
	if(!b.has_move(turn) || d <= 0) 
		return b.utility(me);
	
	int max_v = INT_MIN;
	vector<vector<int>> action_space = b.get_moves();

	for(decltype(action_space.size()) i = 0; i != action_space.size(); ++i) {
		vector<int> move = action_space[i];
		Disc clone[8][8];
		b.clone_board(clone); //Save state

		b.put_disc(move[0], move[1]); //Execute move

		max_v = max(max_v, min_value(b, me, alpha, beta, d-1));

		if(max_v >= beta) //Hittat bättre innan
			return max_v;

		alpha = max(alpha, max_v);

		b.set_state(clone, me);	//Restore state
	}
	return max_v;
}

int min_value(Board& b, Disc me, int alpha, int beta, int d) {
	Disc turn = b.get_turn();
	if(!b.has_move(turn) || d <= 0) 
		return b.utility(me);
	
	int min_v = INT_MAX;
	vector<vector<int>> action_space = b.get_moves();

	for(decltype(action_space.size()) i = 0; i != action_space.size(); ++i) {
		vector<int> move = action_space[i];
		Disc clone[8][8];
		b.clone_board(clone); //Save state

		b.put_disc(move[0], move[1]); //Execute move

		min_v = min(min_v, max_value(b, me, alpha, beta, d-1));

		if(min_v <= alpha) //Hittat bättre innan
			return min_v;

		beta = min(beta, min_v);

		b.set_state(clone, me);	//Restore state
	}
	return min_v;
}

vector<int> action(Board& b, Disc me) {
	vector<vector<int>> action_space = b.get_moves();

	vector<int> best_move = action_space[0];
	int best_value = INT_MIN;

	for(decltype(action_space.size()) i = 0; i != action_space.size(); ++i) {
		vector<int> move = action_space[i];
		Disc clone[8][8];
		b.clone_board(clone); //Save state
		
		b.put_disc(move[0], move[1]); //Execute move

		int min_v = min_value(b, me, INT_MIN, INT_MAX, 7);
		if(min_v > best_value) {
			best_move = move;
 			best_value = min_v;
		}
		
		b.set_state(clone, me);	//Restore state
	}

	return best_move;
}


