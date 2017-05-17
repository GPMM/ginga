/* Copyright (C) 2006-2017 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef NCLDOCUMENTPARSER_H_
#define NCLDOCUMENTPARSER_H_

#include "ncl/NclDocument.h"
using namespace ::ginga::ncl;

#include "formatter/PrivateBaseContext.h" //FIXME: This is an architectural problem!
using namespace ::ginga::formatter;

#include <vector>
using namespace std;

GINGA_NCLCONV_BEGIN

class NclConnectorsParser;
class NclImportParser;
class NclTransitionParser;
class NclPresentationControlParser;
class NclComponentsParser;
class NclStructureParser;
class NclPresentationSpecificationParser;
class NclLayoutParser;
class NclInterfacesParser;
class NclLinkingParser;
class NclMetainformationParser;


// Get the DOMElement tagname as a std::string and free resources allocated by
// Xerces
static string
dom_element_tagname (const DOMElement *el)
{
  char *tagname = XMLString::transcode (el->getTagName ());
  string tagname_str (tagname);
  XMLString::release(&tagname);

  return tagname_str;
}

// Leak free check if DOMElement* has an attribute
static bool
dom_element_has_attr (const DOMElement *el, const string &attr)
{
  XMLCh *attr_xmlch = XMLString::transcode(attr.c_str());
  bool result = el->hasAttribute(attr_xmlch);
  XMLString::release(&attr_xmlch);

  return result;
}

// Get the value of an attribute of DOMElement* as a std::string and free
// resources allocated by Xerces
static string
dom_element_get_attr (const DOMElement *element, const string &attr)
{
  XMLCh *attr_xmlch = XMLString::transcode(attr.c_str());
  char *attr_value_ch =  XMLString::transcode(element->getAttribute (attr_xmlch));
  string attr_value_str(attr_value_ch);

  XMLString::release(&attr_xmlch);
  XMLString::release(&attr_value_ch);

  return attr_value_str;
}

#define FOR_EACH_DOM_ELEM_CHILD(X, Y) \
  for ( X = Y->getFirstElementChild(); \
        X != nullptr; \
        X = X->getNextElementSibling() )

static vector <DOMElement *>
dom_element_children(DOMElement *el)
{
  vector <DOMElement *> vet;

  DOMElement *child;
  FOR_EACH_DOM_ELEM_CHILD(child, el)
    {
      vet.push_back(child);
    }

  return vet;
}


static vector <DOMElement *>
dom_element_children_by_tagname(DOMElement *el, const string &tagname)
{
  vector <DOMElement *> vet;

  DOMElement *child;
  FOR_EACH_DOM_ELEM_CHILD(child, el)
    {
      if (dom_element_tagname(child) == tagname)
        {
          vet.push_back(child);
        }
    }

  return vet;
}

static vector <DOMElement *>
dom_element_children_by_tagnames(DOMElement *el, const vector<string> &tagnames)
{
  vector <DOMElement *> vet;

  DOMElement *child;
  FOR_EACH_DOM_ELEM_CHILD(child, el)
    {
      if (std::find(tagnames.begin(), tagnames.end(), dom_element_tagname(child))
          != tagnames.end() )
        {
          vet.push_back(child);
        }
    }

  return vet;
}


class NclParser : public ErrorHandler
{

private:
  PrivateBaseContext *privateBaseContext;
  bool ownManager;

protected:
  NclDocument *ncl;             // NCL document
  string path;                  // document's absolute path
  string dirname;               // directory part of document's path

  NclConnectorsParser *connectorsParser;
  NclImportParser *importParser;
  NclTransitionParser *transitionParser;
  NclPresentationControlParser *presentationControlParser;
  NclComponentsParser *componentsParser;
  NclStructureParser *structureParser;
  NclPresentationSpecificationParser *presentationSpecificationParser;
  NclLayoutParser *layoutParser;
  NclInterfacesParser *interfacesParser;
  NclLinkingParser *linkingParser;
  NclMetainformationParser *metainformationParser;
  DeviceLayout *deviceLayout;

public:
  NclParser (PrivateBaseContext *pbc, DeviceLayout *deviceLayout);
  virtual ~NclParser ();

  Node *getNode (const string &id);
  PrivateBaseContext *getPrivateBaseContext ();
  NclDocument *importDocument (string &docLocation);

public:
  NclTransitionParser *getTransitionParser ();
  NclConnectorsParser *getConnectorsParser ();
  NclImportParser *getImportParser ();
  NclPresentationControlParser *getPresentationControlParser ();
  NclComponentsParser *getComponentsParser ();
  NclStructureParser *getStructureParser ();
  NclPresentationSpecificationParser *getPresentationSpecificationParser ();
  NclLayoutParser *getLayoutParser ();
  NclInterfacesParser *getInterfacesParser ();
  NclMetainformationParser *getMetainformationParser ();
  NclLinkingParser *getLinkingParser ();

protected:
  NclDocument *parseRootElement (DOMElement *rootElement);
public:
  string getPath ();
  string getDirName ();
  void setNclDocument (NclDocument *);
  NclDocument *getNclDocument ();

  void warning (const SAXParseException &);
  void error (const SAXParseException &);
  void fatalError (const SAXParseException &);
  void resetErrors () {};

  NclDocument *parse (const string &);
};

GINGA_NCLCONV_END

#endif /*NCLDOCUMENTPARSER_H_*/