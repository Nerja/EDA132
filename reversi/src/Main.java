import java.util.Scanner;

import ai.MiniMax;
import ai.StupidAi;
import world.Board;
import world.Disc;

public class Main {

	public static void main(String[] args) {
		Board board = new Board();
		Scanner scan = new Scanner(System.in);
		
		System.out.print("Bitte command(black): ");
		String input = "";
		while(board.gameAlive() && !input.toLowerCase().equals("close")) 
			play(board, scan);
		scan.close();
		
		System.out.println("Winner is " + board.getWinner());
	}

	private static void play(Board board, Scanner scan) {
		Disc turn = board.getTurn();
		while(board.gameAlive() && board.getTurn() == turn) {
			if(turn == Disc.WHITE) {
				int[] move = MiniMax.action(board);
				board.set(move[0], move[1]);
				board.printBoard();
			} else {
				System.out.println();
				board.printBoard();
				System.out.print("Bitte command("+board.getTurn()+")");
				try{
					applyCommand(scan.nextLine(), board, scan);
				} catch(Exception e) {
					
				}
			}
		}
	}

	private static void applyCommand(String input, Board board, Scanner scan) {
		clear();
		input = input.toLowerCase();
		int row = (input.charAt(0) - '0') - 1;
		int col = input.charAt(1) - 'a';
		board.set(row, col);
	}

	private static void clear() {
		for(int i = 0; i < 20; i++)
			System.out.println();
	}
}
