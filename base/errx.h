/*************************************************************************
	> File Name: errx.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: 2016年03月05日 星期六 12时58分51秒
 ************************************************************************/

#ifndef _ERRX_H_
#define _ERRX_H_

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(func, m) \
        do \
        { \
         std::cerr << "err:"<< func <<"("<<__LINE__<<")errno:"<< errno<< "msg:"<< m <<std::endl; \
        } \
        while (0)

#endif
