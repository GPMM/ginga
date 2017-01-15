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

#ifndef FOCUSSOURCEMANAGER_H_
#define FOCUSSOURCEMANAGER_H_

#include "mb/IMBDefs.h"
#include "mb/SDLSurface.h"
using namespace ::br::pucrio::telemidia::ginga::core::mb;

#include <string>
using namespace std;

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace ncl {
namespace model {
namespace presentation {
namespace focus {
  class FocusSourceManager {
	public:
		static GingaSurfaceID getFocusSourceComponent(
				GingaScreenID screenId, string src);
  };
}
}
}
}
}
}
}
}

#endif /*FOCUSSOURCEMANAGER_H_*/