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
#include "Map.h"

using namespace CppUnit;
using namespace std;
 
//-----------------------------------------------------------------------------
 
class TestCoordinate : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestCoordinate);
  CPPUNIT_TEST(testOnTheGrid);
  CPPUNIT_TEST_SUITE_END();
   
  public:
    void setUp(void);
    void tearDown(void);
   
  protected:
    void testOnTheGrid(void);
   
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
  // Value out of bounds
  MinValueDirection *mvd = mMap->minSurroundingNode(100, 100);
  CPPUNIT_ASSERT(! mvd->directionSet());
  CPPUNIT_ASSERT(RESET_MIN == mvd->getNodeValue());
}

void TestMap::testClear(void) {
}

void TestMap::testUnPropagate(void) {
}

void TestMap::testPropagateWavefront(void) {
}

void TestMap::testGridLocationFromCenterRadius(void) {
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
  ofstream xmlFileOut("cppTestBasicMathResults.xml");
  XmlOutputter xmlOut(&collectedresults, xmlFileOut);
  xmlOut.write();

  // return 0 if tests were successful
  return collectedresults.wasSuccessful() ? 0 : 1;
}  
