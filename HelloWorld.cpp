//============================================================================
// Name        : HelloWorld.cpp
// Author      : Qing
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "BufferPool.h"
using namespace std;

int main() {
	enum {
		BSIZE = 10
	};

	BasicBufferPool b;
	try {
		b.Create(BSIZE+2);
		unsigned char a[15] = {0, 1, 2, 3, 4, 5, 6, 7,8,9,10,11,12,13,14};
		int count = 10;
		unsigned char * c  = NULL;
		while(count)
		{
			b.Insert(a, BSIZE);
			c = (unsigned char *)b.Pull(BSIZE);

			// Compare what is written and read
			for (int i=0; i != BSIZE; ++i)
			{
				cout<<"C("<<i<<") = "<< (int)c[i]<<endl;
				if (a[i] != c[i])
				{
					cout<<"Failed!"<<endl;
					return 0;
				}
			}
			--count;
		}
	}
	catch (FMBufferPoolException & e){
		cout<<e.GetMessage()<<endl;
	}
cout<<"Success"<<endl;
return 0;
}
