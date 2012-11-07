
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (blockwalker)

SET (SRC_LIST  main.cpp  readmtf.cpp)
SET (HEAD_LIST mtf.h mtftype.h readmtf.h) 

ADD_EXECUTABLE(blockwalker ${SRC_LIST} ${HEAD_LIST})

