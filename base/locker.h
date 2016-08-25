/*************************************************************************
> File Name: locker.h
> Author:CaiYaodeng 
> Mail: 512324291@qq.com 
> Created Time: Fri 26 Jun 2015 09:25:52 PM
************************************************************************/

#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <exception>
#include <pthread.h>
#include <semaphore.h>

class Sem
{
public:
    Sem()
    {
        if( sem_init( &m_sem, 0, 0 ) != 0 )
        {
            throw std::exception();
        }
    }
    ~Sem()
    {
        sem_destroy( &m_sem );
    }
    bool wait() // -
    {
        return sem_wait( &m_sem ) == 0;
    }
    bool post() // +
    {
        return sem_post( &m_sem ) == 0;
    }

private:
    sem_t m_sem;
};

class Locker
{
public:
    Locker()
    {
        if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
        {
            throw std::exception();
        }
    }
    ~Locker()
    {
        pthread_mutex_destroy( &m_mutex );
    }
    bool lock()
    {
        return pthread_mutex_lock( &m_mutex ) == 0;
    }
    bool unlock()
    {
        return pthread_mutex_unlock( &m_mutex ) == 0;
    }

private:
    pthread_mutex_t m_mutex;
};

class Cond
{
public:
    Cond()
    {
        if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
        {
            throw std::exception();
        }
        if ( pthread_cond_init( &m_cond, NULL ) != 0 )
        {
            pthread_mutex_destroy( &m_mutex );
            throw std::exception();
        }
    }
    ~Cond()
    {
        pthread_mutex_destroy( &m_mutex );
        pthread_cond_destroy( &m_cond );
    }
    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock( &m_mutex );
        ret = pthread_cond_wait( &m_cond, &m_mutex );
        pthread_mutex_unlock( &m_mutex );
        return ret == 0;
    }
    bool signal()
    {
        return pthread_cond_signal( &m_cond ) == 0;
    }

private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif
