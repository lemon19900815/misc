#include "thread.h"

#ifdef PLATFORM_WINDOWS
#include "win_thread.h"
#define NewThreadImpl(runable) new WinThreadImpl(pRunable)
#else
#include "unix_thread.h"
#define NewThreadImpl(runable) new UnixThreadImpl(pRunable)
#endif

namespace Engine
{
	Thread::Thread(IRunable* pRunable)
	{
		m_pThreadImpl = NewThreadImpl(pRunable);
	}

	Thread::~Thread()
	{
		if(m_pThreadImpl) {
			delete m_pThreadImpl;
			m_pThreadImpl = NULL;
		}
	}

	// start thread work
	bool Thread::start()
	{
		return m_pThreadImpl->start();
	}

	// stop thread work
	bool Thread::stop()
	{
		return m_pThreadImpl->stop();
	}

	// join thread(wait thread until task over)
	void Thread::join()
	{
		m_pThreadImpl->join();
	}

	// detach thread(when thread terminate, its resources released back to system without the need for another
	// thread to join with the terminated thread.)
	void Thread::detach()
	{
		m_pThreadImpl->detach();
	}

	// thread sleep
	void Thread::thread_sleep(uint64_t millisec)
	{
#ifdef PLATFORM_WINDOWS
		Sleep((DWORD)millisec);
#else
		timespec req, rem;
		req.tv_sec = millisec / 1000;
		req.tv_nsec = millisec % 1000 * 1000000;

		rem.tv_sec = 0;
		rem.tv_nsec = 0;

		int retval = nanosleep(&req, &rem); // if return -1, set errno EINTR the sleep interrupt by a signal
		if(retval == -1 && errno == EINTR) {
			printf("not support recall nanosleep when interrupted by a signal");
		}
#endif
	}
}
