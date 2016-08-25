/*************************************************************************
> File Name: thread_pool.h
> Author:CaiYaodeng
> Mail: 512324291@qq.com
> Created Time: Fri 26 Jun 2015 09:25:52 PM
************************************************************************/

#ifndef _TREAD_POOL_H_
#define _TREAD_POOL_H_

#include <list>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include "locker.h"

namespace mynamespace {
    template< typename T >
    class ThreadPool
    {
        public:
        ThreadPool( int thread_number = 1, int max_requests = 10000 );
        ~ThreadPool();
        bool append( T* request );

        private:
        static void* worker( void* arg );
        void run();

        private:
        int m_thread_number;
        int m_max_requests;
        pthread_t* m_threads;
        std::list< T* > m_workqueue;
        Locker m_queuelocker;
        Sem m_queuestat;
        bool m_stop;
    };

    template< typename T >
    ThreadPool< T >::ThreadPool( int thread_number, int max_requests )
    :
    m_thread_number( thread_number ),
    m_max_requests( max_requests ),
    m_threads( NULL ) {
        m_stop = false;

        if( ( thread_number <= 0 ) || ( max_requests <= 0 ) )
        {
            throw std::exception();
        }

        m_threads = new pthread_t[ m_thread_number ];
        if( ! m_threads )
        {
            throw std::exception();
        }

        for ( int i = 0; i < thread_number; ++i )
        {
            printf( "create the %dth thread\n", i );
            if( pthread_create( m_threads + i, NULL, worker, this ) != 0 )
            {
                delete [] m_threads;
                throw std::exception();
            }
            if( pthread_detach( m_threads[i] ) )
            {
                delete [] m_threads;
                throw std::exception();
            }
        }
    }

    template< typename T >
    ThreadPool< T >::~ThreadPool()
    {
        delete [] m_threads;
        m_stop = true;
    }

    template< typename T >
    bool ThreadPool< T >::append( T* request )
    {
        m_queuelocker.lock();
        if ( m_workqueue.size() > m_max_requests )
        {
            m_queuelocker.unlock();
            return false;
        }
        std::cout << "undo_task_thread" << request << std::endl;
        m_workqueue.push_back( request );
        m_queuestat.post();
        m_queuelocker.unlock();
        return true;
    }

    template< typename T >
    void* ThreadPool< T >::worker( void* arg )
    {
        ThreadPool* pool = ( ThreadPool* )arg;
        pool->run();
        return NULL;
    }

    template< typename T >
    void ThreadPool< T >::run()
    {
        while ( ! m_stop )
        {
            m_queuestat.wait(); // ? >0
            m_queuelocker.lock();
            if ( m_workqueue.empty() )
            {
                m_queuelocker.unlock();
                continue;
            }
            T* request = m_workqueue.front();
            m_workqueue.pop_front();
            m_queuelocker.unlock();
            if ( ! request )
            {
                continue;
            }
            std::cout << "pthread id :" << pthread_self() << std::endl;
            request->process(request->get_wr_flag());
        }
    }

}
#endif
