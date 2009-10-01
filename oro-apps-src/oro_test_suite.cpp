// To be benchmarked, the ontology server should be started with "oro_bench.conf" configuration file.

#include <string>
#include <iostream>
#include <iterator>
#include <map>

#include "oro.h"
#include "oro_library.h"
#include "oro_connector.h"
#include "socket_connector.h"

#include <boost/test/unit_test.hpp>
#define BOOST_TEST_MODULE LiboroTest


using namespace std;


using namespace oro;

int add(int a, int b){
	return a + b;
}
		

BOOST_AUTO_TEST_CASE( simple_assertions )
{
    // seven ways to detect and report the same error:
    BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error

    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error

    if( add( 2,2 ) != 4 )
      BOOST_ERROR( "Ouch..." );            // #3 continues on error

    if( add( 2,2 ) != 4 )
      BOOST_FAIL( "Ouch..." );             // #4 throws on error

    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error

    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
                         "add(..) result: " << add( 2,2 ) );

    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error

}


