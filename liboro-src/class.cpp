
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

#include "oro.h"
#include "oro_event.h"

using namespace std;

namespace oro {
	
Class::Class(const std::string& name): _name(name){}
Class::~Class(){}

void Class::onNewInstance(OroEventObserver& callback, bool repeatable) const {
	set<string> pattern;
	pattern.insert(_name);
	
	Ontology::getInstance()->registerEvent(
					callback,
					NEW_CLASS_INSTANCE,
					repeatable ? ON_TRUE : ON_TRUE_ONE_SHOT,
					pattern,
					"");
}

}
