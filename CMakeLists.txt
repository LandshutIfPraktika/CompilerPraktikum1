cmake_minimum_required(VERSION 3.5)
project(GPLACompiler)

set(CMAKE_CXX_STANDARD 98)

set(PARSER_FILES
        error.cxx
        global.h
        init.cxx
        lexan.cxx
        parsemain.cxx
        parser.cxx
        symbol.cxx)

set(LEXER_FILES
        error.cxx
        global.h
        init.cxx
        lexan.cxx
        lexmain.cxx
        symbol.cxx)



add_executable(GPLACompiler ${PARSER_FILES})

add_executable(Lexer ${LEXER_FILES})

set_property(TARGET Lexer PROPERTY CXX_STANDARD 98)
set(COMPILE_OPTIONS PUBLIC -Wall PUBLIC -g)
target_compile_options(GPLACompiler ${COMPILE_OPTIONS})
target_compile_options(Lexer ${COMPILE_OPTIONS})