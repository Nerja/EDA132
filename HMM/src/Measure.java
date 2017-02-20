import model.HMMLocalizer;

public class Measure {

	/**
	 * Run several instances of 40 observations to
	 * get some more accurate measurement.
	 */
	public static void main(String[] args) {
		int nbrObservations = 40, nbrRuns = 40;
		System.out.println("Running " + nbrRuns + " runs with " + nbrObservations + " observations each");
		double manhattan[] = new double[nbrObservations];
		double correctRatio = 0;
		for(int run = 0; run < nbrRuns; run++) {
			System.out.println("Run\t" + (run+1) + "/" + nbrRuns);
			HMMLocalizer robot = new HMMLocalizer( 8, 8, 4);
			double correct = 0;
			for(int obs = 0; obs < nbrObservations; obs++) {
				robot.update();
				manhattan[obs] += robot.getManhattanError();
				if(robot.correctEst())
					correct++;
			}
			correctRatio += (correct / nbrObservations);
		}
		
		System.out.println();
		System.out.println("Statistics:");
		System.out.println("Correct(completely):\t" + (correctRatio/nbrRuns));
		System.out.println("Observation:\t\tManhattan Distance(Average)");
		for(int obs = 0; obs < nbrObservations; obs++) {
			manhattan[obs] /= nbrRuns;
			System.out.println((obs+1) + "\t\t\t" + manhattan[obs]);
		}
	}

}
