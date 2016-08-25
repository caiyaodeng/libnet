/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 01 Apr 2016 04:31:00 AM EDT
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <iostream>
#include "db_mysql.h"
using namespace std;

int main () {
    mynamespace::MyDB *objDB = new mynamespace::MyDB();

    int row=0;
    int coulmn=0;
    char ***result = NULL;
    char *temp;

    objDB->init_db();

    int ret = objDB->conn_db("localhost", "root", "19940308", "SmartHome");
    if (ret < 0) {
        perror("connect");
    }

    //objDB->sql_db("select * from user;", (char ***&)result, &row, &coulmn);
    //objDB->sql_db("select userId from user where userName='caiyaodeng' and password='1234';", (char ***&)result, &row, &coulmn);
    objDB->sql_db("SELECT * FROM device WHERE deviceId IN (SELECT deviceId FROM roomdevice WHERE userId=1 UNION SELECT deviceId FROM modeldevice WHERE userId=1);", (char ***&)result, &row, &coulmn);

    cout << row << endl;
    cout << coulmn << endl;


    cout << "-----ok----" << endl;
    int flag=objDB->get_string(temp);
    cout<<temp<<":"<<flag<<endl;
    objDB->disconn_db();
    return -1;
}
