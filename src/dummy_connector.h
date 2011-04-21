
/*
 * Copyright (c) 2008-2010 LAAS-CNRS Séverin Lemaignan slemaign@laas.fr
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

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

				ServerResponse execute(const std::string& query,
									   const std::vector<server_param_types>& args,
									   bool waitForAck = true);
				ServerResponse execute(const std::string& query,
									   const server_param_types& arg,
									   bool waitForAck = true);
				ServerResponse execute(const std::string& query,
									   bool waitForAck = true);


                virtual void setEventCallback(
                                void (*evtCallback)(const std::string& event_id,
                                                                        const server_return_types& raw_event_content)
                                ) {};
			
	private:
		std::vector<Statement> _stmt_storage;
};
}

#endif /* DUMMY_CONNECTOR_H_ */
