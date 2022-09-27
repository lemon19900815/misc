# Abstract Factory

- 优缺点

  - 分离了具体的类
  - 易于交换产品系列（带魔法的迷宫或者带炸弹的迷宫等）

  - 有利于产品的一致性（见`MazeGame.CreateMaze(MazeFactory& factory)`）
  - 缺点：每个子对象的构建都需要定义一个子类工厂
  - 缺点：扩展新的产品时，需要修改父类以及所有的子类

- UML 图
  ![](../img/AbstractFactory.png)

 - 代码示例

   ```c++
   Maze::Ptr CreateMaze(MazeFactory& factory) {
       auto maze = factory.MakeMaze();
   
       auto r1 = factory.MakeRoom(1);
       auto r2 = factory.MakeRoom(2);
       auto door = factory.MakeDoor(r1, r2);
   
       maze->AddRoom(r1);
       maze->AddRoom(r2);
   
       r1->SetSide(North, factory.MakeWall());
       r1->SetSide(East, door);
       r1->SetSide(South, factory.MakeWall());
       r1->SetSide(West, factory.MakeWall());
   
       r2->SetSide(North, factory.MakeWall());
       r2->SetSide(East, factory.MakeWall());
       r2->SetSide(South, factory.MakeWall());
       r2->SetSide(West, door);
   
       return maze;
   }
   
   void factoryTest() {
       MazeGame game;
   
       // 硬编码创建maze；不能适应新的游戏玩法，
       // 新增带魔法或者bomb的的类型数据，需要修改创建maze的规则
       auto maze1 = game.CreateMaze();
   
       // 抽象工厂创建普通的maze
       MazeFactory factory;
       auto maze2 = game.CreateMaze(factory);
   
       // 抽象工厂创建带spell的maze
       EnchantedMazeFactory enchanted_factory;
       auto maze3 = game.CreateMaze(enchanted_factory);
   
       // 抽象工厂创建带bomb的maze
       BombedMazeFactory bombed_factory;
       auto maze4 = game.CreateMaze(bombed_factory);
   
       // 设计模式不是不允许修改代码，而是把经常变化的那部分进行封装、抽象；
       // 这里经常变化的可能就是属性的玩法，而maze的创建过程一般不会发生变化。
       // 隔离变化：通常就是为了维持代码的不变。
   
       /*
           抽象工厂模式的优点：
           1、分离了具体的类
           2、使得易于交换产品系列（带魔法的迷宫或者带炸弹的迷宫等）
           3、有利于产品的一致性
   
           缺点：
           1、难以支持新种类的产品（新增产品需要修改Factory基类和所有子类）
   
           相关模式：
           1、AbstractFactory抽象工厂类通常用工厂方法（Factory Method）实现，但它们也可以用Prototype实现。
           2、一个具体的工厂通常是一个单件（Singleton）。
       */
   }
   ```

