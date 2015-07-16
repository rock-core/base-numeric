#define BOOST_TEST_MODULE DiscreteFilt
#include <boost/test/included/unit_test.hpp>
#include <numeric/DiscreteFilter.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE( filter_test )
{
	numeric::DiscreteFilter uniFilter(0.2, 1);
	numeric::DiscreteFilter multiFilter(0.2, 4);

	int uniFilterElements;
	double uniSamplingTime;
	double uniPole1 = -3;
	double uniPole2 = -4;
	double uniFilteredSignal = 0;
	double uniInputSignal = 3;

	int multiFilterElements;
	double multiSamplingTime;
	double multiPole1 = -1;
	double multiPole2 = 1;
	base::VectorXd multiFilteredSignal = base::VectorXd::Zero(4);
	base::VectorXd multiInputSignal = base::VectorXd::Zero(4);

	for (int i = 0; i < 4; i++)
		multiInputSignal[i] = 1;

	bool uniStatus = true;
	bool multiStatus = true;

	uniStatus = uniFilter.getSamplingTime(uniSamplingTime);
	uniStatus = uniFilter.getNumberElements(uniFilterElements);
	uniStatus = uniFilter.setPoles(uniPole1, uniPole2);
	uniStatus = uniFilter.getPoles(uniPole1, uniPole2);
	multiStatus = multiFilter.getSamplingTime(multiSamplingTime);
	multiStatus = multiFilter.getNumberElements(multiFilterElements);
	multiStatus = multiFilter.setPoles(multiPole1, multiPole2);
	multiStatus = multiFilter.getPoles(multiPole1, multiPole2);

	for (int i = 0; i < 20; i++)
	{
		uniStatus = uniFilter.calcOutput(uniFilteredSignal, uniInputSignal);
		multiStatus = multiFilter.calcOutput(multiFilteredSignal, multiInputSignal);
		if (i == 18)
			uniFilter.resetFilter();
	}

	BOOST_CHECK(uniSamplingTime == 0.2);
	BOOST_CHECK(uniFilterElements == 1);
	BOOST_CHECK(uniPole1 == -3);
	BOOST_CHECK(uniPole2 == -4);
	BOOST_CHECK(multiFilterElements == 4);
	BOOST_CHECK(multiSamplingTime == 0.2);
	BOOST_CHECK(multiPole1 == -1);
	BOOST_CHECK(multiPole2 == -1);


	BOOST_CHECK(uniStatus == true);
	BOOST_CHECK(multiStatus == true);

//	std::cout << "\n\n uniFilteredSignal \n" << uniFilteredSignal << std::endl;
//	std::cout << "\n multiFilteredSignal \n" << multiFilteredSignal << std::endl;
//	std::cout << "\n uniPole1 \n" << uniPole1 << std::endl;
//	std::cout << "\n uniPole2 \n" << uniPole2 << std::endl;
//	std::cout << "\n multiPole1 \n" << multiPole1 << std::endl;
//	std::cout << "\n multiPole2 \n" << multiPole2 << std::endl;
//	std::cout << "\n\n uniSamplingTime " << uniSamplingTime << std::endl;
//	std::cout << "\n uniFilterElements " << uniFilterElements << std::endl;
}
