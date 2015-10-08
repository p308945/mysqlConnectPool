/*************************************************************************
  > File Name: db.cpp
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 09:34:21 PM
 ************************************************************************/

#include<iostream>
#include <mysql/mysql.h>
#include <pthread.h>
#include "MyTime.h"
#include "MyLock.h"
#include "DBPool.h"
#include <unistd.h>

using namespace std;
namespace MyNameSpace
{
	class MyDBConn
	{
		public:
			enum CONN_STATUS
			{
				CONN_INVALID,
				CONN_USING,
				CONN_VALID,
			};
			MyDBConn(DBPool::DBConnInfo &info) : connInfo(info), timeout(5), 
			mysql(NULL), myId(0), connStatus(CONN_INVALID),
			threadId(0)
		{
		}
			~MyDBConn()
			{
				fini();
			}
			bool init()
			{
				if (mysql)
				{
					mysql_close(mysql);
					mysql = NULL;
				}
				if (connDB())
				{
					return true;
				}
				cerr<<"init DB error"<<endl;
				return false;
			}
			void fini()
			{
				if (mysql)
				{
					mysql_close(mysql);
				}
			}
			void getConn()
			{
				threadId = pthread_self();
				cout<<"get ("<<threadId<<":"<<myId<<")"<<endl;
				connStatus = CONN_USING;
				myTime.now();
			}
			void releaseConn()
			{
				cout<<"release ("<<threadId<<":"<<myId<<")"<<endl;
				threadId = 0;
				connStatus = CONN_VALID;
			}
			time_t getUsingTime()
			{
				return myTime.eplase();
			}

			int getMyId()
			{
				return myId;
			}

			CONN_STATUS getConnStatus()
			{
				return connStatus;
			}

			pthread_t getThreadId()
			{
				return threadId;
			}
		private:
			bool connDB()
			{
				mysql = mysql_init(NULL);
				if (NULL == mysql)
				{
					cerr<<"mysql init error:"<<endl;
					return false;
				}
				mysql_options(mysql, MYSQL_OPT_READ_TIMEOUT, (const char *)&timeout);
				mysql_options(mysql, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&timeout);
				if (!mysql_real_connect(mysql, connInfo.host.c_str(), connInfo.user.c_str(), connInfo.passwd.c_str(), connInfo.dbName.c_str(), connInfo.port, 0, 0))
				{
					cerr<<"mysql connect error:"<<mysql_error(mysql)<<endl;
					mysql_close(mysql);
					mysql = NULL;
					return false;
				}
				myId = connId++;
				connStatus = CONN_VALID;
				cout<<"DB:"<<myId<<" conn ok!"<<endl;
				return true;
			}
		private:
			static int connId;
			DBPool::DBConnInfo connInfo;
			const int timeout;
			MYSQL *mysql;
			int myId;
			CONN_STATUS connStatus;
			pthread_t threadId;
			MyTime myTime;
	};

	int MyDBConn::connId = 0; 

	int DBPool::getConn()
	{
		while(1)
		{
			MyScopeLock scopeLock(myLock);
			MyDBConn *invalidDb = NULL;
			for(ConnPoolType::iterator iter = connPool.begin(); iter != connPool.end(); )
			{
				MyDBConn *tmp = NULL;
				tmp = iter->second;
				if (NULL == tmp)
				{
					connPool.erase(iter++);
					continue;
				}
				switch(tmp->getConnStatus())
				{
					case MyDBConn::CONN_INVALID:
						{
							invalidDb = tmp;
						}
						break;
					case MyDBConn::CONN_USING:
						{
							if (tmp->getUsingTime() > 600)
							{
								cerr<<"thread:"<<tmp->getThreadId()<<" using("<<tmp->getMyId()<<")"<<tmp->getUsingTime()<<"s"<<endl;
							}
						}
						break;
					case MyDBConn::CONN_VALID:
						{
							tmp->getConn();
							return tmp->getMyId();
						}
						break;
				}
				++iter;
			}
			if (NULL != invalidDb)
			{
				if (invalidDb->init())
				{
					invalidDb->getConn();
					return invalidDb->getMyId();
				}
				else
				{
					cerr<<"conn init error"<<endl;
				}
			}
			if (connPool.size() < MAX_CONN_COUNT)
			{
				MyDBConn *pConn = new MyDBConn(connInfo);
				if (NULL != pConn)
				{
					if (!pConn->init())
					{
						cerr<<"conn init error"<<endl;
						delete pConn;
					}
					else
					{
						if (connPool.insert(ConnPoolType::value_type(pConn->getMyId(), pConn)).second)
						{
							pConn->getConn();
							return pConn->getMyId();
						}
						else
						{
							cerr<<"conn insert pool error"<<endl;
							delete pConn;
						}
					}
				}
			}
			cerr<<"get db conn usleep"<<endl;
			usleep(1000);
		}
	}

	void DBPool::releaseConn(int connId)
	{
		ConnPoolType::iterator iter = connPool.find(connId);		
		if (iter == connPool.end())
		{
			cerr<<"release conn error not found("<<connId<<"in pool"<<endl;
		}
		iter->second->releaseConn();
	}
}
