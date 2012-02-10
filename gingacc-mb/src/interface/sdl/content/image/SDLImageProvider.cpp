/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Direitos Autorais Reservados (c) 1989-2007 PUC-Rio/Laboratorio TeleMidia

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright: 1989-2007 PUC-RIO/LABORATORIO TELEMIDIA, All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
ncl @ telemidia.puc-rio.br
http://www.ncl.org.br
http://www.ginga.org.br
http://www.telemidia.puc-rio.br
*******************************************************************************/

#include "util/Color.h"

#include "mb/interface/sdl/content/image/SDLImageProvider.h"
#include "mb/interface/sdl/output/SDLWindow.h"
#include "mb/interface/sdl/output/SDLSurface.h"
#include "mb/interface/sdl/SDLDeviceScreen.h"
#include "mb/LocalScreenManager.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace mb {

	bool SDLImageProvider::initialized = false;
	short SDLImageProvider::imageRefs   = 0;

	SDLImageProvider::SDLImageProvider(
			GingaScreenID screenId, const char* mrl) {

		if (!initialized) {
			initialized = true;
			if (IMG_Init(0) < 0) {
				cout << "SDLFontProvider::SDLFontProvider ";
				cout << "Couldn't initialize TTF: " << SDL_GetError();
				cout << endl;
			}
		}

		imageRefs++;

		imgUri   = "";
		myScreen = screenId;

		imgUri.assign(mrl);
	}

	SDLImageProvider::~SDLImageProvider() {
		imageRefs--;

		if (imageRefs == 0) {
			IMG_Quit();
			initialized = false;
		}
	}

	void* SDLImageProvider::getContent() {
		return NULL;
	}

	void SDLImageProvider::playOver(ISurface* surface) {
		SDL_Surface* renderedSurface;
		IWindow* parent;

		renderedSurface = IMG_Load(imgUri.c_str());
		surface->setContent((void*)renderedSurface);

		parent = (IWindow*)(surface->getParent());
		if (parent != NULL) {
			parent->renderFrom(surface);
		}
	}

	ISurface* SDLImageProvider::prepare(bool isGif) {
		ISurface* surface;
		SDL_Surface* renderedSurface = IMG_Load(imgUri.c_str());

		surface = LocalScreenManager::getInstance()->createSurfaceFrom(
				myScreen, renderedSurface);

		return surface;
	}

	bool SDLImageProvider::releaseAll() {
		return false;
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::mb::IImageProvider*
		createSDLImageProvider(GingaScreenID screenId, const char* mrl) {

	return (new ::br::pucrio::telemidia::ginga::core::mb::
			SDLImageProvider(screenId, mrl));
}

extern "C" void destroySDLImageProvider(
		::br::pucrio::telemidia::ginga::core::mb::IImageProvider* ip) {

	delete ip;
}
