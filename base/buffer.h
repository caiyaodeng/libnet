/*************************************************************************
	> File Name: buffer.h
	> Author: CaiYaodeng
	> Mail: 512324291@qq.com 
	> Created Time: Fri 11 Mar 2016 06:02:50 AM EST
 ************************************************************************/

#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFFER_SIZE 1024*10
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "errx.h"

namespace mynamespace {
    class Buffer {
        public:
            Buffer();
            ~Buffer();
            int get_size();
            uint32_t get_buf(uint8_t *buf_out);
            uint32_t put_buf(const uint8_t *buf_in, uint32_t buf_length);
            void clean();
            bool is_full();
            bool is_empty();

            uint8_t *&get_buf_addr() {
                return m_buf;
            }
            
            uint32_t *get_buf_length_addr() {
                return &m_length;
            }

        private:
            uint8_t *m_buf;
            uint32_t m_length;
    };
}
#endif
