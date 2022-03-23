#ifndef __Proxy_Image_Inc_H__
#define __Proxy_Image_Inc_H__

#include "../stdafx.h"

namespace Proxy
{
	class Image
	{
	public:
		virtual ~Image() {

		}

		virtual void display() = 0;
	};

	class RealImage : public Image
	{
	public:
		RealImage(std::string& file) {
			filename_ = file;
			loadFromDisk();
		}

		~RealImage() override {

		}

		void display() override {
			std::cout << "display real image." << std::endl;
		}

	private:
		void loadFromDisk() {
			std::cout << "load " << filename_ << " from disk." << std::endl;
		}

	private:
		std::string filename_;
	};

	class ProxyImage : public Image
	{
	public:
		ProxyImage(std::string filename) {
			realImage_ = new RealImage(filename);
		}

		~ProxyImage() override {
			Safe_Delete(realImage_);
		}

		void display() override {
			realImage_->display();
		}

	private:
		Image* realImage_;
	};

	namespace ProxyPatternDemo
	{
		void test()
		{
			std::cout << "\n\n proxy pattern." << std::endl;

			Image* image = new ProxyImage("beautiful.jpg");
			image->display();
			image->display();

			Safe_Delete(image);
		}
	}
}

#endif //  __Proxy_Image_Inc_H__
