#pragma comment(lib, "BasicBufferPool.lib")
#include "..\BasicBufferPool\Src\BufferPool.h"  
#include <gtest/gtest.h>  
#include <iostream>

using std::cout;
using std::endl;

class BasicBufferPoolTest:public testing::Test
{
public:
    static void SetUpTestCase() 
    {
        for (int i=0; i != sizeof(input_char); ++i)
        {
            input_char[i] = i;
        }
    }///<The first begining

    virtual void SetUp() {/*cout<<"------Start-------"<<endl;*/}
    virtual void TearDown(){/*cout<<"-------End------"<<endl;*/}

    static BasicBufferPool m_Pool1;
    static BasicBufferPool m_Pool2;
    enum {
        POOL1_SIZE = 1024 * 10,
        POOL2_SIZE = 2046 * 10
    };

    enum {
        INPUT_LEN = 20
    };

    static unsigned char input_char[INPUT_LEN] ;
    static int input_count;
    static void modify_input_data()
    {
        for (int i=0; i != sizeof(input_char); ++i)
        {
            input_char[i] += sizeof(input_char);
        }
        ++input_count;
    }
};

unsigned char BasicBufferPoolTest::input_char[INPUT_LEN] = {0};
int BasicBufferPoolTest::input_count = 0;
BasicBufferPool BasicBufferPoolTest::m_Pool1;
BasicBufferPool BasicBufferPoolTest::m_Pool2;

TEST_F(BasicBufferPoolTest, Test_Create) 
{
    //
    BasicBufferPool ZeroPool;
    bool passFlag = false;
    try{
        ZeroPool.Create(0);
    } 
    catch (BufferPoolException & e)
    {
        cout<<e.GetMessage()<<endl;
        passFlag = true;
    }
    EXPECT_EQ(true, passFlag);

    cout<<"Create Buffer For Buffer Pool"<<endl;
    try
    {
        BasicBufferPoolTest::m_Pool1.Create(BasicBufferPoolTest::POOL1_SIZE);
        BasicBufferPoolTest::m_Pool2.Create(BasicBufferPoolTest::POOL2_SIZE);
    }
    catch (BufferPoolException & e)
    {
        cout<<e.GetMessage()<<endl;;
        passFlag = false;
    }

    EXPECT_EQ(true, passFlag);

}

TEST_F(BasicBufferPoolTest, Test_Insert_Pull_No_Silent)
{
    BasicBufferPoolTest::m_Pool1.Insert(input_char, sizeof(input_char));
    unsigned char * out = (unsigned char * )BasicBufferPoolTest::m_Pool1.Pull(sizeof(input_char));
    for (int i=0; i != sizeof(input_char); ++i)
    {
        EXPECT_EQ(input_char[i], out[i]);
    }
}
