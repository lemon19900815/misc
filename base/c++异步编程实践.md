[TOC]



# C++异步编程实践

本文主要讲述C++异步编程实践相关内容。



## 1. thread

https://en.cppreference.com/w/cpp/thread/thread.html

```c++
std::thread thd([]() {
    // do something....
});

if(thd.joinable()) {
	thd.join();
}
```

注：线程库未提供优先级、CPU亲和度控制等，可以通过`native_handle`接口可以获取原始线程对象，再调用平台相关的接口进行设置；



## 2. mutex

https://en.cppreference.com/w/cpp/thread/mutex.html

### 2.1 mutex

### 2.2 timed_mutex

### 2.3 recursive_mutex

### 2.4 recursive_timed_mutex

### 2.5 lock_guard

构造加锁，析构解锁，只支持单个锁；

```c++
std::mutex m;
std::lock_guard<std::mutex> lk(m);

// since c++14
std::lock_guard lk(m);
```



### 2.6 scoped_lock: since c++17

按顺序加解锁，支持多个锁，可有效避免死锁，保证顺序一致；

以下是`msvc`标准库实现，从源码看，可以直观的看到加解锁的逻辑；

```c++
template <class... _Mutexes>
class _NODISCARD_LOCK scoped_lock { // class with destructor that unlocks mutexes
public:
    explicit scoped_lock(_Mutexes&... _Mtxes) : _MyMutexes(_Mtxes...) { // construct and lock
        _STD lock(_Mtxes...);
    }

    explicit scoped_lock(adopt_lock_t, _Mutexes&... _Mtxes) noexcept // strengthened
        : _MyMutexes(_Mtxes...) {} // construct but don't lock

    ~scoped_lock() noexcept {
        _STD apply([](_Mutexes&... _Mtxes) _STATIC_CALL_OPERATOR { (..., (void) _Mtxes.unlock()); }, _MyMutexes);
    }

    scoped_lock(const scoped_lock&)            = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;

private:
    tuple<_Mutexes&...> _MyMutexes;
};
```



### 2.7 unique_lock（写锁）

### 2.8 shared_lock（读锁）：since c++14



## 3. condition_variable

https://en.cppreference.com/w/cpp/thread/condition_variable.html

注意虚假唤醒问题，使用wait第二个参数探测；

```c++
std::mutex mtx;
std::condition_variable cv;

// c++14之后可以直接写std::unique_lock lk(mtx)；它可以直接推导模板参数类型
std::unique_lock<std::mutex> lk(mtx);
cv.wait(lock, []() { return i == 4; });
```



## 4. atomic

https://en.cppreference.com/w/cpp/atomic/atomic.html

https://github.com/CnTransGroup/EffectiveModernCppChinese/blob/master/src/7.TheConcurrencyAPI/item40.md

c++20 提供了与条件变量相同的唤醒接口，等待值发生改变，wait接口传入旧值。

```c++
int main()
{
    std::atomic<bool> all_tasks_completed{false};
    std::atomic<unsigned> completion_count{};
    std::future<void> task_futures[16];
    std::atomic<unsigned> outstanding_task_count{16};
 
    // Spawn several tasks which take different amounts of
    // time, then decrement the outstanding task count.
    for (auto& task_future : task_futures) {
        task_future = std::async([&] {
            // This sleep represents doing real work...
            std::this_thread::sleep_for(50ms);
 
            ++completion_count;
            --outstanding_task_count;
 
            // When the task count falls to zero, notify
            // the waiter (main thread in this case).
            if (outstanding_task_count.load() == 0) {
                all_tasks_completed = true;
                all_tasks_completed.notify_one();
            }
        });
    }
 
    // 等待值发生该改变，wait时传入旧值
    all_tasks_completed.wait(false);
 
    std::cout << "Tasks completed = " << completion_count.load() << '\n';
}
```



## 5. async

https://en.cppreference.com/w/cpp/thread/async.html

https://github.com/CnTransGroup/EffectiveModernCppChinese/blob/master/src/7.TheConcurrencyAPI/item36.md

```c++
auto fut = std::async([]() {
   // do something... 
});

// wait async task finish.
fut.wait();
```

注意：

1. 如果不对future进行wait操作，则对象在析构时，仍然会阻塞等待异步任务完成；
2. 如果想正常进行异步操作，需要持有future对象，直至任务完成；



## 6. promise/future

https://github.com/CnTransGroup/EffectiveModernCppChinese/blob/master/src/7.TheConcurrencyAPI/item39.md

有时，一个任务通知另一个异步执行的任务发生了特定的事件很有用，因为第二个任务要等到这个事件发生之后才能继续执行。事件也许是一个数据结构已经初始化，也许是计算阶段已经完成，或者检测到重要的传感器值。这种情况下，线程间通信的最佳方案是什么？

一个明显的方案就是使用条件变量（*condition variable*）。如果我们将检测条件的任务称为**检测任务**（*detecting task*），对条件作出反应的任务称为**反应任务**（*reacting task*），策略很简单：反应任务等待一个条件变量，检测任务在事件发生时改变条件变量。代码如下：

```c++
std::condition_variable cv;         //事件的条件变量
std::mutex m;                       //配合cv使用的mutex
```

检测任务中的代码不能再简单了：

```c++
…                                   //检测事件
cv.notify_one();                    //通知反应任务
```

如果有多个反应任务需要被通知，使用`notify_all`代替`notify_one`，但是这里，我们假定只有一个反应任务需要通知。

这时我们就可以使用promise/future来处理这种场景。

检测任务代码就可以很简洁：

```c++
std::promise<void> p;                   //通信信道的promise
…                                       //检测某个事件
p.set_value();                          //通知反应任务
```

反应任务代码也同样简单：

```c++
…                                       //准备作出反应
p.get_future().wait();                  //等待对应于p的那个future
…                                       //对事件作出反应
```



## 7. packaged_task

https://en.cppreference.com/w/cpp/thread/packaged_task.html

https://github.com/progschj/ThreadPool

```c++
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop{false};
};

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}
```



## 8. jthread: since c++20

https://en.cppreference.com/w/cpp/thread/jthread.html

- 自动析构线程：request_stop & join

  ```c++
  std::jthread thd([]() {
      std::this_thread::sleep_for(1s);
      // do something...
  });
  
  // thd析构时，自动调用request_stop & join，无需像thread那样再去处理join操作；
  ```

- 线程运行控制：stop_token & request_stop

  ```c++
  void print(auto txt)
  {
      std::cout << std::this_thread::get_id() << ' ' << txt;
  }
   
  int main()
  {
      // A sleepy worker thread
      std::jthread sleepy_worker([](std::stop_token stoken) {
          for (int i = 10; i; --i) {
              std::this_thread::sleep_for(300ms);
              if (stoken.stop_requested()) {
                  print("Sleepy worker is requested to stop\n");
                  return;
              }
              print("Sleepy worker goes back to sleep\n");
          }
      });
   
      // A waiting worker thread
      // The condition variable will be awoken by the stop request.
      std::jthread waiting_worker([](std::stop_token stoken) {
          std::mutex mutex;
          std::unique_lock lock(mutex);
          std::condition_variable_any().wait(lock, stoken, []{ return false; });
          print("Waiting worker is requested to stop\n");
          return;
      });
   
      // Sleep this thread to give threads time to spin
      std::this_thread::sleep_for(400ms);
   
      // std::jthread::request_stop() can be called explicitly:
      print("Requesting stop of sleepy worker\n");
      sleepy_worker.request_stop();
      sleepy_worker.join();
      print("Sleepy worker joined\n");
   
      // Or automatically using RAII:
      // waiting_worker's destructor will call request_stop()
      // and join the thread automatically.
  }
  ```
  
  

## 9. semaphore: since c++20

https://en.cppreference.com/w/cpp/thread/counting_semaphore.html

哲学家就餐问题：https://blog.csdn.net/theLostLamb/article/details/80741319

- P(s)：acquire。如果s是非零的，那么P将s减1，并且立即返回。如果s为零，那么就挂起这个线程，直到s变为非零，而一个V操作会重启这个线程。在重启之后，P操作将s减1，并将控制返回给调用者；
- V(s)：release。V操作将s加1。如果有任何线程阻塞在P操作等待s变成非零，那么V操作会重启这些线程中的一个，然后该线程将s减1，完成它的P操作。



### 9.1 哲学家就餐问题

#### 9.1.1 问题描述

由Dijkstra提出并解决的哲学家就餐问题是典型的同步问题。该问题描述的是五个哲学家共用一张圆桌，分别坐在周围的五张椅子上，在圆桌上有五个碗和五只筷子，他们的生活方式是交替的进行思考和进餐。平时，一个哲学家进行思考，饥饿时便试图取用其左右最靠近他的筷子，只有在他拿到两只筷子时才能进餐。进餐完毕，放下筷子继续思考。

#### 9.1.2 解题思路

因为是五位哲学家，并且每位哲学家的各自做自己的事情（思考和吃饭），因此可以创建五个线程表示五位哲学家，五个线程相互独立（异步）。并对五位哲学家分别编号为`0~4`。

 同时，有五根筷子，每根筷子只对其相邻的两位哲学家是共享的，因此这五根筷子可以看做是五种不同的临界资源（不是一种资源有5个，因为每根筷子只能被固定编号的哲学家使用）。并对五根筷子分别编号为`0~4`，其中第`i`号哲学家左边的筷子编号为`i`，则其右边的筷子编号就应该为`(i + 1) % 5`。

 因为筷子是临界资源，因此当一个线程在使用某根筷子的时候，应该给这根筷子加锁，使其不能被其他进程使用。

 根据以上分析，可以使用`pthread_create`函数创建五个线程，可以使用`pthread_mutex_t chops[5]`表示有五根筷子，五个不同的临界资源，并用`pthread_mutex_init(&chops[i], NULL);`来初始化他们。

#### 9.1.3 问题求解

根据上面的分析，可以得到一个基本的解决方案如下：

```c++
void philosopher (void* arg) {
    while (1) {
        think();
        hungry();
        pthread_mutex_lock(&chopsticks[left]);
        pthread_mutex_lock(&chopsticks[right]);
        eat();
        pthread_mutex_unlock(&chopsticks[left]);    
        pthread_mutex_unlock(&chopsticks[right]);
    }
}
```

这段伪代码的思路很明确，这个函数代表的是一个哲学家的活动，可以将其创建为五个不同的线程代表五位不同的哲学家。每位哲学家先思考，当某位哲学家饥饿的时候，就拿起他左边的筷子，然后拿起他右边的筷子，然后进餐，然后放下他左右的筷子并进行思考。因为筷子是临界资源，所以当一位哲学家拿起他左右的筷子的时候，就会对他左右的筷子进行加锁，使其他的哲学家不能使用，当该哲学家进餐完毕后，放下了筷子，才对资源解锁，从而使其他的哲学家可以使用。

 这个过程看似没什么问题，但是当你仔细分析之后，你会发现这里面有一个很严重的问题，就是死锁，就是每个线程都等待其他线程释放资源从而被唤醒，从而每个线程陷入了无限等待的状态。在哲学家就餐问题中，一种出现死锁的情况就是，假设一开始每位哲学家都拿起其左边的筷子，然后每位哲学家又都尝试去拿起其右边的筷子，这个时候由于每根筷子都已经被占用，因此每位哲学家都不能拿起其右边的筷子，只能等待筷子被其他哲学家释放。由此五个线程都等待被其他进程唤醒，因此就陷入了死锁。

#### 9.1.4 死锁问题解决

解决死锁问题的办法有很多，下面对各种办法做一下详细的介绍：

- Plan A

   第一种解决死锁问题的办法就是同时只允许四位哲学家同时拿起同一边的筷子，这样就能保证一定会有一位哲学家能够拿起两根筷子完成进食并释放资源，供其他哲学家使用，从而实现永动，避免了死锁。举个最简单的栗子，假定0~3号哲学家已经拿起了他左边的筷子，然后当4号哲学家企图去拿他左边的筷子的时候，将该哲学家的线程锁住，使其拿不到其左边的筷子，然后其左边的筷子就可以被3号哲学家拿到，然后3号哲学家进餐，释放筷子，然后更多的哲学家拿到筷子并进餐。

   如何才能实现当4号哲学家企图拿起其左边的筷子的时候将该哲学家的线程阻塞？这个时候就要用到该问题的提出者迪杰斯特拉（这货还提出了迪杰斯特拉最短路径算法，著名的银行家算法也是他发明的）提出的信号量机制。因为同时只允许有四位哲学家同时拿起左筷子，因此我们可以设置一个信号量r，使其初始值为4，然后每当一位哲学家企图去拿起他左边的筷子的时候，先对信号量做一次P操作，从而当第五位哲学家企图去拿做筷子的时候，对r做一次P操作，r = -1，由r < 0得第五位哲学家的线程被阻塞，从而不能拿起左筷子，因此也就避免了死锁问题。然后当哲学家放下他左边的筷子的时候，就对r做一次V操作。

   根据上面的分析，代码可以修改为：

  ```c++
  void philosopher (void* arg) {
      while (1) {
          think();
          hungry();
          P(&r);//C语言提供的P操作的函数是sem_wait
          pthread_mutex_lock(&chopsticks[left]);
          pthread_mutex_lock(&chopsticks[right]);
          eat();
          pthread_mutex_unlock(&chopsticks[left]);
          V(&r);//C语言提供的V操作的函数是sem_post
          pthread_mutex_unlock(&chopsticks[right]);
      }
  }
  ```

- Plan B

  第二种解决死锁问题的办法就是使用AND信号量机制，意思就是如果想给某个哲学家筷子，就将他需要的所有资源都给他，然后让他进餐，否则就一个都不给他。

   根据上面的分析，可以将代码修改为：

  ```c++
  void philosopher (void* arg) {
      while (1) {
          think();
          hungry();
          Swait(chopsticks[left], chopsticks[right]);
          eat();
          Spost(chopsticks[left], chopsticks[right]);
      }
  }
  ```

  但是C语言的库里面并没有给提供AND型信号量，但是我们可以利用互斥量简单的替代一下AND信号量，就是设置一个全局互斥量mutex，用来锁住全部的临界资源，当一个哲学家企图拿筷子的时候，就将所有的资源锁住，然后让他去拿他需要的筷子，等他取到他需要的筷子之后，就解锁，然后让其他哲学家取筷子。代码如下：

  ```c++
  void philosopher (void* arg) {
      while (1) {
          think();
          hungry();
          pthread_mutex_lock(mutex);
          pthread_mutex_lock(&chopsticks[left]);
          pthread_mutex_lock(&chopsticks[right]);
          pthread_mutex_unlock(mutex);
          eat();
          pthread_mutex_unlock(&chopsticks[left]);
          pthread_mutex_unlock(&chopsticks[right]);
      }
  }
  ```

  

- Plan C

  第三种解决的办法就是规定奇数号哲学家先拿起他左边的筷子，然后再去拿他右边的筷子，而偶数号的哲学家则相反，这样的话总能保证一个哲学家能获得两根筷子完成进餐，从而释放其所占用的资源，代码如下：

  ```c++
  void philosopher (void* arg) {
      int i = *(int *)arg;
      int left = i;
      int right = (i + 1) % N;
      while (1) {
          printf("哲学家%d正在思考问题\n", i);
          delay(50000);
  
          printf("哲学家%d饿了\n", i);
          if (i % 2 == 0) {//偶数哲学家，先右后左
              pthread_mutex_lock(&chopsticks[right]);
              pthread_mutex_lock(&chopsticks[left]);
              eat();
              pthread_mutex_unlock(&chopsticks[left]);
              pthread_mutex_unlock(&chopsticks[right]);
          } else {//奇数哲学家，先左后又
              pthread_mutex_lock(&chopsticks[left]);
              pthread_mutex_lock(&chopsticks[right]);
              eat();
              pthread_mutex_unlock(&chopsticks[right]);
              pthread_mutex_unlock(&chopsticks[left]);
          }
      }
  }
  ```

  

## 10. latch: since c++20

https://en.cppreference.com/w/cpp/thread/latch.html

非可重入，只能wait一次；

```c++
struct Job
{
    const std::string name;
    std::string product{"not worked"};
    std::thread action{};
};
 
int main()
{
    Job jobs[]{{"Annika"}, {"Buru"}, {"Chuck"}};
 
    std::latch work_done{std::size(jobs)};
    std::latch start_clean_up{1};
 
    auto work = [&](Job& my_job)
    {
        my_job.product = my_job.name + " worked";
        work_done.count_down();
        start_clean_up.wait();
        my_job.product = my_job.name + " cleaned";
    };
 
    std::cout << "Work is starting... ";
    for (auto& job : jobs)
        job.action = std::thread{work, std::ref(job)};
 
    work_done.wait();
    std::cout << "done:\n";
    for (auto const& job : jobs)
        std::cout << "  " << job.product << '\n';
 
    std::cout << "Workers are cleaning up... ";
    start_clean_up.count_down();
    for (auto& job : jobs)
        job.action.join();
 
    std::cout << "done:\n";
    for (auto const& job : jobs)
        std::cout << "  " << job.product << '\n';
}
```

输出：

```
Work is starting... done:
  Annika worked
  Buru worked
  Chuck worked
Workers are cleaning up... done:
  Annika cleaned
  Buru cleaned
  Chuck cleaned
```



## 11.  barrier: since c++20

https://en.cppreference.com/w/cpp/thread/barrier.html

可重入，wait之后可以继续调用；完成之后会重置计数器。

```c++
int main()
{
    const auto workers = {"Anil", "Busara", "Carl"};
 
    auto on_completion = []() noexcept {
        // locking not needed here
        static auto phase =
            "... done\n"
            "Cleaning up...\n";
        std::cout << phase;
        phase = "... done\n";
    };
 
    std::barrier sync_point(std::ssize(workers), on_completion);
 
    auto work = [&](std::string name) {
        std::string product = "  " + name + " worked\n";
        std::osyncstream(std::cout) << product;  // ok, op<< call is atomic
        sync_point.arrive_and_wait();
 
        product = "  " + name + " cleaned\n";
        std::osyncstream(std::cout) << product;
        sync_point.arrive_and_wait(); // 可重入
    };
 
    std::cout << "Starting...\n";
    std::vector<std::jthread> threads;
    threads.reserve(std::size(workers));
    for (auto const& worker : workers)
        threads.emplace_back(work, worker);
}
```

输出：

```
Starting...
  Anil worked
  Carl worked
  Busara worked
... done
Cleaning up...
  Busara cleaned
  Carl cleaned
  Anil cleaned
... done
```