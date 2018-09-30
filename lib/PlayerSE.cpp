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
    : Player (formatter, media)
{
	cout << "Criou player" << endl;
}

PlayerSE::~PlayerSE ()
{
	cout << "Destruiu player" << endl;
}

void
PlayerSE::incTime(Time time)
{
  Player::incTime(time);
}

void
PlayerSE::start ()
{
  Player::start ();
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
  xmlBufferPtr buf;
  xmlTextWriterPtr writer;

  buf = xmlBufferCreate();

  g_assert_nonnull (buf);

  writer = xmlNewTextWriterMemory(buf, 0);

  g_assert_nonnull (write);

  rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);

  if (rc < 0)
  {
    cerr << "erro start document" << endl;
    return;
  }

  rc = xmlTextWriterStartElement(writer, BAD_CAST "Effect");

  if (rc < 0)
  {
    cerr << "erro start element" << endl;
    return;
  }

  for(const auto &pair : _properties)
  {
    string attribute, value;

    // TODO check if property is a value mpegv attribute

    attribute = translate_property(pair.first);
    value = pair.second;

    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST attribute.c_str(),
        BAD_CAST value.c_str());

    if (rc < 0)
    {
      cerr << "erro attribute" << endl;
      return;
    }
  }

  rc = xmlTextWriterEndDocument(writer);

  if (rc < 0)
  {
    cerr << "erro end document" << endl;
    return;
  }

  xmlFreeTextWriter(writer);

  fprintf(stderr, "%s\n\n\n", (const char*) buf->content);

  xmlBufferFree(buf);

  Player::reload ();
}

bool
PlayerSE::doSetProperty (Property code, const string &name,
                          const string &value)
{
  if (name == "location")
    return true;

  return Player::doSetProperty(code, name, value);
}

GINGA_NAMESPACE_END
