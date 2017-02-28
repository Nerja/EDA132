import model.HMMLocalizer;

public class Measure {

	/**
	 * Run several instances of 40 observations to
	 * get some more accurate measurement.
	 */
	public static void main(String[] args) {
		int nbrObservations = 100, nbrRuns = 20;
		System.out.println("Running " + nbrRuns + " runs with " + nbrObservations + " observations each");
		double manhattan[] = new double[nbrObservations];
		double correctRatio = 0;
		for(int run = 0; run < nbrRuns; run++) {
			System.out.println("Run\t" + (run+1) + "/" + nbrRuns);
			HMMLocalizer robot = new HMMLocalizer( 8, 8, 4);
			double correct = 0;
			for(int obs = 0; obs < nbrObservations; obs++) {
				robot.update();
				manhattan[obs] += robot.getManhattanError() / ((double)nbrRuns);
				if(robot.correctEst())
					correct++;
			}
			correctRatio += (correct / nbrObservations) / nbrRuns;
		}
		
		System.out.println();
		System.out.println("Statistics:");
		System.out.printf("Correct(completely):\t%.5f\n", correctRatio);
		System.out.println("Observation:\t\tManhattan Distance(Average)");
		for(int obs = 0; obs < nbrObservations; obs++)
			System.out.printf((obs+1) + "\t\t\t%.5f\n", manhattan[obs]);
	}

}
