#include "sql/connect.h"
#include "config.h"


#include <mysql/mysql.h>



void connect(){
  MYSQL mysql;
  mysql_init(&mysql);


  //if(!mysql_real_connect(&mysql, , const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long clientflag))

}


