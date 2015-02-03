#include <Arduino.h>
#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "Coordinate.h"
#include "MinValueDirection.h"
#include "IWavefront.h"
#include "Map.h"

/**
 * Uncomment this if you want to dump the map for each
 * iteration of the wavefront. Very handy for debugging
 */
// #define DUMP_WAVEFRONT

using namespace CppUnit;
using namespace std;

class WaveFrontFormatter : public IWavefront {
  public:
    void wave(Map& map);

  private:
    char *printCellValue(char *str, int cellValue);
};

void WaveFrontFormatter::wave(Map& map) {
  // Each time we come in here, we need to print out the entire map with the current
  // values at each cell. For now, we don't print out NOTHING values.
  char buffer[20];
  cout << endl;
  for (int x=0; x<10; x++) {
    for (int y=0; y<10; y++) {
      cout << printCellValue(buffer, map.getValue(x, y)) << " ";
    }
    cout << endl;
  }
  cout << endl << endl;
}

char *WaveFrontFormatter::printCellValue(char *str, int cellValue) {
  if (cellValue == NOTHING) {
    sprintf(str, "%s", "   "); // Just three spaces
  } else if (cellValue == ROBOT) {
    sprintf(str, " %s ", "R"); // Just a centered 'R'
  } else if (cellValue == GOAL) {
    sprintf(str, " %s ", "G"); // Just a centered 'G'
  } else if (cellValue == WALL) {
    sprintf(str, " %s ", "W"); // Just a centered 'W'
  } else {
    sprintf(str, "%03d", cellValue);
  }

  return str;
}
 
//-----------------------------------------------------------------------------
 
class TestCoordinate : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestCoordinate);
  CPPUNIT_TEST(testOnTheGrid);
  CPPUNIT_TEST(testDefaultConstructor);
  CPPUNIT_TEST(testSetCoordinates);
  CPPUNIT_TEST(testGetValues);
  CPPUNIT_TEST_SUITE_END();
   
  public:
    void setUp(void);
    void tearDown(void);
   
  protected:
    void testOnTheGrid(void);
    void testDefaultConstructor(void);
    void testSetCoordinates(void);
    void testGetValues(void);
   
  private:
   
    Coordinate *mCoordinate;
};

class TestMinValueDirection : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestMinValueDirection);
  CPPUNIT_TEST(testDirectionSet);
  CPPUNIT_TEST(testGetNodeValue);
  CPPUNIT_TEST(testGetDirection);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp(void);
    void tearDown(void);

  protected:
    void testDirectionSet(void);
    void testGetNodeValue(void);
    void testGetDirection(void);

  private:
    MinValueDirection *mMvd;
};

class TestMap : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestMap);
  CPPUNIT_TEST(testGetSizeX);
  CPPUNIT_TEST(testGetSizeY);
  CPPUNIT_TEST(testPlaceValue);
  CPPUNIT_TEST(testMinSurroundingNode);
  CPPUNIT_TEST(testNodeLessThanMinimum);
  CPPUNIT_TEST(testCoordinateInRange);
  CPPUNIT_TEST(testClear);
  CPPUNIT_TEST(testUnPropagate);
  CPPUNIT_TEST(testPropagateWavefront);
  CPPUNIT_TEST(testGridLocationFromCenterRadius);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp(void);
    void tearDown(void);

  protected:
    void testGetSizeX(void);
    void testGetSizeY(void);
    void testPlaceValue(void);
    void testMinSurroundingNode(void);
    void testNodeLessThanMinimum(void);
    void testCoordinateInRange(void);
    void testClear(void);
    void testUnPropagate(void);
    void testPropagateWavefront(void);
    void testGridLocationFromCenterRadius(void);

  private:
    Map *mMap;
};

//-----------------------------------------------------------------------------
 
void TestCoordinate::testOnTheGrid(void) {
  CPPUNIT_ASSERT(mCoordinate->onTheGrid(10,10));
  CPPUNIT_ASSERT(! mCoordinate->onTheGrid(5,10));
  CPPUNIT_ASSERT(! mCoordinate->onTheGrid(10,5));
  CPPUNIT_ASSERT(! mCoordinate->onTheGrid(5,5));
}


void TestCoordinate::setUp(void) {
  mCoordinate = new Coordinate(5, 5);
}
 
void TestCoordinate::tearDown(void) {
  delete mCoordinate;
}

void TestCoordinate::testDefaultConstructor(void) {
  Coordinate c;
  CPPUNIT_ASSERT(0 == c.getX());
  CPPUNIT_ASSERT(0 == c.getY());
}

void TestCoordinate::testSetCoordinates(void) {
  CPPUNIT_ASSERT(5 == mCoordinate->getX());
  CPPUNIT_ASSERT(5 == mCoordinate->getY());
  mCoordinate->setCoordinates(82, 13);
  CPPUNIT_ASSERT(82 == mCoordinate->getX());
  CPPUNIT_ASSERT(13 == mCoordinate->getY());
}

void TestCoordinate::testGetValues(void) {
  CPPUNIT_ASSERT(5 == mCoordinate->getX());
  CPPUNIT_ASSERT(5 == mCoordinate->getY());
}

void TestMinValueDirection::testDirectionSet(void) {
  CPPUNIT_ASSERT(! mMvd->directionSet());
  mMvd->setDirection(1);
  CPPUNIT_ASSERT(mMvd->directionSet());
}

void TestMinValueDirection::testGetNodeValue(void) {
  CPPUNIT_ASSERT(1 == mMvd->getNodeValue());
  mMvd->setNodeValue(85);
  CPPUNIT_ASSERT(85 == mMvd->getNodeValue());
}

void TestMinValueDirection::testGetDirection(void) {
  CPPUNIT_ASSERT(0 == mMvd->getDirection());
  mMvd->setDirection(3);
  CPPUNIT_ASSERT(3 == mMvd->getDirection());
}

void TestMinValueDirection::setUp(void) {
  mMvd = new MinValueDirection(1, 0);
}
 
void TestMinValueDirection::tearDown(void) {
  delete mMvd;
}

void TestMap::testGetSizeX(void) {
  CPPUNIT_ASSERT(10 == mMap->getSizeX());
}

void TestMap::testGetSizeY(void) {
  CPPUNIT_ASSERT(10 == mMap->getSizeY());
}

void TestMap::testPlaceValue(void) {
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(5, 5));
  mMap->placeValue(5, 5, 12);
  CPPUNIT_ASSERT(12 == mMap->getValue(5, 5));
  mMap->placeValue(100, 100, 88);
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(100, 100));
}

void TestMap::testMinSurroundingNode(void) {
  mMap->placeValue(0, 6, 8);

  MinValueDirection mvd(RESET_MIN, NOTHING);

  mMap->placeValue(1, 5, 8);
  mMap->minSurroundingNode(0, 5, mvd);
  CPPUNIT_ASSERT(mvd.directionSet());
  CPPUNIT_ASSERT(DOWN == mvd.getDirection());
  CPPUNIT_ASSERT(8 == mvd.getNodeValue());
  mMap->placeValue(5, 5, 12);

  // Coordinate out of bounds
  mMap->minSurroundingNode(100, 100, mvd);
  CPPUNIT_ASSERT(! mvd.directionSet());
  CPPUNIT_ASSERT(RESET_MIN == mvd.getNodeValue());

  mMap->minSurroundingNode(4, 5, mvd);
  CPPUNIT_ASSERT(mvd.directionSet());
  CPPUNIT_ASSERT(DOWN == mvd.getDirection());
  CPPUNIT_ASSERT(12 == mvd.getNodeValue());

  mMap->minSurroundingNode(6, 5, mvd);
  CPPUNIT_ASSERT(mvd.directionSet());
  CPPUNIT_ASSERT(UP == mvd.getDirection());
  CPPUNIT_ASSERT(12 == mvd.getNodeValue());

  mMap->minSurroundingNode(5, 6, mvd);
  CPPUNIT_ASSERT(mvd.directionSet());
  CPPUNIT_ASSERT(LEFT == mvd.getDirection());
  CPPUNIT_ASSERT(12 == mvd.getNodeValue());

}

void TestMap::testCoordinateInRange(void) {
  CPPUNIT_ASSERT(mMap->coordinateInRange(5, 5));
  CPPUNIT_ASSERT(! mMap->coordinateInRange(100, 100));
}

void TestMap::testNodeLessThanMinimum(void) {
  CPPUNIT_ASSERT(! mMap->nodeLessThanMinimum(100, 100, 8)); // out of range
  CPPUNIT_ASSERT(! mMap->nodeLessThanMinimum(5, 5, 8)); 
  mMap->placeValue(5, 5, 12);
  CPPUNIT_ASSERT(mMap->nodeLessThanMinimum(5, 5, 13)); 
  CPPUNIT_ASSERT(! mMap->nodeLessThanMinimum(5, 5, 12)); 
  CPPUNIT_ASSERT(! mMap->nodeLessThanMinimum(5, 5, 11)); 
}

void TestMap::testClear(void) {
  mMap->placeValue(1, 1, NOTHING);
  mMap->placeValue(1, 2, ROBOT);
  mMap->placeValue(1, 3, GOAL);
  mMap->placeValue(1, 4, 23);
  mMap->placeValue(1, 5, WALL);

  mMap->clear();

  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 1));
  CPPUNIT_ASSERT(ROBOT == mMap->getValue(1, 2));
  CPPUNIT_ASSERT(GOAL == mMap->getValue(1, 3));
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 4));
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 5));
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 6));
}

void TestMap::testUnPropagate(void) {
  mMap->placeValue(1, 1, NOTHING);
  mMap->placeValue(1, 2, ROBOT);
  mMap->placeValue(1, 3, GOAL);
  mMap->placeValue(1, 4, 23);
  mMap->placeValue(1, 5, WALL);

  mMap->unpropagate();

  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 1));
  CPPUNIT_ASSERT(ROBOT == mMap->getValue(1, 2));
  CPPUNIT_ASSERT(GOAL == mMap->getValue(1, 3));
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 4));
  CPPUNIT_ASSERT(WALL == mMap->getValue(1, 5));
  CPPUNIT_ASSERT(NOTHING == mMap->getValue(1, 6));
}

void TestMap::testPropagateWavefront(void) {
  /*
   * Robot on one edge, goal at the opposite edge
   * with a wall in-between.
   */
  mMap->placeValue(4, 0, ROBOT);
  mMap->placeValue(4, 9, GOAL);
  mMap->placeValue(3, 4, WALL);
  mMap->placeValue(4, 4, WALL);
  mMap->placeValue(5, 4, WALL);

#ifdef DUMP_WAVEFRONT
  WaveFrontFormatter wff;

  CPPUNIT_ASSERT(DOWN == mMap->propagateWavefront(&wff));
#else
  CPPUNIT_ASSERT(DOWN == mMap->propagateWavefront(NULL));
#endif

  CPPUNIT_ASSERT(5 == mMap->getValue(0, 9));
  CPPUNIT_ASSERT(4 == mMap->getValue(1, 9));
  CPPUNIT_ASSERT(3 == mMap->getValue(2, 9));
  CPPUNIT_ASSERT(2 == mMap->getValue(3, 9));
  CPPUNIT_ASSERT(GOAL == mMap->getValue(4, 9));
  CPPUNIT_ASSERT(2 == mMap->getValue(5, 9));
  CPPUNIT_ASSERT(3 == mMap->getValue(6, 9));
  CPPUNIT_ASSERT(4 == mMap->getValue(7, 9));
  CPPUNIT_ASSERT(5 == mMap->getValue(8, 9));
  CPPUNIT_ASSERT(6 == mMap->getValue(9, 9));

  CPPUNIT_ASSERT(6 == mMap->getValue(0, 8));
  CPPUNIT_ASSERT(5 == mMap->getValue(1, 8));
  CPPUNIT_ASSERT(4 == mMap->getValue(2, 8));
  CPPUNIT_ASSERT(3 == mMap->getValue(3, 8));
  CPPUNIT_ASSERT(2 == mMap->getValue(4, 8));
  CPPUNIT_ASSERT(3 == mMap->getValue(5, 8));
  CPPUNIT_ASSERT(4 == mMap->getValue(6, 8));
  CPPUNIT_ASSERT(5 == mMap->getValue(7, 8));
  CPPUNIT_ASSERT(6 == mMap->getValue(8, 8));
  CPPUNIT_ASSERT(7 == mMap->getValue(9, 8));

  CPPUNIT_ASSERT(7 == mMap->getValue(0, 7));
  CPPUNIT_ASSERT(6 == mMap->getValue(1, 7));
  CPPUNIT_ASSERT(5 == mMap->getValue(2, 7));
  CPPUNIT_ASSERT(4 == mMap->getValue(3, 7));
  CPPUNIT_ASSERT(3 == mMap->getValue(4, 7));
  CPPUNIT_ASSERT(4 == mMap->getValue(5, 7));
  CPPUNIT_ASSERT(5 == mMap->getValue(6, 7));
  CPPUNIT_ASSERT(6 == mMap->getValue(7, 7));
  CPPUNIT_ASSERT(7 == mMap->getValue(8, 7));
  CPPUNIT_ASSERT(8 == mMap->getValue(9, 7));

  CPPUNIT_ASSERT(8 == mMap->getValue(0, 6));
  CPPUNIT_ASSERT(7 == mMap->getValue(1, 6));
  CPPUNIT_ASSERT(6 == mMap->getValue(2, 6));
  CPPUNIT_ASSERT(5 == mMap->getValue(3, 6));
  CPPUNIT_ASSERT(4 == mMap->getValue(4, 6));
  CPPUNIT_ASSERT(5 == mMap->getValue(5, 6));
  CPPUNIT_ASSERT(6 == mMap->getValue(6, 6));
  CPPUNIT_ASSERT(7 == mMap->getValue(7, 6));
  CPPUNIT_ASSERT(8 == mMap->getValue(8, 6));
  CPPUNIT_ASSERT(9 == mMap->getValue(9, 6));

  CPPUNIT_ASSERT(9 == mMap->getValue(0, 5));
  CPPUNIT_ASSERT(8 == mMap->getValue(1, 5));
  CPPUNIT_ASSERT(7 == mMap->getValue(2, 5));
  CPPUNIT_ASSERT(6 == mMap->getValue(3, 5));
  CPPUNIT_ASSERT(5 == mMap->getValue(4, 5));
  CPPUNIT_ASSERT(6 == mMap->getValue(5, 5));
  CPPUNIT_ASSERT(7 == mMap->getValue(6, 5));
  CPPUNIT_ASSERT(8 == mMap->getValue(7, 5));
  CPPUNIT_ASSERT(9 == mMap->getValue(8, 5));
  CPPUNIT_ASSERT(10 == mMap->getValue(9, 5));

  CPPUNIT_ASSERT(10 == mMap->getValue(0, 4));
  CPPUNIT_ASSERT(9 == mMap->getValue(1, 4));
  CPPUNIT_ASSERT(8 == mMap->getValue(2, 4));
  CPPUNIT_ASSERT(WALL == mMap->getValue(3, 4));
  CPPUNIT_ASSERT(WALL == mMap->getValue(4, 4));
  CPPUNIT_ASSERT(WALL == mMap->getValue(5, 4));
  CPPUNIT_ASSERT(8 == mMap->getValue(6, 4));
  CPPUNIT_ASSERT(9 == mMap->getValue(7, 4));
  CPPUNIT_ASSERT(10 == mMap->getValue(8, 4));
  CPPUNIT_ASSERT(11 == mMap->getValue(9, 4));

  CPPUNIT_ASSERT(11 == mMap->getValue(0, 3));
  CPPUNIT_ASSERT(10 == mMap->getValue(1, 3));
  CPPUNIT_ASSERT(9 == mMap->getValue(2, 3));
  CPPUNIT_ASSERT(10 == mMap->getValue(3, 3));
  CPPUNIT_ASSERT(11 == mMap->getValue(4, 3));
  CPPUNIT_ASSERT(10 == mMap->getValue(5, 3));
  CPPUNIT_ASSERT(9 == mMap->getValue(6, 3));
  CPPUNIT_ASSERT(10 == mMap->getValue(7, 3));
  CPPUNIT_ASSERT(11 == mMap->getValue(8, 3));
  CPPUNIT_ASSERT(12 == mMap->getValue(9, 3));

  CPPUNIT_ASSERT(12 == mMap->getValue(0, 2));
  CPPUNIT_ASSERT(11 == mMap->getValue(1, 2));
  CPPUNIT_ASSERT(10 == mMap->getValue(2, 2));
  CPPUNIT_ASSERT(11 == mMap->getValue(3, 2));
  CPPUNIT_ASSERT(12 == mMap->getValue(4, 2));
  CPPUNIT_ASSERT(11 == mMap->getValue(5, 2));
  CPPUNIT_ASSERT(10 == mMap->getValue(6, 2));
  CPPUNIT_ASSERT(11 == mMap->getValue(7, 2));
  CPPUNIT_ASSERT(12 == mMap->getValue(8, 2));
  CPPUNIT_ASSERT(13 == mMap->getValue(9, 2));

  CPPUNIT_ASSERT(0 == mMap->getValue(0, 1));
  CPPUNIT_ASSERT(12 == mMap->getValue(1, 1));
  CPPUNIT_ASSERT(11 == mMap->getValue(2, 1));
  CPPUNIT_ASSERT(12 == mMap->getValue(3, 1));
  CPPUNIT_ASSERT(13 == mMap->getValue(4, 1));
  CPPUNIT_ASSERT(12 == mMap->getValue(5, 1));
  CPPUNIT_ASSERT(11 == mMap->getValue(6, 1));
  CPPUNIT_ASSERT(12 == mMap->getValue(7, 1));
  CPPUNIT_ASSERT(13 == mMap->getValue(8, 1));
  CPPUNIT_ASSERT(14 == mMap->getValue(9, 1));

  CPPUNIT_ASSERT(0 == mMap->getValue(0, 0));
  CPPUNIT_ASSERT(0 == mMap->getValue(1, 0));
  CPPUNIT_ASSERT(12 == mMap->getValue(2, 0));
  CPPUNIT_ASSERT(13 == mMap->getValue(3, 0));
  CPPUNIT_ASSERT(ROBOT == mMap->getValue(4, 0));
  CPPUNIT_ASSERT(13 == mMap->getValue(5, 0));
  CPPUNIT_ASSERT(12 == mMap->getValue(6, 0));
  CPPUNIT_ASSERT(13 == mMap->getValue(7, 0));
  CPPUNIT_ASSERT(14 == mMap->getValue(8, 0));
  CPPUNIT_ASSERT(15 == mMap->getValue(9, 0));
}

void TestMap::testGridLocationFromCenterRadius(void) {
  Coordinate coord;
  for (int angle=0; angle<360; angle += 30) {
    mMap->gridLocationFromCenterRadius(2, 0, angle, 33.0 + 17.0, coord);
    switch (angle) {
      case 0:
        CPPUNIT_ASSERT(4 == coord.getX());
        CPPUNIT_ASSERT(0 == coord.getY());
        break;
      case 30:
        CPPUNIT_ASSERT(3 == coord.getX());
        CPPUNIT_ASSERT(1 == coord.getY());
        break;
      case 60:
        CPPUNIT_ASSERT(3 == coord.getX());
        CPPUNIT_ASSERT(1 == coord.getY());
        break;
      case 90:
        CPPUNIT_ASSERT(2 == coord.getX());
        CPPUNIT_ASSERT(2 == coord.getY());
        break;
      case 120:
        CPPUNIT_ASSERT(1 == coord.getX());
        CPPUNIT_ASSERT(1 == coord.getY());
        break;
      case 150:
        CPPUNIT_ASSERT(1 == coord.getX());
        CPPUNIT_ASSERT(1 == coord.getY());
        break;
      case 180:
        CPPUNIT_ASSERT(0 == coord.getX());
        CPPUNIT_ASSERT(0 == coord.getY());
        break;
      case 210:
        CPPUNIT_ASSERT(1 == coord.getX());
        CPPUNIT_ASSERT(coord.getY() & 0xff);
        break;
      case 240:
        CPPUNIT_ASSERT(1 == coord.getX());
        CPPUNIT_ASSERT(coord.getY() & 0xff);
        break;
      case 270:
        CPPUNIT_ASSERT(2 == coord.getX());
        CPPUNIT_ASSERT(coord.getY() & 0xff);
        break;
      case 300:
        CPPUNIT_ASSERT(3 == coord.getX());
        CPPUNIT_ASSERT(coord.getY() & 0xff);
        break;
      case 330:
        CPPUNIT_ASSERT(3 == coord.getX());
        CPPUNIT_ASSERT(coord.getY() & 0xff);
        break;
    }
  }
}

void TestMap::setUp(void) {
  mMap = new Map();
}
 
void TestMap::tearDown(void) {
  delete mMap;
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( TestCoordinate );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMinValueDirection );
CPPUNIT_TEST_SUITE_REGISTRATION( TestMap );

int main(int argc, char* argv[]) {
  // informs test-listener about testresults
  CPPUNIT_NS::TestResult testresult;

  // register listener for collecting the test-results
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  // register listener for per-test progress output
  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  // insert test-suite at test-runner by registry
  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
  testrunner.run(testresult);

  // output results in compiler-format
  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write ();

  // Output XML for Jenkins CPPunit plugin
  //ofstream xmlFileOut("cppTestBasicMathResults.xml");
  //XmlOutputter xmlOut(&collectedresults, xmlFileOut);
  //xmlOut.write();

  // return 0 if tests were successful
  return collectedresults.wasSuccessful() ? 0 : 1;
}  
