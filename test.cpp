/*************************************************************************
  > File Name: test.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 11:21:20 PM
 ************************************************************************/

#include<iostream>
#include "DBPool.h"
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

bool exit = 0;

void signal_ctrlc(int sig)
{
//	cout<<"get signal:"<<sig<<endl;
	exit = 1;
}

void* fun(void *arg)
{
	pthread_detach(pthread_self());
	MyNameSpace::DBPool * pool = (MyNameSpace::DBPool *)arg;
	int id = pool->getConn();
	cout<<"pthread :"<<pthread_self()<<"get Handle:"<<id<<"......operate db in here!"<<endl;
	sleep(2);
	pool->releaseConn(id);
}

int main()
{
	MyNameSpace::DBPool::DBConnInfo info;
	info.host = "localhost";
	info.user = "root";
	info.passwd = "123123";
	info.dbName = "test";
	info.port = 3306;
	MyNameSpace::DBPool pool(info);
	signal(SIGINT, signal_ctrlc);
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	//ctrl+c exit loop
	while(!exit)
	{
		pthread_create(&pid1, NULL, fun, (void *)&pool);
		pthread_create(&pid2, NULL, fun, (void *)&pool);
		pthread_create(&pid3, NULL, fun, (void *)&pool);
		sleep(1);
	}
	return 0;
}
