/*************************************************************************
* > File Name: IpcShm.h
* > Author:CaiYaodeng 
* > Mail: 512324291@qq.com 
* > Created Time: Fri 26 Jun 2015 09:25:52 PM
* ************************************************************************/

#ifndef _IPC_SHM_H_
#define _IPC_SHM_H_

#include <memory.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "errx.h"

#ifdef __cplusplus 
extern "C" {
#endif

int create_shm(char *shm_seed_file_ptr, int shm_size, int *shm_hdl_ptr);
int map_shm(int shm_hdl, void **map_addr_ptr);
int unmap_shm(void *unmap_addr_ptr);
int del_shm(int shm_hdl);


#ifdef __cplusplus
}
#endif

#endif


