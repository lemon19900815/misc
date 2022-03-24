#include "unix_thread.h"

#ifndef PLATFORM_WINDOWS

namespace Engine
{
	static void* thread_func(void* thread)
	{
		ThreadImpl* impl = static_cast<ThreadImpl*>(thread);
		impl->getRunable()->run();

		return NULL;
	}

	UnixThreadImpl::UnixThreadImpl(IRunable* pRunable)
		: ThreadImpl(pRunable), m_isDetached(false)
	{
	}

	UnixThreadImpl::~UnixThreadImpl()
	{
		stop();

		if(m_pRunable != NULL) {
			delete m_pRunable;
			m_pRunable = NULL;
		}

		m_isWorked = false;
	}

	// thread start
	bool UnixThreadImpl::start()
	{
		if(!m_isWorked) {
			int retval = pthread_create(&m_threadHandle, NULL, thread_func, this);
			if(retval != 0) {
				printf("pthread_create failed...\n");
				return false;
			}

			m_isWorked = true;
			m_threadDescriptor = m_threadhandle;

			printf("thread(0x%x) started\n", m_threadDescriptor);
		}

		return true;
	}

	// thread stop work
	bool UnixThreadImpl::stop()
	{
		if(m_isWorked && !m_isDetached) {
			pthread_cancel(m_threadHandle);
			pthread_detach(m_threadHandle);

			m_isDetached = true;

			return true;
		}

		return false;
	}

	// thread join, block until task finished
	bool UnixThreadImpl::join()
	{
		if(!m_isWorked) {
			printf("current thread is not started...\n");
			return false;
		}

		if(m_isDetached) {
			printf("thread(%x) is detached, can't be join...\n", m_threadDescriptor);
			return false;
		}

		void* status;
		pthread_join(m_threadHandle, &status);

		return true;
	}

	// thread detach, when terminate release its resource to system
	bool UnixThreadImpl::detach()
	{
		if(m_isWorked && !m_isDetached) {
			pthread_detach(m_threadHandle);

			m_isDetached = true;

			return true;
		}

		return false;
	}
};

#endif // PLATFORM_WINDOWS
