#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "hello.pb.h"
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using hello::HelloRequest;
using hello::HelloReply;
using namespace hello;

class HelloClient
{
public:
    HelloClient(std::shared_ptr<Channel> channel)
        : _stub(Query::NewStub(channel))
    {
    }
    std::string sayHello()
    {
        HelloRequest req;
        req.set_name("lalala");
        ClientContext context;
        HelloReply rep;
        Status status = _stub->queryHello(&context, req, &rep);
        if(status.ok()) {
            return rep.name();
        }
        return "error";
    }


private:
    std::unique_ptr<Query::Stub> _stub;

};

int main()
{
    HelloClient client(grpc::CreateChannel("localhost:41422", grpc::InsecureChannelCredentials()));
    std::cout << client.sayHello() << "\n";
}
