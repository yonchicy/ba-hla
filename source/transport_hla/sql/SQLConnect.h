
#ifndef __SQL_CONNECT_H__
#define __SQL_CONNECT_H__

#include <mysql/mysql.h>
class SQLConnector {
private:
  MYSQL *con_;
  MYSQL_RES* res_;
  MYSQL_ROW row_;
  int field_idx_ ;

public:
  SQLConnector();
  ~SQLConnector();
  void execute(const char *sql);
  MYSQL_ROW getRow();
  MYSQL_ROW beginRow(const char* sql);
  bool endRow();
  MYSQL_ROW nextRow();
  int get_num_fields();
  char* beginField(MYSQL_ROW);
  char* nextField(MYSQL_ROW);
  bool endField(MYSQL_ROW);
};

#endif // !__SQL_CONNECT_H__
