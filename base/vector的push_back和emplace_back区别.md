## vector.push_back&emplace_back区别

- 首先来看一段代码示例

  ```c++
  struct Box {
    int32_t xmin = 0;
    int32_t xmax = 0;
  
    Box(int32_t a) {
      xmin = a;
      //xmax = b;
      std::cout << "default constructor..." << std::endl;
    }
  
    Box(const Box& rhs) {
      std::cout << "copy constructor..." << std::endl;
    }
  
    Box(const Box&& rhs) noexcept {
      std::cout << "move constructor..." << std::endl;
    }
  
    Box& operator=(const Box& rhs) {
      std::cout << "operator= ..." << std::endl;
      return *this;
    }
  
    Box& operator=(const Box&& rhs) noexcept {
      std::cout << "move operator= ..." << std::endl;
      return *this;
    }
  
    ~Box() {
      std::cout << "destructor: " << this << std::endl;
    }
  };
  
  void test() {
      std::vector<Box> boxes;
  
      // 修改vector的容量；
      // 消除在push_back和emplace_back的过程中扩容产生的影响
      boxes.reserve(10);
  
      Box box1(1);
    	Box box2(2);
      
      std::cout << "push_back: " << std::endl;
      boxes.push_back(box1); // 调用拷贝构造
      std::cout << "emplace_back: " << std::endl;
      boxes.emplace_back(box2); // 调用拷贝构造
      // 注意：如果push_back或emplace_back的过程中导致扩容，
      // 则emplace_back会多调用1次移动构造
  
      std::cout << "push_back: std::move" << std::endl;
      boxes.push_back(std::move(box1)); // 调用1次移动构造
      std::cout << "emplace_back: std::move" << std::endl;
      boxes.emplace_back(std::move(box2)); // 调用1次移动构造
  
      std::cout << "================================";
  
      // 直接通过参数传递；而非对象
      std::cout << "push_back: param" << std::endl;
      boxes.push_back(3); // 1次构造，1次移动构造
      std::cout << "emplace_back: param" << std::endl;
      boxes.emplace_back(3); // 调用1次构造
  }
  ```

- 结论

  - 以上代码的分别在`windows`和`linux`下做了测试；
- `boxes.reserve(10);`指定容器的`capacity`容量，避免扩容导致二者的比较出现额外的影响；
  - `emplace_back`只有在<font color=red>通过参数传递</font>，在容器内部构建对象时效率才高（少一次移动构造过程）；在发生扩容时效率反而低于`push_back`，其他时候这二者效果一致；
- 其他时候都建议使用`push_back`，而非使用`emplace_back`;
  - 在通过对象传递时，`push_back`和`emplace_back`结果一致（如果发生扩容，则`push_back`效率更优）。
  - 更多详情可以参考：[条款四十二：考虑使用置入代替插入](https://github.com/CnTransGroup/EffectiveModernCppChinese/blob/master/src/8.Tweaks/item42.md)
