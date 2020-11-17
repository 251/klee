#===------------------------------------------------------------------------===#
#
#                     The KLEE Symbolic Virtual Machine
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#

add_executable(klee-ptree main.cpp Tree.cpp Info.cpp Dot.cpp)

target_link_libraries(klee-ptree PUBLIC ${SQLITE3_LIBRARIES})

install(TARGETS klee-ptree DESTINATION bin)
