/* Copyright (C) 2006-2018 PUC-Rio/Laboratorio TeleMidia

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
along with Ginga.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef PLAYER_SE_H
#define PLAYER_SE_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "Player.h"
#include "MQTTClient.h"

GINGA_NAMESPACE_BEGIN

class PlayerSE : public Player
{
public:
  PlayerSE (Formatter *, Media *);
  ~PlayerSE ();

  void incTime (Time) override;

  void start () override;
  void stop () override;
  void pause () override;
  void resume () override;

  void reload () override;

protected:
  bool doSetProperty (Property, const string &, const string &) override;

private:
  xmlDocPtr _doc;
  MQTTClient::MQTTClient mqtt;
 
  bool parseMpegV ();
  void createSEDLDoc ();

};

GINGA_NAMESPACE_END

#endif // PLAYER_MPEGV
