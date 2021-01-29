#pragma once

#include <pthread.h>

class Mutex {
public:
	Mutex() { pthread_mutex_init(&m_Mutex, NULL); }
	~Mutex() { }
	
	inline void lock() { pthread_mutex_lock(&m_Mutex); }
	inline void unlock() { pthread_mutex_unlock(&m_Mutex); }
	
private:
	pthread_mutex_t m_Mutex;
};
