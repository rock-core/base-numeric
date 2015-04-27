set(HEADERS
    Stats.hpp
    Histogram.hpp
    FitPolynom.hpp
    PlaneFitting.hpp
    MatchTemplate.hpp
    backwards/Histogram.hpp
    backwards/PlaneFitting.hpp
    backwards/Stats.hpp
)
rock_install_headers(${HEADERS})

set(DEPS_PKGCONFIG "base-types")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/numeric.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/numeric.pc @ONLY)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/numeric.pc
    DESTINATION lib/pkgconfig)


