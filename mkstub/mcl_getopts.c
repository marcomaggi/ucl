/* mcl_getopts.c --

   Part of: Marco's C Library
   Contents: parser functions for command line options
   Date: Oct 1, 2001

   Abstract:



   Copyright (c)  2001, 2002, 2003, 2004, 2005, 2007  Marco Maggi
   
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
 ** Header files.
 ** ----------------------------------------------------------*/

#define DEBUGGING		0
#include "internal.h"

/* stub module name getopts */

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Defines
 ** ----------------------------------------------------------*/

#define has_arg(index)		(session->table.options[index].has_arg == 'y')
#define end_of_opts(token)	(strcmp(token, "--") == 0)

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Static variables.
 ** ----------------------------------------------------------*/

stub(2007-09-19-11-25-58) mcl_result_t
mcl_result_getopts (void)
{
  static mcl_result_tag_t result = { MCL_KEY_RESULT, MCL_GETOPTS_BAD_ALLOC, "" };

  return &result;
}
stub(2007-09-19-11-26-34) mcl_result_t
mcl_result_getopts_unrecognised_option (void)
{
  static mcl_result_tag_t result = {
    MCL_KEY_RESULT, MCL_GETOPTS_UNRECOGNISED_OPTION, "unrecognised option"
  };

  return &result;
}
stub(2007-09-19-11-26-36) mcl_result_t
mcl_result_getopts_syntax_error (void)
{
  static mcl_result_tag_t result = {
    MCL_KEY_RESULT, MCL_GETOPTS_SYNTAX_ERROR, "syntax error"
  };

  return &result;
}
stub(2007-09-19-11-26-37) mcl_result_t
mcl_result_getopts_option_requires_value (void)
{
  static mcl_result_tag_t result = {
    MCL_KEY_RESULT, MCL_GETOPTS_OPTION_REQUIRES_VALUE, "option requires value"
  };

  return &result;
}
stub(2007-09-19-11-26-38) mcl_result_t
mcl_result_getopts_option_with_no_value (void)
{
  static mcl_result_tag_t result = {
    MCL_KEY_RESULT, MCL_GETOPTS_OPTION_WITH_NO_VALUE, "option doesn't require value"
  };

  return &result;
}
stub(2007-09-19-11-26-38) mcl_result_t
mcl_result_getopts_train_with_value (void)
{
  static mcl_result_tag_t result = {
    MCL_KEY_RESULT, MCL_GETOPTS_TRAIN_WITH_VALUE, "separate value from option train please"
  };

  return &result;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Helper functions.
 ** ----------------------------------------------------------*/

/* Scan  'table' looking for  a brief  option. Return  the index  of the
   option or '-1' if the option is not found. */
static int
getopts_find_brief(const mcl_getopts_option_t *table, int count, char brief)
{
  int i;

  for (i=0; i<count; ++i)
    {
      if (table[i].brief != '\0' && table[i].brief == brief)
	{
	  return i;
	}
    }
  return -1;

}
/* Scan  'table' looking  for a  long option.  Return the  index  of the
   option or '-1' if the option is not found. */
static int
getopts_find_long (const mcl_getopts_option_t *table, int count,
		   const char *name, size_t len)
{
  int i;

  debug("name len %d, name '%s'; table len %d", len, name, count);
  for (i=0; ((i<count) && (table[i].has_arg!='\0')); ++i)
    {
      debug("table name len %d, table name %s", strlen(table[i].name), table[i].name);
      if ((strlen(table[i].name) == len) &&
	  (strncmp(table[i].name, name, len) == 0))
	{
	  return i;
	}
    }
  return -1;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------*/

stub(2005-10-13-18-35-59) void
mcl_getopts_initialise (mcl_getopts_session_t session, mcl_getopts_option_array_t options_to_parse, const mcl_getopts_option_table_t table)
{
  session->options_to_parse = options_to_parse;
  session->table	= table;

  session->state.eop	= 0;
  session->state.in	= 0;
  session->i		= 0;
  session->j		= 0;
  session->string	= 0;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Parsing.
 ** ----------------------------------------------------------*/

stub(2005-10-13-18-36-05) int
mcl_getopts_next (mcl_location_t there, mcl_getopts_session_t session)
{
  const char *		token;
  char *		h;
  size_t		size, j;
  size_t		i;
  int			k;
  char			ch;
  

  i = session->i;
  debug("mark %d, %d", i, session->options_to_parse.optc);
  /*
   * more tokens to parse?
   */
  if (i == session->options_to_parse.optc)
    {
      debug("no options left to parse (last is %d)", session->options_to_parse.optc);
      return 0;
    }

  j	= session->j;
  token = session->options_to_parse.optv[i];
  size	= strlen(token);

  /*
   * in the middle of a token parsing?
   */
  if (session->state.in)
    {
      goto Token_parsing;
    }

  /*
   * an end-of-option was found?
   */
  if (session->state.eop)
    {
      /* signal an argument */
      session->index	= -1;
      /* store the argument ptr */
      session->string	= token;
      session->len	= size;
      /* increment the index for the next round */
      ++( session->i );
      return 1;
    }

  /*
   * the token is "--"?
   */
  if (end_of_opts(token))
    {
      ++i;
      if (i == session->table.options_number)
	{
	  /* the last token on the command line is "--", weird... */
	  return 0;
	}
      else
	{
	  /* no more options */
	  session->state.eop	= 1;
	  /* signal an argument */
	  session->index	= -1;
	  /* store the argument ptr */
	  session->string	= session->options_to_parse.optv[i];
	  session->len		= strlen(session->options_to_parse.optv[i]);
	  /* increment the index for the next round */
	  session->i		+= 2;
	  return 1;
	}
    }


  /*
   * an option has at least 2 chars and begins with a dash; if it's not
   * so, this token is an argument
   */
  if (size == 1 || token[0] != '-')
    {
      /* signal an argument */
      session->index	= -1;
      /* store the argument ptr */
      session->string	= token;
      session->len	= size;
      /* increment the index for the next round */
      ++( session->i );
      return 1;
    }
  

  /*
   * here: (size > 1) && (token[0] == '-')
   */


  /*
   * is it a long option?
   */
  if (token[0] == '-' && token[1] == '-')
    {
      h = strchr(&(token[2]), '=');
      k = getopts_find_long(session->table.options, session->table.options_number,
			    &(token[2]),
			    ((0==h)? (size-2) : (size_t)(h-&(token[2]))));

      /* if unrecognised, signal the error */
      if (k < 0)
	{
	  /*
	   * store  the pointer to  the offending option,  including the
	   * leading "--"
	   */
	  session->string	= token;
	  session->len		= (0==h)? size : (size_t)(h - token);

	  mcl_location_error(there, mcl_result_getopts_unrecognised_option());
	}

      /* 
       * argument required?
       */
      if (has_arg(k))
	{
	  /*
	   * if no '=' in the token, look for the next token
	   */
	  if (h == 0)
	    {
	      if (i+1 < session->options_to_parse.optc)
		{
		  /*
		   * found  an option with  value; store the  pointer to
		   * the value string and the index of the option
		   */
		  ++i;
		  session->string	= session->options_to_parse.optv[i];
		  session->len		= strlen(session->options_to_parse.optv[i]);
		  session->index	= k;
		  /* increment the index for the next round */
		  session->i += 2;
		  return 1;
		}
	      else
		{
		  /*
		   * error:  an argument was  expected but there  are no
		   *  more  tokens; store  the pointer to  the offending
		   *  option, including the leading "--"
		   */
		  session->string	= token;
		  session->len		= size;
		  mcl_location_error(there, mcl_result_getopts_option_requires_value());
		}
	    }
	  else
	    {
	      /*
	       * found a '=' in the token; store the pointer to the rest
	       * of the string as the option value; if this is NULL, the
	       * option value is the empty string
	       */
	      ++h;
	      session->string	= h;
	      session->len	= strlen(h);
	      session->index	= k;
	      ++( session->i );
	      return 1;
	    }
	}
      else
	{
	  if (h == 0)
	    {
	      /*
	       * found a long option with no value
	       */
	      session->string	= 0;
	      session->len	= 0;
	      session->index	= k;
	      /* increment the index forthe next round */
	      ++( session->i );
	      return 1;
	    }
	  else
	    {
	      /*
	       * error: this is an option but it doesn't require value
	       */
	      session->string	= token;
	      session->len	= h - token;

	      mcl_location_error(there, mcl_result_getopts_option_with_no_value());
	    }
	}
    }

  /*
   * maybe this  is a brief option;  if size==2, this may be  a * single
   * brief option  (-h); else can it  can be a train of  brief * options
   * (-hif) or a brief option with the option value attached * (-afile)
   */

 Token_parsing:

  j = (session->state.in)? session->j : 1;

  for (; j<size; ++j)
    {
      ch = token[j];
      
      k = getopts_find_brief(session->table.options, session->table.options_number, token[j]);
      if (k < 0)
	{
	  /*
	   * unrecognised option
	   */
	  session->len		= 1;
	  session->string	= &(token[j]);

	  mcl_location_error(there, mcl_result_getopts_unrecognised_option());
	}
      else
	{
	  /*
	   * found a brief option; require value?
	   */
	  if (has_arg(k))
	    {
	      if (j > 1 && j+1 != size)
		{
		  /*
		   * error: this brief option require value but it's not
		   * a  single option with value  attached (-Ovalue) and
		   * neither  the last in  the token (-abO  value); this
		   * could be correct but  it's difficult to read, so we
		   * forbid it
		   */
		  session->string	= token;
		  session->len		= size;

		  mcl_location_error(there, mcl_result_getopts_train_with_value());
		}
	      else if (j+1 < size)
		{
		  /*
		   * found a brief option with attached value (-Ovalue)
		   */
		  session->string	= &(token[j+1]);
		  session->len		= strlen(&(token[j+1]));
		  session->index	= k;
		  /* increment the index for the next round, the parsing
                     of brief options is over */
		  ++(session->i);
		  session->j		= 0;
		  session->state.in	= 0;
		  return 1;
		}
		

	      if (i+1 < session->options_to_parse.optc)
		{
		  /*
		   * found a brief option with value in the next token
		   */
		  ++i;
		  session->string	= session->options_to_parse.optv[i];
		  session->len		= strlen(session->options_to_parse.optv[i]);
		  session->index	= k;
		  /* increment the index for the next round, the parsing
                     of brief options is over */
		  session->i		+= 2;
		  session->j		= 0;
		  session->state.in	= 0;
		  return 1;
		}
	      else
		{
		  /*
		   * error:  an argument was  expected but there  are no
		   *  more  tokens; store  the pointer to  the offending
		   *  option char
		   */
		  session->string	= &(token[1]);
		  session->len		= 1;

		  mcl_location_error(there, mcl_result_getopts_option_requires_value());
		}
	    }
	  else
	    {
	      /*
	       * found a brief option with no value
	       */
	      session->string	= 0;
	      session->len	= 0;
	      session->index	= k;
	      /*
	       * check if  this is the last char in the  token, if it is
	       * the parsing of the token is over
	       */
	      if (j+1 == size)
		{
		  ++( session->i );
		  session->j		= 0;
		  session->state.in	= 0;
		}
	      else
		{
		  session->j		= j+1;
		  session->state.in	= 1;
		}
	      return 1;
	    }
	}
    }
  
  return 1;
}

/* ------------------------------------------------------------ */


/** ------------------------------------------------------------
 ** Inspection.
 ** ----------------------------------------------------------*/

stub(2005-11-27-09-39-18) int
mcl_getopts_option_index (mcl_getopts_session_t session)
{
  return session->index;
}
stub(2005-10-13-18-36-13) ucl_ascii_t
mcl_getopts_get_option_name (mcl_getopts_session_t session)
{
  ucl_ascii_t	name;

  ucl_ascii_const(name, (session)->table.options[(session)->index].name);
  return name;
}
stub(2005-11-27-09-53-50) ucl_ascii_t
mcl_getopts_get_argument (mcl_getopts_session_t session)
{
  ucl_ascii_t	argument;

  ucl_ascii_const(argument, session->string);
  return argument;
}
stub(2005-11-27-08-07-44) ucl_ascii_t
mcl_getopts_get_current_token (mcl_getopts_session_t session)
{
  ucl_ascii_t	token;

  ucl_ascii_set(token, (char *)(session->options_to_parse.optv[session->i]), session->len);
  return token;
}


/* end of file */
