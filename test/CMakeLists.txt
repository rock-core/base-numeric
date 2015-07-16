rock_testsuite(unit_test
    suite.cpp
    test.cpp
    test_Combinatorics.cpp
    test_IntegerPartitioning.cpp
    test_LimitedCombination.cpp
    numeric.cpp
    DEPS numeric)
    
rock_testsuite(unit_test_filter
	test_filter.cpp
    #DEPS_PKGCONFIG base-types
    DEPS numeric
)

#do not build this test if gsl is not installed 

pkg_check_modules(GSL "gsl")
if(GSL_FOUND)
    rock_testsuite(
        unit_test-fit_polynom test_FitPolynom.cpp
        DEPS_PKGCONFIG gsl)
else(GSL_FOUND)
    message(STATUS "Cannot find gsl. Skip unit test for FitPolynom")
endif(GSL_FOUND)
