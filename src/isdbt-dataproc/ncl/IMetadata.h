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

#ifndef _IMetadata_H_
#define _IMetadata_H_

#include "IDataFile.h"
#include "INCLStructure.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_NCL_BEGIN


typedef struct {
	char* stream;
	int size;
} StreamData;

	class IMetadata : public INCLStructure {
		public:
			virtual ~IMetadata(){};

			int getType(){return ST_METADATA;};
			int getId(){return 0;};

			virtual string getRootUri()=0;
			virtual string getName()=0;
			virtual double getTotalLength()=0;
			virtual void setTargetTotalLength(double targetSize)=0;
			virtual bool isConsolidated()=0;

			virtual void setBaseUri(string uri)=0;
			virtual string getBaseUri()=0;

			virtual void setRootObject(IDataFile* rootObject)=0;
			virtual IDataFile* getRootObject()=0;

			virtual void addDataFile(IDataFile* dataObject)=0;
			virtual IDataFile* getDataFile(int structureId)=0;
			virtual map<int, IDataFile*>* getDataFiles()=0;

			virtual vector<StreamData*>* createNCLSections()=0;
	};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_NCL_END
#endif //_IMetadata_H_
