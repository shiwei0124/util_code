//
//  test.cpp
//  my-code
//
//  Created by luoning on 14-8-5.
//  Copyright (c) 2014年 luoning. All rights reserved.
//
#include "io_loop.h"
#include "test.h"
#include "base_thread.hpp"
#include <stdio.h>
#include <uuid/uuid.h>
#include <string>

typedef struct tag_TestA
{
    int a;
    char* b;
    char c[32];
}TestA;


void* func(void* param)
{
    printf("func..\n");
    return NULL;
}

void thread_test()
{
    CBaseThread thread;
    thread.Start(func, NULL);
    thread.Wait();
    if (thread) {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }
}

void io_loop_test()
{
    CIOLoop io_loop;
    io_loop.Start();
    printf("aaa\n");
    io_loop.Stop();
}

void uuid_test()
{
    uuid_t uu;
    int i;
    uuid_generate( uu );
    uuid_string_t str_uuid;
    uuid_unparse(uu, str_uuid);
    printf("%s\n", (char*)str_uuid);
}

int main()
{
    TestA* a = new TestA[2]();
    TestA b;
    printf("%d, %d, %s\n", b.a, b.b, b.c);
    thread_test();
    io_loop_test();
}