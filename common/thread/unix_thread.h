#ifndef _Unix_Thread_H_
#define _Unix_Thread_H_

#include "common.h"
#include "threadimpl.h"

#ifndef PLATFORM_WINDOWS

namespace Engine
{
	class UnixThreadImpl : public ThreadImpl
	{
	public:
		UnixThreadImpl(IRunable* pRunable);
		~UnixThreadImpl();

		// thread start
		bool start();
		// thread stop work
		bool stop();
		// thread join, block until task finished
		bool join();
		// thread detach, when terminate release its resource to system
		bool detach();

	private:
		// thread status is detached or not
		bool m_isDetached;
	};
};


#endif // PLATFORM_WINDOWS

#endif // _Unix_Thread_H_
