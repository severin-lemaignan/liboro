/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
*/

#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>

#include "liboro.h"

using namespace yarp::os;
using namespace std;

namespace openrobots {

	const char* OK = "ok";
	const char* ERROR = "error";

/**
 * The YarpConnector class does actually send and recieve YARP messages.
 *
 * The structure of these messages is based on YARP bottles. *
 * This lib send that kind of message to the server:
 * \verbatim
 * ([YARP port for answering] [name of the method] ([param1] [param2] [...]))
 * \endverbatim
 * Parameters are enclosed in a nested bottle (a list), and these parameters can themselves be lists.
 * Currently, these list are casted to vectors of string.
 *
 * The server answer a result bottle of this kind:
 * \verbatim
 * ([ok|error] [result|error msg])
 * \endverbatim
 * Result is a list (a nested bottle) of objects.
 */
class YarpConnector {
	private:

		// Create two ports that we'll be using to transmit "Bottle" objects.
		// The ports are buffered, so that sending and receiving can happen
		// in the background.
		BufferedPort<Bottle> in;
		BufferedPort<Bottle> out;

		string in_port;
		string out_port;

		string oro_server;

	public:


		YarpConnector(const string port_name, const string oro_in_port_name){

			// Initialize YARP - some OSes need network and time service initialization
			Network yarp_network;

			// Work locally - don't rely on name server (just for this example).
			// If you have a YARP name server running, you can remove this line.
			//Network::setLocalMode(true);


			// we will want to read every message, with no skipping of "old" messages
			// when new ones come in
			in.setStrict();


			// Name the ports
			oro_server = oro_in_port_name;

			in_port = "/" + port_name + "_in";
			out_port = "/" + port_name + "_out";
			in.open( in_port.c_str() );
			out.open( out_port.c_str());

			// Connect the ports so that anything written from /out arrives to /in
			Network::connect(out_port.c_str() ,("/" + oro_in_port_name).c_str() );

			// Connect the ports so that anything written from /out arrives to /in
			Network::connect(("/" + oro_in_port_name + "_out").c_str(), in_port.c_str());

		}

		void execute(string query, string args){
			// Send one "Bottle" object.  The port is responsible for creating
			// and reusing/destroying that object, since it needs to be sure
			// it exists until communication to all recipients (just one in
			// this case) is complete.

			Bottle& outBot = out.prepare();   // Get the object
			outBot.clear();
			outBot.fromString((in_port + " " + query).c_str()); //Prepend the query with the port name where the server should send back the result.
			Bottle& argsBot = outBot.addList();
			argsBot.fromString(args.c_str());

			printf("Writing bottle (%s)\n", outBot.toString().c_str());

			out.write();                       // Now send it on its way

		}

		void execute(string query, Bottle& args){
			// Send one "Bottle" object.  The port is responsible for creating
			// and reusing/destroying that object, since it needs to be sure
			// it exists until communication to all recipients (just one in
			// this case) is complete.

			Bottle& outBot = out.prepare();   // Get the object
			outBot.clear();
			outBot.fromString((in_port + " " + query).c_str()); //Prepend the query with the port name where the server should send back the result.
			Bottle& argsBot = outBot.addList();
			argsBot.append(args);

			printf("Writing bottle (%s)\n", outBot.toString().c_str());

			out.write();                       // Now send it on its way

		}


		void read(Bottle& inBot){

			 Bottle *rawResult = in.read();

			 if (!rawResult->get(0).isString() || !(rawResult->get(0).asString() == OK || rawResult->get(0).asString() == "error")) throw OntologyServerException("The server answer should start with \"ok\" or \"error\"");

			 if (rawResult->get(0).asString() == ERROR)
				 throw OntologyServerException(rawResult->get(1).asString());

			 inBot = *(rawResult->pop().asList());

			 //printf("Bottle recieved is: %s\n", inBot->toString().c_str());
		}

		void vectorToBottle(const vector<string>& data, Bottle& bottle)
		{
			string tmp;

			for(unsigned int i = 0 ; i < data.size() ; i++)
		    {
				tmp = data.at(i).c_str();

				//if (data.at(i).find(" ") != string::npos) tmp = "\"" + tmp + "\"";

				bottle.addString(tmp.c_str());

		    }
		  }


		virtual ~YarpConnector()
		{


			Network::disconnect(out_port.c_str() ,("/" + oro_server).c_str() );
			Network::disconnect(("/" + oro_server + "_out").c_str(), in_port.c_str());

			in.close();
			out.close();

		}

	};



	Oro::Oro(const string port_name, const string oro_in_port_name) {

		yarp = new YarpConnector(port_name, oro_in_port_name);

	}

	Oro::~Oro()
	{
		delete yarp;
	}

	bool Oro::add(const string& statement)
	{
		Bottle inBot;

		yarp->execute("add", "\"" + statement + "\"");

		yarp->read(inBot);

		if (inBot.pop().asString() == "true") return true;
		else return false;
	}

	bool Oro::add(const vector<string>& statements)
	{
		Bottle inBot;
		Bottle args;

		yarp->vectorToBottle(statements, args.addList());

		yarp->execute("addMultiple", args);

		yarp->read(inBot);




		if (inBot.pop().asString() == "true") return true;
				else return false;
	}

	int Oro::find(const string& resource, const vector<string>& partial_statements, const vector<string>& restrictions, vector<string>& result)
	{
		Bottle inBot;
		Bottle args;

		args.addString(resource.c_str());
		yarp->vectorToBottle(partial_statements, args.addList());
		if (restrictions.size() > 0) yarp->vectorToBottle(restrictions, args.addList());

		yarp->execute("filteredFind", args);

		yarp->read(inBot);


		pourBottle(*(inBot.pop().asList()), result);

		return 0;
	}

	int Oro::find(const string& resource, const vector<string>& partial_statements, vector<string>& result)
	{
		vector<string> restrictions;
		return Oro::find(resource, partial_statements, restrictions, result);
	}

	int Oro::getInfos(const string& resource, vector<string>& result)
	{
		Bottle inBot;

		yarp->execute("getInfos", resource);

		yarp->read(inBot);

		pourBottle(inBot, result);

		return 0;
	}


	int Oro::getHumanReadableInfos(const string& resource, vector<string>& result)
	{
		Bottle inBot;

		yarp->execute("getHumanReadableInfos", resource);

		yarp->read(inBot);

		pourBottle(inBot, result);

		return 0;
	}

	int Oro::guess(const string& resource, const double threshold, const vector<string>& partial_statements, vector<string>& result)
	{
		Bottle inBot;
		Bottle args;

		args.addString(resource.c_str());
		args.addDouble(threshold);
		yarp->vectorToBottle(partial_statements, args.addList());

		yarp->execute("guess", args);

		yarp->read(inBot);


		pourBottle(*(inBot.pop().asList()), result);

		return 0;
	}

	int Oro::query(const string& var_name, const string& query, vector<string>& result)
	{
		Bottle inBot;

		yarp->execute("query", var_name + " \"" + query + "\"");

		yarp->read(inBot);

		pourBottle(inBot, result);

		return 0;
	}

	int Oro::queryAsXML(const string& query, string& result)
	{
		Bottle inBot;

		yarp->execute("queryAsXML", "\"" + query + "\"");

		yarp->read(inBot);

		result = inBot.pop().asString().c_str();

		return 0;
	}

	int Oro::test(const string& test, string& result)
	{
		Bottle inBot;

		yarp->execute("test", test);

		yarp->read(inBot);

		result = inBot.pop().asString().c_str();

		return 0;
	}

	void Oro::pourBottle(Bottle& bottle, vector<string>& result)
	{
		result.clear();

		while (bottle.size() > 0)
		{
			string tmp = bottle.pop().asString().c_str();
			result.push_back(tmp);
		}
	}



}


