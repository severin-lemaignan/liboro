#ifndef DUMMY_CONNECTOR_H_
#define DUMMY_CONNECTOR_H_

#include <vector>
#include <string>

#include "oro.h"
#include "oro_connector.h"


namespace oro
{
	
/**
 * This class defines a dummy connector for the ontology server. It doesn't actually connect to any ontology. For debugging purposes. 
 */
class DummyConnector : public IConnector {

	public:
		DummyConnector();
		
		ServerResponse execute(const std::string query, const std::vector<server_param_types>& args);
		ServerResponse execute(const std::string query, const server_param_types& arg);
		ServerResponse execute(const std::string query);
			
	private:
		std::vector<Statement> _stmt_storage;
};
}

#endif /* DUMMY_CONNECTOR_H_ */
