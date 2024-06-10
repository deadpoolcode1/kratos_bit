all: main.cpp
	$(CXX) $(CXXFLAGS) -Og main.cpp -g -o ${OUTPUT} -lpthread  
clean:
	rm -f ${OUTPUT}