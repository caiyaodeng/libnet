/*************************************************************************
	> File Name: buffer.cpp
	> Author:  CaiYaodeng
	> Mail: 512324291@qq.com
	> Created Time: Fri 11 Mar 2016 06:28:48 AM EST
 ************************************************************************/
 #include "buffer.h"

namespace mynamespace {
    Buffer::Buffer()
    :
    m_buf (NULL),
    m_length (1){
        m_buf = (uint8_t *)malloc(BUFFER_SIZE);
        memcpy(m_buf, "1", 1);
    }
    Buffer::~Buffer() {
        if (m_buf != NULL) {
            free(m_buf);
            m_buf = NULL;
        }
    }
    int Buffer::get_size() {
        return m_length;
    }
    uint32_t Buffer::get_buf(uint8_t *buf_out) {
        if (m_length != 0) {
            memcpy(buf_out, m_buf, m_length);
            return m_length;
        }
        else {
            return 0;
        }
    }
    uint32_t Buffer::put_buf(const uint8_t *buf_in, uint32_t buf_length) {
        if (buf_in == NULL| buf_length == 0) {
            ERR_EXIT("Buffer", "");
            return -1;
        }
        else {
            memcpy(m_buf, buf_in, buf_length);
            m_length = buf_length;
            return 0;
        }
    }
    void Buffer::clean() {
        memset(m_buf, 0, BUFFER_SIZE);
        m_length = 0;
    }
    bool Buffer::is_full() {
        return m_length == BUFFER_SIZE;
    }
    bool Buffer::is_empty() {
        return m_length == 0;
    }
}
