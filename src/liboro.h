/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
 *
 * The documentation is available on
 * http://www.laas.fr/~slemaign/onto/doc/laas/openrobots/ontology/connectors/YarpConnector.html
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

 #ifndef LIBORO_H_
#define LIBORO_H_

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

namespace yarp {
	namespace os{
		class Bottle;
	}
}

namespace openrobots {

class YarpConnector;

/**
 * This class defines which methods are open to <a href="http://eris.liralab.it/yarp/">YARP</a> requests.<br/>
 * All methods listed here should be implemented by compliant clients. For C++, <pre><a href="https://www.laas.fr/~slemaign/wiki/doku.php?id=liboro">liboro</a></pre> does the job.</br>
 * You'll find below C++ code samples using <pre>liboro</pre>.
 */
class Oro {

	public:
		Oro(const std::string port_name, const std::string oro_in_port_name);
		virtual ~Oro();

		/**
		 * Adds a new statement to the ontology.
		 * YARP interface to {@link laas.openrobots.ontology.OpenRobotsOntology#add(String)}. Please follow the link for details.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		oro.add("gorilla rdf:type Monkey");
		 * 		oro.add("gorilla age 12^^xsd:int");
		 * 		oro.add("gorilla weight 75.2");
		 *
		 * 		return 0;
		 * }
		 * </pre>
		 * @throws IllegalStatementException
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#add(String)
		 */
		bool add(const std::string& statement);

		/**
		 * Adds a set of statements to the ontology.
		 * Like {@link #add(String)} but for sets of statements.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 *
		 * 		vector<string> stmts;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		stmts.push_back("gorilla rdf:type Monkey");
		 * 		stmts.push_back("gorilla age 12^^xsd:int");
		 * 		stmts.push_back("gorilla weight 75.2");
		 *
		 * 		oro.add(stmts);
		 *
		 * 		return 0;
		 * }
		 * </pre>
		 * @throws MalformedYarpMessageException
		 * @throws IllegalStatementException
		 *
		 */
		bool add(const std::vector<std::string>& statements);

		/**
		 * Tries to identify a resource given a set of partially defined statements plus restrictions about this resource.
		 * YARP interface to {@link laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector, Vector)}. Please follow the link for details.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 * 		vector&lt;string&gt; filters;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back("?mysterious rdf:type oro:Monkey");
		 * 		partial_stmts.push_back("?mysterious oro:weight ?value");
		 *
		 * 		filters.push_back("?value >= 50");
		 *
		 * 		oro.find(&quot;mysterious&quot;, partial_stmts, filters, result);
		 * 		return 0;
		 * }
		 * </pre>
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector, Vector)
		 */
		int find(const std::string& resource, const std::vector<std::string>& partial_statements, const std::vector<std::string>& restrictions, std::vector<std::string>& result);

		/**
		 * Tries to identify a resource given a set of partially defined statements about this resource.
		 * YARP interface to {@link laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector)}. Please follow the link for details.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back("?mysterious oro:eats oro:banana_tree");
		 * 		partial_stmts.push_back("?mysterious oro:isFemale true^^xsd:boolean");
		 *
		 * 		oro.find(&quot;mysterious&quot;, partial_stmts, result);
		 * 		return 0;
		 * }
		 * </pre>
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector)
		 */
		int find(const std::string& resource, const std::vector<std::string>& partial_statements, std::vector<std::string>& result);

		/**
		 * Returns the set of asserted and inferred statements whose the given node
		 * is part of.<br/>
		 * YARP interface to
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#getInfos(String)}.
		 * Please follow the link for details.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 * 		oro.getInfos(&quot;monkey&quot;, result);
		 * 		return 0;
		 * }
		 * </pre>
		 *
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#getInfos(String)
		 */
		int getInfos(const std::string& resource, std::vector<std::string>& result);

		/**
		 * Same as {@link #getInfos(Value)} but returns the set of asserted and
		 * inferred statements in a human-friendly way.<br/>
		 *
		 * @throws MalformedYarpMessageException
		 *
		 * @see #getInfos(String)
		 */
		int getHumanReadableInfos(const std::string& resource, std::vector<std::string>& result);

		/**
		 * Tries to approximately identify an individual given a set of known
		 * statements about this resource.<br/>
		 * YARP interface to
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#guess(String, Vector, double)}
		 * . Please follow the link for details.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back(&quot;?mysterious age \&quot;40\&quot;&circ;&circ;xsd:int&quot;);
		 * 		partial_stmts.push_back(&quot;?mysterious weight \&quot;60\&quot;&circ;&circ;xsd:double&quot;);
		 *
		 * 		oro.guess(&quot;mysterious&quot;, 0.8, partial_stmts, result);
		 * 		return 0;
		 * }
		 * </pre>
		 *
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#guess(String, Vector,
		 *      double)
		 */
		int guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::vector<std::string>& result);

		/**
		 * Performs a SPARQL query on the OpenRobots ontology.<br/>
		 * YARP interface to
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#query(String)}. Please
		 * follow the link for details.<br/>
		 * This method can only have one variable to select. See
		 * {@link #queryAsXML(Value)} to select several variables.<br/>
		 *
		 * YARP C++ code snippet:
		 *
		 * <pre>
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 * 		oro.query(&quot;instances&quot;, &quot;SELECT ?instances \n WHERE { \n ?instances rdf:type owl:Thing}\n&quot;, result);
		 * 		return 0;
		 * }
		 * </pre>
		 *
		 * @throws MalformedYarpMessageException
		 *
		 * @see laas.openrobots.ontology.OpenRobotsOntology#query(String)
		 */
		int query(const std::string& var_name, const std::string& query, std::vector<std::string>& result);

		/**
		 * Returns the complete XML-encoded SPARQL result (works as well with
		 * several selected variables).<br/>
		 * YARP interface to
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#queryAsXML(String)}.
		 * Please follow the link for details.<br/>
		 * <br/>
		 *
		 * @throws MalformedYarpMessageException
		 *
		 * @see #query(String)
		 * @see laas.openrobots.ontology.OpenRobotsOntology#query(String)
		 */
		int queryAsXML(const std::string& query, std::string& result);

		/**
		 * A simple test to check if the YARP connector is working.<br/>
		 * It can be called with a string as argument and it will return the string appended to another one.
		 *
		 * @throws MalformedYarpMessageException
		 */

		int test(const std::string& test, std::string& result);


	private:
		YarpConnector* yarp;
		void pourBottle(yarp::os::Bottle&, std::vector<std::string>&);
};



/*********************************************************
 *                      Exceptions                       *
 *********************************************************/

class OntologyServerException : public std::runtime_error {
	 public:
		 OntologyServerException() : std::runtime_error("OntologyServerException") { }
		 OntologyServerException(const char* msg) : std::runtime_error(msg) { }
};

}

#endif /* LIBORO_H_ */
