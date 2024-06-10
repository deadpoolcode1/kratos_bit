CXX = g++
CXXFLAGS = -std=c++11 -Wall -I/usr/include/jsoncpp
LDFLAGS = -ljsoncpp

all: TestServer TestClient

TestServer: TestServer.o
	$(CXX) -o TestServer TestServer.o $(LDFLAGS)

TestClient: TestClient.o
	$(CXX) -o TestClient TestClient.o $(LDFLAGS)

TestServer.o: TestServer.cpp
	$(CXX) $(CXXFLAGS) -c TestServer.cpp

TestClient.o: TestClient.cpp
	$(CXX) $(CXXFLAGS) -c TestClient.cpp

clean:
	rm -f *.o TestServer TestClient
