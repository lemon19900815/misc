#ifndef __Memento_Memento_Inc_H__
#define __Memento_Memento_Inc_H__

#include "../stdafx.h"
#include <vector>

namespace Memento
{
	class Memento
	{
	public:
		Memento(std::string state) {
			state_ = state;
		}

		const std::string& state() {
			return state_;
		}

	private:
		std::string state_;
	};

	class CareTaker
	{
	public:
		~CareTaker() {
			for (auto& it : mementoVec_) {
				Safe_Delete(it);
			}
			mementoVec_.clear();
		}

		void add(Memento* mt) {
			mementoVec_.push_back(mt);
		}

		Memento* get(int index) {
			return (size_t)index < mementoVec_.size() ? mementoVec_[index] : nullptr;
		}

	private:
		std::vector<Memento*> mementoVec_;
	};
}

#endif // !__Memento_Memento_Inc_H__
