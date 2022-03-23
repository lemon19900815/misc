#ifndef __Template_Game_Inc_H__
#define __Template_Game_Inc_H__

#include "../stdafx.h"

namespace Template
{
	class Game
	{
	public:
		virtual ~Game() {

		}

		// template 
		void play() {
			initialize();
			startPlay();
			endPlay();
		}

	private:
		virtual void initialize() = 0;
		virtual void startPlay() = 0;
		virtual void endPlay() = 0;
	};

	class Football : public Game
	{
	public:
		~Football() override {

		}

	private:
		void initialize() override {
			std::cout << "Football initialize." << std::endl;
		}

		void startPlay() override {
			std::cout << "Football startPlay." << std::endl;
		}

		void endPlay() override {
			std::cout << "Football endPlay." << std::endl;
		}
	};

	class Basketball : public Game
	{
	public:
		~Basketball() override {

		}

	private:
		void initialize() override {
			std::cout << "Basketball initialize." << std::endl;
		}

		void startPlay() override {
			std::cout << "Basketball startPlay." << std::endl;
		}

		void endPlay() override {
			std::cout << "Basketball endPlay." << std::endl;
		}
	};

	namespace TemplatePatternDemo
	{
		void test()
		{
			std::cout << "\n\n template pattern." << std::endl;

			auto game1 = new Football();
			game1->play();

			std::cout << std::endl;

			auto game2 = new Basketball();
			game2->play();

			Safe_Delete(game1);
			Safe_Delete(game2);
		}
	}
}

#endif // !__Template_Game_Inc_H__
