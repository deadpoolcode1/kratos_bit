CXX = g++
CXXFLAGS = -std=c++11 -Wall -I/usr/include/jsoncpp
LDFLAGS = -ljsoncpp

all: TestServer TestClient

TestServer: main.o TestServer.o
	$(CXX) -o TestServer main.o TestServer.o $(LDFLAGS)

TestClient: TestClient.o
	$(CXX) -o TestClient TestClient.o $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

TestServer.o: TestServer.cpp TestServer.h
	$(CXX) $(CXXFLAGS) -c TestServer.cpp

TestClient.o: TestClient.cpp
	$(CXX) $(CXXFLAGS) -c TestClient.cpp

clean:
	rm -f *.o TestServer TestClient
