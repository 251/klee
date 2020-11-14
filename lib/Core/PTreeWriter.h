//===-- PTreeWriter.h -------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_PTREEWRITER_H
#define KLEE_PTREEWRITER_H

#include <sqlite3.h>

#include <string>

namespace klee {
  class PTreeNode;

#define PTreeWriterBatchSize 1000U

/// @brief Writes the process tree into an SQLite database (ptree.db)
class PTreeWriter {
  ::sqlite3 * db {nullptr};
  ::sqlite3_stmt * insertStmt {nullptr};
  ::sqlite3_stmt * updateStmt {nullptr};
  ::sqlite3_stmt * transactionBeginStmt {nullptr};
  ::sqlite3_stmt * transactionCommitStmt {nullptr};
  std::uint32_t batch {0};

  /// Write in batches of size PTreeWriterBatchSize
  void batchCommit();
  /// Commit data and finalize prepared statements
  void finalize();
public:
  explicit PTreeWriter(const std::string &dbPath);
  ~PTreeWriter();

  PTreeWriter(const PTreeWriter &other) = delete;
  PTreeWriter(PTreeWriter &&other) noexcept = delete;

  PTreeWriter &operator=(const PTreeWriter &other) = delete;
  PTreeWriter &operator=(PTreeWriter &&other) noexcept = delete;

  /// Update node data in database
  void update(const PTreeNode &node);
  /// Write new node into database
  void write(const PTreeNode &node);
};

} // klee
#endif // KLEE_PTREEWRITER_H
