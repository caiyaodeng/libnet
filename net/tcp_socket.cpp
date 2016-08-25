/*************************************************************************
	> File Name: tcp_socket.cpp
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Fri 11 Mar 2016 09:00:57 AM EST
 ************************************************************************/

#include "tcp_socket.h"

namespace mynamespace {
    TcpSocket::TcpSocket()
    :
    m_socket (0),
    m_reference_count (1),
    m_task_id(-1),
    m_send_pos(6),
    m_flag(READ_OP),
    m_send_buf (NULL),
    m_recv_buf (NULL) {
        m_send_buf = new Buffer ();
        m_recv_buf = new Buffer ();
    }
    TcpSocket::~TcpSocket() {
        if (m_send_buf != NULL) {
            delete m_send_buf;
            m_send_buf = NULL;
        }
        if (m_recv_buf != NULL) {
            delete m_recv_buf;
            m_recv_buf = NULL;
        }
    }

    int TcpSocket::get_task_id() {
        return m_task_id;
    }
    void TcpSocket::set_task_id(int task_id) {
        m_task_id = task_id;
    }


    bool TcpSocket::get_wr_flag() {
        return m_flag;
    }

    void TcpSocket::set_wr_flag(bool bFlag) {
        m_flag = bFlag;
    }

    int TcpSocket::send_buf() {
        uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);
        memset(buf, 0, BUFFER_SIZE);
        m_lock.lock();

        std::cout << "send_length:" << m_send_buf->get_size()<< std::endl;
        if (m_reference_count > 0) {
            m_send_pos += m_send_buf->get_size();
            m_reference_count --;
        }
        int left = m_send_pos;
        int ret = m_send_buf->get_buf(buf);
        int send_pos = 0;
        m_lock.unlock();
        if (ret == 0) {
            //std::cout << "exit:ret== 0" << std::endl;
            return 0;
        }

        if (left == 0) {
            //std::cout << "exit:left == 0" << std::endl;
            return 0;
        }

        std::cout << "send:" << buf << std::endl;
        std::cout << "send_length:" << strlen((const char *)buf) << std::endl;

        while ((ret = write(m_socket, buf+send_pos, left)) > 0) {
            send_pos += ret;
            left -= ret;
        }

        if (left == 0) {
            std::cout << "send_ok:" << buf << std::endl;
            //std::cout << "exit :left == 0" << std::endl;
            return 0;
        }

        if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            //std::cout << "EAGANIN" << std::endl;
            m_send_pos -= ret;
           return -1;
        }

        return -1;
    }
    int TcpSocket::recv_buf() {
        std::cout << "recv" << std::endl;
        int ret = 0;
        int recv_buf_length = 0;
        uint8_t recv_buf[BUFFER_SIZE];
        memset(recv_buf, 0, BUFFER_SIZE);
        m_send_pos = 0;

        while ((ret = read (m_socket, recv_buf, BUFFER_SIZE-1)) != 0) {
            recv_buf_length += ret;
            if (ret < 0) {
                if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                    std::cout << "recv_buf:" << recv_buf << std::endl;
                    printf("read later.......\n");
                    break;
                }
            }
            else {
                 break;
            }
        }
        if (ret == 0) {
            std::cout << "peer closed !" << std::endl;
            return -1;
        }

        m_lock.lock();
        m_recv_buf->put_buf(recv_buf, recv_buf_length);
        std::cout << "recv num:" << m_recv_buf->get_buf((uint8_t *)recv_buf);
        m_lock.unlock();
        std::cout << "recv:" << recv_buf << std::endl;
        return 0;
    }
    bool TcpSocket::reset() {
        m_socket = 0;
        m_lock.lock();
        m_send_buf->clean();
        m_recv_buf->clean();
        m_lock.unlock();
        return true;
    }

    int TcpSocket::get_msg(uint8_t *buf_out) {
        return (m_recv_buf->get_buf(buf_out));
    }

    bool TcpSocket::put_msg(const uint8_t *buf_in, const int buf_length_in) {
        return (0 == m_send_buf->put_buf(buf_in, buf_length_in));
    }
    void TcpSocket::set_fd(int conn_fd) {
        m_socket = conn_fd;
    }

    void TcpSocket::close() {
        ::close(m_socket);
    }

    void TcpSocket::process(bool bFlag) {

        if (READ_OP == bFlag) {
            std::cout << "read_op" << std::endl;
            this->recv_buf();
        }
        else if (WRITE_OP == bFlag) {
            std::cout << "write_op" << std::endl;
            this->send_buf();
        }
        else {
            std::cout << "something else" << std::endl;
        }
    }
}

