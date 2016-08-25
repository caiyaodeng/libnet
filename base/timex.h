/*************************************************************************
	> File Name: time.h
	> Author: CaiYaodeng
	> Mail:512324291@qq.com 
	> Created Time: Wed 04 May 2016 09:48:10 AM EDT
 ************************************************************************/

#ifndef _TIME_H_
#define _TIME_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "type.h"

namespace tx {
    /*get UTC*/
    bool get_UTC_E8(unsigned char *strUTC);

    /*get time right now*/
    bool get_Time_E8(unsigned char *strTime);
}
#endif
