/*************************************************************************
	> File Name: tcp_socket.h
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Wed 09 Mar 2016 07:47:11 AM EST
 ************************************************************************/

#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include "../base/socket_util.h"
#include "../base/locker.h"
#include "../base/buffer.h"

namespace mynamespace {

#define READ_OP false
#define WRITE_OP true

    class TcpSocket {
        public:
            TcpSocket();
            virtual ~TcpSocket();

            bool get_wr_flag();
            void set_wr_flag(bool bFlag);
            int get_task_id();
            void set_task_id(int task_id);
            void set_fd(int conn_fd);
            int send_buf();
            int recv_buf();
            int get_msg(uint8_t *buf_out);
            bool put_msg(const uint8_t *buf_in, const int buf_length_in);
            bool reset();
            void close();

            uint8_t *&get_send_buf_addr() {
                return m_send_buf->get_buf_addr();
            }

            uint32_t *get_send_buf_len_addr() {
                m_reference_count ++;
                return m_send_buf->get_buf_length_addr();
            }

            virtual void process(bool bFlag);
        private:
            int m_socket;
            int m_task_id;
            int m_send_pos;
            int m_reference_count;
            bool m_flag;
            Buffer *m_send_buf;
            Buffer *m_recv_buf;
            Locker m_lock;
    };
}
#endif
