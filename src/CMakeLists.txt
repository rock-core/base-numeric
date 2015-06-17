rock_library(numeric
    HEADERS
        backwards/Histogram.hpp
        backwards/PlaneFitting.hpp
        backwards/Stats.hpp
        FitPolynom.hpp
        Histogram.hpp
        IntegerPartitioning.hpp
        MatchTemplate.hpp
        PlaneFitting.hpp
        Stats.hpp
    SOURCES
        IntegerPartitioning.cpp
    DEPS_PKGCONFIG base-types base-lib
)
