/*  
   Part of: Marco's C Library
   Contents: C source processor that composes stub library files
   Date: Sun Aug 31, 2008
  
   Abstract

  
  
   Copyright (c)  2008  Marco Maggi <marcomaggi@gna.org>
   
   This program is free software:  you can redistribute it and/or modify
   it under the terms of the  GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at
   your option) any later version.
   
   This program is  distributed in the hope that it  will be useful, but
   WITHOUT  ANY   WARRANTY;  without   even  the  implied   warranty  of
   MERCHANTABILITY  or FITNESS FOR  A PARTICULAR  PURPOSE.  See  the GNU
   General Public License for more details.
   
   You should  have received  a copy of  the GNU General  Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
*/


/** ------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------*/

#include "internal.h"

/* ------------------------------------------------------------ */

#ifndef QUOTED_VERSION
#  define QUOTED_VERSION	"0.1d0"
#endif

static void getopts	(int argc, const char *const argv[]);

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Global variables.
 ** ----------------------------------------------------------*/

static const char usage_message[] = "usage: %s [options] FILE ?FILE ...?\n\
Description.\n\
options:\n\
   -v, --verbose         print verbose messages\n\
   -h, --help, --usage   print this screen and exit\n\
       --version         print version informations and exit\n\
       --version-only    print version number and exit\n\
       --license         print license informations\n\
";

static const char usage_examples[] = "template woppa";

static const mcl_main_program_data_t progdata = {
  { "make-stub", "Marco Maggi <marcomaggi@gna.org>", "2008", QUOTED_VERSION }
};

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------*/

int
main (int argc, const char *const argv[])
{
  getopts(argc-1, argv+1);


  exit(EXIT_SUCCESS);
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Options parsing.
 ** ----------------------------------------------------------*/

void
getopts (int argc, const char *const argv[])
{
#define ONUM    9
  static const mcl_getopts_option_t option_array[ONUM] = {
    { 'n', 'i', "interactive" },
    { 'n', 'f', "force" },
    { 'n', 'v', "verbose" },
    { 'n', '\0', "version" },
    { 'n', '\0', "version-only" },
    { 'n', '\0', "license" },
    { 'n', 'h', "help" },
    { 'n', '\0', "usage" },
    { 'n', 'H', "brief-help" }
  };
  enum id { interactive, force, verbose, version, version_only,
	    license, help, usage, brief_help };
  static const mcl_getopts_option_table_t recognised_options = {
    option_array, ONUM
  };

  mcl_getopts_option_array_t    options_to_parse = { argc, argv };
  mcl_location_t		here;
  mcl_getopts_session_t         session;

  ucl_ascii_t                   token;
  ucl_ascii_t                   argument;
  ucl_ascii_t                   option_name;


  if (mcl_location_enter(here))
    {
      mcl_getopts_initialise (session, options_to_parse, recognised_options);
      while (mcl_getopts_next(here, session))
	{
	  switch (mcl_getopts_option_index(session))
	    {
	    case -1:
	      argument = mcl_getopts_get_argument(session);
	      printf("parsed argument: '%s'\n", argument.ptr);
	      break;

	    case interactive:
	    case verbose:
	    case force:
	      option_name = mcl_getopts_get_option_name(session);
	      printf("option with no argument: '%s'\n", option_name.ptr);
	      break;

	    case version:
	      mcl_main_print_version_message(progdata, stderr);
	      exit(EXIT_SUCCESS);

	    case version_only:
	      mcl_main_print_version_only_message(progdata, stderr);
	      exit(EXIT_SUCCESS);

	    case license:
	      mcl_main_print_license_message(progdata, stderr);
	      exit(EXIT_SUCCESS);

	    case help:
	    case brief_help:
	    case usage:
	      mcl_main_print_usage_message(progdata, stderr, usage_message, usage_examples);
	      exit(EXIT_SUCCESS);

	    default:
	      fprintf(stderr, "internal error\n"); 
	      exit(EXIT_FAILURE);
	    }
	}
      mcl_location_leave(here);
    }
  else
    {
      mcl_result_t	result = here->error;

      token = mcl_getopts_get_current_token(session);
      printf("%s: parse error: %s: %s\n", argv[0], result->message, token.ptr);
      mcl_location_cleanup(here);
      exit(EXIT_FAILURE);
    }
}


/* end of file */
