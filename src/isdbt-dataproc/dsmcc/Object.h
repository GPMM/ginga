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

#ifndef OBJECT_H_
#define OBJECT_H_

#include "util/functions.h"
using namespace ::br::pucrio::telemidia::util;

#include "Binding.h"

#include <vector>
#include <iostream>
#include <string>
using namespace std;

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_CAROUSEL_BEGIN

	class Object {
		protected:
			unsigned int carouselId;
			unsigned int moduleId;
			unsigned int key;
			string kind;
			string name;

			//bindings (srg and dir only)
			vector<Binding*> bindings;

			//data (fil only)
			char* data;
			unsigned int dataSize;

		public:
			Object();

			virtual ~Object();

			void setCarouselId(unsigned int objectCarouselId);
			void setModuleId(unsigned int objectModuleId);
			void setKey(unsigned int objectKey);
			void setKind(string objectKind);
			void addBinding(Binding* binding);
			void setData(char* data);
			void setDataSize(unsigned int size);

			string getObjectId();
			unsigned int getCarouselId();
			unsigned int getModuleId();
			unsigned int getKey();
			string getKind();
			vector<Binding*>* getBindings();
			char* getData();
			unsigned int getDataSize();
			void print();
	};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_CAROUSEL_END
#endif /*OBJECT_H_*/
