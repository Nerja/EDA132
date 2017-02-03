import java.util.Scanner;
import ai.MiniMaxDepth;
import world.Board;
import world.Disc;

public class Main {

	public static void main(String[] args) {
		Board board = new Board();
		Scanner scan = new Scanner(System.in);
		System.out.print("Bitte enter timewindow in ms(at least 100ms): ");
		long timeWindow = scan.nextLong();
		System.out.print("Bitte command(black): ");
		String input = "";
		while(board.gameAlive() && !input.toLowerCase().equals("close")) 
			play(board, scan, timeWindow);
		scan.close();
		
		System.out.println("Winner is " + board.getWinner());
	}

	private static void play(Board board, Scanner scan, long timeWindow) {
		Disc turn = board.getTurn();
		while(board.gameAlive() && board.getTurn() == turn) {
			if(turn == Disc.WHITE) {
				int[] move = MiniMaxDepth.action(board, timeWindow);
				board.set(move[0], move[1]);
				board.printBoard();
			} else {
				System.out.println();
				board.printBoard();
				System.out.print("Bitte command("+board.getTurn()+")");
				try{
					applyCommand(scan.nextLine(), board);
				} catch(Exception e) {
					
				}
			}
		}
	}

	/**
	 * Clears and performs a command from the human player
	 * @param input The command
	 * @param board the board
	 * @param scan the scann
	 */
	private static void applyCommand(String input, Board board) {
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
