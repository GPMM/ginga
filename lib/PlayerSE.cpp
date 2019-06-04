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

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "MQTT.h"
#include "PlayerSE.h"

GINGA_NAMESPACE_BEGIN

// Public.

PlayerSE::PlayerSE (Formatter *formatter, Media *media)
    : Player (formatter, media)
{
  _client = make_unique<MQTT> ("127.0.0.1");
}

PlayerSE::~PlayerSE ()
{
}

void
PlayerSE::incTime(Time time)
{
  Player::incTime(time);

  if (_prop.positioning == SPHERE)
    {
      string previousLocation = _prop.location;

      if (_animator->update (&_prop.sphere))
        {
          string sedl = getSEDL(previousLocation, false);
          _client->publish(previousLocation, sedl);

          if (_prop.sphere.width == 0.0 && _prop.sphere.height == 0.0)
            _prop.location =  xstrbuild ("%g,%g", _prop.sphere.polar,
                _prop.sphere.azimuthal);
          else
            _prop.location =  xstrbuild ("%g,%g,%g,%g",
                _prop.sphere.polar, _prop.sphere.azimuthal,
                _prop.sphere.width, _prop.sphere.height);

          sedl = getSEDL(_prop.location, true);
          _client->publish(_prop.location, sedl);
        }
    }
}

void
PlayerSE::start ()
{
  if (!Player::getPrepared ())
    _client->connect ();

  string sedl = getSEDL(_prop.location, true);

  _client->publish(_prop.location, sedl);

  Player::start ();
}

void
PlayerSE::stop ()
{
  string sedl = getSEDL(_prop.location, false);

  _client->publish(_prop.location, sedl);
  _client->disconnect ();

  Player::stop ();
}

void
PlayerSE::pause ()
{
  Player::pause ();
}

void
PlayerSE::resume ()
{
  Player::resume ();
}

void
PlayerSE::reload ()
{
  Player::reload ();
}

void
PlayerSE::startPreparation ()
{
  g_assert (_state != PREPARING);
  TRACE ("preparing %s", _id.c_str ());

  _client->connect ([this] () { Player::setPrepared(true); });

  Player::startPreparation ();
}

// Protected.

bool
PlayerSE::doSetProperty (Property code, unused (const string &name),
                           const string &value)
{
  switch (code)
    {
      default:
        return Player::doSetProperty (code, name, value);
    }

  return true;
}

string
PlayerSE::getSEDL (string location, bool activate)
{
  int len;
  xmlDocPtr doc;
  xmlChar *buffer;
  xmlNodePtr effectNode;

  doc = xmlNewDoc(BAD_CAST "1.0");

  g_assert (doc);

  effectNode = xmlNewNode(NULL, BAD_CAST "Effect");

  g_assert (effectNode);

  xmlDocSetRootElement(doc, effectNode);
  
  xmlNewProp(effectNode, BAD_CAST "type", BAD_CAST _prop.type.c_str());
  xmlNewProp(effectNode, BAD_CAST "location", BAD_CAST location.c_str());

  if (_prop.duration != GINGA_TIME_NONE)
    xmlNewProp(effectNode, BAD_CAST "duration", BAD_CAST getProperty("explicitDur").c_str());

  xmlNewProp(effectNode, BAD_CAST "activate", activate ? BAD_CAST "true" : BAD_CAST "false");

  xmlDocDumpMemory (doc, &buffer, &len);

  string sedl ((char *) buffer);
  xmlFree(buffer);
  xmlFreeDoc(doc);

  return sedl;
} 

GINGA_NAMESPACE_END
