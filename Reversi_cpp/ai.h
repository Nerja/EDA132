#include <vector>
#include "board.h"

#ifndef AI_H
#define AI_H
	std::vector<int> action(Board& b, Disc me);
	int min_value(Board& b, Disc me, int alpha, int beta, int d);
	int max_value(Board& b, Disc me, int alpha, int beta, int d);
#endif
