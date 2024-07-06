#include "transport_hla/sql/SQLConnect.h"
#include "config.h"

#include <iostream>
#include <mysql/mysql.h>

SQLConnector::SQLConnector() {
  con_ = new MYSQL;
  field_idx_ = 0;
  res_ = nullptr;
  row_ = nullptr;
  mysql_init(con_);
  if (!mysql_real_connect(con_, sql_host, user, password, db_name, sql_port,
                          nullptr, CLIENT_FOUND_ROWS)) {
    throw mysql_error(con_);
  }
}
SQLConnector::~SQLConnector() {
  mysql_close(con_);
  delete con_;
}

void SQLConnector::execute(const char *sql) {
  field_idx_ = 0;
  if (mysql_query(con_, sql) != 0) {
    throw mysql_error(con_);
  }
  res_ = mysql_store_result(con_);
  
  // while ((row_ = mysql_fetch_row(res_)) != nullptr) {
  //   for (int j = 0; j < mysql_num_fields(res_); j++) {
  //     std::cout << row_[j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
}

MYSQL_ROW SQLConnector::getRow() { return row_; }
MYSQL_ROW SQLConnector::beginRow(const char *sql) {
  field_idx_ = 0;
  if (mysql_query(con_, sql) != 0) {
    throw mysql_error(con_);
  }
  res_ = mysql_store_result(con_);
  row_ = mysql_fetch_row(res_);
  return row_;
}
bool SQLConnector::endRow() {
  if (row_ == nullptr) {
    mysql_free_result(res_);
  }

  return row_ == nullptr;
}
MYSQL_ROW SQLConnector::nextRow() {
  row_ = mysql_fetch_row(res_);

  return row_;
}
int SQLConnector::get_num_fields() { return mysql_num_fields(res_); }

char *SQLConnector::beginField(MYSQL_ROW) {
  field_idx_ = 0;
  return row_[field_idx_++];
}
char *SQLConnector::nextField(MYSQL_ROW) { return row_[field_idx_++]; }
bool SQLConnector::endField(MYSQL_ROW) {
  return field_idx_ < mysql_num_fields(res_);
}
