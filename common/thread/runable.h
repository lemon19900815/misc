#ifndef _IRunable_H_
#define _IRunable_H_

namespace Engine
{
	class IRunable {
	public:
		virtual~IRunable() { }
		virtual void run() = 0;
	};
};

#endif // _IRunable_H_
