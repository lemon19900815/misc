#include "threadimpl.h"

namespace Engine
{
	ThreadImpl::ThreadImpl(IRunable* pRunable)
		: m_pRunable(pRunable), m_isWorked(false)
	{
	}
	ThreadImpl::~ThreadImpl()
	{
	}

	// thread start
	bool ThreadImpl::start()
	{
		return false;
	}

	// thread stop
	bool ThreadImpl::stop()
	{
		return false;
	}

	// thread join, block until task finished
	bool ThreadImpl::join()
	{
		return false;
	}

	// thread detach, when terminate release its resource to system
	bool ThreadImpl::detach()
	{
		return false;
	}
};

