/*
 */
 
#ifndef __MTFTYPE_H__
#define __MTFTYPE_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <vector>
#ifndef _WIN32
#include <unistd.h>
#endif

enum {FALSE, TRUE};
typedef unsigned int     u_int32_t;
typedef unsigned short   u_int16_t;
typedef unsigned char    u_int8_t;
#ifdef WIN32
  typedef unsigned __int64 u_int64_t;
#endif

typedef int     int32_t;
typedef short   int16_t;
typedef signed char    int8_t;
#ifdef WIN32
  typedef __int64 int64_t; 
#endif






#endif // __MTFTYPE_H__


