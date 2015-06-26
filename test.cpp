/*************************************************************************
  > File Name: test.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 11:21:20 PM
 ************************************************************************/

#include<iostream>
#include "DBPool.h"
using namespace std;
using namespace MyNameSpace;
int main()
{
	DBConnInfo info;
	info.host = "localhost";
	info.user = "root";
	info.passwd = "123123";
	info.dbName = "test";
	info.port = 3306;
	DBPool pool(info);

	while(1)
	{
		int id = pool.getConn();
		cout<<"get Handle"<<id<<endl;
		pool.releaseConn(id);
		sleep(1);
	}
	return 0;
}
