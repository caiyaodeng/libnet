# libnetevent
[TOC]
##简介
#####采用同步I/O（Epoll）的ET模式封装实现
##接口
```
int pull_msg(unsigned char *undo_msg, int *undo_msg_length, unsigned char *&done_msg, int *&done_msg_length);
int push_msg(int task_id);
```
##更新日志
#####2016/08/26/rename libnetevent -> libnet
#####2016/08/26/提交版本为多线程模式
