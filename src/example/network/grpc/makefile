CXX:=g++ -std=c++1z -static
LDFLAGS:=`pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lssl -lcrypto -ldl -lz
all: hello.grpc.pb.cc hello.pb.cc hello_server hello_client

hello_client:hello_client.cpp hello.grpc.pb.cc hello.pb.cc
	$(CXX) $^ $(LDFLAGS) -o $@

hello_server:hello_server.cpp hello.grpc.pb.cc hello.pb.cc
	$(CXX) $^ $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES:.grpc.pb.cc .pb.cc

%.grpc.pb.cc:%.proto
	protoc -I. --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin $<

%.pb.cc:%.proto
	protoc -I. --cpp_out=. $<

clean:
	rm -f hello_client hello_server 
