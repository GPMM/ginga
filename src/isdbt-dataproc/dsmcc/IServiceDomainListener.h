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

#ifndef IServiceDomainListener_H_
#define IServiceDomainListener_H_

#include "isdbt-tsparser/IAIT.h"
using namespace ::br::pucrio::telemidia::ginga::core::tsparser::si;

#include <map>
using namespace std;

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_BEGIN

	class IServiceDomainListener {
		public:
			virtual ~IServiceDomainListener(){};
			virtual bool applicationInfoMounted(IAIT* ait)=0;

			virtual void serviceDomainMounted(
					string mountPoint,
					map<string, string>* names,
					map<string, string>* paths)=0;
	};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_END
#endif /*ISTREAMEVENTLISTENER_H_*/
