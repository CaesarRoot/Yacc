cmake_minimum_required(VERSION 3.15)
project(Lexer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Lexer Lexer.cpp RE_Suffix.cpp RE_NFA.cpp RE_Suffix.h type.h NFA_DFA.cpp NFA_DFA.h READ_File.cpp READ_file.h DFA_DFAo.cpp DFA_DFAo.h Utils.cpp Utils.h)