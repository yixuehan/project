#include "hello.grpc.pb.h"
#include "hello.pb.h"

#include <string>

#include <grpcpp/grpcpp.h>

using hello::Query;
using namespace std;
using namespace grpc;

class QueryImpl : public hello::Query::Service
{
    //virtual ::grpc::Status queryHello(::grpc::ServerContext* context, const ::hello::HelloRequest* request, ::hello::HelloReply* response);
    virtual ::grpc::Status queryHello(::grpc::ServerContext *context, const ::hello::HelloRequest *request, ::hello::HelloReply *reply) override
    {
        cout << request->name() << "\n";
        reply->set_name("hello " + request->name());
        return Status::OK;
    }
};

void runServer()
{
    std::string addr("127.0.0.1:41422");
    ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
    QueryImpl queryService;
    builder.RegisterService(&queryService);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "listening on port " << addr << "\n" ;
    server->Wait();
}

int main()
{
    runServer();
}
