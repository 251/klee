//===-- PTreeWriter.cpp ---------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "PTreeWriter.h"

#include "PTree.h"
#include "klee/Support/ErrorHandling.h"

using namespace klee;


void prepare_statement(sqlite3 * db, std::string &query, sqlite3_stmt ** stmt) {
#ifdef SQLITE_PREPARE_PERSISTENT
  if (sqlite3_prepare_v3(db, query.c_str(), -1, SQLITE_PREPARE_PERSISTENT, stmt, nullptr) != SQLITE_OK) {
#else
  if (sqlite3_prepare_v3(db, query.c_str(), -1, 0, stmt, nullptr) != SQLITE_OK) {
#endif
    klee_warning("Process tree database: can't prepare query: %s [%s]", sqlite3_errmsg(db), query.c_str());
    sqlite3_close(db);
    klee_error("Process tree database: can't prepare query: %s", query.c_str());
  }
}


PTreeWriter::PTreeWriter(const std::string &dbPath) {
  // create database file
  if(sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK)
    klee_error("Can't create process tree database: %s", sqlite3_errmsg(db));

  // - set options: asynchronous + WAL
  char * errMsg = nullptr;
  if (sqlite3_exec(db, "PRAGMA synchronous = OFF;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
    klee_warning("Process tree database: can't set option: %s", errMsg);
    sqlite3_free(errMsg);
  }
  if (sqlite3_exec(db, "PRAGMA journal_mode = WAL;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
    klee_warning("Process tree database: can't set option: %s", errMsg);
    sqlite3_free(errMsg);
  }

  // - create table
  std::string query = "CREATE TABLE IF NOT EXISTS nodes ("
                      "ID INT PRIMARY KEY, stateID INT, leftID INT, rightID INT,"
                      "asmLine INT, branchType INT, terminationTypeMask INT);";
  char * zErr = nullptr;
  if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &zErr) != SQLITE_OK) {
    klee_warning("Process tree database: initialisation error: %s", zErr);
    sqlite3_free(zErr);
    sqlite3_close(db);
    klee_error("Process tree database: initialisation error.");
  }

  // create prepared statements
  // - insertStmt
  query = "INSERT INTO nodes VALUES (?, ?, ?, ?, ?, ?, ?);";
  prepare_statement(db, query, &insertStmt);
  // - updateStmt
  query = "UPDATE nodes SET terminationTypeMask=? WHERE ID=?;";
  prepare_statement(db, query, &updateStmt);
  // - transactionBeginStmt
  query = "BEGIN TRANSACTION";
  prepare_statement(db, query, &transactionBeginStmt);
  // - transactionCommitStmt
  query = "COMMIT TRANSACTION";
  prepare_statement(db, query, &transactionCommitStmt);

  // begin transaction
  auto rc = sqlite3_step(transactionBeginStmt);
  if (rc != SQLITE_DONE) {
    klee_warning("Process tree database: can't begin transaction: %s", sqlite3_errmsg(db));
  }
  sqlite3_reset(transactionBeginStmt);
}


void PTreeWriter::finalize() {
  // finalize prepared statements
  sqlite3_finalize(insertStmt);
  sqlite3_finalize(updateStmt);
  sqlite3_finalize(transactionBeginStmt);
  sqlite3_finalize(transactionCommitStmt);

  // commit
  sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
}


PTreeWriter::~PTreeWriter() {
  finalize();
  sqlite3_close(db);
}


void PTreeWriter::batchCommit() {
  ++batch;
  if (batch < PTreeWriterBatchSize) return;

  // commit and begin transaction
  auto rc = sqlite3_step(transactionCommitStmt);
  if (rc != SQLITE_DONE) {
    klee_warning("Process tree database: transaction commit error: %s", sqlite3_errmsg(db));
  }
  sqlite3_reset(transactionCommitStmt);

  rc = sqlite3_step(transactionBeginStmt);
  if (rc != SQLITE_DONE) {
    klee_warning("Process tree database: transaction begin error: %s", sqlite3_errmsg(db));
  }
  sqlite3_reset(transactionBeginStmt);
  batch = 0;
}


void PTreeWriter::update(const PTreeNode &node) {
  // bind values
  auto res = sqlite3_bind_int64(updateStmt, 1, static_cast<std::uint32_t>(node.terminationTypeMask));
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int(updateStmt, 2, node.id);
  assert(res == SQLITE_OK);

  // update
  auto rc = sqlite3_step(updateStmt);
  if (rc != SQLITE_DONE) {
    klee_warning("Process tree database: can't update data for node: %lu: %s", node.id, sqlite3_errmsg(db));
  }
  rc = sqlite3_reset(updateStmt);
  if (rc != SQLITE_OK) {
    klee_warning("Process tree database: error reset node: %lu: %s", node.id, sqlite3_errmsg(db));
  }

  batchCommit();
}


void PTreeWriter::write(const PTreeNode &node) {
  // bind values
  auto res = sqlite3_bind_int64(insertStmt, 1, node.id);
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int(insertStmt, 2, node.state->getID());
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int64(insertStmt, 3, node.left.getPointer() ? node.left.getPointer()->id : 0);
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int64(insertStmt, 4, node.right.getPointer() ? node.right.getPointer()->id : 0);
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int(insertStmt, 5, node.asmLine);
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int(insertStmt, 6, static_cast<std::uint8_t>(node.branchType));
  assert(res == SQLITE_OK);
  res = sqlite3_bind_int(insertStmt, 7, static_cast<std::uint32_t>(node.terminationTypeMask));
  assert(res == SQLITE_OK);

  // insert
  auto rc = sqlite3_step(insertStmt);
  if (rc != SQLITE_DONE) {
    klee_warning("Process tree database: can't persist data for node: %lu: %s", node.id, sqlite3_errmsg(db));
  }
  rc = sqlite3_reset(insertStmt);
  if (rc != SQLITE_OK) {
    klee_warning("Process tree database: error reset node: %lu: %s", node.id, sqlite3_errmsg(db));
  }

  batchCommit();
}
