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

#ifndef SHOWBUTTON_H_
#define SHOWBUTTON_H_

#include "config.h"

#include "system/SystemCompat.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::compat;

#include "system/Thread.h"
using namespace ::br::pucrio::telemidia::ginga::core::system::thread;

#include "mb/SDLWindow.h"
#include "mb/LocalScreenManager.h"
using namespace ::br::pucrio::telemidia::ginga::core::mb;

#include "mb/IMBDefs.h"
#include <string>
#include "ImagePlayer.h"

BR_PUCRIO_TELEMIDIA_GINGA_CORE_PLAYER_BEGIN

  class ShowButton : public Thread {
		private:
			short status;
			static const short NONE = 0;
			static const short PLAY = 1;
			static const short PAUSE = 2;
			static const short STOP = 3;

			GingaScreenID myScreen;
			short previousStatus;
			GingaWindowID win;
			bool running;

		public:
			ShowButton(GingaScreenID screenId);
			virtual ~ShowButton();

			void initializeWindow();
			void stop();
			void pause();
			void resume();

		private:
			void release();
			void render(string mrl);
			void run();
	};

BR_PUCRIO_TELEMIDIA_GINGA_CORE_PLAYER_END
#endif /*ShowButton_H_*/
