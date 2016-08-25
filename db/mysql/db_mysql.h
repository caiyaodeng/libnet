/*************************************************************************
> File Name: db_mysql.h
> Author: caiyaodeng
> Mail: 512324291@qq.com
> Created Time: Fri 01 Apr 2016 02:20:57 AM EDT
************************************************************************/

#ifndef _DB_MYSQL_H_
#define _DB_MYSQL_H_

#include <mysql/mysql.h>
#include <string.h>

namespace mynamespace {
    class MyDB {
        public:
            MyDB ();
            ~MyDB ();
        public:
            void init_db ();
            int conn_db (const char *host_name, const char *user_name, const char *password, const char *db_name);
            void disconn_db ();
            int sql_db(const char *src, char ***&result_out, int *row, int *column);//参数src为要执行的SQL语句
            int get_string(char *&pResult);

        private:
            int exec_db (const char *SQL);
            int open_db (const char *SQL, char ***&result_out, int *row, int *column);
            MYSQL m_mysql;
            MYSQL *m_connection;
            char *m_pResult;
    };
}
#endif
