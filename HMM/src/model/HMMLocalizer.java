package model;

import java.util.Arrays;
import java.util.Random;
import org.ejml.simple.SimpleMatrix;
import control.EstimatorInterface;

public class HMMLocalizer implements EstimatorInterface {

	private int rows, cols, head;
	private double[][] transitionMatrix;
	private State[] states;
	private State trueState, lastSensorReading;
	private double[] probLoc; //Probability distribution over possible locations.
	private static final Random rand = new Random();
	private SimpleMatrix transTransitionMatrix, fm; //fm is forward message(probability over sates including heading)
	private SimpleMatrix[] sensorModels; //O matrices

	public HMMLocalizer(int rows, int cols, int head) {
		this.rows = rows;
		this.cols = cols;
		this.head = head;
		createStateVector(rows, cols);
		createTransitionMatrix(rows, cols);
		createSensorModel();

		trueState = new State(rand.nextInt(rows), rand.nextInt(cols), rand.nextInt(4), rows, cols);
		double[][] forwardMessage = new double[rows * cols * 4][1];
		//Forward message is uniform over all states(including heading)
		for (int i = 0; i < forwardMessage.length; i++)
			forwardMessage[i][0] = (1.0 / (rows * cols * 4));
		fm = new SimpleMatrix(forwardMessage);
		//probLoc is uniform over possible locations(not including heading)
		probLoc = new double[rows * cols];
		Arrays.fill(probLoc, (1.0 / (rows * cols)));
	}

	private void createSensorModel() {
		// Possible outputs is each square + nothing output
		sensorModels = new SimpleMatrix[rows * cols + 1];
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				State s = new State(r, c, 0, rows, cols); // Reporting location row r col c.
				sensorModels[s.getStateNbr() / 4] = new SimpleMatrix(s.sensorMatrix(states));
			}
		}
		double[][] nothing = new double[rows * cols * 4][rows * cols * 4];
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				for (int h = 0; h < 4; h++) {
					State s = new State(r, c, h, rows, cols);
					nothing[s.getStateNbr()][s.getStateNbr()] = s.computePosibilitySensorBlind();
				}
		sensorModels[sensorModels.length - 1] = new SimpleMatrix(nothing);
	}

	private void createTransitionMatrix(int rows, int cols) {
		transitionMatrix = new double[rows * cols * 4][rows * cols * 4];
		double[][] ttm = new double[rows * cols * 4][rows * cols * 4];
		for (int i = 0; i < states.length; i++)
			for (int j = 0; j < states.length; j++) {
				//compute probability for going from state i to state j
				transitionMatrix[i][j] = states[i].computeTransProb(states[j]);
				ttm[j][i] = transitionMatrix[i][j]; //Transpose
			}
		transTransitionMatrix = new SimpleMatrix(ttm);
	}

	private void createStateVector(int rows, int cols) {
		states = new State[rows * cols * 4]; // 4 headings per position
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < cols; c++)
				for (int h = 0; h < 4; h++) {
					State s = new State(r, c, h, rows, cols);
					states[s.getStateNbr()] = s;
				}
	}

	public int getNumRows() {
		return rows;
	}

	public int getNumCols() {
		return cols;
	}

	public int getNumHead() {
		return head;
	}

	public double getTProb(int x, int y, int h, int nX, int nY, int nH) {
		State iState = new State(x, y, h, rows, cols);
		State jState = new State(nX, nY, nH, rows, cols);
		return transitionMatrix[iState.getStateNbr()][jState.getStateNbr()];
	}

	public double getOrXY(int rX, int rY, int x, int y) {
		State r = new State(x, y, 1, rows, cols);
		int rStateNbr = r.getStateNbr();
		if (rX == -1 || rY == -1)
			return sensorModels[sensorModels.length - 1].get(rStateNbr, rStateNbr);
		State s = new State(rX, rY, 1, rows, cols);
		return sensorModels[s.getStateNbr() / 4].get(rStateNbr, rStateNbr);
	}

	public int[] getCurrentTruePosition() {
		return new int[] { trueState.getRow(), trueState.getCol() };
	}

	// http://stackoverflow.com/questions/9330394/how-to-pick-an-item-by-its-probability
	public int[] getCurrentReading() {
		return lastSensorReading == null ? null
				: new int[] { lastSensorReading.getRow(), lastSensorReading.getCol() };
	}

	/**
	 * Returns error measured in manhattan distance
	 * @return manhattan error
	 */
	public int getManhattanError() {
		State est = getEstimation();
		int diffRow = Math.abs(trueState.getRow() - est.getRow());
		int diffCol = Math.abs(trueState.getCol() - est.getCol());
		return diffRow + diffCol;
	}

	/**
	 * Finds the state(location) with highest probability.
	 * @return estimated state indicating location
	 */
	private State getEstimation() {
		State best = states[0];
		double bestValue = probLoc[0];
		for (int i = 0; i < probLoc.length; i++) {
			if (probLoc[i] > bestValue) {
				best = states[4 * i];
				bestValue = probLoc[i];
			}
		}
		return best;
	}
	
	/**
	 * Returns the sensor reading.
	 * @return sensor reading(location not heading)
	 */
	private State getReading() {
		double probReading[] = new double[sensorModels.length];
		int tds = trueState.getStateNbr();
		for(int output = 0; output < sensorModels.length; output++)
			probReading[output] = sensorModels[output].get(tds, tds); //Diagonal matrix
		int index = randomPick(probReading);
		return (index < sensorModels.length - 1) ? states[index * 4] : null;
	}

	public double getCurrentProb(int x, int y) {
		State s = new State(x, y, 0, rows, cols);
		return probLoc[s.getStateNbr() / 4];
	}

	public void update() {
		// Update to new state
		trueState = pickNewState();
		
		// Select O Matrix from the sensor input
		SimpleMatrix oMatrix = selectOMatrix();

		// Compute forward message
		fm = oMatrix.mult(transTransitionMatrix).mult(fm);
		
		// Normalize
		double alpha = 1.0 / fm.elementSum();
		fm = fm.mult(new SimpleMatrix(new double[][] { { alpha } }));
		
		// Compute probability to be in (x, y) for all x, y. 
		// Summing over possible headings
		computeProbLoc();
	}

	private void computeProbLoc() {
		Arrays.fill(probLoc, 0);
		for (int i = 0; i < probLoc.length; i++)
			for (int h = 0; h < 4; h++)
				probLoc[i] += fm.get(i * 4 + h);
	}

	private SimpleMatrix selectOMatrix() {
		lastSensorReading = getReading();
		int index = lastSensorReading == null ? sensorModels.length - 1 : lastSensorReading.getStateNbr() / 4;
		return sensorModels[index];
	}

	public boolean correctEst() {
		State est = getEstimation();
		return est.getRow() == trueState.getRow() && est.getCol() == trueState.getCol();
	}
	
	/**
	 * Returns the new true state
	 * @return new true state
	 */
	private State pickNewState() {
		double[] prob = new double[states.length];
		for (int s = 0; s < states.length; s++)
			prob[s] = transitionMatrix[trueState.getStateNbr()][s];
		return states[randomPick(prob)];
	}
	
	/**
	 * Return random i = 0..pos.length-1 with probability
	 * according to the element pos[i].
	 * This is from: http://stackoverflow.com/questions/9330394/how-to-pick-an-item-by-its-probability
	 * @param pos Array with probabilities for earch index
	 * @return random index
	 */
	private int randomPick(double[] pos) {
		double p = Math.random();
		double cumProb = 0;
		for (int i = 0; i < pos.length; i++) {
			cumProb += pos[i];
			if(p <= cumProb)
				return i;
		}
		return pos.length;
	}

}