#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "dummy_connector.h"

using namespace std;

namespace oro
{

	DummyConnector::DummyConnector() :_stmt_storage ( vector<Statement>() ) {}

	ServerResponse DummyConnector::execute ( const string query, const vector<server_param_types>& args )
	{

		cout<<"[request execution -> " << query << "]" << endl;

/*
		if ( query=="add" )
		{
			set<string>::const_iterator it;

			it = (boost::get<set<string> >(arg)).begin();
			while ( it != (boost::get<set<string> >(arg)).end() )
			{
				_stmt_storage.push_back ( Statement( *it ) );
				++it;
			}

		}
*/

		ServerResponse res;
		res.status = ServerResponse::ok;

		return res;
	}
	
	ServerResponse DummyConnector::execute(const string query, const server_param_types& arg)
	{

		cout<<"[request execution -> " << query << "]" << endl;

		ServerResponse res;
		res.status = ServerResponse::ok;

		return res;
	}

	ServerResponse DummyConnector::execute ( const string query )
	{

		ServerResponse res;
		res.status = ServerResponse::ok;

		return res;
	}

}
