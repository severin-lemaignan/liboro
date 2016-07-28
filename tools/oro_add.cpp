/*
 * Copyright (c) 2016 Plymouth University Séverin Lemaignan severin.lemaignan@plymouth.ac.uk
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


#include <boost/program_options.hpp>

#include <string>
#include <iostream>

#include "oro.h"
#include "socket_connector.h"

using namespace std;


using namespace oro;
using namespace boost;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {

    //////////////////////////////////////////////////////////////////////
    ////////// Command-line parsing
    //////////////////////////////////////////////////////////////////////
    
    po::positional_options_description p;
    p.add("statement", -1);

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("host", po::value<string>()->default_value("localhost"), "knowledge base host")
            ("port", po::value<string>()->default_value("6969"), "knowledge base port");

    po::options_description hidden("Hidden options");
    hidden.add_options()
            ("statement", po::value<vector<string>>(), "statement(s) to add")
            ;

    po::options_description cmd_line("Allowed options");
    cmd_line.add(desc).add(hidden);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                        .options(cmd_line)
                        .positional(p)
                        .run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << "Usage: oro-add [options] \"statement\" [\"statement\"...]" << endl;
        cout << endl;

        cout << desc;
        cout << endl;

        cout << "Adds symbolic statements to a KB-API compatible knowledge base.\n\n";
        cout << "Séverin Lemaignan, Plymouth University 2016, " << endl;
        cout << "Report bugs to: " << endl;
        cout << "https://www.github.com/severin-lemaignan/liboro/issues" << endl;

        return 1;
    }

    if (!vm.count("statement")) {
        cerr << "You must provide at least one statement to add. For instance:\n$ oro_add \"pepper rdf:type Robot\"" << endl;
        return 1;
    }


    cerr << "Adding the following statements:" << endl;
    for (const auto& s : vm["statement"].as<vector<string>>()) {
        cerr << "  - " << s << endl;
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    {
    Ontology* onto;
    SocketConnector connector(vm["host"].as<string>(), vm["port"].as<string>());

    try {
        onto = Ontology::createWithConnector(connector);
    } catch (OntologyServerException ose) {
        cerr << "Server error: " << ose.what() << endl;
        return 0;
    }

    for (const auto& s : vm["statement"].as<vector<string>>()) {
        try {
            onto->add(Statement(s));
        } catch (InvalidQueryException iqe) {
            cerr << "Invalid query: " << iqe.what() << endl;
        } catch (OntologyServerException ose) {
            cerr << "Server error: " << ose.what() << endl;
        }
    
    }
    }


    return 0;

}

