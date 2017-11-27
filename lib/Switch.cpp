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

#include "aux-ginga.h"
#include "Switch.h"

#include "Document.h"

GINGA_NAMESPACE_BEGIN


// Public.

Switch::Switch (const string &id)
  :Composition (id)
{
  _selected = nullptr;
}

Switch::~Switch ()
{
  for (auto item: _rules)
    delete item.second;
}


// Public: Object.

string
Switch::getObjectTypeAsString ()
{
  return "Switch";
}

bool
Switch::startTransition (Event *event, Event::Transition transition)
{
  switch (event->getType ())
    {
    case Event::PRESENTATION:
      g_assert (event->isLambda ());
      switch (transition)
        {
        case Event::START:
          g_assert_null (_selected);
          for (auto item: _rules)
            {
              Object *obj;
              Predicate *pred;
              Event *lambda;

              obj = item.first;
              g_assert_nonnull (obj);
              pred = item.second;
              g_assert_nonnull (pred);

              if (_doc->evalPredicate (pred))
                {
                  lambda = obj->getLambda ();
                  g_assert_nonnull (lambda);
                  if (lambda->transition (transition))
                    _selected = obj;
                  break;
                }
            }
          break;

        case Event::STOP:
          if (_selected != nullptr)
            {
              Event *lambda = _selected->getLambda ();
              g_assert_nonnull (lambda);
              lambda->transition (transition);
              _selected = nullptr;
            }
          break;

        default:
          g_assert_not_reached ();
        }
      break;

    case Event::ATTRIBUTION:
    case Event::SELECTION:
    default:
      g_assert_not_reached ();
    }
  return true;
}

void
Switch::endTransition (Event *event, Event::Transition transition)
{
  switch (event->getType ())
    {
    case Event::PRESENTATION:
      g_assert (event->isLambda ());
     switch (transition)
        {
        case Event::START:
          Object::doStart ();
          TRACE ("start %s@lambda", _id.c_str ());
          if (_selected == nullptr)
            _doc->evalAction (event, Event::STOP);
          break;

        case Event::STOP:
          Object::doStop ();
          TRACE ("stop %s@lambda", _id.c_str ());
          break;

        default:
          g_assert_not_reached ();
        }
     break;

    case Event::ATTRIBUTION:
    case Event::SELECTION:
    default:
      g_assert_not_reached ();
    }
}


// Public.

const list<pair<Object *, Predicate *>> *
Switch::getRules ()
{
  return &_rules;
}

void
Switch::addRule (Object *obj, Predicate *pred)
{
  g_assert_nonnull (obj);
  g_assert_nonnull (pred);
  _rules.push_back (std::make_pair (obj, pred));
}

GINGA_NAMESPACE_END
