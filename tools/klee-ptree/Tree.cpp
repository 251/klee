//===-- Tree.cpp ------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Tree.h"

#include <sqlite3.h>

#include <cstdlib>
#include <iostream>


Tree::Tree(const std::string &path) {
  // open db
  ::sqlite3 * db;
  if(sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK) {
    std::cerr << "Can't open process tree database: " << sqlite3_errmsg(db) << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialise prepared statement
  ::sqlite3_stmt * readStmt;
  std::string query {"SELECT ID, stateID, leftID, rightID, asmLine, branchType, terminationTypeMask FROM nodes;"};
  if (sqlite3_prepare_v3(db, query.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &readStmt, nullptr) != SQLITE_OK) {
    std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
    exit(EXIT_FAILURE);
  }

  ::sqlite3_stmt * maxStmt;
  query = "SELECT MAX(ID) FROM nodes;";
  if (sqlite3_prepare_v3(db, query.c_str(), -1, SQLITE_PREPARE_PERSISTENT, &maxStmt, nullptr) != SQLITE_OK) {
    std::cerr << "Can't prepare statement: " << sqlite3_errmsg(db) << std::endl;
    exit(EXIT_FAILURE);
  }

  // read max id
  int rc;
  std::uint64_t maxID;
  if ((rc = sqlite3_step(maxStmt)) == SQLITE_ROW) {
    maxID = static_cast<std::uint64_t>(sqlite3_column_int(maxStmt, 0));

  } else {
    std::cerr << "Can't read maximum ID: " << sqlite3_errmsg(db) << std::endl;
    exit(EXIT_FAILURE);
  }

  // reserve space
  nodes.resize(maxID + 1);

  // read rows into vector
  while ((rc = sqlite3_step(readStmt)) == SQLITE_ROW) {
    const auto ID = static_cast<std::uint64_t>(sqlite3_column_int(readStmt, 0));
    const auto stateID = static_cast<std::uint32_t>(sqlite3_column_int(readStmt, 1));
    const auto left = static_cast<std::uint64_t>(sqlite3_column_int(readStmt, 2));
    const auto right = static_cast<std::uint64_t>(sqlite3_column_int(readStmt, 3));
    const auto asmLine = static_cast<std::uint32_t>(sqlite3_column_int(readStmt, 4));
    const auto branchType = static_cast<std::uint8_t>(sqlite3_column_int(readStmt, 5));
    const auto terminationTypeMask = static_cast<std::uint32_t>(sqlite3_column_int(readStmt, 6));

    // resize vector
    if (ID >= nodes.size())
      nodes.resize(ID + 1);

    // store children in sorted order
    nodes[ID] = {.left = left, .right = right, .stateID = stateID, .asmLine = asmLine,
                 .branchType = static_cast<BranchType>(branchType),
                 .terminationTypeMask = terminationTypeMask};
  }

  if (rc != SQLITE_DONE) {
    std::cerr << "Error while reading database: " << sqlite3_errmsg(db) << std::endl;
    exit(EXIT_FAILURE);
  }

  // close db
  sqlite3_finalize(maxStmt);
  sqlite3_finalize(readStmt);
  sqlite3_close(db);
}