/*************************************************************************
	> File Name: net_work.h
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Tue 05 Apr 2016 08:31:26 PM EDT
 ************************************************************************/

#ifndef _NET_WORK_H_
#define _NET_WORK_H_

#include <signal.h>
#include <sys/wait.h>
#include "../base/socket_util.h"
#include "../base/errx.h"
#include "../base/buffer.h"
#include "tcp_task_pool.h"

namespace mynamespace {
    class NetWork {
        public:
            NetWork();
            ~NetWork();
        public:
            bool init();
            //return of TaskId , -1 for err
            int pull_msg(unsigned char *undo_msg, int *undo_msg_length, unsigned char *&done_msg, int *&done_msg_length);
            int push_msg(int task_id);
        private:
            static void *run_handler(void *);
            bool run();

       private:
            //thread pool
            //static ThreadPool<TcpSocket> *m_thread_pool;
            //task pool
            static TcpTaskPool *m_task_pool;
    };
    //ThreadPool<TcpSocket> *NetWork::m_thread_pool = new ThreadPool <TcpSocket> (1);
    //TcpTaskPool *NetWork::m_task_pool = new TcpTaskPool();
}
#endif
