#ifndef __Adapter_AdvancedMediaPlayer_Inc_H__
#define __Adapter_AdvancedMediaPlayer_Inc_H__

#include "../stdafx.h"

namespace Adapter
{
	class AdvancedMediaPlayer
	{
	public:
		virtual ~AdvancedMediaPlayer() {

		}

		virtual void play(const std::string& file) = 0;
	};

	class VlcPlayer : public AdvancedMediaPlayer
	{
	public:
		~VlcPlayer() override {

		}

		void play(const std::string& file) override {
			std::cout << "vlc play: " << file << std::endl;
		}
	};

	class Mp4Player : public AdvancedMediaPlayer
	{
	public:
		~Mp4Player() override {

		}

		void play(const std::string& file) override {
			std::cout << "mp4 play: " << file << std::endl;
		}
	};
}

#endif // !__Adapter_AdvancedMediaPlayer_Inc_H__
