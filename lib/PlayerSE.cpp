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

#include "aux-ginga.h"
#include "aux-gl.h"
#include "PlayerSE.h"
#include <regex>

GINGA_NAMESPACE_BEGIN

static map<string, string> translation_table = {
  { "SEtype", "type" },
  { "explicitDur", "duration" } 
};

static string
translate_property (const string &property)
{
  map<string, string>::iterator it;
  if ((it = translation_table.find (property)) == translation_table.end ())
    return property;
  return it->second;
}

PlayerSE::PlayerSE (Formatter *formatter, Media *media)
    : Player (formatter, media),
      mqtt ("ginga-ncl")
{
	cout << "Criou player" << endl;
  _doc = NULL;
  mqtt.connect("127.0.0.1", 1883);
}

PlayerSE::~PlayerSE ()
{
	cout << "Destruiu player" << endl;
  mqtt.disconnect ();
}

void
PlayerSE::incTime(Time time)
{
  Player::incTime(time);
}

void
PlayerSE::start ()
{
  int len;
  xmlChar *buffer;
  string location;

  Player::start ();

  xmlDocDumpMemory (_doc, &buffer, &len);

  cout << buffer << endl;

  if (_prop.polar != -1 && _prop.azimuthal != -1)
    {
      location = to_string(_prop.polar) + "," + to_string(_prop.azimuthal);
    }   
  else
    {
      location = _prop.x_axis + ":" + _prop.y_axis + ":" + _prop.z_axis;
    }

  this->mqtt.publish(location, string((char *) buffer));
  xmlFree(buffer);
}

void
PlayerSE::stop ()
{
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
  int rc;

  if (_doc == NULL)
    createSEDLDoc();

  Player::reload ();
}

bool
PlayerSE::doSetProperty (Property code, const string &name,
                          const string &value)
{
  if (name == "location")
    return true;
  else if (name == "type")
    {
      string prefix ("application/x-sensory-effect-");
      if (value.find(prefix) == 0) {
        return Player::doSetProperty(code, name, value.substr(prefix.size()));
      }
    }

  return Player::doSetProperty(code, name, value);
}

void
PlayerSE::createSEDLDoc ()
{
  string location;
  xmlNodePtr effect_node = NULL;

  _doc = xmlNewDoc(BAD_CAST "1.0");

  effect_node = xmlNewNode(NULL, BAD_CAST "Effect");
  xmlDocSetRootElement(_doc, effect_node);
  
  xmlNewProp(effect_node, BAD_CAST "type", BAD_CAST _prop.type.c_str());

  if (_prop.polar != -1 && _prop.azimuthal != -1)
    {
      location = to_string(_prop.polar) + "," + to_string(_prop.azimuthal);
      xmlNewProp(effect_node, BAD_CAST "location", BAD_CAST location.c_str());
    }
  else
    {
      location = _prop.x_axis + ":" + _prop.y_axis + ":" + _prop.z_axis;
      xmlNewProp(effect_node, BAD_CAST "location", BAD_CAST location.c_str());
    }

  if (_prop.duration !=  GINGA_TIME_NONE)
    {
      xmlNewProp(effect_node, BAD_CAST "duration", BAD_CAST getProperty("explicitDur").c_str());
    }
}

GINGA_NAMESPACE_END
