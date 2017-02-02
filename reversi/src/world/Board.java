package world;

import java.util.LinkedList;
import java.util.List;

public class Board {
	private Disc[][] board;
	private Disc turn;
	
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
	
	public Disc getTurn() {
		return turn;
	}
	
	public void setState(Disc[][] board, Disc turn) {
		this.board = board;
		this.turn = turn;
	}
	
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
	
	public boolean gameAlive() {
		return hasMove(Disc.BLACK) || hasMove(Disc.WHITE);
	}
	
	public void set(int row, int col) {
		if(row < 0 || row > 7 || col < 0 || col > 7) {
			throw new IllegalArgumentException("Wrong indexing " + "Row: " + row + " Col: " + col);
		}
		else if(board[row][col] != Disc.EMPTY) {
			String error = "Row: " + row + ", Col: " + col + " is already filled";
			throw new IllegalArgumentException(error);
		}
		board[row][col] = turn;
		if(!applyPropagation(row, col)) {
			board[row][col] = Disc.EMPTY;
			throw new IllegalArgumentException("Not ok");
		}
		Disc other = turn == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		if(hasMove(other))
			turn = other;
	}
	
	public Disc[][] clone() {
		Disc[][] v = new Disc[8][8];
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				v[i][j] = board[i][j];
		return v;
	}

	private boolean applyPropagation(int row, int col) {
		int nbrFlipps = 0;
		for(int vi = -1; vi <= 1; vi++) {
			for(int vj = -1; vj <= 1; vj++) {
				if(vi != 0 || vj != 0) {
					int n = flippVector(row, col, vi, vj);
					nbrFlipps += n;
				}
			}
		}
		return nbrFlipps > 0;
	}

	private int flippVector(int row, int col, int vi, int vj) {
		Disc target = turn == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		int count = 0;
		int nrow = row + vi;
		int ncol = col + vj;
		//If next is opposite then ok
		if(nrow > -1 && nrow < 8 && ncol > -1 && ncol < 8 && board[nrow][ncol] == target) {
			Disc[][] newBoard = clone(); //Apply changes on clone
			newBoard[nrow][ncol] = turn;
			count++;
			boolean ok = false;
			for(int f = 2; f < 7; f++) {
				nrow += vi;
				ncol += vj;
				if(ncol < 0 || ncol > 7 || nrow < 0 || nrow > 7) {
					break;
				} else if(board[nrow][ncol] == target) {
					count++;
					newBoard[nrow][ncol] = turn;
				} else if(board[nrow][ncol] == Disc.EMPTY){
					break;
				} else {
					ok = true;
					break;
				}
			}
			if(!ok)
				count = 0;
			else
				board = newBoard;
		}
		return count;
	}
	
	public int getPoints(Disc c) {
		int count = 0;
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				if(board[i][j] == c)
					count++;
		return count;
	}
	
	public int reward(Disc c) {
		Disc other = c == Disc.BLACK ? Disc.WHITE : Disc.BLACK;
		return getPoints(c) - getPoints(other);
	}
	
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
