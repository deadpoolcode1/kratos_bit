CXXFLAGS = -std=c++11 -Wall -Ilibs/jsoncpp/include
LDFLAGS = 

# Default output file names (can be overridden by environment variables)
OUTPUT_SERVER ?= TestServer_host
OUTPUT_CLIENT ?= TestClient_host

# Jsoncpp sources
JSONCPP_SRCS = $(wildcard libs/jsoncpp/src/lib_json/*.cpp)

# Object files
OBJ_SERVER = main.o TestServer.o $(JSONCPP_SRCS:.cpp=.o)
OBJ_CLIENT = TestClient.o $(JSONCPP_SRCS:.cpp=.o)

all: $(OUTPUT_SERVER) $(OUTPUT_CLIENT)

$(OUTPUT_SERVER): $(OBJ_SERVER)
	$(CXX) -o $(OUTPUT_SERVER) $(OBJ_SERVER) $(LDFLAGS)

$(OUTPUT_CLIENT): $(OBJ_CLIENT)
	$(CXX) -o $(OUTPUT_CLIENT) $(OBJ_CLIENT) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

TestServer.o: TestServer.cpp TestServer.h
	$(CXX) $(CXXFLAGS) -c TestServer.cpp

TestClient.o: TestClient.cpp
	$(CXX) $(CXXFLAGS) -c TestClient.cpp

# Pattern rule for jsoncpp objects
libs/jsoncpp/src/lib_json/%.o: libs/jsoncpp/src/lib_json/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OUTPUT_SERVER) $(OUTPUT_CLIENT) libs/jsoncpp/src/lib_json/*.o
