rock_library(numeric
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(numeric_bin Main.cpp
    DEPS numeric)

