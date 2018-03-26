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

#include "tests.h"

// Remember to do more thorough testing on every component

int
main (void)
{
  Formatter *fmt;

  // -------------------------------------------------------------------------
  // Sanity checks.
  // -------------------------------------------------------------------------

  // Success: Context
  {
    Formatter *fmt;
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl =\
{'context', 'ncl', nil,\n                    \
   {\n                                       \
      {'context', 'c1', nil,\n               \
       {{'media', 'm1'}}},\n                 \
      {'media', 'm2'}\n                      \
   }\n                                       \
}\n                                          \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 5);
    g_assert (doc->getMedias ()->size () == 3);
    g_assert (doc->getContexts ()->size () == 2);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    Context *c1 = cast (Context *, body->getChildById ("c1"));
    g_assert_nonnull (c1);
    Event *c1_lambda = c1->getLambda ();
    g_assert_nonnull (c1_lambda);

    Media *m1 = cast (Media *, c1->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    Media *m2 = cast (Media *, body->getChildById ("m2"));
    g_assert_nonnull (m2);
    Event *m2_lambda = m2->getLambda ();
    g_assert_nonnull (m2_lambda);

    delete doc;
  }

  // Success: Switch
  {
    Formatter *fmt;
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl =\
{'context', 'ncl', nil,\n                    \
   {\n                                       \
      {'switch', 's1',\n                     \
       {{'media', 'm1'}}},\n                 \
      {'media', 'm2'}\n                      \
   }\n                                       \
}\n                                          \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 5);
    g_assert (doc->getMedias ()->size () == 3);
    g_assert (doc->getContexts ()->size () == 1);
    g_assert (doc->getSwitches ()->size () == 1);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    Switch *s1 = cast (Switch *, body->getChildById ("s1"));
    g_assert_nonnull (s1);
    Event *s1_lambda = s1->getLambda ();
    g_assert_nonnull (s1_lambda);

    Media *m1 = cast (Media *, s1->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    Media *m2 = cast (Media *, body->getChildById ("m2"));
    g_assert_nonnull (m2);
    Event *m2_lambda = m2->getLambda ();
    g_assert_nonnull (m2_lambda);

    delete doc;
  }

  // Success: Media
  {
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl =\
{'context', 'ncl', nil,\n                    \
   {\n                                       \
      {'media', 'm1'},\n                     \
      {'media', 'm2', nil,\n                 \
       {a1={'5s', '8s'},\n                   \
        a2={'1s', nil}}}\n                   \
   }\n                                       \
}\n                                          \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 4);
    g_assert (doc->getMedias ()->size () == 3);
    g_assert (doc->getContexts ()->size () == 1);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    Media *m1 = cast (Media *, body->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    Media *m2 = cast (Media *, body->getChildById ("m2"));
    g_assert_nonnull (m2);
    Event *m2_lambda = m2->getLambda ();
    g_assert_nonnull (m2_lambda);
    Event *m2_a1 = m2->getPresentationEvent ("a1");
    g_assert_nonnull (m2_a1);
    Event *m2_a2 = m2->getPresentationEvent ("a2");
    g_assert_nonnull (m2_a2);

    delete doc;
  }

  // Success: Link
  {
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl = \
{'context', 'ncl', nil,\n                     \
   {\n                                        \
      {'media', 'm1'},\n                      \
   },\n                                       \
   {\n                                        \
      {{{'start', 'm1@lambda', {true}}},\n    \
      {{'stop', 'm1@lambda'}}}\n              \
   }\n                                        \
}\n                                           \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 3);
    g_assert (doc->getMedias ()->size () == 2);
    g_assert (doc->getContexts ()->size () == 1);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    g_assert (body->getLinks ()->size () == 1);

    Media *m1 = cast (Media *, body->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    delete doc;
  }

  // Success: Predicate
  {
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl = \
{'context', 'ncl', nil,\n                     \
   {\n                                        \
      {'media', 'm1'},\n                      \
   },\n                                       \
   {\n                                        \
      {{{'start', 'm1@lambda', {true}}},\n    \
      {{'stop', 'm1@lambda'}}}\n              \
   }\n                                        \
}\n                                           \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 3);
    g_assert (doc->getMedias ()->size () == 2);
    g_assert (doc->getContexts ()->size () == 1);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    g_assert (body->getLinks ()->size () == 1);

    Media *m1 = cast (Media *, body->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    list<Action> cond = body->getLinks ()->front ().first;
    Predicate *pred = cond.front ().predicate;

    g_assert (pred->getType () == Predicate::VERUM);

    delete doc;
  }

  // Success: Switch rule
  {
    Formatter *fmt;
    Document *doc;

    tests_parse_and_start (&fmt, &doc, "ncl =\
{'context', 'ncl', nil,\n                    \
   {\n                                       \
      {'switch', 's1',\n                     \
       {{'media', 'm1'}},\n                  \
       {{'m1', {false}}}\n                   \
      },\n                                   \
      {'media', 'm2'}\n                      \
   }\n                                       \
}\n                                          \
return ncl",
"lua");

    g_assert_nonnull (doc);
    g_assert (doc->getObjects ()->size () == 5);
    g_assert (doc->getMedias ()->size () == 3);
    g_assert (doc->getContexts ()->size () == 1);
    g_assert (doc->getSwitches ()->size () == 1);

    Context *body = cast (Context *, doc->getRoot ());
    g_assert_nonnull (body);
    Event *body_lambda = body->getLambda ();
    g_assert_nonnull (body_lambda);

    Switch *s1 = cast (Switch *, body->getChildById ("s1"));
    g_assert_nonnull (s1);
    Event *s1_lambda = s1->getLambda ();
    g_assert_nonnull (s1_lambda);

    g_assert (s1->getRules ()->size () == 1);

    Media *m1 = cast (Media *, s1->getChildById ("m1"));
    g_assert_nonnull (m1);
    Event *m1_lambda = m1->getLambda ();
    g_assert_nonnull (m1_lambda);

    Media *m2 = cast (Media *, body->getChildById ("m2"));
    g_assert_nonnull (m2);
    Event *m2_lambda = m2->getLambda ();
    g_assert_nonnull (m2_lambda);

    Predicate *rule = s1->getRules ()->front ().second;

    g_assert (rule->getType () == Predicate::FALSUM);

    delete doc;
  }

  exit (EXIT_SUCCESS);
}
