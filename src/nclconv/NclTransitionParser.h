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

#ifndef NCLTRANSITIONPARSER_H_
#define NCLTRANSITIONPARSER_H_

#include "ModuleParser.h"
#include "NclParser.h"

#include "NclImportParser.h"

#include "ncl/TransitionUtil.h"
using namespace ::ginga::ncl;

GINGA_NCLCONV_BEGIN

class NclTransitionParser : public ModuleParser
{
public:
  explicit NclTransitionParser (NclParser *nclParse);

  TransitionBase *parseTransitionBase (DOMElement *parentElement);

  Transition *parseTransition (DOMElement *parentElement);

  void addImportBaseToTransitionBase (TransitionBase *, DOMElement *);
};

GINGA_NCLCONV_END

#endif // NCLTRANSITIONPARSER_H_
