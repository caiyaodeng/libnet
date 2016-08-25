/*************************************************************************
	> File Name: main.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Tue 05 Apr 2016 08:26:48 PM EDT
 ************************************************************************/

#include <iostream>
#include "net_work.h"
using namespace std;

int main () {

    mynamespace::NetWork *objNet = new mynamespace::NetWork ();
    int iTaskId = -1;
    unsigned char *undo_msg = (unsigned char *)malloc(128);
    memset (undo_msg, 0, 128);
    int undo_msg_length = -1;
    unsigned char *done_msg = nullptr;
    int *done_msg_length = nullptr;
    int ret = 0;

    objNet->init();

    //sleep(10);
    while(true) {
        iTaskId = objNet->pull_msg(undo_msg, &undo_msg_length, done_msg, done_msg_length);
        if (iTaskId == -1) {
            continue;
            return -1;
        }
        cout << iTaskId << endl;
        cout << undo_msg << endl;
        memcpy(done_msg, undo_msg, undo_msg_length);
        *done_msg_length = undo_msg_length;

        memset (undo_msg, 0, 128);
        ret = objNet->push_msg(iTaskId);
        //ret = objNet->push_msg(98);
    }

    return 0;
}

