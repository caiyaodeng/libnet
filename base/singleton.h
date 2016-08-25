/*************************************************************************
> File Name: singleton.h
> Author:CaiYaodeng 
> Mail:512324291@qq.com 
> Created Time: 2016年03月04日 星期五 23时09分39秒
************************************************************************/

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <pthread.h>

namespace mynamespace {
    template <typename T>
    class Singleton 
    {
        public:
        static T &get_instance() {
            pthread_once(&m_once_control,init);
            return *m_instance;
        }
        protected:
        Singleton () {}
        Singleton (const singleton &) {}
        private:
        static void init() {
            m_instance = new T();
        }
        private:
        static pthread_once_t m_once_control;
        static T *m_instance;
    };

    template <typename T>
    pthread_once_t Singleton<T>::m_once_control = PTHREAD_ONCE_INIT;

    template <typename T>
    T *Singleton<T>::m_instance = nullptr;
}

#endif
