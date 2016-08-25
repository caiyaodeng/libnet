/*************************************************************************
	> File Name: tcp_task_pool.h
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Sat 12 Mar 2016 08:56:48 PM EST
 ************************************************************************/

#ifndef _TCP_TASK_POOL_H_
#define _TCP_TASK_POOL_H_

#include "tcp_socket.h"
#include "../base/thread_pool.h"
#include <sys/epoll.h>
#include <stack>
#include <list>

#define MAX_TASK 100
#define MAX_EVENTS 100

namespace mynamespace {
    typedef struct _message {
        int task_id = -1;
        unsigned char recv_msg[128];
        uint32_t recv_msg_length;
        unsigned char *send_msg;
        uint32_t *send_msg_length;
    }Message;

    class TcpTaskPool {
        public:
            TcpTaskPool();
            ~TcpTaskPool();

            bool init();
            bool add_connect();
            bool close_connect(int conn_fd);
            int recv_msg(const int fd);
            bool send_msg(const int fd);
            void send_now(const int fd);
            const int get_epoll();
            const int get_listen();
            void push_free_task(int task_id);
            Message *get_undo_task();
        protected:
            int get_free_task();
            bool is_free_tasks_empty();

            void add_recv_task(int task_id);
            bool is_recv_tasks_empty();
            TcpSocket *get_recv_task();

            void add_undo_task(int task_id, TcpSocket *undo_task);
            bool is_undo_tasks_empty();
        private:
            TcpSocket *m_tcp_tasks;
            int *m_ready_list;
            std::stack <int> m_free_tasks;
            std::list <Message> m_undo_tasks;
            std::list <int> m_recv_tasks;
            std::list <int> m_send_tasks;
            int m_listen_fd;
            int m_epoll_fd;
            Locker m_undo_lock;
            Locker m_recv_lock;
            Locker m_send_lock;
            Locker m_free_lock;
            static ThreadPool<TcpSocket> *m_thread_pool; //thread pool
    };
}

#endif
