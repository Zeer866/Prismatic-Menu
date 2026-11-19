cmake_minimum_required(VERSION 3.21)
project(PrismaticMenu VERSION 0.1.0)

add_library(PrismaticMenu SHARED src/main.cpp)
setup_geode_mod(PrismaticMenu)
