#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>

#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TThreadPoolServer.h>

#include <thrift/transport/THttpTransport.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/THttpServer.h>
#include <thrift/transport/TNonblockingServerSocket.h>

#include "HelloWorld.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace example;

class HelloWorldImpl : public example::HelloWorldIf {
public:
    HelloWorldImpl() {}

    virtual void sayHello(
        std::string& _return, const std::string& name) override
    {
        std::cout << __FUNCTION__ << ": recv: " << name << std::endl;
        _return = "hello client.";
    }
};

class HelloWorldIfFactoryImpl : public example::HelloWorldIfFactory {
public:
    virtual HelloWorldIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override {
        return new HelloWorldImpl();
    }
    virtual void releaseHandler(HelloWorldIf* handler) override {
        delete handler;
    }
};

int main(int argc, char** argv)
{
    int port = 9090;
    //::std::shared_ptr<HelloWorldImpl> handler(new HelloWorldImpl());
    //::std::shared_ptr<TProcessor> processor(new example::HelloWorldProcessor(handler));

    //shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    //std::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    //std::shared_ptr<TTransportFactory> transportFactory(new THttpServerTransportFactory());

    //TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    //server.serve();

    //::std::shared_ptr<HelloWorldImpl> handler(new HelloWorldImpl());
    //::std::shared_ptr<TProcessor> processor(new HelloWorldProcessor(handler));
    //auto processorFactory = std::make_shared<HelloWorldIfFactoryImpl>();

    //shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    //std::shared_ptr<TProtocolFactory> protocolFactory(new TJSONProtocolFactory());
    //std::shared_ptr<TTransportFactory> transportFactory(new THttpServerTransportFactory());

    auto handler = std::make_shared<HelloWorldImpl>();
    auto processor = std::make_shared<HelloWorldProcessor>(handler);
    auto serverTransport = std::make_shared<TServerSocket>(port);
    auto protocolFactory = std::make_shared<TJSONProtocolFactory>();
    auto transportFactory = std::make_shared<THttpServerTransportFactory>();

    auto processorFactory = std::make_shared<HelloWorldProcessorFactory>(std::make_shared<HelloWorldIfFactoryImpl>());

    TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();

    system("pause");
    return 0;
}
