#ifndef DISCRETEFILTER_HPP_
#define DISCRETEFILTER_HPP_

#include "base/Eigen.hpp"

namespace numeric {

/**
 * 	This library implements a second order low pass discrete filter from a
 * 	continuous filter using the matched pole-zero methods.
 *
 *		   (CONTINUOUS FILTER)						    (DISCRETE FILTER)
 *			        kc								           kd
 *	G(s) =  -----------------   ======> G(z) = --------------------------------
 *			 (s + p1)(s + p2)					 (1 - ep1T*z^⁻1)(1 - ep2T*z^-2)
 *
 *  where:
 *
 *	- epT: Continuous pole matched in discrete time ===> epT = exp(p*T)
 *	- p:   Continuous pole = -p1 and -p2
 *	- T:   Sample time
 *	- kd:  Unitary static gain			===> kd = 1 - (ep1T + ep2T) + ep1T*ep2T
 *
 *  The resulting difference equation is:
 *
 *	y[k] = kd*u[k] + (ep1T + ep2T)*y[k-1] - ep1T*ep2T*y[k-2]
 *
 *  This difference equation is implemented in this library to filter a
 *  specified input signal.
 *
 *  The user may worry about setting only the pole1, because the pole2 will
 *  copy the pole1's value if pole2's value is not set. The value of pole1
 *  must be less than zero, and basically, the smaller pole1's value the
 *  more intense is the filtering (the cutting frequency is lower).
 *
 */

class DiscreteFilter {

public:
	DiscreteFilter(double sampleTime, int numberElements = 1);

	/**
	 * Calculates the filtered signal for a univariable input.
	 *
	 * @param inputSignal - Signal to be filtered.
	 * @param filteredSignal - Univariable filtered signal.
	 * @result - Boolean variable to confirm that everything went fine.
	 */
	bool calcOutput(double &filteredSignal,	double &inputSignal);

	/**
	 * Calculates the filtered signal for a multivariable input.
	 *
	 * @param inputSignal - Signal to be filtered.
	 * @param filteredSignal - Multivariable filtered signal.
	 * @result - Boolean variable to confirm that everything went fine.
	 */
	bool calcOutput(base::VectorXd &filteredSignal, base::VectorXd &inputSignal);

	/**
	 * Sets the continuous filter poles.
	 *
	 * @param pole1 - First continuous filter pole (must be greater than zero).
	 * @param pole1 - Second continuous filter pole (optional).
	 * @result - Boolean variable to confirm that everything went fine.
	 */
	bool setPoles(double pole1, double pole2 = 1);

	/**
	 * Gets the continuous filter poles.
	 *
	 * @param pole1 - First continuous filter pole.
	 * @param pole1 - Second continuous filter pole.
	 * @result - Boolean variable to confirm that everything went fine.
	 */
	bool getPoles(double &pole1, double &pole2);

	/**
	 * Gets the sampling time.
	 *
	 * @param sampleTime - Sampling time.
	 * @result - Boolean variable to confirm that everything went fine.
	 */
	bool getSamplingTime(double &sampleTime);

	/**
	 * Gets the number of input elements that the filter expects.
	 *
	 * @param pole1 - Number of input elements.
	 * @result - Boolean variable to confirm that everything went fine
	 */
	bool getNumberElements(int &numberElements);

	/**
	 * Resets the filter (resets the past values matrix).
	 *
	 * @result - Boolean variable to confirm that everything went fine
	 */
	bool resetFilter();

private:

	/**
	 * First pole of the discrete filter.
	 */
	double gMappedPole1;

	/**
	 * Second pole of the discrete filter.
	 */
	double gMappedPole2;

	/**
	 * First pole of the continuous filter.
	 */
	double gContinuousPole1;

	/**
	 * Second pole of the continuous filter.
	 */
	double gContinuousPole2;

	/**
	 * Sampling time.
	 */
	double gSamplingTime;

	/**
	 * Number of input elements the filter is supposed to receive.
	 */
	int gNumFilterElements;

	/**
	 * Variable that identifies if there was any error with the filter set up.
	 */
	bool gErrorStatus;

	/**
	 * Variable that contains the last three samples
	 */
	base::MatrixXd gPastValues;

	/**
	 * Status variable for warning that the filter has not been set yet
	 */
	bool gFilterNotSet;

	/**
	 * Calculates the filtering for a multivariable signal.
	 *
	 * @param filteredSignal - Filtered signal.
	 * @param newSignalvalue - New signal input's value.
	 * @param pastSignalValues - Matrix with the past output values.
	 */
	void calcFiltering(base::VectorXd &filteredSignal,
					   base::VectorXd &newSignalValue,
					   base::MatrixXd &pastSignalValues);

	/**
	 * Calculates the filtering for a univariable signal.
	 *
	 * @param filteredSignal - Filtered signal.
	 * @param newSignalvalue - New signal input's value.
	 * @param pastSignalValues - Matrix with the past output values.
	 */
	void calcFiltering(double &filteredSignal, double &newSignalValue,
					   base::MatrixXd &pastSignalValues);

	/**
	 * Updates the matrix that contains the signal's past values.
	 * For univariable filters.
	 *
	 * @param newValues - New signal values to be added to the matrix
	 * @param matrix - The matrix that contains the past values of the signal
	 */
	void updateMatrix(base::MatrixXd &matrix, double &newValue);

	/**
	 * Updates the matrix that contains the signal's past values.
	 * For multivariable filters.
	 *
	 * @param newValues - New signal values to be added to the matrix
	 * @param matrix - The matrix that contains the past values of the signal
	 */
	void updateMatrix(base::MatrixXd &matrix, base::VectorXd &newValues);

	/**
	 * Prints warning message telling that the filter is being used
	 * without beeing set.
	 */
	void printWarn();

	/**
	 * Prints error message telling that the number of expected input elements
	 * is different from the number of received input elements.
	 */
	void printError(int inputElements);
};

} /* namespace numeric */

#endif /* DISCRETEFILTER_HPP_ */
