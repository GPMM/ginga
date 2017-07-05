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

#include "ginga.h"
#include "CompoundCondition.h"

GINGA_PRAGMA_DIAG_IGNORE (-Wsign-conversion)

GINGA_NCL_BEGIN

CompoundCondition::CompoundCondition () : TriggerExpression ()
{
  _expressions = new vector<Condition *>;
}

CompoundCondition::CompoundCondition (Condition *c1,
                                      Condition *c2, short op)
    : TriggerExpression ()
{
  _expressions = new vector<Condition *>;
  _expressions->push_back (c1);
  _expressions->push_back (c2);
  _myOperator = op;
}

CompoundCondition::~CompoundCondition ()
{
  vector<Condition *>::iterator i;

  if (_expressions != NULL)
    {
      i = _expressions->begin ();
      while (i != _expressions->end ())
        {
          delete *i;
          ++i;
        }

      delete _expressions;
      _expressions = NULL;
    }
}

void
CompoundCondition::setOperator (short op)
{
  _myOperator = op;
}

short
CompoundCondition::getOperator ()
{
  return _myOperator;
}

vector<Condition *> *
CompoundCondition::getConditions ()
{
  if (_expressions->empty ())
    return NULL;

  return _expressions;
}

void
CompoundCondition::addCondition (Condition *condition)
{
  if (condition == NULL)
    {
      clog << "CompoundCondition::addCondition ";
      clog << "Warning! Trying to add a NULL condition." << endl;
    }
  else
    {
      _expressions->push_back (condition);
    }
}

void
CompoundCondition::removeCondition (
    Condition *condition)
{
  vector<Condition *>::iterator iterator;
  vector<Condition *>::iterator i;

  iterator = _expressions->begin ();
  while (iterator != _expressions->end ())
    {
      if ((*iterator) == condition)
        {
          i = _expressions->erase (iterator);
          if (i == _expressions->end ())
            return;
        }
      ++iterator;
    }
}

vector<Role *> *
CompoundCondition::getRoles ()
{
  vector<Role *> *roles;
  int i, size;
  Condition *condition;
  vector<Role *> *childRoles;

  roles = new vector<Role *>;
  size = (int) _expressions->size ();
  for (i = 0; i < size; i++)
    {
      condition = (*_expressions)[i];
      if (condition == NULL)
        {
          clog << "CompoundCondition::getRoles ";
          clog << "Warning! condition = NULL." << endl;
        }
      else if (instanceof (SimpleCondition *, condition))
        {
          roles->push_back ((SimpleCondition *)condition);
        }
      else
        {
          if (instanceof (CompoundCondition *, condition))
            {
              childRoles = ((CompoundCondition *)condition)->getRoles ();
            }
          else if (instanceof (AssessmentStatement *, condition))
            {
              childRoles = ((AssessmentStatement *)condition)->getRoles ();
            }
          else
            { // ICompoundStatement
              childRoles = ((CompoundStatement *)condition)->getRoles ();
            }

          vector<Role *>::iterator it;
          for (it = childRoles->begin (); it != childRoles->end (); ++it)
            {
              roles->push_back (*it);
            }

          delete childRoles;
        }
    }
  return roles;
}

GINGA_NCL_END
