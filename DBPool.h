/*************************************************************************
  > File Name: DbPool.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 09:44:15 PM
 ************************************************************************/

#ifndef __DBPOOLH__
#define __DBPOOLH__

#include<iostream>
#include <string>
#include <map>
#include "MyLock.h"

using namespace std;

namespace MyNameSpace
{
	struct DBConnInfo
	{
		string host;	
		string user;
		string passwd;
		string dbName;
		int port;
	};
	class MyDBConn;
	class DBPool
	{
		public:
			DBPool(DBConnInfo & info) : connInfo(info), MAX_CONN_COUNT(100)
		{

		}
			void setConnInfo(DBConnInfo & info)
			{
				connInfo = info;
			}
			int getConn();
			void releaseConn(int connId);
		private:
			typedef map<int, MyDBConn *> ConnPoolType;
			ConnPoolType connPool;
			DBConnInfo connInfo;
			const int MAX_CONN_COUNT;
			MyLock myLock;
	};
}

#endif
