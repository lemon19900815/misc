#ifndef __Singleton_Inc_H__
#define __Singleton_Inc_H__

namespace Singleton
{
	template<typename T>
	class Singleton
	{
	public:
		static T* getInstance() {
			if (!inst_) {
				inst_ = new T();
			}

			return inst_;
		}

	protected:
		Singleton() {
			inst_ = nullptr;
		}
		virtual ~Singleton() {
			if (inst_) {
				delete inst_;
				inst_ = nullptr;
			}
		}

	private:
		static T* inst_;
	};

	template<class T>
	T* Singleton<T>::inst_ = nullptr;
}

#endif // __Singleton_Inc_H__
