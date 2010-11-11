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

#include "tsparser/TOT.h"

namespace br {
namespace pucrio {
namespace telemidia {
namespace ginga {
namespace core {
namespace tsparser {
namespace si {
	TOT::TOT() {
		memset(UTC3Time, 0, 5);
		descriptorsLoopLength = 0;
		descriptors           = new vector<IMpegDescriptor*>;
	}

	TOT::~TOT() {
		vector<IMpegDescriptor*>::iterator i;

		if (descriptors != NULL) {
			for (i = descriptors->begin(); i != descriptors->end(); ++i) {
				delete(*i);
			}

			delete descriptors;
			descriptors = NULL;
		}
	}

	string TOT::getUTC3TimeStr() {
		stringstream str;

		str << time.tm_mday << "/";
		str << time.tm_mon + 1 << " ";
		// tm_mon goes from 0 to 11, so + 1 to print month in 1-12.
		str << time.tm_hour << ":";
		str << time.tm_min;

		return str.str();
	}

	struct tm TOT::getUTC3TimeTm() {
		return time;
	}

	struct tm TOT::convertMJDtoUTC(unsigned int mjd) {
		struct tm time;
		int year,month,day,k, weekDay;

		year  = ((mjd-15078.2) / 365.25);
		month = (( mjd - 14956.1 - (int) (year * 365.25) ) / 30.6001 ) ;
		day   = mjd - 14956 - ((int) (year * 365.25)) - ((int)(month * 30.6001));

		if (month == 14 || month == 15) {
			k = 1;

		} else {
			k = 0;
		}

		year    = year + k;
		month   = month - 1 - k * 12;
		weekDay = (((mjd + 2) % 7) + 1);

		time.tm_year = year;
		time.tm_mon  = month - 1;
		/*tm.mon goes from 0(January) to 11 (December).
		 *if month is february ('month'= 2), tm.mon is 1
		 */
		time.tm_mday = day;
		time.tm_wday = weekDay;
		time.tm_hour = 0;
		time.tm_min  = 0;
		time.tm_sec  = 0;

		return time;
	}

	int TOT::convertBCDtoDecimal(int bcd) {
		return ((bcd >> 4) * 10) + bcd % 16;
	}

	void TOT::calculateTime() {
		unsigned int mjd;

		if (((UTC3Time[0] & 0xFF) == 0xFF) &&
				((UTC3Time[1] & 0xFF) == 0xFF) &&
				((UTC3Time[2] & 0xFF) == 0xFF) &&
				((UTC3Time[3] & 0xFF) == 0xFF) &&
				((UTC3Time[4] & 0xFF) == 0xFF)) {

			return;
		}

		mjd  = (((UTC3Time[0] << 8) & 0xFF00) | (UTC3Time[1] & 0xFF));

		time = convertMJDtoUTC(mjd);

		time.tm_hour = convertBCDtoDecimal((int)UTC3Time[2]);
		time.tm_min  = convertBCDtoDecimal((int)UTC3Time[3]);
		time.tm_sec  = convertBCDtoDecimal((int)UTC3Time[4]);
	}

	vector<IMpegDescriptor*>* TOT::getDescriptors() {
		return descriptors;
	}

	void TOT::print() {
		vector<IMpegDescriptor*>::iterator i;

		cout << "TOT::print printing..." << endl;
		cout << "UTC3Time: " << getUTC3TimeStr() << endl;
/*
		for (i = descriptors->begin(); i != descriptors->end(); ++i) {
			(*i)->print();
		}
*/
	}

	void TOT::process(void* payloadBytes, unsigned int payloadSize) {
		char* data;
		size_t pos, localpos;
		unsigned short remainingBytes, value;
		LocalTimeOffsetDescriptor* localDescriptor;

		cout << "TOT::process" << endl;

		pos  = 0;
		data = new char[payloadSize];
		memcpy((void*)&(data[0]), payloadBytes, payloadSize);

		memcpy(UTC3Time, data, 5);
		pos += 5;
		calculateTime();

		descriptorsLoopLength = (
				(((data[pos] & 0x0F) << 8) & 0xFF00) | (data[pos+1] & 0xFF));

		pos +=2;

		remainingBytes  = descriptorsLoopLength;

		cout << "TOT::process going to descriptors with descriptorsLength:";
		cout << descriptorsLoopLength << endl;

		while (remainingBytes) {
			value = ((data[pos+1] & 0xFF) + 2);
			remainingBytes -= value;

			if (data[pos] == DT_LOCAL_TIME_OFFSET) {
				cout << "TOT::process LocalTimeOffsetDescriptor" << endl;
				localDescriptor = new LocalTimeOffsetDescriptor();
				localpos = localDescriptor->process(data, pos);
				pos += value;
				cout << "TOT::process Desc finished with pos: " << pos;
				cout << " and localpos: " << localpos << endl;

				descriptors->push_back((IMpegDescriptor*)localDescriptor);
			}
		}
	}
}
}
}
}
}
}
}



extern "C" ::br::pucrio::telemidia::ginga::core::tsparser::si::
		ITOT* createTOT() {

	return new ::br::pucrio::telemidia::ginga::core::tsparser::si::TOT();
}

extern "C" void destroyTOT(
		::br::pucrio::telemidia::ginga::core::tsparser::si::ITOT* tot) {

	delete tot;
}
