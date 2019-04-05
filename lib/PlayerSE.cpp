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

#include "MQTT.h"
#include "PlayerSE.h"

GINGA_NAMESPACE_BEGIN

// Public.

PlayerSE::PlayerSE (Formatter *formatter, Media *media)
    : Player (formatter, media)
{
  _client = make_unique<MQTT> ("127.0.0.1");
  cout << "criou o player" << endl;
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
      if (_animator->update (&_prop.sphere))
        cout << "animator polar " << _prop.sphere.polar << " azimuthal " << _prop.sphere.azimuthal << endl;
    }
}

void
PlayerSE::start ()
{
  if (!Player::getPrepared ())
    _client->connect ();

  if (_prop.positioning == AXIS)
    cout << "x " << _prop.axis.x << " y " << _prop.axis.y << " z " << _prop.axis.z << endl;
  else if (_prop.positioning == SPHERE)
    cout << "start polar " << _prop.sphere.polar << " azimuthal " << _prop.sphere.azimuthal << endl;

  _client->publish("teste", "start");

  Player::start ();
}

void
PlayerSE::stop ()
{
  _client->publish("teste", "stop");
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
  cout << "reload" << endl;
  Player::reload ();
}

void
PlayerSE::startPreparation ()
{
  g_assert (_state != PREPARING);
  TRACE ("preparing %s", _id.c_str ());

  _client->connect ([this] () { cout << "conectou" << endl; Player::setPrepared(true); });

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

GINGA_NAMESPACE_END
