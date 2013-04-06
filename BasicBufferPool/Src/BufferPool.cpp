//****************************
//!@file	BUFFERPOOL.CPP	 
//!@author	QingYang
//!@date 	2013/4/4
//!@brief		
//****************************

#include "BufferPool.h"


static inline void check_allocate_ptr(const void * p)
{
	if (NULL == p)
	{
		wxString failed = wxT("Memory Allocate Failed!");
		throw BufferPoolException(failed);
	}
}

static inline void check_input_ptr(const void * p)
{
	if (NULL == p)
	{
		wxString failed = wxT("Input Param NULL!");
		throw BufferPoolException(failed);
	}
}

BasicBufferPool::BasicBufferPool()
{
	mpbuffer    = NULL;
	mpoutput	= NULL;
	mReadOffset = 0;
	mWriteOffset= 0;
	mbIsStart   = 0;
	mbWarn      = 0;
	mSize       = 0;
    mbIsSilent   = false;
}

BasicBufferPool::~BasicBufferPool()
{
	delete mpbuffer;
}

void BasicBufferPool::Create(int size)
{
	check_input_len(size);
	mpbuffer = new byte[size];
	check_allocate_ptr(mpbuffer);
	mSize = size;
}

void BasicBufferPool::Insert(const void * buffer, int len)
{
	check_input_ptr(buffer);
	check_input_len(len);
	
	check_write_over_flow(len);

	// Write data now
	byte * input = (byte *)buffer;
	for (int i=0; i != len; ++i)
	{
		if (mWriteOffset == mSize)
		{
			mWriteOffset = 0;
		}

		mpbuffer[mWriteOffset] = *input++;
		++mWriteOffset;
	}
}

void * BasicBufferPool::Pull(int len)
{
	check_input_len(len);
	check_read_over_flow(len);

	//allocate output memory
	delete mpoutput;
	mpoutput = NULL;
	mpoutput = new byte[len];
	check_allocate_ptr(mpoutput);
	byte * output = mpoutput;
	for (int i=0; i != len; ++i)
	{
		if (mReadOffset == mSize)
		{
			mReadOffset = 0;
		}
		*output++ = mpbuffer[mReadOffset];
		++mReadOffset;
	}
	return mpoutput;
}


inline void BasicBufferPool::check_input_len(int len)
{
	if (!(len > 0))
	{
		wxString what_happend = wxT("Input Value should be positive");
		throw BufferPoolException(what_happend);
	}

//	if (len > mSize && mSize)
//	{
//		wxString what_happend = wxT("Input length is too long.");
//		throw FMBufferPoolException(what_happend);
//	}
}

// Check if wirte length with writeLen exceed.
void BasicBufferPool::check_write_over_flow(int writeLen)
{
	if (mWriteOffset == mReadOffset)
	{
		// Two Condition would happen here.
		// 1. first start
		// 2. write near overflow
		if (mbIsStart)
		{
			if (writeLen > mSize)
			{
				over_flow_happened(WRITE);
			}
		}
		
	} else if (mWriteOffset < mReadOffset)
	{
		// Write offset must not exceed mReadOffset
		if ((mWriteOffset + writeLen) > mReadOffset)
		{
			over_flow_happened(WRITE);
		}
	} else 
	{
		// mWriteOffset > mReadOffset
		if ((writeLen - (mSize - mWriteOffset)) > mReadOffset)
		{
			over_flow_happened(WRITE);
		}
	}
}

void BasicBufferPool::check_read_over_flow(int readLen)
{
	if (mWriteOffset == mReadOffset)
	{
		// Two Condition would happen here.
		// 1. first start
		// 2. write near overflow
		if (mbIsStart)
		{
			if (readLen > mSize)
			{
				over_flow_happened(READ);
			}
		}

	} else if (mReadOffset < mWriteOffset)
	{
		// Write offset must not exceed mReadOffset
		if ((mReadOffset + readLen) > mWriteOffset)
		{
			over_flow_happened(READ);
		}
	} else 
	{
		// mWriteOffset > mReadOffset
		if ((readLen - (mSize - mReadOffset)) > mWriteOffset)
		{
			over_flow_happened(READ);
		}
	}
}

void BasicBufferPool::over_flow_happened(int operation)
{
	wxString what_happend;
	switch (operation)
	{
	case READ:
		{
			what_happend = wxT("Read Over Flow!");
			break;
		}
	case WRITE:
		{
			what_happend = wxT("Write Over Flow!");
			break;
		}
	default:
		{
			what_happend = wxT("Unexpected happend!");
			break;
		}
	}

	if (mbIsSilent)
	{
        throw BufferPoolException(what_happend);
	}
}

int BasicBufferPool::GetReadLength() const
{
	if (mWriteOffset >= mReadOffset)
	{
		return (mWriteOffset - mReadOffset);
	} else {
		return (mSize - mReadOffset + mWriteOffset);
	}
}

int BasicBufferPool::GetWriteLength() const
{
	if (mReadOffset >= mWriteOffset)
	{
		return (mReadOffset - mWriteOffset);
	} else {
		return (mSize - mWriteOffset + mReadOffset);
	}
}

void BasicBufferPool::SilenceMode(bool bSilent)
{
    mbIsSilent = bSilent;
}