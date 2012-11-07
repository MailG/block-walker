///\brief MTF file reader
//! it offer a method to read the MTF file.
/// \author MailG
/// \Email gomailgm@gmail.com
/// 2011-4-18

#include "readmtf.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <assert.h>

StateInfo StateFactory::s_stateinfo;
States::~States(){}

bool BlockState::nextState( States* &newstates,std::ifstream &fin )
{
	const int mtf_db_hdr_size = sizeof(MTF_DB_HDR);
	fin.read( (char*)&m_common_head, mtf_db_hdr_size );

	/// alignment position. verify by the example of ntbackup.exe
	short alignoffset = m_common_head.off;
	short tailvalue = alignoffset % 4;
	if ( 0 != tailvalue ){
		alignoffset = alignoffset + ( 4 - tailvalue );
	}
	///
	long position = alignoffset - mtf_db_hdr_size;

	/// get and init the size of SFMB
	if(MTF_TAPE == m_type){
		fin.seekg(-mtf_db_hdr_size,std::ios::cur);

		MTF_TAPE_BLK tapeblk;
		const int mtf_tape_blk_size = sizeof(MTF_TAPE_BLK);

		fin.read((char*)&tapeblk,mtf_tape_blk_size);
		StateFactory::s_stateinfo.sfmbsize = tapeblk.sfmSize;
		fin.seekg(-mtf_tape_blk_size,std::ios::cur);

		position += mtf_db_hdr_size;
	}

	fin.seekg( position,std::ios::cur );

	newstates = StateFactory::create(fin);

	assert(m_common_head.type == m_type);
	PrintMacro(m_type);

	if(NULL == newstates){
		fin.seekg(-m_common_head.off,std::ios::cur);
		return false;
	}
	return true;
}

int BlockState::myState()
{
	return m_type;
}
bool StreamState::nextState( States* &newstates,std::ifstream &fin )
{
	fin.read((char*)&streamhead, sizeof(MTF_STREAM_HDR));
	assert(m_type == streamhead.ID);
	PrintMacro(m_type);
	///stream length do not include alignment.
	long position = sizeof(MTF_STREAM_HDR) + streamhead.length;///< uint64 or long?
	int tailvalue = position % 4;
	if ( 0 != tailvalue ){
		position = position + ( 4 - tailvalue );
	}

	/// we have read stream head. so 
	position -= sizeof(MTF_STREAM_HDR);
	int a = MTF_STREAM_VARIABLE & streamhead.mediaAttr;
	if( a == 0){
		fin.seekg(position,std::ios::cur);
	}else{
		/// I do not know how to handle this stuff.
		return false;
	}

	newstates = StateFactory::create(fin);
	if( NULL == newstates ){
		fin.seekg(-position,std::ios::cur);
		return false;
	}
	return true;
}

int StreamState::myState()
{
	return m_type;
}
bool SfmbState::nextState( States* &newstates, std::ifstream &fin )
{
	PrintMacro(m_type);
	if( 0 > m_sfmbbytesize ){
		return false;
	}
	fin.seekg(m_sfmbbytesize,std::ios::cur);
	newstates = StateFactory::create(fin);
	if( NULL == newstates ){
		/// test
		if(!fin.eof()){
			fin.seekg(-m_sfmbbytesize,std::ios::cur);
		}
		return false;
	}
	return true;
}

int SfmbState::myState()
{
	return m_type;
}
States* StateFactory::create( const int& type )
{
	switch( type )
	{
	case MTF_TAPE:
	case MTF_SSET:
	case MTF_VOLB:
	case MTF_DIRB:
	case MTF_FILE:
	case MTF_EOTM:
	case MTF_ESET:
		return new BlockState(type);
	case MTF_SFMB:
		return new SfmbState(type,s_stateinfo.sfmbsize);
	case MTF_STAN:
	case MTF_PNAM:
	case MTF_FNAM:
	case MTF_CSUM:
	case MTF_CRPT:
	case MTF_SPAD:
	case MTF_SPAR:
	case MTF_TSMP:
	case MTF_TFDD:
	case MTF_MAP2:
	case MTF_FDD2:
	case MTF_WINNT_ADAT:
	case MTF_WINNT_NTEA:
	case MTF_WINNT_NACL:
	case MTF_WINNT_NTED:
	case MTF_WINNT_NTQU:
	case MTF_WINNT_NTPR:
	case MTF_WINNT_NTRP:
	case MTF_WINNT_NTOI:
		return new StreamState(type);
	default:/// To be good 		
		return NULL;
	}
}

States* StateFactory::create( std::ifstream &fin )
{
	int type = 0;
	if(0 != fin.read((char*) &type,sizeof(type))){
		States* retval =  StateFactory::create(type);
		fin.seekg(-MTFTYPESIZE,std::ios::cur);
		return retval;
	};
	return NULL;	
}

StateManager::StateManager()
{
	m_state = NULL;
}
StateManager::~StateManager()
{
	delete m_state;
}

bool StateManager::init( std::wstring filename )
{
	m_file_buffer.open(filename.c_str(),std::ios::binary);

	if(m_file_buffer.good()){
		m_state = StateFactory::create(m_file_buffer);
		///debug
		m_file_buffer.seekg(0,std::ios::end);
		DebugPrint( m_file_buffer.tellg() );
		m_file_buffer.seekg(0,std::ios::beg);
		///
		if(NULL != m_state){
			return true;
		}
	}
	return false;
}

void StateManager::uninit()
{
	m_file_buffer.close();
}

bool StateManager::nextState()
{
	States *tempState = NULL;
	bool retval = m_state->nextState(tempState,m_file_buffer);
	if(m_file_buffer.eof()){
		PrintStr("End of file");
	}else{
		PrintStr("Cur file position :");
		DebugPrint(m_file_buffer.tellg());
	}
	if(NULL == tempState){
		return false;
	}else{
		delete m_state;
		m_state = tempState;
	}
	return retval;
}

int StateManager::curStateType()
{
	return this->m_state->myState();
}
int readmtf(std::wstring mtffile)
{
	std::cout<<std::hex;
	StateManager statemgr;
	if(! statemgr.init(mtffile)){
		std::cout<<"can not open file\n";
		return -1;
	}

	u_int32_t filecount = 0;
	while(statemgr.nextState()){
		if(MTF_FILE == statemgr.curStateType()){
			filecount++;
			if (filecount == 3679)
				;
		}
	}
	std::cout<<"total number of files is "<<std::dec<<filecount<<std::endl;
	return 0;
}
