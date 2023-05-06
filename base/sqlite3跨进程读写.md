# sqlite3跨进程读写

本次实验内容为一个进程写，另外一个进程读，2个进程在同一台机器上

SQLite 实现[原子提交和](https://www.sqlite.org/atomiccommit.html)回滚的默认方法是[回滚日志](https://www.sqlite.org/lockingv3.html#rollback)

## 传统模式

- 在该模式下，读会阻塞写，写同样会阻塞读

- 通常会出现`close statment error code 5-->5 - database file is busy`的错误

## WAL模式

预写日志模式：[Write-Ahead Logging (sqlite.org)](https://www.sqlite.org/wal.html)

- 开启方式

  ```python
  # 建立连接
  db = sqlite3.connect('test.db')
  
  # 执行sql语句开启WAL模式
  db.execute("PRAGMA journal_mode=WAL;")
  ```

- 优势

  - 在大多数情况下，WAL 的速度要快得多
  - WAL 提供了更好的并发性，Reader不会阻塞Writer，Writer也不会阻塞Reader（极端情况除外：[Write-Ahead Logging#9](https://www.sqlite.org/wal.html)）
  - 使用 WAL 的磁盘 I/O 操作顺序写入，写入效率更高
  - WAL 使用更少的 `fsync() `操作，因此不易受到 `fsync() `系统调用中断的系统上的问题

- 缺点

  - WAL 通常要求 [VFS](https://www.sqlite.org/vfs.html) 支持**共享内存原语**，Windows和Unix内置支持（嵌入式系统可能不支持）

  - 使用数据库的所有进程必须位于同一台主机上， WAL 不能在网络文件系统上工作

  - 在读取多而很少写入的应用程序中，比传统的回滚日志方法，WAL可能会稍微慢一点（可能慢 1% 或 2%）

  - 还有一个额外的准持久性“`-wal`”文件和与每个共享内存文件关联的“`-shm`”共享内存文件数据库

  - 有[额外的检查点](https://www.sqlite.org/wal.html#ckpt)操作，虽然默认是自动处理的，但是仍然需要应用程序开发人员注意

  - 检查操作系统查看是否支持共享内存原语

    ```sh
    # Linux:: ipcs: show information on IPC facilities
    ipcs -m
    
    # Windows:: https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
    ```

- 测试用例（基于ormpp框架）：**writer需要先启动，reader后启动**

  ```c++
  struct Person
  {
      int32_t id;
      int32_t sex;
      std::string name;
  };
  
  REFLECTION(Person, id, sex, name);
  
  void writer()
  {
      ormpp::dbng<ormpp::sqlite> db;
      auto rc = db.connect("test.db");
  
      // 开启WAL模式（Write-Ahead-Logging：预写日志模式）
      rc = db.execute("PRAGMA journal_mode=WAL;");
  
      rc = db.create_datatable<Person>();
  
      int32_t id = 0;
      while (rc)
      {
          id++;
          Person person{id, 0, "lemon"+std::to_string(id)};
          if (db.insert(person) != 1)
          {
              LOG(ERROR) << "write error: " << db.get_last_error();
          }
          else
          {
              json_object obj;
              serialization::struct_to_json(obj, person);
              LOG(INFO) << "write " << obj.to_pretty_string();
          }
  
          using namespace std::chrono_literals;
          std::this_thread::sleep_for(1ms);
      }
  }
  
  void reader()
  {
      ormpp::dbng<ormpp::sqlite> db;
      auto rc = db.connect("./test.db");
  
      int32_t id = 0;
      while (rc)
      {
          auto person_list =
              db.query<Person>("where id>"+std::to_string(id) + " limit 1");
          if (!person_list.empty())
          {
              json_object obj;
              serialization::batch_struct_to_json<Person>(obj, person_list);
  
              LOG(INFO) << obj.to_pretty_string();
              ++id;
          }
          else if (db.get_last_error() != "")
          {
              LOG(ERROR) << db.get_last_error();
          }
          else
          {
              //LOG(WARNING) << "no person data.";
          }
  
          //using namespace std::chrono_literals;
          //std::this_thread::sleep_for(1ms);
      }
  }
  ```

  

