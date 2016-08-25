/*************************************************************************
* > File Name: ipc_sem.h
* > Author:CaiYaodeng 
* > Mail: 512324291@qq.com 
* > Created Time: Fri 26 Jun 2015 09:25:52 PM
* ************************************************************************/

#ifndef _IPC_SEM_H_
#define _IPC_SEM_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/sem.h>
#include "errx.h"

#ifdef __cplusplus 
extern "C" {
#endif

#define SEMERR_BASE 100
#define SEMERR_PARAM    (SEMERR_BASE + 1)
#define SEMERR_EEXIST    (SEMERR_BASE + 2)

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int create_sem(int key, int *semid_ptr);
int open_sem(int key, int *semid_ptr);
int set_sem_val(int semid, int val);
int get_sem_val(int semid, int *val_ptr);
int sem_p(int semid);
int sem_v(int semid);


#ifdef __cplusplus
}
#endif

#endif





