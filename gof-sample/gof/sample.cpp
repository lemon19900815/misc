#include "Factory/shape.h"
#include "Factory/producer_factory.h"
#include "Singleton/gameworld.h"
#include "Builder/meal.h"
#include "Prototype/shape.h"
#include "Adapter/mediaplayer.h"
#include "Bridge/shape.h"
#include "Filter/criteria.h"
#include "Composite/employee.h"
#include "Decoraator/shape.h"
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
#include "Factory/maze.h"

int main(int argc, char** argv)
{
    // ����ģʽ
    Factory::FactoryPatternDemo::test();

    // ���󹤳�ģʽ
    AbstractFactory::AbstractFactoryPatternDemo::test();

    // ����ģʽ
    Singleton::SingletonPatternDemo::test();

    // ������ģʽ
    Builder::BuilderPatternDemo::test();

    // ԭ��ģʽ
    Prototype::PrototypePatternDemo::test();

    // ������ģʽ
    Adapter::test();

    // �Ž�ģʽ
    Bridge::test();

    // ������ģʽ
    Filter::CriteriaPatternDemo::test();

    // ���ģʽ
    Composite::CompositePatternDemo::test();

    // װ����ģʽ
    Decorator::test();

    // ���ģʽ
    Facade::FacadePatternDemo::test();

    // ����ģʽ
    Proxy::ProxyPatternDemo::test();

    // ������ģʽ
    Chain::ChainPatternDemo::test();

    // ����ģʽ
    Command::CommandPatternDemo::test();

    // ������ģʽ
    Interpreter::InterpreterPatternDemo::test();

    // ������ģʽ
    Iterator::IteratorPatternDemo::test();

    // �н�ģʽ
    Mediator::MediatorPatternDemo::test();

    // ����¼ģʽ
    Memento::MementoPatternDemo::test();

    // �۲���ģʽ
    Observer::ObserverPatternDemo::test();

    // ״̬ģʽ
    State::StatePatternDemo::test();
    TcpState::TcpStateDemo::test();

    // �ն���ģ��
    NullObject::NullObjectPatternDemo::test();

    // ����ģʽ
    Strategy::StragegyPatternDemo::test();

    // ģ��ģʽ
    Template::TemplatePatternDemo::test();

    // ������ģʽ
    Visitor::VisitorPatternDemo::test();

    maze::test();

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}

