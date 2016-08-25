#include "ipc_sem.h"


//@����ֵ 0 ��ȷ ��������
int create_sem(int key, int *semid_ptr)
{
    int ret = 0;

    if (semid_ptr == NULL)
    {
        ret = SEMERR_PARAM;
        ERR_EXIT ("create_sem", "");
        return ret;
    }
    ret = semget(key, 1, 0666| IPC_CREAT | IPC_EXCL);
    if (ret == -1)
    {
        ret = errno;
        if (errno == EEXIST)
        {
            ret = SEMERR_EEXIST;
            ERR_EXIT ("create_sem", "already exist");
            return ret;
        }
    }
    *semid_ptr = ret;

    ret = set_sem_val(*semid_ptr, 1);
    if (ret != 0)
    {
        ERR_EXIT ("create_sem", "");
        return ret;
    }
    ret = 0;
    return ret;
}

int open_sem(int key, int *semid_ptr)
{
    int ret = 0;

    if (semid_ptr == NULL)
    {
        ret = SEMERR_PARAM;
        printf("func sem_open() err:%d\n", ret);
        return ret;
    }

    ret = semget(key, 0, 0);
    if (ret == -1)
    {
        ret = errno;
        ERR_EXIT ("open_sem", "");
        return ret;
    }
    *semid_ptr = ret;
    ret = 0;
    return ret;
}

int set_sem_val(int semid, int val)
{
    int ret = 0;
    union semun su;
    su.val = val;
    ret = semctl(semid, 0,  SETVAL, su);
    return ret;
}

int get_sem_val(int semid, int *val_ptr)
{
    int ret = 0;
    int val;
    union semun su;
    val = su.val ;
    //�ź��� ����ֵ
    ret = semctl(semid, 0, GETVAL, su);

    *val_ptr = ret;
    ret = 0;
    return ret;
}

//�ź���p����ʱ����Ҫ���ݺü�����Ϣ��linux�ں�
//����linux�ں˶�����һ���ṹ
int sem_p(int semid)
{
    struct sembuf buf = {0, -1, 0};
    int ret = 0;
    ret = semop(semid, &buf, 1);
    return ret;
}

int sem_v(int semid)
{
    struct sembuf buf = {0, 1, 0};
    int ret = 0;
    ret = semop(semid, &buf, 1);
    return ret;
}

