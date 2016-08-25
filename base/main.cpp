/*************************************************************************
	> File Name: main.cpp
	> Author:
	> Mail:
	> Created Time: Sat 07 May 2016 10:41:47 AM EDT
 ************************************************************************/

#include <iostream>
#include "thread_pool.h"
using namespace std;


#define READ_OP false
#define WRITE_OP true

class test{
    public:
        test (int numInt, bool bOp)
        :
        m_fd(0),
        m_data(numInt),
        m_bFlag(READ_OP){

        }

        ~test () {

        }
    public:
        int m_fd;
        int m_data;
        bool m_bFlag;
    public:
        void process(int numLeft, bool bOpt) {
            if (READ_OP == bOpt) {
                cout << "read_op" << endl;
            }

            else if (WRITE_OP == bOpt) {
                cout << "write_op" << endl;
            }

            else {
                cout << "something else" << endl;
            }
        }
};

int main () {

    mynamespace::ThreadPool <test> * myTrdPool = new mynamespace::ThreadPool <test> ();
    test myTest(7, WRITE_OP);
    myTrdPool->append(&myTest);
    while(1);
    return true;
}
