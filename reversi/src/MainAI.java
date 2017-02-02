import java.util.Scanner;

import ai.MiniMax;
import ai.MiniMaxDepth;
import ai.StupidAi;
import world.Board;
import world.Disc;

public class MainAI {

	public static void main(String[] args) {
		int black = 0, white = 0;
		for(int i = 0; i < 20; i++) {
			Board board = new Board();
			while(board.gameAlive()) 
				play(board);
			Disc winner = board.getWinner();
			if(winner == Disc.WHITE)
				white++;
			else if(winner == Disc.BLACK)
				black++;
			System.out.println(winner + " " + (i+1)+"/20");
		}
		System.out.println("Black: " + black + "  White: " + white);
	}

	private static void play(Board board) {
		Disc turn = board.getTurn();
		while(board.gameAlive() && board.getTurn() == turn) {
			if(turn == Disc.WHITE) {
				int[] move = MiniMax.action(board);
				clear();
				board.set(move[0], move[1]);
				board.printBoard();
			} else {
				int[] move = MiniMaxDepth.action(board);
				clear();
				board.set(move[0], move[1]);
				board.printBoard();
			}
		}
	}

	private static void clear() {
		for(int i = 0; i < 20; i++)
			System.out.println();
	}
}
