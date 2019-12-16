cmake_minimum_required(VERSION 3.15)
project(Yacc)

set(CMAKE_CXX_STANDARD 11)

add_executable(Yacc main.cpp type.h type.cpp PPT.cpp PPT.h ALLProductions.cpp ALLProductions.h)