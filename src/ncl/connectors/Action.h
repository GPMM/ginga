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

#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>
#include <set>
#include <iostream>
using namespace std;

#include "Role.h"

BR_PUCRIO_TELEMIDIA_NCL_CONNECTORS_BEGIN

	class Action {
		protected:
			set<string> typeSet;

		private:
			string delay;

		public:
			Action();
			Action(string newDelay);
			virtual ~Action(){};
			string getDelay();
			void setDelay(string dur);
			virtual string toString(){return "";};
			virtual bool instanceOf(string type) {
				return typeSet.count(type);
			}
	};

BR_PUCRIO_TELEMIDIA_NCL_CONNECTORS_END
#endif //_ACTION_H_
