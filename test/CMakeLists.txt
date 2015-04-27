rock_testsuite(
    unit_test suite.cpp
        test.cpp
        numeric.cpp
    DEPS_PKGCONFIG base-types)

#do not build this test if gsl is not installed 

pkg_check_modules(GSL "gsl")
if(GSL_FOUND)
    rock_testsuite(
        unit_test_poly_fit test_polyfit.cpp
        DEPS_PKGCONFIG gsl)
else(GSL_FOUND)
    message(STATUS "Cannot find gsl. Skip unit test for FitPolynom")
endif(GSL_FOUND)
