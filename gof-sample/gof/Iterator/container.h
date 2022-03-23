#ifndef __Iterator_Container_Inc_H__
#define __Iterator_Container_Inc_H__

#include "../stdafx.h"
#include "iterator.h"

namespace Iterator
{
	template<typename T>
	class Container
	{
	public:
		virtual ~Container() {

		}

		virtual Iterator<T>* getIterator() = 0;
	};

	class NameRepository : public Container<std::string>
	{
	public:
		NameRepository(std::string* names, int len) {
			len_ = len;
			names_ = names;
			iterator_ = nullptr;
		}

		~NameRepository() override {
			Safe_Delete(iterator_);
		}

		Iterator<std::string>* getIterator() override {
			if (!iterator_) {
				iterator_ = new NameIterator(names_, len_);
			}
			return iterator_;
		}

	private:
		class NameIterator : public Iterator<std::string>
		{
			int			 index_;
			int			 len_;
			std::string* names_;

		public:
			NameIterator(std::string* name, int len) {
				index_ = 0;
				len_ = len;
				names_ = name;
			}

			bool hasNext() override {
				return index_ < len_;
			}

			std::string* next() override {
				return hasNext() ? &names_[index_++] : nullptr;
			}
		};

	private:
		int			 len_;
		std::string* names_;
		Iterator<std::string>* iterator_;
	};

	namespace IteratorPatternDemo
	{
		void test()
		{
			std::cout << "\n\n iterator pattern." << std::endl;

			std::string str[] = { "Jack", "Json", "Python", "Java" };
			auto container = new NameRepository(str, 4);
			auto iterator = container->getIterator();

			while (iterator->hasNext())
			{
				std::cout << *(iterator->next()) << ", ";
			}

			std::cout << std::endl;
		}
	}
}

#endif // !__Iterator_Container_Inc_H__
