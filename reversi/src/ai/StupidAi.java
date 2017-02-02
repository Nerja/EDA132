package ai;

import java.util.Random;

import world.Board;

public class StupidAi {
	private static final Random rand = new Random();
	public static int[] action(Board board) {
		int[][] moves = board.getMoves();
		int move = rand.nextInt(moves.length);
		return moves[move];
	}
}
