#include "net_work.h"

using namespace std;

namespace mynamespace {
    void handle(int signum) {
        int pid = 0;
        printf("recv signum:%d \n", signum);

        while ((pid = waitpid(-1, NULL, WNOHANG) ) > 0) {
            printf("exit! child pid%d \n", pid);
            fflush(stdout);
        }
    }

    NetWork::NetWork() {

    }

    NetWork::~NetWork() {

    }

    bool NetWork::init () {
        pthread_t trd;
        int ret = -1;

        ret = pthread_create(&trd, NULL, run_handler, this);
        if (ret < 0) {
            perror("create trd Start false");
            return false;
        }
        return true;
    }

    void *NetWork::run_handler(void *arg) {
        NetWork *net_stack = static_cast <NetWork *> (arg);
        if (!net_stack->run()) {
            perror("TcpTask:run_hander");
        }
        return nullptr;
    }

    TcpTaskPool *NetWork::m_task_pool = new TcpTaskPool();

    bool NetWork::run() {
        struct epoll_event events[MAX_EVENTS];
        int listen_fd, nfds, epoll_fd;
        signal(SIGCHLD, handle);

        epoll_fd = m_task_pool->get_epoll();
        if (epoll_fd == -1) {
            perror("epoll_fd");
            exit(EXIT_FAILURE);
        }
        listen_fd = m_task_pool->get_listen();
        if (listen_fd == -1) {
            perror("listen_fd");
            exit(EXIT_FAILURE);
        }
        for (;;) {
            nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            std::cout << "**********************************************************" << std::endl;
            if (nfds == -1) {
                if (errno != EINTR) {
                    perror("err:epoll_wait");
                    exit(EXIT_FAILURE);
                }
            }
            for (int n = 0; n < nfds; n++) {
                cout << "loop 2 epoll_wait num:" << nfds << endl;
                if (events[n].events & (EPOLLRDHUP| EPOLLERR| EPOLLHUP)) {
                    cout << "-------------------------------enter err" << endl;
                    printf("close %d\n", events[n].data.fd); //err
                    m_task_pool->close_connect(events[n].data.fd);
                    printf("close %d\n", events[n].data.fd);
                    cout << "-------------------------------exit err" << endl;
                }
                else if (events[n].events & EPOLLIN) {
                    if (events[n].data.fd == listen_fd) {
                        cout << "-------------------------------enter listen" << endl;
                        m_task_pool->add_connect();
                        cout << "-------------------------------exit listen" << endl;
                    }
                    else {
                        std::cout << "------------------------enter read" << std::endl;
                        printf("read %d\n", events[n].data.fd); //read
                        if (m_task_pool->recv_msg(events[n].data.fd) == -1) {
                            m_task_pool->close_connect(events[n].data.fd);
                            std::cout << "close for peer done" << std::endl;
                            std::cout << "--------------------------exit read" << std::endl;
                            continue;
                        }
                        std::cout << "--------------------------exit read" << std::endl;
                    }
                }
                else if (events[n].events & EPOLLOUT) {
                    std::cout << "------------------------enter write" << std::endl;
                    printf("send %d\n", events[n].data.fd); //read
                    /*  if ( !m_task_pool->send_msg(events[n].data.fd)) {
                        ERR_EXIT("err::main", "write");
                        exit(EXIT_FAILURE);
                    }
                    */
                    //std::cout << buf << std::endl;
                    std::cout << "------------------------exit write" << std::endl;
                }
                else {
                    std::cout << "other events!" << std::endl;
                }
            }
        }
        exit(EXIT_FAILURE);
    }

    //return of TaskId , -1 for NULL
    int NetWork::pull_msg(unsigned char *undo_msg, int *undo_msg_length, unsigned char *&done_msg, int *&done_msg_length) {
        Message *undo_task_msg = m_task_pool->get_undo_task();
        if (undo_task_msg == nullptr) {
            return -1;
        }

        std::cout << "pull" << std::endl;
        *undo_msg_length = undo_task_msg->recv_msg_length;
        memcpy(undo_msg, undo_task_msg->recv_msg, *undo_msg_length);
        done_msg = undo_task_msg->send_msg;
        done_msg_length = (int *)undo_task_msg->send_msg_length;

        return undo_task_msg->task_id;
    }
    int NetWork::push_msg(int task_id) {
        std::cout << "push" << std::endl;
        m_task_pool->send_now(task_id);
        return -1;
    }
}
