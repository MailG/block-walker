#ifndef READMTF_HEAD_
#define READMTF_HEAD_
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <assert.h>
#include "mtf.h"

#define MTFTYPESIZE 4
#define PrintMacro print_mtf_id
#define PrintStr(x) std::cout<<x

#define DebugPrint(x) std::cout<<(x)<<"\n"

int readmtf(std::string mtffile);

inline void print_mtf_id(const int& type)
{
	char *ptr = (char*) &type;
	printf("Type -- 0X%08x -- %c%c%c%c\n",type, ptr[0],ptr[1],ptr[2],ptr[3]);   
}

class StateManager;
//typedef int16_t StateInfo;
class StateInfo{
public:
	int16_t sfmbsize;
};

///abstract class;
class States{
public:
	virtual ~States();
	virtual bool nextState(States* &newstates,std::ifstream &fin) = 0;
	virtual int myState() = 0;
};


class StateFactory{
public:
	static States* create(std::ifstream &fin);
	static States* create(const int& type);
	static StateInfo s_stateinfo;
};

class BlockState : public States{
public:
	BlockState(const int& type) : m_type(type){memset(&m_common_head,0,sizeof(MTF_DB_HDR));}
	~BlockState(){}
	bool nextState(States* &newstates,std::ifstream &fin);
	int myState();
protected:
	MTF_DB_HDR m_common_head;
	int m_type;
};

class StreamState : public States{
public:
	StreamState(const int& type) : m_type(type){}
	~StreamState(){}
public:
	bool nextState( States* &newstates,std::ifstream &fin );
	int myState();
protected:
	int m_type;
	MTF_STREAM_HDR streamhead;
};

class SfmbState : public States{
public:
	SfmbState(const int& type, int16_t sfmbsize) : m_type(type), m_sfmbbytesize(sfmbsize * 512){}
	~SfmbState(){}
	bool nextState( States* &newstates, std::ifstream &fin );
	int myState();
protected:
	int16_t m_sfmbbytesize;
	int m_type;
};

class StateManager{
public:
	StateManager();
	~StateManager();
public:
	bool nextState();
	int curStateType();
public:
	bool init(std::string filename);
	void uninit();
protected:
	std::ifstream m_file_buffer;
	States* m_state;
	//TODO ADD CONSISTENT CHECKER
};

typedef struct {
	int type;
	MTF_DB_HDR common_head;
	MTF_TAPE_BLK tapeblock;
	MTF_STREAM_HDR streamhead;
	int sfmbsize;
} StateData;

#endif// READMTF_HEAD_
