package ai;

import world.Board;
import world.Disc;

public class MiniMaxDepth {
	
	/**
	 * Returns the "best" action for the current player
	 * for the current state of the game.
	 * @param board The board
	 * @param timeWindow time window in which the bot needs to make
	 * a decision. (At least 100ms)
	 * @return Best action in format {row, col} as int array
	 */
	public static int[] action(Board board, long timeWindow) {
		long startTime = System.currentTimeMillis();
		long hardDeadline = System.currentTimeMillis() + timeWindow - 100;
		Disc me = board.getTurn(); //ai player
		
		int[][] moves = board.getMoves(); //Get available moves
		int[] bestMove = moves[0];
		int bestValue = Integer.MIN_VALUE;
		for(int[] move : moves) {
			if(System.currentTimeMillis() >= hardDeadline)
				break;
			
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);
			
			//Call minvalue and update if better
			int minValue = minValue(board, me, Integer.MIN_VALUE, Integer.MAX_VALUE, 5, hardDeadline);
			if(minValue > bestValue) {
				bestMove = move;
				bestValue = minValue;
			}
			
			//Restore state
			board.setState(disc, turn);
		}
		System.out.println("Took "  + (System.currentTimeMillis() - startTime) + " ms");
		return bestMove;
	}
	
	/**
	 * Returns min value with alpha-beta pruning
	 * @param board The current board
	 * @param me Ai player
	 * @param alpha Best estimation of max along the branch
	 * @param beta Best estimation of min along the branch
	 * @param d depth left to explore
	 * @param timeWindow time window in which the bot needs to make
	 * a decision.
	 * @return
	 */
	private static int minValue(Board board, Disc me, int alpha, int beta, int d, long hardDeadline) {
		if(!board.gameAlive() || d <= 0 || System.currentTimeMillis() >= hardDeadline) 
			return board.reward(me);
		
		int min = Integer.MAX_VALUE;
		for(int[] move : board.getMoves()) {
			if(System.currentTimeMillis() >= hardDeadline)
				break;
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);

			//Pick min
			min = Math.min(min, maxValue(board, me, alpha, beta, d - 1, hardDeadline));
			
			if(min <= alpha)
				return min;
			
			beta = Math.min(min, beta);
			
			//Restore state
			board.setState(disc, turn);	
		}
		
		return min;
	}
	
	/**
	 * Returns max value with alpha-beta pruning
	 * @param board The current board
	 * @param me Ai player
	 * @param alpha Best estimation of max along the branch
	 * @param beta Best estimation of min along the branch
	 * @param d depth left to explore
	 * @param timeWindow time window in which the bot needs to make
	 * a decision. 
	 * @return depth left
	 */
	private static int maxValue(Board board, Disc me, int alpha, int beta, int d, long hardDeadline) {
		//1 for me winning, -1 for loosing
		if(!board.gameAlive() || d <= 0 || System.currentTimeMillis() >= hardDeadline) 
			return board.reward(me);
		
		int max = Integer.MIN_VALUE;
		for(int[] move : board.getMoves()) {
			if(System.currentTimeMillis() >= hardDeadline)
				break;
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);

			//Pick max
			max = Math.max(max, minValue(board, me, alpha, beta, d - 1, hardDeadline));
			
			if(max >= beta)
				return max;
			
			alpha = Math.max(max, alpha);
			
			//Restore state
			board.setState(disc, turn);	
		}
		return max;
	}
}
