/*************************************************************************
> File Name: db_mysql.cpp
> Author: caiyaodeng
> Mail:512324291@qq.com 
> Created Time: Fri 01 Apr 2016 02:42:12 AM EDT
************************************************************************/

#include <iostream>
#include <stdlib.h>
#include "db_mysql.h"
using namespace std;

namespace mynamespace {
    MyDB::MyDB () {
    }

    MyDB::~MyDB () {

    }

    void MyDB::init_db () {
        mysql_init(&m_mysql);//初始化mysql
    }

    int MyDB::conn_db (const char *host_name, const char *user_name, const char *password, const char *db_name) {
        if (m_connection)
        mysql_close(m_connection);
        m_connection = mysql_real_connect(&m_mysql, host_name, user_name, password, db_name, 0, 0, 0);//连接到mysql

        if (m_connection == NULL) {
            std::cout << mysql_error(&m_mysql) << std::endl;
            return -1;//连接失败，返回-1
        }

        exec_db("set names utf8;");

        std::cout << "success connect to mysql" << std::endl;
        return 0;
    }

    void MyDB::disconn_db () {
        if (m_connection) {
            mysql_close(m_connection);
            m_connection = NULL;
        }
    }

    int MyDB::sql_db(const char *src, char ***&result_out, int *row, int *column) {//参数src为要执行的SQL语句
        if ((strncmp(src, "select", 6) == 0) || (strncmp(src, "SELECT", 6) == 0)
                    || (strncmp(src, "show", 4) == 0) || (strncmp(src, "SHOW", 4) == 0)
                    || (strncmp(src, "desc", 4) == 0) || (strncmp(src, "DESC", 4) == 0)) {
                        return open_db(src, result_out, row, column);//如果src为有返回数据集SQL语句，那么调用open_db函数
        } else {
            return exec_db(src);
        }
    }

    int MyDB::exec_db (const char *SQL) {
        int state = mysql_query(m_connection, SQL);//执行SQL语句
        if (state != 0) {
            std::cout << mysql_error(&m_mysql) << std::endl;
            return -1;
        }
        std::cout << "query is ok, " << (unsigned int)mysql_affected_rows(m_connection) << "rows affected" << std::endl;
        return 0;
    }

    int MyDB::open_db (const char *SQL, char ***&result_out, int *row, int *column) {
        int state = mysql_query(m_connection, SQL);//执行SQL语句
        if (state != 0) {
            std::cout << mysql_error(m_connection) << std::endl;
            return -1;//执行失败，返回-1
        }
        //sql strtok
        char *table_name=NULL;
        char temp_sql[200];
        memset(temp_sql,0,200);
        int flag_strtok=0;

        strncpy(temp_sql,SQL,sizeof(char)*strlen(SQL));
        char * par=temp_sql;
        while((table_name=strtok(par," "))!=NULL){
            par=NULL;
            if((strcasecmp(table_name,"from")==0)||(strcasecmp(table_name,"*from")==0)){
                table_name=strtok(par," ");
                flag_strtok=1;
                break;
            }
        }

	if(table_name!=NULL){
		if(table_name[strlen(table_name)-1]==';')
			table_name[strlen(table_name)-1]='\0';
       		if(flag_strtok==0)
       			memset(table_name,0,strlen(table_name));
	}

        MYSQL_RES *result = mysql_store_result(m_connection);//得到查询结果
        if (result == (MYSQL_RES *) NULL) {
            std::cout << mysql_error(m_connection) << std::endl;
            return -1;//执行失败，返回-1
        } else {
            MYSQL_FIELD *sqlField;
            int iFieldCount = 0;
            *column= mysql_num_fields(result);
            *row = mysql_num_rows(result);

            int m_pResult_length=0;
            //memory
            result_out = (char ***)malloc((1+*row)*sizeof(char**));
            for (int i=0; i<1+*row; i++) {
            result_out[i] = (char **)malloc(*column*sizeof(char*));
            }

            while (1){ //循环遍历所有字段
                sqlField = mysql_fetch_field(result);
                if (sqlField == NULL)
                    break;
                result_out[0][iFieldCount] = (char *)malloc((1+strlen(sqlField->name))*sizeof(char));
                strncpy(result_out[0][iFieldCount], sqlField->name, 1+strlen(sqlField->name));
                m_pResult_length=strlen(sqlField->name)*(*row)+m_pResult_length;
                iFieldCount++; // lie
            }        

            MYSQL_ROW sqlRow;
            int iCount = 1;
            while (1) {//循环到每一行
                using namespace std;
                sqlRow = mysql_fetch_row(result);
                if (sqlRow == NULL)
                    break;
                    int i;
                    for (i=0; i<*column; i++) {//循环得到每一行中的每个字段
                        if (sqlRow[i] == NULL) {
                            cout << iCount << endl;
                                result_out[iCount][i] = (char *)malloc((5*sizeof(char)));
                                strncpy(result_out[iCount][i], (const char *)"NULL", 1+strlen("NULL"));
                                m_pResult_length=4+m_pResult_length;
                        }
                        else{
                            result_out[iCount][i] = (char *)malloc((1+strlen((const char *)sqlRow[i]))*sizeof(char));
                            strncpy(result_out[iCount][i], (const char *)sqlRow[i], 1+strlen((const char *)sqlRow[i]));
                            m_pResult_length=strlen((const char *)sqlRow[i])+m_pResult_length;
                        }
                    }
                iCount ++;
                }
            std::cout << "query is ok, " << (unsigned int)mysql_affected_rows(m_connection) << "rows affected" << std::endl;

            if(table_name!=NULL)
	    	m_pResult_length=m_pResult_length+strlen(table_name);
            m_pResult_length=m_pResult_length+2+2+1+2+2*(*row)+(*row)*(2*(*column)-1+4*(*column))+*row-1;
            m_pResult=(char *)malloc(m_pResult_length);
		memset(m_pResult,0,m_pResult_length);
            strcat(m_pResult,"{");
            strcat(m_pResult,"\"");
	    if(table_name!=NULL)
            	strcat(m_pResult,table_name);
            strcat(m_pResult,"\"");
            strcat(m_pResult,":");
            strcat(m_pResult,"[");
            for(int i=1;i<=*row;i++){
                if(i!=1)
                    strcat(m_pResult,",");
                strcat(m_pResult,"{");

                for(int j=0;j<*column;j++){
                    if(j!=0)
                        strcat(m_pResult,",");
                    
                    strcat(m_pResult,"\"");
                    strcat(m_pResult,result_out[0][j]);
                    strcat(m_pResult,"\"");
                    strcat(m_pResult,":");
                    strcat(m_pResult,"\"");
                    strcat(m_pResult,result_out[i][j]);
                    strcat(m_pResult,"\"");

                }
                strcat(m_pResult,"}");
            }
            strcat(m_pResult,"]");
            strcat(m_pResult,"}");

            mysql_free_result(result);
        }
        return 0;
    }
    int MyDB::get_string(char *&pResult){
    	pResult=m_pResult;
    	return strlen(pResult);
    }
}
