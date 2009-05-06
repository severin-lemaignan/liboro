#!/usr/bin/python
# coding=utf-8

from xml.dom import minidom
import os

ontology = "openrobots.owl"
path = "../oro/"

dom = minidom.parse(path + ontology)

def formatLines(text, length):
    if len(text) > length:
	posLastSep = text[:length].rfind("\n") #first look for new lines
        if posLastSep == -1: posLastSep = text[:length].rfind(" ") #then, if no newline, look for the last space before the end of the line
    	return text[:posLastSep] + "\n" + formatLines(text[posLastSep + 1:], length)
    else:
	return text

def getAboutAttribute(element):
    if element.attributes.length > 0 and element.attributes["rdf:about"].value.find("#Thing") == -1:
    	return element.attributes["rdf:about"].value[1:].replace("-","_")

def getComment(element):
    for node in element.childNodes:
	if node.nodeName == "rdfs:comment": return formatLines(node.firstChild.data, 70).replace("\n", "\n\t\t* ")
    return None

def addPropertiesDeclarations(dom, propertyType,printDeclaration):
    properties = dom.getElementsByTagName(propertyType)
    names=[[getAboutAttribute(p), getComment(p)] for p in properties]

    if printDeclaration:
	    for p in names:
		print
		if p[1] != None: print "\t\t/**\n\t\t* %s\n\t\t*/" % p[1]
		print "\t\tstatic const Property %s;" % p[0]
    else:
	    for p in names:
		print "const Property Properties::%s = Property(\"%s\");" % (p[0], p[0])

def addClassesDeclarations(dom):
    classes = dom.getElementsByTagName("owl:Class")
    names=[[getAboutAttribute(c), getComment(c)] for c in classes]

    for n in names:
	if n[0] == None: names.remove(n)

    for c in names:
	print
	if c[1] != None: print "\t\t/**\n\t\t* %s\n\t\t*/" % c[1]
	print "\t\tstatic const Class %s;" % c[0]
    print "};"
    print
    for c in names:
	print "const Class Classes::%s = Class(\"%s\");" % (c[0], c[0])


print "/** \\file"
print " * This header defines a \"library\" of C++ properties and classes (or concepts) matching the ones defines in the " + ontology + " ontology.\\n"
print " * It has been automatically generated from " + ontology + " by the \"generate_oro_library_from_ontology.py\" Python script."
print " * If you need to regenerate this file (for your own ontology or after a modification of the ontology), just run <tt>./generate_oro_library_from_ontology.py > src/oro_library.h</tt>"
print " */"
#print "// openrobots.owl version: " + os.system("../oro/git log -1 --pretty=format%ai")
print "#ifndef ORO_LIBRARY_H_"
print "#define ORO_LIBRARY_H_"
print
print "#include \"oro.h\""
print
print "namespace oro {"
print
print "/** This class lists all the OWL properties defined in the " + ontology + " ontology.\\n"
print " * It provides handy shortcuts when asserting new facts on concepts."
print " * \see Concept::assert(const Property& predicate, const Concept& value)"
print " */"
print "class Properties {"
print "\tpublic:"
print
print "// Object properties"

addPropertiesDeclarations(dom, "owl:ObjectProperty", True)
print
print "// Datatype properties"
addPropertiesDeclarations(dom, "owl:DatatypeProperty", True)

print "};"

addPropertiesDeclarations(dom, "owl:ObjectProperty", False)
addPropertiesDeclarations(dom, "owl:DatatypeProperty", False)

print
print "/** This class lists all the OWL concepts defined in the " + ontology + " ontology.\\n"
print " * It provides handy shortcuts when asserting the class of a concept."
print " * \see Concept::isA(const Class& type) const"
print " */"
print "class Classes {"
print "\tpublic:"

addClassesDeclarations(dom)

print

print "}"
print
print "#endif /* ORO_LIBRARY_H_ */"

