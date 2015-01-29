CXX = g++
INCLUDES = -I . -I ../lib/Wavefront
CXXFLAGS = -g $(INCLUDES) -std=gnu++11
# SRCM = ../Wavefront/lib/Wavefront/Coordinate.cpp
OBJM = Coordinate.o MinValueDirection.o Map.o
LINKFLAGS = -lcppunit

testcoordinate: TestCoordinate.cpp $(OBJM)
	$(CXX) $(CXXFLAGS) -o $@ TestCoordinate.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)


Coordinate.o: ../lib/Wavefront/Coordinate.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

MinValueDirection.o: ../lib/Wavefront/MinValueDirection.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Map.o: ../lib/Wavefront/Map.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Default Compile
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
