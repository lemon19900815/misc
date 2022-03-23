#ifndef __Adapter_MediaPlayer_Inc_H__
#define __Adapter_MediaPlayer_Inc_H__

#include "../stdafx.h"
#include "advancedmediaplayer.h"

namespace Adapter
{
	class MediaPlayer
	{
	public:
		virtual ~MediaPlayer() {

		}

		virtual void play(const std::string& audioType, const std::string& file) = 0;
	};

	class MediaAdapter : public MediaPlayer
	{
	public:
		MediaAdapter(const std::string& name) {
			if (name == "vlc") {
				advancedPlayer_ = new VlcPlayer();
			}
			else if (name == "mp4") {
				advancedPlayer_ = new Mp4Player();
			}
			else {
				advancedPlayer_ = nullptr;
			}
		}

		~MediaAdapter() override {
			Safe_Delete(advancedPlayer_);
		}

		void play(const std::string& audioType, const std::string& file) override {
			if (advancedPlayer_) {
				advancedPlayer_->play(file);
			}
		}

	private:
		AdvancedMediaPlayer* advancedPlayer_;
	};

	class AudioPlayer : public MediaPlayer
	{
	public:
		AudioPlayer() {
			vlcAdapter_ = nullptr;
			mp4Adapter_ = nullptr;
		}
		~AudioPlayer() override {
			Safe_Delete(vlcAdapter_);
			Safe_Delete(mp4Adapter_);
		}

		void play(const std::string& audioType, const std::string& file) override {
			if (audioType == "mp3") {
				std::cout << "player mp3: " << file << std::endl;
			}
			else {
				auto mediaAdapter = adapter(audioType);
				if (mediaAdapter) {
					mediaAdapter->play(audioType, file);
				}
			}
		}

	private:
		MediaAdapter* adapter(const std::string& audioType) {
			if (audioType == "vlc") {
				if (!vlcAdapter_)
					vlcAdapter_ = new MediaAdapter(audioType);
				return vlcAdapter_;
			}
			else if (audioType == "mp4") {
				if (!mp4Adapter_)
					mp4Adapter_ = new MediaAdapter(audioType);

				return mp4Adapter_;
			}
			else {
				return nullptr;
			}
		}

	private:
		MediaAdapter* vlcAdapter_;
		MediaAdapter* mp4Adapter_;
	};

	namespace AdapterPatternDemo
	{
		void test()
		{
			std::cout << "\n\n adapter pattern." << std::endl;

			MediaPlayer* audioPlayer = new AudioPlayer();
			audioPlayer->play("mp3", "my love.mp3");
			audioPlayer->play("vlc", "as long as you love me.vlc");
			audioPlayer->play("mp4", "sexlove.mp4");

			Safe_Delete(audioPlayer);
		}
	}
}

#endif // !__Adapter_MediaPlayer_Inc_H__
