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

// clang-format off
GINGA_PRAGMA_DIAG_IGNORE (-Wfloat-equal)
// clang-format on

int
main (void)
{
  g_assert_cmpfloat (xstrtod ("1"), ==, 1.);
  g_assert_cmpfloat (xstrtod ("   1"), ==, 1.);
  g_assert_cmpfloat (xstrtod ("50"), ==, 50.);
  g_assert_cmpfloat (xstrtod ("1   "), ==, 1.);
  exit (EXIT_SUCCESS);
}
