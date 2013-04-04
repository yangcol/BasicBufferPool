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
private:
    byte * mpbuffer;
    int  mReadOffset;
    int  mWriteOffset;
    bool   mbIsStart;
    bool   mbWarn;
    int    mSize;

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

class FMBufferPoolException 
{
public:
    FMBufferPoolException(wxString &message) {mMessage = message;}
    ~FMBufferPoolException(){};
    wxString GetMessage() const { return mMessage;}
protected:
    wxString mMessage;
};