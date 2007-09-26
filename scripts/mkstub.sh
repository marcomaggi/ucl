#!/bin/bash
# mkstub.sh --
# 
# Part of: Marco's C Library
# Contents: builds stub tables for C libraries
# Date: Thu Nov  4, 2004
# 
# Abstract
# 
# 
# Copyright (c) 2004, 2005 Marco Maggi
# 
# This is free  software you can redistribute it  and/or modify it under
# the terms of  the GNU General Public License as  published by the Free
# Software Foundation; either  version 2, or (at your  option) any later
# version.
# 
# This  file is  distributed in  the hope  that it  will be  useful, but
# WITHOUT   ANY  WARRANTY;  without   even  the   implied  warranty   of
# MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
# General Public License for more details.
# 
# You  should have received  a copy  of the  GNU General  Public License
# along with this file; see the file COPYING.  If not, write to the Free
# Software Foundation,  Inc., 59  Temple Place -  Suite 330,  Boston, MA
# 02111-1307, USA.
# 

#page
## ------------------------------------------------------------
## MBFL's related options and variables.
## ------------------------------------------------------------

script_PROGNAME=mkstub.sh
script_VERSION=0.1
script_COPYRIGHT_YEARS='2004, 2005'
script_AUTHOR="Marco Maggi"
script_LICENSE=GPL
script_USAGE="usage: ${script_PROGNAME} [options] FILE FILE ..."
script_DESCRIPTION='Builds stub tables for C language libraries.'

source "${MBFL_LIBRARY:=$(mbfl-config)}"

# keyword default-value brief-option long-option has-argument description

mbfl_declare_option ACTION_PRINT_MODULE no \
    "" module noarg "print module names"
mbfl_declare_option ACTION_PRINT_FUNCTIONS no \
    "" functions noarg "print function names"

mbfl_declare_option ACTION_PRINT_STUB_HEADER no \
    "" make-stub-header noarg "print the stub header"
mbfl_declare_option ACTION_PRINT_STUB_SHARED_LIBRARY yes \
    "" make-stub-shared-library noarg "print stub shared library code"
mbfl_declare_option ACTION_PRINT_STUB_STATIC_LIBRARY no \
    "" make-stub-static-library noarg "print stub static library code"


mbfl_declare_option LIBRARY "LIB" \
    "" library witharg "selects the library identifier (prefix for public names)"
mbfl_declare_option HEADER_FILE '' \
    "" header-file witharg "selects header files to include in stub code"
mbfl_declare_option SHARED_LIBRARY_NAME '' \
    "" shared-library-name witharg "selects the name of the shared library"


mbfl_declare_program gawk
mbfl_declare_program sort

#page
## ------------------------------------------------------------
## Global variable.
## ------------------------------------------------------------

# A space separated list of header file names to include in the
# stub library code.
HEADER_FILES=

# NOTE: all the variables below are exported so that they are
# available in the 'gawk' subprocessed used by this script.

# The name of the source file being parsed. This script parses
# a set of source files at each run, the files are selected from
# the command line ('ARGV' global array); this variable holds the
# name of the file currently being parsed.
export FILENAME

# The identifier of the library. A string to be used as prefix
# to all the function and type names; it is selected from the
# command line with the '--library' option.
export LIBRARY

# The identifier of the library in upper case characters.
export upper_LIBRARY=

# The name of the module being processed. A string used as
# component for names of functions and struct types. It is selected
# in the source file with a comment of the form:
#
#       /* stub module name MODULE */
#
# where 'MODULE' is the module identifier.
export MODULE

# The name of a preprocessor symbol used to include/exclude the
# stub declarations in the library header file. The *client*
# libraries must be compiled with this symbol defined to enable
# the use of the stub mechanism. When not defined: the header file
# defines normal function prototypes; when defined: the header
# file defines the stub table struct types and the preprocessor
# macros used to invoke the functions through pointers in the
# stub tables.
export STUB_ENABLE_DEFINE

# The name of the module stub table struct typedef. This typedef
# is visible in the library header file.
export STUB_TABLE_TYPE

# The name of the module stub table struct variable. A variable
# declared as static in the stub module; it holds pointers to the
# public functions of a module.
export STUB_TABLE_NAME

# The name of the pointer to a module stub table. Defined in the
# stub library: it is used by preprocessor macros to invoke the
# module functions.
export STUB_TABLE_POINTER

# The name of the struct typedef for the main stub table. The main
# stub table holds pointers to the module stub tables.
export MAIN_STUB_TABLE_TYPE

# The name of the main stub table variable. The main stub table
# is declared as a static variable in the library module built
# with the '--stub-table-code' option.
export MAIN_STUB_TABLE_NAME

# The name of the stub initialisation function. The function is
# defined in the stub library and linked in client libraries that
# make use of the stub mechanism. When the client library wants
# to initialise the stub mechanism it invokes this function.
export MAIN_STUB_TABLE_INIT_FUNCTION
export MAIN_STUB_TABLE_FINAL_FUNCTION

# The name of the function used by the stub init function to
# retrieve a pointer to the main stub table. This function is
# defined in the stub module, part of the library; it is the only
# function whose address must be retrieved with a call to 'dlsym()'.
export MAIN_STUB_TABLE_RETRIEVE_FUNCTION



#page
## ------------------------------------------------------------
## Option update functions.
## ------------------------------------------------------------

function script_option_update_header_file () {
    if test -z "${HEADER_FILES}" ; then
        HEADER_FILES=${script_option_HEADER_FILE}
    else
        HEADER_FILES="${HEADER_FILES} ${script_option_HEADER_FILE}"
    fi
}

#page
## ------------------------------------------------------------
## Basic action functions.
## ------------------------------------------------------------

function script_action_print_module () {
    fill_main_variables
    for FILENAME in "${ARGV[@]}" ; do
        fill_module_variables
        printf '%s\n' "${MODULE}"
    done
}
function script_action_print_functions () {
    fill_main_variables
    for FILENAME in "${ARGV[@]}" ; do
        fill_module_variables
        parse_source_file_get_functions <"${FILENAME}"
    done
}

#page
## ------------------------------------------------------------
## Header, stub module and stub library action functions.
## ------------------------------------------------------------

function script_action_print_stub_header () {
    fill_main_variables
    printf '\n\n#ifndef __%s_H\n' "${upper_LIBRARY}_STUB"
    printf '#define __%s_H\n\n\n' "${upper_LIBRARY}_STUB"
    for FILENAME in "${ARGV[@]}" ; do
        fill_module_variables
        printf '/* ------------------------------------------------------------ \n'
        printf '   Module "%s".\n' "${MODULE}"
        printf '   ------------------------------------------------------------ */\n\n'
        header_section__function_typedefs <"${FILENAME}"
        header_section__stub_table_typedef <"${FILENAME}"
        header_section__stub_table_macros <"${FILENAME}"
    done

    printf '/* ------------------------------------------------------------ \n'
    printf '   Main stub table.\n'
    printf '   ------------------------------------------------------------ */\n\n'
    header_section__main_stub_table_defn "${ARGV[@]}"
    printf '\n\n#endif /* defined __%s_H */\n' "${upper_LIBRARY}_STUB"
    printf '\n\n/* end of file */\n'
}
function script_action_print_stub_shared_library () {
    local item
    fill_main_variables

    test -n "${HEADER_FILES}" && printf '/* Header files. */\n\n'
    for item in ${HEADER_FILES} ; do
        printf '#include "%s"\n' "${item}"
    done
    test -n "${HEADER_FILES}" && printf '\n\n'

    for FILENAME in "${ARGV[@]}" ; do
        fill_module_variables
        stub_module_section__module_stub_table_decl <"${FILENAME}"
    done
    stub_module_section__main_stub_table_decl "${ARGV[@]}"
    printf '\n\n/* end of file */\n'
}
function script_action_print_stub_static_library () {
    fill_main_variables

    test -n "${HEADER_FILES}" && printf '/* Library specific header files. */\n\n'
    for item in ${HEADER_FILES} ; do
        printf '#include "%s"\n' "${item}"
    done
    test -n "${HEADER_FILES}" && printf '\n\n'

    make_stub_library "${ARGV[@]}"

    printf '#include <dlfcn.h>

static void *library_handle = NULL;

const char *
%s (void)
{
  const char *	err;
  void * table;

  /* this requires glibc 2.2 or later, see dlopen(3) for details */
  library_handle = dlopen("%s", RTLD_NOLOAD|RTLD_GLOBAL|RTLD_NOW);
  if (NULL == library_handle)
    {
      err = dlerror();
      if (err) return err;
      library_handle = dlopen("%s", RTLD_GLOBAL|RTLD_NOW);
      err = dlerror();
      if (err) return err;
    }
  table = dlsym(library_handle, "%s");
  err = dlerror();
  if (err) { dlclose(library_handle); return err; }

  init_pointers(table);
  return NULL;
}
int
%s (void)
{
  int	errcode;

  if (library_handle)
    {
      errcode = dlclose(library_handle);
      if (0 == errcode) { library_handle = NULL; }
      return errcode;
    }
  return 0;
}
'   "${MAIN_STUB_TABLE_INIT_FUNCTION}" \
    "${script_option_SHARED_LIBRARY_NAME}" \
    "${script_option_SHARED_LIBRARY_NAME}" \
    "${MAIN_STUB_TABLE_NAME}" \
    "${MAIN_STUB_TABLE_FINAL_FUNCTION}"

    printf '\n\n/* end of file */\n'
}

#page
## ------------------------------------------------------------
## Global variables initialisation.
## ------------------------------------------------------------

function fill_main_variables () {
    LIBRARY=${script_option_LIBRARY}
    upper_LIBRARY=$(mbfl_string_toupper ${LIBRARY})

    MAIN_STUB_TABLE_TYPE=$(printf '%s_stub_table_t' "${LIBRARY}")
    MAIN_STUB_TABLE_NAME=$(printf '%s_stub_table' "${LIBRARY}")
    MAIN_STUB_TABLE_RETRIEVE_FUNCTION=$(printf \
        '%s_retrieve_main_stub_table_address' "${LIBRARY}")
    MAIN_STUB_TABLE_INIT_FUNCTION=$(printf '%s_init_stub_table' "${LIBRARY}")
    MAIN_STUB_TABLE_FINAL_FUNCTION=$(printf '%s_final_stub_table' "${LIBRARY}")
}
function fill_module_variables () {
    MODULE=$(parse_source_file_get_module <"${FILENAME}") || exit $?
    STUB_TABLE_TYPE=$(printf '%s_%s_stub_table_t' "${LIBRARY}" "${MODULE}")
    STUB_TABLE_NAME=$(printf '%s_%s_stub_table' "${LIBRARY}" "${MODULE}")
    STUB_TABLE_POINTER=$(printf '%s_%s_stub_table_p' "${LIBRARY}" "${MODULE}")
    STUB_ENABLE_DEFINE=$(printf '%s_ENABLE_STUB' ${upper_LIBRARY})
}

#page
## ------------------------------------------------------------
## Source file parsers.
## ------------------------------------------------------------

function parse_source_file_get_module () {
    local MODULE=$(program_gawk '
/stub[ \t]+module[ \t]+name/ { \
  module_name = gensub(/\/\*[ \t]*stub[ \t]+module[ \t]+name[ \t]+([^ \t]+)[ \t]*\*\/$/,
		       "\\1", 1, $0)
  printf "%s\n", module_name;
  exit
}') || return $?
    if test -z "${MODULE}" ; then
        mbfl_message_error "null module in '${FILENAME}'"
        return 2
    fi
    printf '%s\n' "${MODULE}"
}
function parse_source_file_get_functions () {
    program_gawk '
/^stub\([0-9\-]+\) (.+)$/ {
  printf "function|";

  # Prints the stub timestamp.
  printf "%s|", gensub(/^stub\(([0-9\-]+)\)[ \t]*.+$/, "\\1", 1, $0);
  # Prints the return value.
  printf "%s|", gensub(/^stub\([0-9\-]+\)[ \t]*(.+)[ \t]*$/, "\\1", 1, $0);

  getline;
  # Prints the function name.
  printf "%s|", gensub(/^([^ \t]+)[ \t]*(\(.+\))[ \t]*$/, "\\1", 1, $0);
  # Prints the arguments list including parenthesis.
  printf "%s|", gensub(/^([^ \t]+)[ \t]*(\(.+\))[ \t]*$/, "\\2", 1, $0);

  getline;
  # Prints GCC declaration attributes.
  if ($0 != "{") printf("%s", gensub(/^[ \t]*attribute\(([^\)]+)\).*$/, "\\1", 1, $0));

  print "\n";
}
' | program_sort
}

#page
## ------------------------------------------------------------
## Header section: main stub table type definition.
## ------------------------------------------------------------

function header_section__main_stub_table_defn () {
    local TYPE
    fill_main_variables

    printf 'typedef struct %s {\n' "${MAIN_STUB_TABLE_TYPE}"
    for FILENAME in "$@" ; do
        fill_module_variables
        TYPE=$(printf 'const %s *' "${STUB_TABLE_TYPE}")
        printf '  %-50s%s;\n' "${TYPE}" "${MODULE}"
    done
    printf '} %s;\n\n' "${MAIN_STUB_TABLE_TYPE}"
    printf 'extern const char * %s (void);\n' "${MAIN_STUB_TABLE_INIT_FUNCTION}"
    printf 'extern int %s (void);\n' "${MAIN_STUB_TABLE_FINAL_FUNCTION}"
    printf 'extern const %s %s;\n' "${MAIN_STUB_TABLE_TYPE}" "${MAIN_STUB_TABLE_NAME}"
}

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Header section: a typedef for each function.
## ------------------------------------------------------------

function header_section__function_typedefs () {
    parse_source_file_get_functions | program_gawk '
BEGIN {
  MODULE  = ENVIRON["MODULE"];
  printf "/* Function type declarations for the \"%s\" module.\n   A typedef for each function in the module. */\n\n", MODULE;
  FS="|";
}
/^function|.+$/ {
  timestamp = $2;
  return_value = $3;
  function_name = $4;
  arguments = $5;
  attributes = $6;
#   getline; return_value = $0;
#   getline; function_name = $0;
#   getline; arguments = $0;
  function_type = sprintf("%s_proto_t", function_name);
  left = sprintf("typedef %-10s %s", return_value, function_type);
  printf("%-45s %s;\n", left, arguments);
}
END { printf "\n"; }
' || exit $?
}

#page
## ------------------------------------------------------------
## Header section: a prototype for each function.
## ------------------------------------------------------------

function header_section__function_declarations () {
    parse_source_file_get_functions | program_gawk '
BEGIN {
  DEFINE  = ENVIRON["STUB_ENABLE_DEFINE"];
  MODULE  = ENVIRON["MODULE"];

  printf "/* Function declarations for the \"%s\" module.\n   A prototype for each function in the module. */\n\n", MODULE;
  printf "#ifndef %s\n\n", DEFINE;
  FS="|";
}
/^function|.+$/ {
  timestamp = $2;
  return_value = $3;
  function_name = $4;
  arguments = $5;
  attributes = $6;
  function_type = sprintf("%s_proto_t", function_name);
  if (attributes == "")
     printf("extern %-35s %s;\n", function_type, function_name);
  else
     printf("extern %-35s %s\n\t__attribute__((%s));\n",
            function_type, function_name, attributes);
}
END {
  printf "\n#endif /* not defined %s */\n\n", DEFINE;
}
' || exit $?
}
#page
## ------------------------------------------------------------
## Header section: function stub table typedef.
## ------------------------------------------------------------

function header_section__stub_table_typedef () {
    parse_source_file_get_functions | program_gawk '
BEGIN {
  STUB_TABLE_TYPE = ENVIRON["STUB_TABLE_TYPE"];
  MODULE          = ENVIRON["MODULE"];

  printf "/* Stub table type declaration for the \"%s\" module.\n   A struct holding a pointer for each function in the module. */\n\n", MODULE;
  printf "typedef struct %s {\n", STUB_TABLE_TYPE;
  FS="|";
}
/^function|.+$/ {
  timestamp = $2;
  return_value = $3;
  function_name = $4;
  arguments = $5;
  attributes = $6;
  function_type = sprintf("%s_proto_t", function_name);
  printf("  %-35s * stub_%s;\n", function_type, function_name);
}
END {
  printf "} %s;\n\n", STUB_TABLE_TYPE;
}
' || exit $?
}

#page
## ------------------------------------------------------------
## Header section: a preprocessor macro for each function.
## ------------------------------------------------------------

function header_section__stub_table_macros () {
    parse_source_file_get_functions | program_gawk '
BEGIN {
  STUB_TABLE_NAME = ENVIRON["STUB_TABLE_NAME"];
  STUB_TABLE_TYPE = ENVIRON["STUB_TABLE_TYPE"];
  POINTER         = ENVIRON["STUB_TABLE_POINTER"];
  DEFINE          = ENVIRON["STUB_ENABLE_DEFINE"];
  MODULE          = ENVIRON["MODULE"];

  printf "/* Stub table macros definition for the \"%s\" module.\n   A preprocessor macro for each function in the module,\n   used to invoke the function through the stub table. */\n\n", MODULE;
  printf "#ifdef %s\n\n", DEFINE;
  printf "extern const %s * %s;\n\n", STUB_TABLE_TYPE, POINTER;
  FS="|";
}
/^function|.+$/ {
  timestamp = $2;
  return_value = $3;
  function_name = $4;
  arguments = $5;
  attributes = $6;
  function_type = sprintf("%s_proto_t", function_name);
  printf("#define %-25s  ((%s)->stub_%s)\n", function_name, POINTER, function_name);
}
END {
#  printf "\nextern const * %s %s;\n", STUB_TABLE_TYPE, POINTER;
  printf "\n"
  printf "#endif /* defined %s */\n\n", DEFINE;
}
' || exit $?
}

#page
## ------------------------------------------------------------
## Stub module section: module stub table declaration.
## ------------------------------------------------------------

function stub_module_section__module_stub_table_decl () {
    parse_source_file_get_functions | program_gawk '
BEGIN {
  STUB_TABLE_NAME = ENVIRON["STUB_TABLE_NAME"];
  STUB_TABLE_TYPE = ENVIRON["STUB_TABLE_TYPE"];
  POINTER         = ENVIRON["STUB_TABLE_POINTER"];
  DEFINE          = ENVIRON["STUB_ENABLE_DEFINE"];
  MODULE          = ENVIRON["MODULE"];

  printf "/* Functions stub table for the \"%s\" module. */\n\n", MODULE;
  # The final purpose of the code dealing with this variable is to
  # output the last function in the stub table struct declaration
  # with no comma at the end.
  previous_line = "";

  printf "static const %s %s = {\n", STUB_TABLE_TYPE, STUB_TABLE_NAME;
  FS="|";
}
/^function|.+$/ {
  timestamp = $2;
  return_value = $3;
  function_name = $4;
  arguments = $5;
  attributes = $6;
  function_type = sprintf("%s_proto_t", function_name);

  if (length(previous_line) > 0) printf "%s,\n", previous_line;
  previous_line = sprintf("  %s", function_name);
}
END {
  if (length(previous_line) > 0) printf "%s\n", previous_line;

  printf "};\n";
#  table_type_decl = sprintf("const %s *", STUB_TABLE_TYPE);
#  printf("\n%s %s = &%s;\n", table_type_decl, POINTER, STUB_TABLE_NAME);
  printf("\n");
}
' || exit $?
}
#page
## ------------------------------------------------------------
## Stub module section: main stub table declaration.
## ------------------------------------------------------------

# This should output something like:
#
#   /* Library stub table.
#      A pointer for each module's stub table. */
#
#   const ucl_stub_table_t ucl_stub_table = {
#     &ucl_btree_stub_table,
#     &ucl_base_stub_table,
#     &ucl_circular_stub_table,
#     &ucl_debug_stub_table
#   }

function stub_module_section__main_stub_table_decl () {
    local last_line
    fill_main_variables

    printf '/* Library stub table.\n'
    printf '   A pointer for each module stub table. */\n\n'
    printf 'const %s %s = {\n' "${MAIN_STUB_TABLE_TYPE}" "${MAIN_STUB_TABLE_NAME}"
    
    for FILENAME in "$@" ; do
        fill_module_variables
        test -n "${last_line}" && printf '%s,\n' "${last_line}"
        last_line=$(printf '  &%s' "${STUB_TABLE_NAME}")
    done
    test -n "${last_line}" && printf '%s\n' "${last_line}"
    printf '};\n\n'
}

## ------------------------------------------------------------

#page
## ------------------------------------------------------------
## Stub library.
## ------------------------------------------------------------

function make_stub_library () {
    fill_main_variables

    printf '/* ------------------------------------------------------------
   Stub tables initialiser for "%s".
   ------------------------------------------------------------ */\n\n'

    for FILENAME in "$@" ; do
        fill_module_variables
        printf 'const %s *\t%s;\n' "${STUB_TABLE_TYPE}" "${STUB_TABLE_POINTER}"
    done

    printf '\n\nstatic void\ninit_pointers (%s *table_p)\n{\n' "${MAIN_STUB_TABLE_TYPE}"

    for FILENAME in "$@" ; do
        fill_module_variables
        printf '  %-30s = table_p->%s;\n' "${STUB_TABLE_POINTER}" "${MODULE}"
    done
    printf '\n}\n'
}

#page
## ------------------------------------------------------------
## Program intefaces.
## ------------------------------------------------------------

function program_gawk () {
    local PROGPATH=$(mbfl_program_found gawk)
    mbfl_program_exec "${PROGPATH}" "${@}"
}
function program_sort () {
    local PROGPATH=$(mbfl_program_found sort)
    mbfl_program_exec "${PROGPATH}" "${@}"
}

#page
## ------------------------------------------------------------
## Let's go.
## ------------------------------------------------------------

mbfl_main

### end of file
# Local Variables:
# mode: sh
# End:
