#include "win_thread.h"

#ifdef PLATFORM_WINDOWS

namespace Engine
{
	static DWORD WINAPI thread_func(void* thread)
	{
		ThreadImpl* impl = static_cast<ThreadImpl*>(thread);
		impl->getRunable()->run();

		return 0;
	}

	WinThreadImpl::WinThreadImpl(IRunable* pRunable)
		: ThreadImpl(pRunable), m_isDetached(false)
	{
	}

	WinThreadImpl::~WinThreadImpl()
	{
		stop();

		if(m_pRunable != NULL) {
			delete m_pRunable;
			m_pRunable = NULL;
		}

		m_isWorked = false;
	}

	// thread start
	bool WinThreadImpl::start()
	{
		if(!m_isWorked) {
			m_threadHandle = CreateThread(
				NULL, 0, thread_func, this, 0, &m_threadDescriptor);
			if(m_threadHandle == NULL)
				return false;

			m_isWorked = true;

			printf("thread(0x%x) started\n", m_threadDescriptor);
		}

		return true;
	}

	// thread stop work
	bool WinThreadImpl::stop()
	{
		if(m_isWorked && !m_isDetached) {
			TerminateThread(m_threadHandle, 0);
			CloseHandle(m_threadHandle);

			m_isDetached = true;

			return true;
		}

		return false;
	}

	// thread join, block until task finished
	bool WinThreadImpl::join()
	{
		if(!m_isWorked) {
			printf("current thread is not started...\n");
			return false;
		}

		if(m_isDetached) {
			printf("thread(%x) is detached, can't be join...\n", m_threadDescriptor);
			return false;
		}

		WaitForSingleObject(m_threadHandle, INFINITE);
		CloseHandle(m_threadHandle);

		m_isDetached = true;

		return true;
	}

	// thread detach, when terminate release its resource to system
	bool WinThreadImpl::detach()
	{
		if(m_isWorked && !m_isDetached) {
			CloseHandle(m_threadHandle); // don't care the thread handle
			m_isDetached = true;

			return true;
		}

		return false;
	}
};

#endif // PLATFORM_WINDOWS
