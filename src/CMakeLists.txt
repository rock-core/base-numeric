install(FILES
    stats.hpp
    histogram.hpp
    FitPolynom.hpp
    MatchTemplate.hpp
    DESTINATION include/numeric)

set(DEPS_PKGCONFIG "base-types")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/numeric.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/numeric.pc @ONLY)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/numeric.pc
    DESTINATION lib/pkgconfig)


