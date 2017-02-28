package model;

import java.util.LinkedList;
import java.util.List;

public class State {
	private int row, col, heading, cols, rows;
	private static final int[][] headings = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

	public State(int row, int col, int heading, int rows, int cols) {
		this.row = row;
		this.col = col;
		this.heading = heading;
		this.cols = cols;
		this.rows = rows;
	}

	/**
	 * Enumerates the state
	 * @return unique nbr enumerating this state
	 */
	public int getStateNbr() {
		return row * cols * 4 + col * 4 + heading;
	}
	
	/**
	 * Is the state facing a wall?
	 * @return true if facing a wall
	 */
	private boolean facesWall() {
		int nrow = row + headings[heading][0];
		int ncol = col + headings[heading][1]; 
		return (nrow < 0 || nrow >= rows || ncol < 0 || ncol >= cols);
	}
	
	/**
	 * Do i end up in the target heading from current position
	 * @param target
	 * @return true if i do
	 */
	private boolean movingTowards(State target) {
		int nrow = row + headings[target.heading][0];
		int ncol = col + headings[target.heading][1]; 
		return nrow == target.row && ncol == target.col;
	}
	
	/**
	 * Returns a list of good moves in form (0, ...)
	 * @return List of good moves
	 */
	private List<Integer> goodMoves() {
		List<Integer> moves = new LinkedList<Integer>();
		for(int h = 0; h < 4; h++) {
			int nrow = row + headings[h][0];
			int ncol = col + headings[h][1]; 
			if(!(nrow < 0 || nrow >= rows || ncol < 0 || ncol >= cols))
				moves.add(h);
		}
		return moves;
	}

	/**
	 * Computes probability to go from this state
	 * to the target state.
	 * @param target The target state
	 * @return the probability
	 */
	public double computeTransProb(State target) {
		// zero probability to move more than one step
		int rowdiff = Math.abs(row - target.row);
		int coldiff = Math.abs(col - target.col);
		int manhattan = rowdiff + coldiff;
		
		// Must move one step
		if(manhattan != 1 || !movingTowards(target))
			return 0;
		
		List<Integer> goodMoves = goodMoves();
		if(!goodMoves.contains(target.heading)) //Dont do anything stupid
			return 0;
		if(!facesWall()) {
			//If i dont face wall continue with possibility 0.7 and
			//distribute uniform 0.3 on other good moves
			return heading != target.heading ? 0.3 / (goodMoves.size() - 1) : 0.7;
		} else {
			//distribute uniform 1.0 on good moves
			return 1.0 / goodMoves.size();
		}
	}

	public int getRow() {
		return row;
	}
	
	public int getCol() {
		return col;
	}

	/**
	 * Computes the sensor matrix for this state
	 * as output.
	 * @param states All possible states
	 * @return the sensor matrix
	 */
	public double[][] sensorMatrix(State[] states) {
		double[][] matrix = new double[states.length][states.length];
		for(int i = 0; i < matrix.length; i++)
			matrix[i][i] = states[i].sensorReports(this);
		return matrix;
	}

	/**
	 * Returns the probability for the sensor
	 * reporting the state report when "this"
	 * is the current state
	 * @param report The state reported by the sensor
	 * @return The probability
	 */
	private double sensorReports(State report) {
		int rowdiff = Math.abs(report.row - row);
		int coldiff = Math.abs(report.col - col);
		if(rowdiff > 2 || coldiff > 2) //Max 2:th ring
			return 0;
		else if(rowdiff == 2 || coldiff == 2)
			return 0.025;
		else if(rowdiff == 1 || coldiff == 1)
			return 0.05;
		else //Must be 0 ring
			return 0.1;
	}

	/**
	 * Computes the probability for the sensor
	 * reporting nothing when in this state.
	 * @return The probability for blind sensor
	 */
	public double computePosibilitySensorBlind() {
		return 1.0 - 0.1 - nbrLevelOneRing()*0.05 - nbrLevelTwoRing() * 0.025;
	}
	
	/**
	 * Computes nbr squares in lvl one ring
	 * @return nbr squares
	 */
	private double nbrLevelOneRing() {
		int count = 0;
		for(int dr = -1; dr <= 1; dr++)
			for(int dc = -1; dc <= 1; dc++) {
				int r = row + dr;
				int c = col + dc;
				if((dc != 0 || dr != 0) && !((r < 0 || r >= rows || c < 0 || c >= cols)))
					count++;
			}
		return count;
	}
	
	/**
	 * Computes nbr squares in lvl two ring
	 * @return nbr squares
	 */
	private double nbrLevelTwoRing() {
		int count = 0;
		for(int dr = -2; dr <= 2; dr++)
			for(int dc = -2; dc <= 2; dc++) {
				int r = row + dr;
				int c = col + dc;
				//Only in lvl 2
				if(Math.abs(r - row) == 2 || Math.abs(c - col) == 2)
					if((dc != 0 || dr != 0) && !((r < 0 || r >= rows || c < 0 || c >= cols)))
						count++;
			}
		return count;
	}
}
