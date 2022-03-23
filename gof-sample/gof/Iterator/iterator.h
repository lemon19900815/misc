#ifndef __Iterator_Iterator_Inc_H__
#define __Iterator_Iterator_Inc_H__

namespace Iterator
{
	template<typename T>
	class Iterator
	{
	public:
		virtual ~Iterator() {

		}

		virtual bool hasNext() = 0;
		virtual T* next() = 0;
	};
}

#endif // !__Iterator_Iterator_Inc_H__
