#include "Factory/shape.h"
#include "Factory/producer_factory.h"
#include "Singleton/gameworld.h"
#include "Builder/meal.h"
#include "Prototype/shape.h"
#include "Adapter/mediaplayer.h"
#include "Bridge/shape.h"
#include "Filter/criteria.h"
#include "Composite/employee.h"
#include "Decoraator/shapedecorator.h"
#include "Facade/shapemaker.h"
#include "Proxy/image.h"
#include "Chain/logger.h"
#include "Command/order.h"
#include "Interpreter/expression.h"
#include "Iterator/container.h"
#include "Mediator/user.h"
#include "Memento/originator.h"
#include "Observer/subject.h"
#include "State/context.h"
#include "State/tcpstate.h"
#include "NullObject/customer.h"
#include "Strategy/strategy.h"
#include "Template/game.h"
#include "Visitor/computer.h"

int main(int argc, char** argv)
{
	// 工厂模式
	Factory::FactoryPatternDemo::test();

	// 抽象工厂模式
	AbstractFactory::AbstractFactoryPatternDemo::test();

	// 单例模式
	Singleton::SingletonPatternDemo::test();

	// 建造者模式
	Builder::BuilderPatternDemo::test();

	// 原型模式
	Prototype::PrototypePatternDemo::test();

	// 适配器模式
	Adapter::AdapterPatternDemo::test();

	// 桥接模式
	Bridge::BridgePatternDemo::test();

	// 过滤器模式
	Filter::CriteriaPatternDemo::test();

	// 组合模式
	Composite::CompositePatternDemo::test();

	// 装饰者模式
	Decorator::DecoratorPatternDemo::test();

	// 外观模式
	Facade::FacadePatternDemo::test();

	// 代理模式
	Proxy::ProxyPatternDemo::test();

	// 任务链模式
	Chain::ChainPatternDemo::test();

	// 命令模式
	Command::CommandPatternDemo::test();

	// 解释器模式
	Interpreter::InterpreterPatternDemo::test();

	// 迭代器模式
	Iterator::IteratorPatternDemo::test();

	// 中介模式
	Mediator::MediatorPatternDemo::test();

	// 备忘录模式
	Memento::MementoPatternDemo::test();

	// 观察者模式
	Observer::ObserverPatternDemo::test();

	// 状态模式
	State::StatePatternDemo::test();
	TcpState::TcpStateDemo::test();

	// 空对象模型
	NullObject::NullObjectPatternDemo::test();

	// 策略模式
	Strategy::StragegyPatternDemo::test();

	// 模板模式
	Template::TemplatePatternDemo::test();

	// 访问者模式
	Visitor::VisitorPatternDemo::test();

	system("pause");
	return 0;
}

