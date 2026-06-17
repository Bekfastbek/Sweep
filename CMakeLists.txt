cmake_minimum_required(VERSION 4.2)
project(sweep C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(sweep src/main.c
        src/lexer.c
        include/lexer.h
        include/tokens.h
        include/types.h
        include/anyarr.h
        src/parse_words.c
        include/parse_words.h
        tests/char.c
        tests/keywords.c
        tests/keywords_parse.c
        include/tests.h
        tests/parse.c
        src/ast.c
)

target_compile_options(sweep PRIVATE
        -O3
        -march=native
)