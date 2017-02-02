package ai;

import world.Board;
import world.Disc;

public class MiniMaxDepth {
	public static int a, b;
	
	public static int[] action(Board board) {
		long startTime = System.currentTimeMillis();
		Disc me = board.getTurn();
		
		int[][] moves = board.getMoves();
		int[] bestMove = moves[0];
		int bestValue = Integer.MIN_VALUE;
		int i = 1;
		for(int[] move : moves) {
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);
			
			//Call minvalue and update if better
			int minValue = minValue(board, me, Integer.MIN_VALUE, Integer.MAX_VALUE, 7);
			if(minValue > bestValue) {
				bestMove = move;
				bestValue = minValue;
			}
			
			//Restore state
			board.setState(disc, turn);
			
			System.out.println(i++);
		}
		System.out.println("Picked row: " + bestMove[0] + " col " + bestMove[1]);
		System.out.println("Took "  + (System.currentTimeMillis() - startTime) + " ms");
		return bestMove;
	}
	
	private static int minValue(Board board, Disc me, int alpha, int beta, int d) {
		//1 for me winning, -1 for loosing
		if(!board.hasMove(me)) 
			return board.reward(me);
		else if(d <= 0)
			return board.reward(me);
		
		int min = Integer.MAX_VALUE;
		for(int[] move : board.getMoves()) {
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);

			//Pick min
			min = Math.min(min, maxValue(board, me, alpha, beta, d - 1));
			
			if(min <= alpha)
				return min;
			
			beta = Math.min(min, beta);
			
			//Restore state
			board.setState(disc, turn);	
		}
		
		return min;
	}
	
	private static int maxValue(Board board, Disc me, int alpha, int beta, int d) {
		//1 for me winning, -1 for loosing
		if(!board.hasMove(me)) 
			return board.reward(me);
		else if(d <= 0)
			return board.reward(me);
		
		int max = Integer.MIN_VALUE;
		for(int[] move : board.getMoves()) {
			//Save state
			Disc[][] disc = board.clone();
			Disc turn = board.getTurn();
			
			//Execute mote
			board.set(move[0], move[1]);

			//Pick max
			max = Math.max(max, minValue(board, me, alpha, beta, d - 1));
			
			if(max >= beta)
				return max;
			
			max = Math.max(max, alpha);
			
			//Restore state
			board.setState(disc, turn);	
		}
		return max;
	}
}
