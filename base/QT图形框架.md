# QT图形框架

参考：https://github.com/fralx/LimeReport.git



## 控件绘制 && 模板自适应绘制

### 图形视图框架简介

QGraphicsView框架结构主要包含三个主要的类QGraphicsScene(场景)、QGraphicsView(视图)、QGraphicsItem(图元)。QGraphicsScene本身不可见，是一个存储图元的容器，必须通过与之相连的QGraphicsView视图来显示及与外界进行交互，主要提供图元的操作接口、传递事件和管理各个图元状态，提供无变换的绘制功能(如打印)；QGraphicsView提供一个可视的窗口，用于显示场景中的图元，一个场景中可以有多个视图。QGraphicsItem是场景中各个图元的基础类

 

**GraphicsView**是一个基于图元的Model/View架构的框架，每一个组件都是一个独立的元素。QPainter采用面向过程的描述方式绘图；GraphicsView采用面向对象的描述方式绘图。GraphicsView绘图时首先创建一个场景，然后创建图元对象(如一个直线对象、一个多边形对象)，再使用场景的add()函数，将图元对象添加到场景中，最后通过视图进行显示。对于复杂的图像来说，如果图像包含大量的直线、曲线、多边形等图元对象，管理图元对象比管理QPainter的绘制过程语句要容易，并且图元对象更符合面向对象的思想，图形的可复用性更好。

 

### QGraphicsScene 场景

QGraphicsScene场景是QGraphicsItem对象的容器，主要功能如下：

- 提供一个快速的接口，用于管理大量图元。
- 向每个图元传递事件
- 管理图元的状态，如:选中、焦点处理
- 提供未进行坐标转换的渲染功能，主要用于打印

　　通过函数QGraphicsScene::addItem()可以加入一个图元到场景中。图元可以通过多个函数进行检索：QGraphicsScene::items()及重载函数可以返回和点、矩形、多边形或向量路径相交的所有图元。QGraphicsScene::itemAt()返回指定点的最顶层图元。所有图元查找函数按照递减栈顺序返回图元，第一个返回的图元位置最顶层，最后一个返回的图元位于最底层。

QGraphicsScene的事件传播体系将场景事件发送给图元，同时也管理图元之间的事件传播。如果场景收到了在某一点的鼠标单击事件，场景会把事件传给在这一点的最顶层图元。QGraphicsScene负责管理一些图元的状态，如图元选择和焦点。通过QGraphicsScene::setSeletionArea()函数选择多个图元，选择区域可以是任意的形状，使用 QPainterPath表示；要得到当前选择的图元列表可以使用 QGraphicsScene::selectedItems()函数；QGraphicsScene还管理图元的键盘输入焦点状态，可以通过QGraphicsScene::setFocusItem()函数或者QGraphicsItem::setFoucs()函数来设置图元的焦点；获得当前具有焦点的图元使用函数QGraphicsScene::foucsItem()。可以使用 QGraphicsScene::render()函数在绘图设备上绘制场景。

- **焦点（Focus）**：指的是当前能够接收键盘输入的部件。一个窗口中同一时间只能有一个部件拥有焦点。用户可以通过鼠标点击或者使用Tab键来切换不同部件的焦点。例如，在文本编辑器中，当你点击一个文本框时，这个文本框就获得了焦点，此时你可以在里面输入文字。如果一个部件有焦点，它通常会有一些视觉上的提示，比如边框颜色变化或闪烁的光标。

 

### QGraphicsView 视图

QGraphicsView是视图窗口部件，使场景内容可视化，可以连接多个视图到一个场景，也可以为相同数据源的数据集提供不同的视图。QGraphicsView是可滚动的窗口部件，可以提供滚动条来浏览大的场景。如果需要使用OpenGL,可以使用QGraphicsView::setViewport()将视图设置为QGLWidget组件。

　　视图接收键盘和鼠标的输入事件，并把事件翻译为场景事件(将坐标转换为场景的坐标)，再发送到场景。使用变换矩阵函数QGraphicsView::martix()可以变换场景的坐标系统,通过变换场景的坐标系统可以实现场景的缩放和旋转。为了方便，QGraphicsView提供了视图和场景的坐标转换函数：QGraphicsView::mapToScene()和QGraphicsView::mapFromScene()。

设计区域的视图缩放，按住Ctrl+鼠标滚轮，并控制最大最小缩放级别（就是继承该类，重写*wheelEvent*）

 

### QGraphicsItem 图元

控件基类同时继承于QGraphicsItem 和 QObject **.**

QGraphicsItem是图元的基类。QGraphics View框架提供了多种标准的图元，例如：矩形(QGraphicsRectlem)、椭圆(QGraphicsEllipseItem)和文本项(QGraphicsTextItcm)等等。

**标准图元：**

- QGraphicsEllipseItem 提供了一个椭圆图元
- QGraphicsLineItem 提供了一个线段图元
- QGraphicsPathItem 提供任意路径图元
- QGraphicsPixmapItem 提供了一个像素图图元
- QGraphicsPolygonItem 提供了一个多边形图元
- QGraphicsRectItem 提供了一个矩形图元
- QGraphicsSimpleTextItem 提供了一个简单的文本标签图元
- QGraphicsTextItem 提供了一个高级的文本浏览图元

用户可以继承QGraphicsItem实现自定义的图元。QGraphicsItem图元主要特性如下：

- 支持鼠标按下、移动、释放、双击、悬停、滚动和右键菜单事件。
- 支持键盘输入焦点和按键事件
- 支持拖拽事件
- 支持分组，使用父子关系和QGraphicsItemGroup
- 支持碰撞检测

图元存在于本地坐标系统上，场景提供了在图元和场景间、图元与图元间进行坐标变换的函数。QGraphicsItem::transform()函数可以使用矩阵转换坐标系统。这对于翻转和缩放图元是有用的。

　　图元可以包含其他图元，父图元的变换会被其所有的子图元继承。无论一个图元本身有多少变换，图元的所有函数(QGraphicsItem::contains(), QGraphicsItem::boundingRect(), QGraphicsItem::collidesWith())仍旧执行在本地坐标系上。

控件和模板的绘制其实就是重写paint()、boundingRect()函数，结合业务绘制出你想要的图形。

 

控件基类同时继承于QGraphicsItem 和 QObject 的原因：

QGraphicsItem 提供了一套丰富的事件处理机制，可以响应多种图形视图系统中的事件，如鼠标点击、键盘输入、焦点变化等。它定义了多个虚函数（例如 paint()、boundingRect()、mousePressEvent() 等），这些函数可以被重写以自定义事件处理行为。

但是，QGraphicsItem 的事件处理机制与 QObject 的事件系统是不同的。虽然 QGraphicsItem 支持事件处理，但它并不支持 QObject 所提供的更广泛的特性，比如：

1. **信号和槽机制**：QObject 提供了强大的信号和槽机制，用于对象之间的通信。这在构建复杂的应用程序时非常有用，尤其是在需要解耦组件的情况下。
2. **属性系统**：QObject 包含了一个元对象系统，允许为类定义动态属性，并且可以通过 Q_PROPERTY 宏进行声明。这使得属性可以在运行时被访问和修改。
3. **定时器事件**：QObject 可以处理定时器事件，这对于动画或其他基于时间的操作非常重要。
4. **事件过滤器**：通过 QObject 的事件过滤功能，可以在事件到达目标对象之前对其进行拦截和处理。
5. **国际化支持**：QObject 也提供了对字符串翻译的支持，有助于创建多语言应用程序。

### 事件传递顺序

在Qt的QGraphicsView框架中，事件传播遵循一个特定的顺序，这确保了事件能够正确地传递给合适的对象。以下是事件传播的一般顺序：

1. **QGraphicsView**：

   - 当用户与图形视图（QGraphicsView）交互时，例如鼠标点击或键盘输入，首先由QGraphicsView接收这些事件。
   - 如果QGraphicsView对事件感兴趣（比如处理滚动条），它可能会直接处理事件并可能接受它。

2. **QGraphicsScene**：

   - 如果QGraphicsView不处理事件或者没有接受事件，那么事件会传递给关联的QGraphicsScene。
   - QGraphicsScene可以处理一些全局性的事件，但它通常的任务是根据事件位置找到最顶层的QGraphicsItem，并将事件传递给它。

3. **QGraphicsItem**：

   - QGraphicsScene根据事件的位置查找位于该点上的所有QGraphicsItem，按照堆叠顺序从上到下遍历。
   - 事件首先传递给最顶层的、可交互（即acceptsHoverEvents()、isSelectable()等返回true）且可见的QGraphicsItem。
   - 如果某个QGraphicsItem接受了事件（通过调用event->accept()），则停止进一步传递；如果它忽略了事件，则继续尝试传递给下一个合适的QGraphicsItem。

4. **子项（Child Items）**：

   - 如果QGraphicsItem是一个容器，它可能会将事件传递给它的子项，这个过程会递归进行，直到事件被某个子项接受为止。

5. **事件过滤器**：

   - 在整个过程中，任何安装了事件过滤器的对象都可以拦截事件。如果事件过滤器选择阻止事件，则事件不会传递给目标对象。

6. **焦点管理**：

   - 对于键盘事件，焦点管理也起作用。只有拥有焦点的QGraphicsItem才会接收到键盘事件。如果场景中的任何QGraphicsItem有焦点，并且事件类型是键盘事件，那么QGraphicsScene会优先将事件发送给具有焦点的QGraphicsItem。

7. **自定义行为**：

   - 开发者可以通过重写QGraphicsView、QGraphicsScene或QGraphicsItem中的事件处理函数来自定义事件传播逻辑。例如，可以在QGraphicsScene::mousePressEvent或QGraphicsItem::mousePressEvent中添加额外的逻辑。

总结来说，事件首先到达QGraphicsView，然后传递给QGraphicsScene，最后由QGraphicsScene负责将事件分发给适当的QGraphicsItem。如果你在任意一层重写了事件处理器，记得考虑是否需要调用父类的方法以维持正常的事件传播链。

 

<font color='red'>注意：QGraphicsView框架的事件循环，只是调用event->ignore(). 事件并不会继续传播，需要手动调用其父类的默认实现；</font>

在QWidget及其派生类中，事件的传播机制与QGraphicsView框架有所不同。对于QWidget，如果你调用event->ignore()，事件会自动传递给父部件。这是因为在QWidget层次结构中，Qt设计了默认的事件传播规则，使得未被处理（即被忽略）的事件会自动向上冒泡到父部件。

**QWidget 中的事件传播**

1. **事件首先传递给最底层的子部件**：当一个事件发生时，它首先传递给位于鼠标位置最顶层的子部件。
2. **如果子部件忽略了事件**：如果这个子部件通过调用event->ignore()表示它不处理该事件，那么事件会自动传递给它的父部件。
3. **事件继续向上传播**：这个过程会持续进行，直到某个部件接受了事件或到达顶级窗口为止。

这种机制简化了事件处理逻辑，允许开发者仅需关注特定部件的事件处理，而不必显式地管理事件的传播路径。

**QGraphicsScene 和 QGraphicsItem 的区别**

相比之下，在QGraphicsScene和QGraphicsItem中，事件的传播更为复杂，并且默认情况下不会自动将被忽略的事件传递给父图元。原因如下：

- **不同的事件模型**：QGraphicsScene中的事件系统更类似于节点树（scene graph），其中每个图元可以独立处理事件，而不仅仅是简单的父子关系。
- **堆叠顺序优先**：如前所述，事件首先传递给位于点击位置最上方的图元（根据堆叠顺序），并且只有当所有可能的图元都拒绝了事件之后，才可能会有进一步的传播行为。
- **没有自动向上传播**：QGraphicsItem之间的事件传播不像QWidget那样有明确的“向上冒泡”机制。因此，如果你重写了QGraphicsScene的事件处理器并调用了event->ignore()，但没有调用父类的方法或手动分发事件，事件不会自动传递给其他图元。



## 控件属性

控件的属性依赖于Qt的属性系统和元系统。

### Qt 元系统

Qt中的元对象系统（Meta-Object System）提供了对象间通信的信号和槽机制、运行时类型信息和动态属性系统。元对象系统是基于以下3个条件的：

1.   该类必须继承自QObject类；

2.   必须在类的私有声明区声明Q_OBJECT宏（在类定义时，如果没有指定public或者private，则默认为private)；

3.   元对象编译器Meta-Object Compiler（moc），为QObject的子类实现元对象特性提供必要的代码。

其中，moc工具读取一个C+ +源文件，如果它发现一个或者多个类的声明中包含有Q_OBJECT宏，便会另外创建一个C+ +源文件（就是在项目目录中的debug目录 下看到的以moc开头的C+ +源文件），其中包含了为每一个类生成的元对象代码。 这些产生的源文件或者被包含进类的源文件中，或者和类的实现同时进行编译和链接。

 

元对象系统主要是为了实现信号和槽机制才被引入的，不过除了信号和槽机制以外，元对象系统还提供了其他一些特性：

1. QObjeCt::metaObject()函数可以返回一个类的元对象，它是QMetaObject类的对象；

2. QMetaObject::className()可以在运行时以字符串形式返回类名，而不需要C+ +编辑器原生的运行时类型信息（RTTI）的支持；

3. QObject:: “inherits()函数返回一个对象是否是QObject继承树上一个类的实例的信息；

4. QObject: :tr()和QObject: :trUtf8()迸行字符串翻译来实现国际化；

5. QObject::setProperty()和QObject::property()通过名字来动态设置或者获取对象属性；

6. QMetaObject: :newlnstance()构造该类的一个新实例。

 

### Qt属性系统

 

在**QObject**的子类中，使用宏**Q_PROPERTY**定义属性；格式如下：

```c++
Q_PROPERTY(type name
           READ getFunction
           [WRITE setFunction]
           [RESET resetFunction]
           [NOTIFY notifySignal]
           [DESIGNABLE bool]
           [SCRIPTABLE bool]
           [STORED bool]
           [USER bool]
           [CONSTANT]
           [FINAL])
```



MEMBER：指定一个成员变量与属性关联，使之成为可读可写的属性，指定后无需再设置READ和WRITE。

RESET：是可选的，用于将属性设置为上下文指定的默认值

1. Q_PROPERTY这个宏，简单用法如下：

2. Q_PROPERTY(type name READ getFunction WRITE setFunction)

3. Q_PROPERTY(参数类型 参数名称 READ 获得属性值函数 WRITE 设置属性值函数)

4. 比如Q_PROPERTY(bool bIsDoubi READ getDoubi WRITE setDoubi)，属性类型是bool类型，bIsDoubi是属性名称。除此之外还需要写两个函数，第一个是设置属性的函数void setDoui(bool)，第二个是获得属性的函数bool getDoubi()。

5. QMetaObject类的一些函数可以提供元对象所描述类的属性元数据，属性元数据用QMetaProperty类描述，它有各种函数可反应属性的一些特性，例如下面的一段代码：

```c++
//获取一个SpinBox的元对象
const QMetaobject *meta=ui->spinBoy->metaobject();
//获取属性value的序号
int index-meta->indexofProperty("value");

//获取属性valuef的元数据
OMetaProperty prop=meta->property(index);

/属性是否可写，值为true
bool res=prop.isWritable();
//属性是否可设计，值为true
res=prop.isDesignable();

//属性是否有反映属性值变化的信号，值为true
res=prop.hasNotifySignal();
```

选中一个控件，通过元对象系统获取到该对象的所有属性，并加载对应属性的窗口。

## 遇到错误

在多线程环境中，每个线程通常有自己的事件循环，并且负责管理自己拥有的对象。如果一个对象是由一个线程创建的，并且该线程正在管理这个对象的事件循环，那么其他线程通常不应该直接对这个对象发送事件或消息。这是因为直接跨线程操作对象可能会导致数据竞争、死锁或其他并发问题，这些问题可能会破坏数据的完整性和程序的稳定性。为了解决这个问题，有几种常见的策略：

- 消息传递：可以使用线程安全的消息队列或其他同步机制，让一个线程向另一个线程发送消息。接收消息的线程可以在其事件循环中处理这些消息，并据此更新其拥有的对象。
- <font color='red'>使用信号和槽机制（如在Qt框架中）：这种机制允许一个线程中的对象发出信号，而另一个线程中的对象可以通过连接到这个信号的槽来响应。框架内部会处理线程间的同步问题。</font>
- 避免跨线程访问：尽量设计程序，使得每个线程只操作它自己创建和管理的对象。这样可以大大简化线程间的同步问题。
- 使用锁或其他同步原语：如果确实需要在多个线程间共享数据或对象，可以使用互斥锁（mutexes）、读写锁（read-write locks）、条件变量（condition variables）等同步原语来确保线程安全的数据访问。

总之，当你遇到“cannot send events to objects owned by a different thread”这样的错误或限制时，你应该重新审视你的线程设计和对象所有权，确保线程间的交互是安全且高效的。

 