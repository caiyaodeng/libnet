/*************************************************************************
  > File Name: type.h
  > Author: 
  > Mail: 
  > Created Time: Wed 04 May 2016 09:58:04 AM EDT
 ************************************************************************/

#ifndef _TYPE_H_
#define _TYPE_H_

struct BitFied{  
	unsigned char a1:1;  
	unsigned char b1:1;  
	unsigned char c1:1;  
	unsigned char d1:1;  
	unsigned char e1:1;  
	unsigned char f1:1;  
	unsigned char g1:1;  
	unsigned char h1:1;  

	unsigned char a2:1;  
	unsigned char b2:1;  
	unsigned char c2:1;  
	unsigned char d2:1;  
	unsigned char e2:1;  
	unsigned char f2:1;  
	unsigned char g2:1;  
	unsigned char h2:1;  

	unsigned char a3:1;  
	unsigned char b3:1;  
	unsigned char c3:1;  
	unsigned char d3:1;  
	unsigned char e3:1;  
	unsigned char f3:1;  
	unsigned char g3:1;  
	unsigned char h3:1;  

	unsigned char a4:1;  
	unsigned char b4:1;  
	unsigned char c4:1;  
	unsigned char d4:1;  
	unsigned char e4:1;  
	unsigned char f4:1;  
	unsigned char g4:1;  
	unsigned char h4:1;  
};  

union type_int{  
	struct BitFied b_t;  
	int i_t;  
	unsigned char str_t[4];  
};  
#endif
