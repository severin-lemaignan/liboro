#!/usr/bin/python
# coding=utf-8

from xml.dom import minidom
import os, sys

ontology = "openrobots.owl"
path = "../oro/"
output_path = "liboro-src/"

dom = minidom.parse(path + ontology)

f_h = open(output_path + "oro_library.h", "w")
f_cpp = open(output_path + "oro_library.cpp", "w")

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

def addPropertiesDeclarations(dom, propertyType, printDeclaration, destFile):
    properties = dom.getElementsByTagName(propertyType)
    names=[[getAboutAttribute(p), getComment(p)] for p in properties]

    if printDeclaration:
	    for p in names:
		print >>destFile
		if p[1] != None: print >> destFile, "\t\t/**\n\t\t* %s\n\t\t*/" % p[1]
		print >> destFile,"\t\tstatic const Property %s;" % p[0]
    else:
	    for p in names:
		print >> destFile,"const Property Properties::%s = Property(\"%s\");" % (p[0], p[0])

def addClassesDeclarations(dom, printDeclaration, destFile):
    classes = dom.getElementsByTagName("owl:Class")
    names=[[getAboutAttribute(c), getComment(c)] for c in classes]

    for n in names:
	    if n[0] == None: names.remove(n)

    if printDeclaration:
	    for c in names:
		print >> destFile
		if c[1] != None: print >> destFile,"\t\t/**\n\t\t* %s\n\t\t*/" % c[1]
		print >> destFile,"\t\tstatic const Class %s;" % c[0]
	    print >> destFile,"};"
	    print >> destFile
    else:
	    for c in names:
		print >> destFile,"const Class Classes::%s = Class(\"%s\");" % (c[0], c[0])


#################
# oro_library.h #
#################

print >> f_h, "/** \\file"
print >> f_h, " * This header defines a \"library\" of C++ properties and classes (or concepts)"
print >> f_h, " * matching the ones defines in the " + ontology + " ontology.\\n"
print >> f_h, " * It has been automatically generated from " + ontology + " by the "
print >> f_h, " * \"generate_oro_library_from_ontology.py\" Python script."
print >> f_h, " * If you need to regenerate this file (for your own ontology or after a "
print >> f_h, " * modification of the ontology), just run "
print >> f_h, " * <tt>./generate_oro_library_from_ontology.py > src/oro_library.h</tt>"
print >> f_h, " */"
#print >> f_h, "// openrobots.owl version: " + os.system("../oro/git log -1 --pretty=format%ai")
print >> f_h, "#ifndef ORO_LIBRARY_H_"
print >> f_h, "#define ORO_LIBRARY_H_"
print >> f_h
print >> f_h, "#include \"oro.h\""
print >> f_h
print >> f_h, "namespace oro {"
print >> f_h
print >> f_h, "/** This class lists all the OWL properties defined in the " + ontology
print >> f_h, " * ontology.\\n"
print >> f_h, " * It provides handy shortcuts when asserting new facts on concepts."
print >> f_h, " * \see Concept::assert(const Property& predicate, const Concept& value)"
print >> f_h, " */"
print >> f_h, "class Properties {"
print >> f_h, "\tpublic:"
print >> f_h
print >> f_h, "// Object properties"

addPropertiesDeclarations(dom, "owl:ObjectProperty", True, f_h)
print >> f_h
print >> f_h, "// Datatype properties"
addPropertiesDeclarations(dom, "owl:DatatypeProperty", True, f_h)

print >> f_h, "};"

print >> f_h
print >> f_h, "/** This class lists all the OWL concepts defined in the " + ontology + " ontology.\\n"
print >> f_h, " * It provides handy shortcuts when asserting the class of a concept."
print >> f_h, " * \see Concept::isA(const Class& type) const"
print >> f_h, " */"
print >> f_h, "class Classes {"
print >> f_h, "\tpublic:"
print >> f_h,"\t\t/**\n\t\t* The most generic concept available.\n\t\t*/"
print >> f_h, "\t\tstatic const Class Thing;"

addClassesDeclarations(dom, True, f_h)

print >> f_h

print >> f_h, "}"
print >> f_h
print >> f_h, "#endif /* ORO_LIBRARY_H_ */"


###################
# oro_library.cpp #
###################

print >> f_cpp, "#include \"oro_library.h\""
print >> f_cpp
print >> f_cpp, "namespace oro {"
addPropertiesDeclarations(dom, "owl:ObjectProperty", False, f_cpp)
print >> f_cpp
addPropertiesDeclarations(dom, "owl:DatatypeProperty", False, f_cpp)
print >> f_cpp
print >> f_cpp, "const Class Classes::Thing = Class(\"owl:Thing\");"
addClassesDeclarations(dom, False, f_cpp)
print >> f_cpp
print >> f_cpp, "}"
