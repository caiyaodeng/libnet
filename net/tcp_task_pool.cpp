/*************************************************************************
  > File Name: tcp_task_pool.cpp
  > Author: CaiYaodeng
  > Mail: 512324291@qq.com
  > Created Time: Sat 12 Mar 2016 09:29:51 PM EST
 ************************************************************************/

#include "tcp_task_pool.h"

using namespace std;

namespace mynamespace {

    ThreadPool <TcpSocket> *TcpTaskPool::m_thread_pool = new ThreadPool <TcpSocket> (9);

    TcpTaskPool::TcpTaskPool()
        :
    m_tcp_tasks (NULL),
    m_ready_list (NULL),
    m_listen_fd (-1),
    m_epoll_fd (-1){
        init();
    }

    const int TcpTaskPool::get_epoll() {
        return m_epoll_fd;
    }

    const int TcpTaskPool::get_listen() {
        return m_listen_fd;
    }

    bool TcpTaskPool::init() {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        //ev.events = EPOLLIN;

        if ((m_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            ERR_EXIT("TcpTaskPool", "socket");
            return false;
        }
        cout << "listen socket:" << m_listen_fd << endl;

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(8001);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        char *on;
        if (setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        {
            ERR_EXIT("TcpTaskPool","setsockopt");
            return false;
        }

        if (bind(m_listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
            ERR_EXIT("TcpTaskPool", "bind");
            return false;
        }
        if (listen(m_listen_fd, SOMAXCONN) < 0)
        {
            ERR_EXIT("TcpTaskPool", "listen");
            return false;
        }

        ev.data.fd = m_listen_fd;

        m_epoll_fd = epoll_create(MAX_EVENTS);
        cout << "epoll socket:" << m_epoll_fd << endl;
        if (m_epoll_fd == -1) {
            perror("epoll_create");
            exit(EXIT_FAILURE);
        }
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &ev) == -1) {
            perror("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }

        m_tcp_tasks = new TcpSocket [MAX_TASK];
        for (int i=0; i<MAX_TASK; i++) {
            m_free_tasks.push(i);
        }

        m_ready_list = new int [MAX_TASK];
        for (int i=0; i<MAX_TASK; i++) {
            m_ready_list[i] = -1;
        }
        return true;
    }

    void TcpTaskPool::push_free_task(int task_id) {
        m_free_lock.lock();
        m_free_tasks.push(task_id);
        m_free_lock.unlock();
    }

    int TcpTaskPool::get_free_task() {
        int task_id = -1;

        m_free_lock.lock();
        if (m_free_tasks.empty()) {
            m_free_lock.unlock();
            return -1;
        }
        else {
            m_free_lock.unlock();
            task_id = m_free_tasks.top();
            m_free_tasks.pop();
            return task_id;
        }
    }

    bool TcpTaskPool::is_free_tasks_empty() {
        m_free_lock.lock();
        if (!m_free_tasks.empty()) {
            m_free_lock.unlock();
            return false;
        }
        else {
            m_free_lock.unlock();
            return true;
        }
    }

    void TcpTaskPool::add_recv_task(int task_id) {
        std::cout << "add recv" << std::endl;
        m_recv_lock.lock();
        m_recv_tasks.push_back(task_id);

        TcpSocket *undo_task = get_recv_task();

        add_undo_task(task_id, undo_task);
        m_recv_lock.unlock();
    }

    TcpSocket *TcpTaskPool::get_recv_task() {
        int task_id = -1;
        if (m_recv_tasks.empty()) {
            return NULL;
        }
        else {
            task_id = m_recv_tasks.front();
            TcpSocket *undo_task = &m_tcp_tasks[task_id];

            m_recv_tasks.pop_front();
            return undo_task;
        }
    }

    bool TcpTaskPool::is_recv_tasks_empty() {
        m_recv_lock.lock();
        if (!m_recv_tasks.empty()) {
            m_recv_lock.unlock();
            return false;
        }
        else {
            m_recv_lock.unlock();
            return true;
        }
    }
    bool TcpTaskPool::add_connect() {
        int free_subscript = -1;
        int conn_fd = -1;
        unsigned int peerlen;
        struct sockaddr_in peeraddr;
        memset(&peeraddr, 0, sizeof(peeraddr));
        struct epoll_event ev;

        if ((conn_fd = accept(m_listen_fd, (struct sockaddr*)&peeraddr, &peerlen)) < 0) {
            ERR_EXIT("TcpTaskPool","add_connect");
            return false;
        }

        std::cout << "=======>>>>>>>" << conn_fd << std::endl;
        set_nonblocking(conn_fd);
        //ev.events = EPOLLIN | EPOLLOUT;
        //ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.events = EPOLLIN | EPOLLET;
        //ev.events = EPOLLOUT | EPOLLET;
        //ev.events = EPOLLIN;
        ev.data.fd = conn_fd;
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
            perror("epoll_ctl: conn_sock");
            exit(EXIT_FAILURE);
        }

        if (is_free_tasks_empty()) {
            return false;
        }

        free_subscript = get_free_task();

        m_ready_list[conn_fd] = free_subscript;
        if (m_ready_list[conn_fd] == -1) {
            return false;
        }

        m_tcp_tasks[free_subscript].set_fd(conn_fd);
        return true;
    }
    bool TcpTaskPool::close_connect(int conn_fd) {
        int del_subscript = -1;

        del_subscript = m_ready_list[conn_fd];
        if (! m_tcp_tasks[del_subscript].reset()) {
            if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, conn_fd, NULL) == -1) {
                perror("epoll_ctl: del_sock");
                exit(EXIT_FAILURE);
            }
            close(conn_fd);

            return false;
        }
        if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, conn_fd, NULL) == -1) {
            perror("epoll_ctl: del_sock");
            exit(EXIT_FAILURE);
        }
        close(conn_fd);

        /*close event*/
        TcpSocket *undo_task = new TcpSocket();
        add_undo_task(del_subscript, undo_task);

        push_free_task(del_subscript);
        std::cout << "reset id free" << del_subscript << endl;

        delete undo_task;
        undo_task = nullptr;
        return true;
    }
    int TcpTaskPool::recv_msg(const int fd) {
        std::cout << "TcpTaskPool::recv_msg" << std::endl;
        struct epoll_event ev;
        int ready_subscript = -1;
        ready_subscript = m_ready_list[fd];
        //ev.events = EPOLLOUT | EPOLLET;
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.data.fd = fd;

        m_tcp_tasks[ready_subscript].set_wr_flag(READ_OP);
        //m_tcp_tasks[ready_subscript].set_task_id(ready_subscript);
        add_recv_task(ready_subscript);
        m_thread_pool->append(&(m_tcp_tasks[ready_subscript]));

        return 0;
    }
    bool TcpTaskPool::send_msg(const int fd) {
        struct epoll_event ev;
        int ready_subscript = -1;
        ready_subscript = m_ready_list[fd];
        ev.events = EPOLLIN | EPOLLET;
        //ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.data.fd = fd;

        sleep(2);
        if (0 != m_tcp_tasks[ready_subscript].send_buf()) {
            perror("tcpi_single:send");
            return false;
        }

        /*if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            perror("epoll_ctl: reset send");
            exit(EXIT_FAILURE);
        }*/

        return true;
    }

    void TcpTaskPool::send_now(const int task_id) {
        std::cout <<  "send_now" << std::endl;
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLOUT |EPOLLET;
        ev.data.fd = 6;

        std::cout << "task_id" << task_id << std::endl;
        m_tcp_tasks[task_id].send_buf();

        /*if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, 6, &ev) == -1) {
            perror("epoll_ctl: reset send");
            exit(EXIT_FAILURE);
        } */
    }

    void TcpTaskPool::add_undo_task(int task_id, TcpSocket *undo_task) {
        Message *undo_task_msg = new Message ();
        undo_task_msg->task_id = task_id;
        undo_task_msg->recv_msg_length = undo_task->get_msg(undo_task_msg->recv_msg);
        undo_task_msg->send_msg = undo_task->get_send_buf_addr();
        undo_task_msg->send_msg_length = undo_task->get_send_buf_len_addr();

        m_undo_tasks.push_back(*undo_task_msg);
    }

    Message *TcpTaskPool::get_undo_task() {

        m_undo_lock.lock();
        if (is_undo_tasks_empty()) {
            m_undo_lock.unlock();
            return nullptr;
        }
        Message *undo_task = new Message();
        memcpy(undo_task, &m_undo_tasks.front(), sizeof(Message));

        m_undo_tasks.pop_front();
        m_undo_lock.unlock();

        std::cout << "get_undo_task = done" << std::endl;
        return undo_task;
    }

    bool TcpTaskPool::is_undo_tasks_empty() {
        if (!m_undo_tasks.empty()) {
            return false;
        }
        else {
            return true;
        }
    }

}
