package world;

import java.util.LinkedList;
import java.util.List;

public class Board {
	private Disc[][] board;
	private Disc turn;
	
	/**
	 * Creates a board object that contains
	 * the starting position for the Othello game
	 */
	public Board() {
		board = new Disc[8][8];
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				board[i][j] = Disc.EMPTY;
		board[3][3] = Disc.WHITE;
		board[4][4] = Disc.WHITE;
		
		board[4][3] = Disc.BLACK;
		board[3][4] = Disc.BLACK;
		
		turn = Disc.BLACK;
	}
	
	/**
	 * Returns the current player
	 * @return current player
	 */
	public Disc getTurn() {
		return turn;
	}
	
	/**
	 * Set the current state
	 * @param board New board
	 * @param turn  New player turn
	 */
	public void setState(Disc[][] board, Disc turn) {
		this.board = board;
		this.turn = turn;
	}
	
	/**
	 * Checks if there is a available move for a player
	 * @param color The player to check for
	 * @return Returns true if there is a valid move
	 */
	public boolean hasMove(Disc color) {
		Disc saveTurn = turn;
		turn = color;
		boolean move = false;
		for(int i = 0; i < 8 && !move; i++) {
			for(int j = 0; j < 8 && !move; j++) {
				if(board[i][j] == Disc.EMPTY) {
					Disc[][] boardClone = clone();
					if(applyPropagation(i, j))
						move = true;
					board = boardClone;
				}
			}
		}
		turn = saveTurn;
		return move;
	}
	
	/**
	 * Returns the winner. Empty if draw.
	 * @return The winner
	 */
	public Disc getWinner() {
		int white = getPoints(Disc.WHITE);
		int black = getPoints(Disc.BLACK);
		if(white == black)
			return Disc.EMPTY;
		else if(white > black)
			return Disc.WHITE;
		else
			return Disc.BLACK;
	}
	
	/**
	 * Checks if the game is still alive.
	 * @return true if game is alive
	 */
	public boolean gameAlive() {
		return hasMove(Disc.BLACK) || hasMove(Disc.WHITE);
	}
	
	/**
	 * Performs a move with error checking
	 * @param row Row with index 0...7
	 * @param col Col with index 0...7
	 */
	public void set(int row, int col) {
		if(row < 0 || row > 7 || col < 0 || col > 7) {
			throw new IllegalArgumentException("Wrong indexing " + "Row: " + row + " Col: " + col);
		}
		else if(board[row][col] != Disc.EMPTY) { //Already a disc here
			String error = "Row: " + row + ", Col: " + col + " is already filled";
			throw new IllegalArgumentException(error);
		}
		board[row][col] = turn; //Place the disc
		if(!applyPropagation(row, col)) { //Try to perform the move, if not valid throw error
			board[row][col] = Disc.EMPTY;
			throw new IllegalArgumentException("Not ok");
		}
		Disc other = turn == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		if(hasMove(other))
			turn = other;
	}
	
	/*
	 * Clone the current board state
	 */
	public Disc[][] clone() {
		Disc[][] v = new Disc[8][8];
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				v[i][j] = board[i][j];
		return v;
	}
	
	/**
	 * Checks every direction from row, col and tries to flip discs
	 * @param row Row with index 0...7
	 * @param col Col with index 0...7
	 * @return true if 1 or more discs were flipped
	 */
	private boolean applyPropagation(int row, int col) {
		int nbrFlipps = 0;
		for(int vi = -1; vi <= 1; vi++) {
			for(int vj = -1; vj <= 1; vj++) {
				if(vi != 0 || vj != 0) {
					int n = flipVector(row, col, vi, vj);
					nbrFlipps += n;
				}
			}
		}
		return nbrFlipps > 0;
	}

	/**
	 * @param row Row with index 0...7
	 * @param col Col with index 0...7
	 * @param vi row-direction -1..1
	 * @param vj col-direction -1..1
	 * @return number of flipped discs
	 */
	private int flipVector(int row, int col, int vi, int vj) {
		//Disc to flip
		Disc target = turn == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		int count = 0;
		//Move one step in the given direction
		int nrow = row + vi;
		int ncol = col + vj;
		//If next is target then ok
		if(nrow > -1 && nrow < 8 && ncol > -1 && ncol < 8 && board[nrow][ncol] == target) {
			Disc[][] newBoard = clone(); //Clone the board
			newBoard[nrow][ncol] = turn; //Flip the disc in the clone
			count++;
			boolean ok = false; //Flag true if we encounter our color again
			for(int f = 1; f <= 6; f++) {
				//Take one step
				nrow += vi;
				ncol += vj;
				if(ncol < 0 || ncol > 7 || nrow < 0 || nrow > 7) { //If outside fail
					break;
				} else if(board[nrow][ncol] == target) { //If target increase count and flip
					count++;
					newBoard[nrow][ncol] = turn;
				} else if(board[nrow][ncol] == Disc.EMPTY){ //If Empty fail
					break;
				} else { //Encountered our color again set flag ok
					ok = true;
					break;
				}
			}
			if(!ok)
				count = 0;
			else
				board = newBoard; //Apply clone changes to board
		}
		return count;
	}
	
	/**
	 * Counts the number of discs for player c
	 * @param c player to count discs for
	 * @return returns the number of discs for player c
	 */
	public int getPoints(Disc c) {
		int count = 0;
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				if(board[i][j] == c)
					count++;
		return count;
	}
	
	/**
	 * reward for player c
	 * @param c player
	 * @return return reward
	 */
	public int reward(Disc c) {
		Disc other = c == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		return getPoints(c) - getPoints(other);
	}
	
	/**
	 * Returns the available move space for the current player
	 * @return Returns the available move space
	 */
	public int[][] getMoves() {
		List<int[]> moves = new LinkedList<int[]>();
		for(int i = 0; i < 8; i++) {
			for(int j = 0; j < 8; j++) {
				if(board[i][j] == Disc.EMPTY) {
					Disc[][] boardClone = clone();
					if(applyPropagation(i, j))
						moves.add(new int[]{i, j});
					board = boardClone;
				}
			}
		}
		int[][] movesMatrix = new int[moves.size()][2];
		for(int i = 0; i < moves.size(); i++)
			movesMatrix[i] = moves.get(i);
		return movesMatrix;
	}
	
	/**
	 * Standard ASCII print of the board state
	 * X for player Black and O for player white.
	 * ? for valid move for the current player
	 */
	public void printBoard() {
		System.out.println("White: " + getPoints(Disc.WHITE) + "  Black: " + getPoints(Disc.BLACK));
		for(int i = 0; i < 8; i++) {
			System.out.print((i+1) + "\t");
			for(int j = 0; j < 8; j++) {
				char marker = '_';
				if(board[i][j] == Disc.BLACK)
					marker = 'X';
				else if(board[i][j] == Disc.WHITE)
					marker = 'O';
				else {
					Disc[][] boardClone = clone();
					board[i][j] = turn;
					boolean ok = applyPropagation(i, j);
					board = boardClone;
					if(ok) {
						marker = '?';
					}
				}
				System.out.print(marker + "   ");
			}
			System.out.println("\n");
		}
		System.out.print("\n\t");
		for(int i = 0; i < 8; i++)
			System.out.print(((char)('a' + i)) + "   ");
		System.out.println();
	}

}
