#include "ipc_shm.h"

static int g_shm_flag = 0;
static int g_shm_key = 0;

/***********************************************************************
  ����������    ���������ڴ�
  ����˵����    shmname  [in]  �ǹ����ڴ���,ϵͳ��Ψһ��־
                shmsize  [in]  ��Ҫ�����Ĺ����ڴ�Ĵ�С��
                shmhdl   [out] �����ڴ�ľ��.
  ����ֵ��      ����0����ִ�гɹ�����0���ش�����
************************************************************************/
int create_shm(char *shm_seed_file_ptr, int shm_size, int *shm_hdl_ptr)
{
    if(g_shm_flag == 0)			//�жϽӿ��й����ڴ�key�Ƿ��Ѿ�����
    {
        g_shm_key = ftok(shm_seed_file_ptr, 'c');
        if (g_shm_key == -1)
        {
            ERR_EXIT("create_shm", "");
            return -1;
        }

        g_shm_flag = 1;
    }

    //���������ڴ�
    *shm_hdl_ptr = shmget(g_shm_key,shm_size,IPC_CREAT|0666);
    if (*shm_hdl_ptr == -1)			//����ʧ��
        ERR_EXIT("create_shm", "");
    return 0;

}
/***********************************************************************
  ����������    ���������ڴ�
  ����˵����    shmhdl	[in]  ����ľ��
                mapaddr [out] �����ڴ��׵�ַ
  ����ֵ��      ����0����ִ�гɹ�����0���ش�����
************************************************************************/
int
map_shm(int shm_hdl, void **map_addr_ptr)
{
    void *temp_ptr = NULL;

    //���ӹ����ڴ�
    temp_ptr = (void *)shmat(shm_hdl,0,SHM_RND);
    if (temp_ptr == NULL)		//�����ڴ�����ʧ��
    {
        ERR_EXIT("map_shm", "");
        return errno;
    }
    *map_addr_ptr = temp_ptr;			//���������ڴ���ָ��

    return 0;
}
/***********************************************************************
  ����������    ȡ�������ڴ����
  ����˵����    unmapaddr   [in] �����ڴ��׵�ַ
  ����ֵ��      ����0����ִ�гɹ�����0���ش�����
************************************************************************/
int unmap_shm(void *unmap_addr_ptr)
{
    int rv;
    //ȡ�����ӹ����ڴ�
    rv = shmdt((char *)unmap_addr_ptr);
    if (rv == -1)			//ȡ������ʧ��
    {
        ERR_EXIT("unmap_shm", "");
        return errno;
    }

    return 0;
}
/***********************************************************************
  ����������    ɾ�������ڴ�
  ����˵����    shmhdl	[in]  ����ľ��
  ����ֵ��      ����0����ִ�гɹ�����0���ش�����
************************************************************************/
int del_shm(int shm_hdl)
{
    int rv;
    //ɾ�������ڴ�
    rv = shmctl(shm_hdl,IPC_RMID,NULL);
    if(rv < 0)				//ɾ�������ڴ�ʧ��
    {
        ERR_EXIT("del_shm", "");
        return errno;
    }
    return 0;
}
