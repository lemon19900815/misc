#ifndef __Singleton_Gameworld_Inc_H__
#define __Singleton_Gameworld_Inc_H__

#include "singleton.h"
#include <iostream>

namespace Singleton
{
	class GameWorld : public Singleton<GameWorld>
	{
		friend class Singleton<GameWorld>;

	public:
		void showMessage() {
			std::cout << "in gameworld." << std::endl;
		}

	protected:
		GameWorld() {

		}
		~GameWorld() override {

		}
	};

	namespace SingletonPatternDemo
	{
		void test()
		{
			std::cout << "\n\n singleton pattern." << std::endl;

			GameWorld::getInstance()->showMessage();
		}
	}
}

#endif //  __Singleton_Gameworld_Inc_H__
