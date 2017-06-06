#include <iostream>
#include <cppunit/TestCase.h>
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
#include <boost/asio.hpp>

#include "../server/game_board.hpp"
#include "../server/ball.hpp"
#include "../server/player.hpp"
#include "../server/element.hpp"
#include "../server/session.hpp"


using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------

class TestGameBoard : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestGameBoard);
    CPPUNIT_TEST(testGetBall);
    CPPUNIT_TEST(testGetFood);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    virtual void testGetBall(void);
    virtual void testGetFood(void);

private:
   websocket::Ball *testBall_;
   websocket::FoodItem * testFoodItem_;
   websocket::GameBoard * testObj_;
   boost::asio::io_service io_service_;
   websocket::player_ptr testPlayerPtr_;
};

//------------------------------------------------------------

void TestGameBoard::testGetBall(void)
{
    CPPUNIT_ASSERT( testBall_->getRadius() == 0 );
    CPPUNIT_ASSERT( testBall_->getOwner() == nullptr );
    
}

void TestGameBoard::testGetFood(void)
{
    CPPUNIT_ASSERT( testBall_->getRadius() == 0 );
    CPPUNIT_ASSERT( testBall_->getOwner() == nullptr );
    testBall_->setOwner(testPlayerPtr_);
    CPPUNIT_ASSERT( testBall_->getOwner() != nullptr );
}

void TestGameBoard::setUp(void)
{
    double x=0;
    double y=0;
    double z = 0 ;

    testObj_  = new websocket::GameBoard();
    testBall_ = new websocket::Ball(x,y,z);
    testFoodItem_ = new websocket::FoodItem(x,y,z);
    testPlayerPtr_ = websocket::player_ptr(new websocket::Session(io_service_, *testObj_)) ;
}

void TestGameBoard::tearDown(void)
{
    delete testObj_;
    delete testBall_;
    delete testFoodItem_;
}

//----------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( TestGameBoard );

int main(int argc, char* argv[])
{
    CPPUNIT_NS::TestResult testresult;

    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML
    ofstream xmlFileOut("cppTestResults.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
