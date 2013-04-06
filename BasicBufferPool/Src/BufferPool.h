//****************************
//!@file	BUFFERPOOL.H	 
//!@author	QingYang
//!@date 	2013/4/4
//!@brief		
//****************************
#pragma once

#include <string>
#define wxT(a)  a
using std::string;

typedef string wxString;
typedef unsigned char byte;

class BasicBufferPool {
public:
    BasicBufferPool();
    ~BasicBufferPool();
    void Create(int size);
    void Insert(const void * buffer, int len);
    void *Pull(int len);
	int GetReadLength() const;
	int GetWriteLength() const;

    //! If silent flag is set, none overflow exception will be throwed.
	void SilenceMode(bool bSilent);
private:
    byte * mpbuffer;
    int  mReadOffset;
    int  mWriteOffset;
    bool   mbIsStart;
    bool   mbWarn;
    int    mSize;
	bool    mbIsSilent;
	byte * mpoutput;
    enum {
        READ,
        WRITE
    };
    inline void check_input_len(int len);
    void check_write_over_flow(int writeLen);

    void check_read_over_flow(int readLen);
    void over_flow_happened(int operation);
};

class BufferPoolException 
{
public:
    BufferPoolException(wxString &message) {mMessage = message;}
    ~BufferPoolException(){};
    wxString GetMessage() const { return mMessage;}
protected:
    wxString mMessage;
};