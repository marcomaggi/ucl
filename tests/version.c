/*
  Part of: UCL
  Contents: test for version functions
  Date: Feb 17, 2019

  Abstract

	Test file for version functions.

  Copyright (C) 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ucl.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc UCL_UNUSED, char const * const argv[] UCL_UNUSED)
{
  ucl_library_init();

  printf("version number string: %s\n", ucl_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ucl_version_interface_current(),
	 ucl_version_interface_revision(),
	 ucl_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
