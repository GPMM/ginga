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

#include "config.h"
#include "StreamMode.h"

BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_DSMCC_NPT_BEGIN

StreamMode::StreamMode() : MpegDescriptor(0x03) {
	streamMode = 0;
	descriptorLength = 2;
}

StreamMode::~StreamMode() {

}

int StreamMode::process() {
	int pos = MpegDescriptor::process();
	streamMode = stream[pos++] & 0xFF;
	return 0;
}

int StreamMode::updateStream() {
	int pos = MpegDescriptor::updateStream();

	stream[pos++] = streamMode & 0xFF;
	stream[pos++] = 0xFF;

	return pos;
}

unsigned int StreamMode::calculateDescriptorSize() {
	int pos = MpegDescriptor::calculateDescriptorSize();
	return pos + 2;
}

unsigned char StreamMode::getStreamMode() {
	return streamMode;
}

void StreamMode::setStreamMode(unsigned char mode) {
	streamMode = mode;
}


BR_PUCRIO_TELEMIDIA_GINGA_CORE_DATAPROCESSING_DSMCC_NPT_END
