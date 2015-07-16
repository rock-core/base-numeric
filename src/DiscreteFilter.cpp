
#include "DiscreteFilter.hpp"
#include <base/Logging.hpp>

namespace numeric {

DiscreteFilter::DiscreteFilter(double samplingTime, int numFilterElements)
{
	gContinuousPole1 = -1;
	gContinuousPole2 = -1;

	gMappedPole1 = exp(gContinuousPole1*gSamplingTime);
	gMappedPole2 = exp(gContinuousPole2*gSamplingTime);

	gErrorStatus = false;

	if(samplingTime > 0)
		gSamplingTime = samplingTime;
	else
	{
		gSamplingTime = 0.1;

		LOG_ERROR("\n\n\x1b[31m (Library: DiscreteFilter.cpp)"
				  " The provided samplingTime (%f) is not positive."
				  "\x1b[0m\n\n", samplingTime);

		gErrorStatus = true;
	}

	if(numFilterElements > 0)
		gNumFilterElements = numFilterElements;
	else
	{
		gNumFilterElements = 1;

		LOG_ERROR("\n\n\x1b[31m (Library: DiscreteFilter.cpp)"
				  " The provided number of filter elements (%i) is not"
				  " positive.\x1b[0m\n\n", numFilterElements);

		gErrorStatus = true;
	}

	gPastValues = base::MatrixXd::Zero(gNumFilterElements,2);

	gFilterNotSet = true;
}

bool DiscreteFilter::calcOutput(double &filteredSignal, double &inputSignal)
{
	if(gErrorStatus)
		return false;

	int numFilterElements = gPastValues.rows();
	int numInputElements = 1;

	if(gFilterNotSet)
		printWarn();

	if(numFilterElements == 1)
	{
		calcFiltering(filteredSignal, inputSignal, gPastValues);

		updateMatrix(gPastValues, filteredSignal);

		return true;
	}
	else
	{
		printError(numInputElements);
		return false;
	}
}

bool DiscreteFilter::calcOutput(base::VectorXd &filteredSignal, base::VectorXd &inputSignal)
{
	if(gErrorStatus)
		return false;

	int numFilterElements = gPastValues.rows();
	int numInputElements = inputSignal.size();

	if(gFilterNotSet)
		printWarn();

	if(numFilterElements == numInputElements)
	{
		calcFiltering(filteredSignal, inputSignal, gPastValues);

		updateMatrix(gPastValues, filteredSignal);

		return true;
	}
	else
	{
		printError(numInputElements);
		return false;
	}
}

bool DiscreteFilter::setPoles(double pole1, double pole2)
{
	if(gErrorStatus)
		return false;

	if (pole1 < 0)
		gContinuousPole1 = pole1;
	else
	{
		LOG_ERROR("\n\n\x1b[31m (Library: DiscreteFilter.cpp) The value of "
				  "pole 1 (%d) should be less than 0.\x1b[0m\n\n"
				  ,gContinuousPole1);
		return false;
	}

	if (pole2 < 0)
		gContinuousPole2 = pole2;
	else
		gContinuousPole2 = pole1;

	gMappedPole1 = exp(gContinuousPole1*gSamplingTime);
	gMappedPole2 = exp(gContinuousPole2*gSamplingTime);
	gFilterNotSet = false;
	return true;
}

bool DiscreteFilter::getPoles(double &pole1, double &pole2)
{
	pole1 = gContinuousPole1;
	pole2 = gContinuousPole2;

	if(gErrorStatus)
		return false;
	return true;
}

bool DiscreteFilter::getSamplingTime(double &samplingTime)
{
	samplingTime = gSamplingTime;

	if(gErrorStatus)
		return false;

	return true;
}

bool DiscreteFilter::getNumberElements(int &numFilterElements)
{
	numFilterElements = gNumFilterElements;

	if(gErrorStatus)
		return false;
	return true;
}

bool DiscreteFilter::resetFilter()
{
	if(gErrorStatus)
		return false;

	gPastValues = base::MatrixXd::Zero(gPastValues.rows(), gPastValues.cols());

	return true;
}

void DiscreteFilter::calcFiltering(double &filteredSignal, double &newSignalValue,
								   base::MatrixXd &pastSignalValues)
{
	base::VectorXd vecFilteredSignal = base::VectorXd::Zero(1);
	base::VectorXd vecNewSignalValue = base::VectorXd::Zero(1);

	vecNewSignalValue[0] = newSignalValue;

	calcFiltering(vecFilteredSignal, vecNewSignalValue, pastSignalValues);

	filteredSignal = vecFilteredSignal[0];
}

void DiscreteFilter::calcFiltering(base::VectorXd &filteredSignal,
								   base::VectorXd &newSignalValue,
								   base::MatrixXd &pastSignalValues)
{
	for (int i = 0; i < filteredSignal.size(); i++)
	{
		filteredSignal[i] = (1 - (gMappedPole1 + gMappedPole2) +
							gMappedPole1*gMappedPole2)*newSignalValue[i] +
							(gMappedPole1+gMappedPole2)*pastSignalValues(i,1) -
							gMappedPole1*gMappedPole2*pastSignalValues(i,0);
	}
}

void DiscreteFilter::updateMatrix(base::MatrixXd &matrix, double &newValue)
{
	base::VectorXd newValues = base::VectorXd::Zero(1);

	newValues[0] = newValue;

	updateMatrix(gPastValues, newValues);
}

void DiscreteFilter::updateMatrix(base::MatrixXd &matrix,
								  base::VectorXd &newValues)
{
	int numRows = matrix.rows();
	int numColumns = matrix.cols();

	matrix.block(0, 0, numRows, numColumns - 1) =
	matrix.block(0, 1, numRows, numColumns - 1);

	matrix.block(0, numColumns - 1, numRows, 1) =
	newValues;
}

void DiscreteFilter::printWarn()
{
	static bool warned = false;

	if(!warned)
	{
		LOG_WARN("\n\n\x1b[33m (Library: DiscreteFilter.cpp) The filter "
		"was used before being set. Using default parameters...\x1b[0m\n\n");
		warned = true;
	}
}

void DiscreteFilter::printError(int numInputElements)
{
	int numFilterElements = gPastValues.rows();

	LOG_ERROR("\n\n\x1b[31m (Library: DiscreteFilter.cpp) This filter "
			"is supposed to receive %i elements at once,"
			" but has received %i elements instead.\x1b[0m\n\n", numFilterElements, numInputElements);
}
}/* namespace numeric */
