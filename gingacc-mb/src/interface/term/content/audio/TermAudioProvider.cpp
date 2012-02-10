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

#include "mb/LocalScreenManager.h"
#include "mb/interface/term/content/audio/TermAudioProvider.h"
#include "mb/interface/term/output/TermSurface.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace mb {
	TermAudioProvider::TermAudioProvider(
			GingaScreenID screenId, const char* mrl) {

		myScreen = screenId;
	}

	TermAudioProvider::~TermAudioProvider() {

	}


	void TermAudioProvider::setLoadSymbol(string symbol) {

	}

	string TermAudioProvider::getLoadSymbol() {
		return "";
	}

	void TermAudioProvider::setAVPid(int aPid, int vPid) {

	}

	void TermAudioProvider::feedBuffers() {

	}

	bool TermAudioProvider::checkVideoResizeEvent(ISurface* frame) {
		return true;
	}

	double TermAudioProvider::getTotalMediaTime() {
		return 0;
	}

	int64_t TermAudioProvider::getVPts() {
		return 0;
	}

	double TermAudioProvider::getMediaTime() {
		return 0;
	}

	void TermAudioProvider::setMediaTime(double pos) {

	}

	void TermAudioProvider::playOver(
			ISurface* surface,
			bool hasVisual, IProviderListener* listener) {

	}

	void TermAudioProvider::resume(ISurface* surface, bool hasVisual) {

	}

	void TermAudioProvider::pause() {

	}

	void TermAudioProvider::stop() {

	}

	void TermAudioProvider::setSoundLevel(float level) {

	}

	void* TermAudioProvider::getContent() {
		return NULL;
	}

	void TermAudioProvider::getOriginalResolution(int* height, int* width) {

	}

	bool TermAudioProvider::releaseAll() {
		return true;
	}
}
}
}
}
}
}

extern "C" ::br::pucrio::telemidia::ginga::core::mb::
		IContinuousMediaProvider* createAudioProvider(
				GingaScreenID screenId, const char* mrl) {

	return (new ::br::pucrio::telemidia::ginga::core::mb::
			TermAudioProvider(screenId, mrl));
}

extern "C" void destroyAudioProvider(
		::br::pucrio::telemidia::ginga::core::mb::
		IContinuousMediaProvider* cmp) {

	delete cmp;
}
