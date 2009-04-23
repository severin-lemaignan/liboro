#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "dummy_connector.h"

using namespace std;

namespace oro
{

	DummyConnector::DummyConnector() :_stmt_storage ( vector<Statement>() ) {}

	ServerResponse DummyConnector::execute ( const std::string query, const std::vector<std::string>& args )
	{

		std::cout<<"[request execution -> " << query << "(";
		copy ( args.begin(), args.end(), ostream_iterator<string> ( cout, ", " ) );
		std::cout << ")]" << std::endl;

		if ( query=="add" )
		{
			vector<string>::const_iterator it;

			it = args.begin();
			while ( it != args.end() )
			{
				_stmt_storage.push_back ( Statement::create ( *it ) );
				++it;
			}

		}


		ServerResponse res;
		res.status = ServerResponse::ok;

		return res;
	}

	ServerResponse DummyConnector::execute ( const std::string query )
	{

		ServerResponse res;
		res.status = ServerResponse::ok;

		return res;
	}

}
