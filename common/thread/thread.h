#ifndef _Thread_H_
#define _Thread_H_

#include "common.h"

namespace Engine {
	class IRunable;
	class ThreadImpl;

	class Thread {
	public:
		Thread(IRunable* pRunable);
		~Thread();

		// start thread work
		bool start();

		// stop thread work
		bool stop();

		// join thread(wait thread until task over)
		void join();

		// detach thread(when thread terminate, its resources released back to system without the need for another
		// thread to join with the terminated thread.)
		void detach();

		// thread sleep
		static void thread_sleep(uint64_t millisec);

	private:
		// thread implement(windows or unix)
		ThreadImpl* m_pThreadImpl;
	};
};

#endif // _Thread_H_
