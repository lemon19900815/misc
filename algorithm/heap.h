#include <cstdint>
#include <cstring>

// 最大堆实现
template<class T>
class heap {
 public:
  heap(int capacity) {
	size_ = 0;
	capacity_ = capacity;
	value_ = new T[capacity+1];
  }
  ~heap() {
	release();
  }
  
  void insert(T t) {
	if(size_ > capacity_) {
		realloc();
	}
	
	size_++;
	value_[size_] = t;
	swim(size_);
  }
  
  T get() {
	if(size_ <= 0)
	  return -1;
	return value_[1];
  }
  
  T pop() {
	T t = value_[1];
	std::swap(value_[1], value_[size_]);
	--size_;
	sink(1);
	return t;
  }
  
  int size() {
	return size_;
  }
  
 private:
  // 上浮
  void swim(int i) {
	while(i > 1 && value_[parent(i)] < value_[i]) {
	  std::swap(value_[parent(i)], value_[i]);
	  i = parent(i);
	}
  }
  
  // 下沉
  void sink(int i) {
	while(left(i) <= size_) {
		int child = left(i);
		if(right(i) <= size_ && value_[child] < value_[right(i)])
			child = right(i);
		if(value_[i] > value_[child])
			break;
		std::swap(value_[i], value_[child]);
		i = child;
	}
  }
  
  int parent(int i) {
	return i/2;
  }
  
  int left(int i) {
	return i*2;
  }
  
  int right(int i) {
	return i*2+1;
  }
  
  void release() {
	delete [] value_;
	value_ = nullptr;
  }
  
  void realloc() {
	T* tmp = new T[2*capacity_+1];
	memcpy(tmp, value_, sizeof(T)*(capacity_+1));
	
	release();
	
	value_ = tmp;
	capacity_ = 2*capacity_+1;
  }
  
 private:
  int size_;
  int capacity_;
  T* value_;
};