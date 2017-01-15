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

#ifndef BDAPROVIDER_H_
#define BDAPROVIDER_H_

#include "system/compat/SystemCompat.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::compat;

extern "C" {
	#include <sys/types.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>
}

#include "IDataProvider.h"
#include "BDAGraph.h"

#include <string>
#include <iostream>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace tuning {
	class BDAProvider : public IDataProvider {
		protected:
			BDAGraph *bda;
			short capabilities;
			ITProviderListener* listener;
			long frequency;
			Channels channels;

		public:
			BDAProvider(long freq);
			virtual ~BDAProvider();

			void setListener(ITProviderListener* listener);
			void attachFilter(IFrontendFilter* filter){};
			void removeFilter(IFrontendFilter* filter){};

			short getCaps();
			bool tune();

			IChannel* getCurrentChannel();
			bool getSTCValue(uint64_t* stc, int* valueType);
			bool changeChannel(int factor);
			bool setChannel(string channelValue);
			int createPesFilter(int pid, int pesType, bool compositeFiler);
			string getPesFilterOutput();
			void close();

			char* receiveData(int* len);

			Channels* getChannels();
	};
}
}
}
}
}
}

#endif /*BDAPROVIDER_H_*/