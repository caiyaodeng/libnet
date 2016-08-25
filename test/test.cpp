/*************************************************************************
	> File Name: main.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Tue 05 Apr 2016 08:26:48 PM EDT
 ************************************************************************/

#include <iostream>
#include "../net/net_work.h"
//#include "leveldb/db.h"
using namespace std;

int main () {

    mynamespace::NetWork *objNet = new mynamespace::NetWork ();
    int iTaskId = -1;
    unsigned char *pUndoMsg = (unsigned char *)malloc(128);
    memset (pUndoMsg, 0, 128);
    int iUndoMsgLength = -1;
    unsigned char *pDoneMsg = nullptr;
    int *pDoneMsgLength = nullptr;
    objNet->init();

     /*leveldb::DB      *db;
        leveldb::Options  options;
        options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options,"/tmp/testdb", &db);
        assert(status.ok());
        */

    while(true) {
        iTaskId = objNet->pull_msg(pUndoMsg, &iUndoMsgLength, pDoneMsg, pDoneMsgLength);
        if (iTaskId == -1) {
            continue;
        }

        //pMessage = objParser->changeToMessage(pUndoMsg, iUndoMsgLength, pDoneMsg, pDoneMsgLength);
        //objBll->pushMessage(iTaskId, pMessage);

        memcpy(pDoneMsg, "hello", 6);
        std::cout << "pDoneMsg" << pDoneMsg << std::endl;
        *pDoneMsgLength = 6;
        objNet->push_msg(iTaskId);

        memset (pUndoMsg, 0, 128);
    }

    cout << "-----ok----" << endl;

    return 0;
}
