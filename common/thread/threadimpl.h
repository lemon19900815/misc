#ifndef _Thread_Impl_H_
#define _Thread_Impl_H_

#include "common.h"
#include "runable.h"

#ifdef PLATFORM_WINDOWS
#define ThreadDescriptor DWORD
#define ThreadHandle HANDLE
#else
#include <pthread.h>
#define ThreadDescriptor pthread_t
#define ThreadHandle pthread_t
#endif

namespace Engine
{
	class ThreadImpl
	{
	public:
		ThreadImpl(IRunable* pRunable);
		virtual ~ThreadImpl();

		// thread start
		virtual bool start();
		// thread stop
		virtual bool stop();
		// thread join, block until task finished
		virtual bool join();
		// thread detach, when terminate release its resource to system
		virtual bool detach();

		// get thread descriptor
		ThreadDescriptor getThreadId() {
			return m_threadDescriptor;
		}

		// get runable instance
		inline IRunable* getRunable() {
			return m_pRunable;
		}

	protected:
		// thread runable instance
		IRunable* m_pRunable;

		// thread work status
		bool m_isWorked;

		// thread descriptor
		ThreadDescriptor m_threadDescriptor;

		// thread handle
		ThreadHandle m_threadHandle;
	};
};

#endif // _Thread_Impl_H_
