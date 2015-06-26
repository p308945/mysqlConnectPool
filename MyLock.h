/*************************************************************************
  > File Name: mylock.h
  > Author: huangyun
  > Mail: 895175589@qq.com 
  > Created Time: Fri 26 Jun 2015 09:25:52 PM
 ************************************************************************/
#ifndef __MYLOCK_H
#define __MYLOCK_H
#include <pthread.h>

namespace MyNameSpace
{
	class MyLock
	{
		public:
			MyLock()
			{
				pthread_mutex_init(&mutex, NULL);
			}
			~MyLock()
			{
				pthread_mutex_destroy(&mutex);
			}
			void lock()
			{
				pthread_mutex_lock(&mutex);
			}
			void unlock()
			{
				pthread_mutex_unlock(&mutex);
			}
		private:
			pthread_mutex_t mutex;

	};

	class MyScopeLock
	{
		public:
			MyScopeLock(MyLock & lock) : myLock(lock)
			{
				myLock.lock();
			}
			~MyScopeLock()
			{
				myLock.unlock();
			}
		private:
			MyLock &myLock;
	};
}

#endif
