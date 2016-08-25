#include "ipc_shm.h"

static int g_shm_flag = 0;
static int g_shm_key = 0;

/***********************************************************************
  功能描述：    创建共享内存
  参数说明：    shmname  [in]  是共享内存名,系统中唯一标志
                shmsize  [in]  是要创建的共享内存的大小；
                shmhdl   [out] 共享内存的句柄.
  返回值：      返回0函数执行成功；非0返回错误码
************************************************************************/
int create_shm(char *shm_seed_file_ptr, int shm_size, int *shm_hdl_ptr)
{
    if(g_shm_flag == 0)			//判断接口中共享内存key是否已经存在
    {
        g_shm_key = ftok(shm_seed_file_ptr, 'c');
        if (g_shm_key == -1)
        {
            ERR_EXIT("create_shm", "");
            return -1;
        }

        g_shm_flag = 1;
    }

    //创建共享内存
    *shm_hdl_ptr = shmget(g_shm_key,shm_size,IPC_CREAT|0666);
    if (*shm_hdl_ptr == -1)			//创建失败
        ERR_EXIT("create_shm", "");
    return 0;

}
/***********************************************************************
  功能描述：    关联共享内存
  参数说明：    shmhdl	[in]  共享的句柄
                mapaddr [out] 共享内存首地址
  返回值：      返回0函数执行成功；非0返回错误码
************************************************************************/
int
map_shm(int shm_hdl, void **map_addr_ptr)
{
    void *temp_ptr = NULL;

    //连接共享内存
    temp_ptr = (void *)shmat(shm_hdl,0,SHM_RND);
    if (temp_ptr == NULL)		//共享内存连接失败
    {
        ERR_EXIT("map_shm", "");
        return errno;
    }
    *map_addr_ptr = temp_ptr;			//导出共享内存首指针

    return 0;
}
/***********************************************************************
  功能描述：    取消共享内存关联
  参数说明：    unmapaddr   [in] 共享内存首地址
  返回值：      返回0函数执行成功；非0返回错误码
************************************************************************/
int unmap_shm(void *unmap_addr_ptr)
{
    int rv;
    //取消连接共享内存
    rv = shmdt((char *)unmap_addr_ptr);
    if (rv == -1)			//取消连接失败
    {
        ERR_EXIT("unmap_shm", "");
        return errno;
    }

    return 0;
}
/***********************************************************************
  功能描述：    删除共享内存
  参数说明：    shmhdl	[in]  共享的句柄
  返回值：      返回0函数执行成功；非0返回错误码
************************************************************************/
int del_shm(int shm_hdl)
{
    int rv;
    //删除共享内存
    rv = shmctl(shm_hdl,IPC_RMID,NULL);
    if(rv < 0)				//删除共享内存失败
    {
        ERR_EXIT("del_shm", "");
        return errno;
    }
    return 0;
}
