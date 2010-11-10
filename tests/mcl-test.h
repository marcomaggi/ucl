/*
  Part of: Marco's C Library
  Contents: test header
  Date: Sat Nov 26, 2005

  Abstract

	Common infrastructure for testing.

  This program is  free software: you can redistribute  it and/or modify
  it under the  terms of the GNU General Public  License as published by
  the Free Software Foundation, either  version 3 of the License, or (at
  your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have received  a copy  of the  GNU General  Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MCL_TEST_H
#define MCL_TEST_H 1



#include <mcl-common.h>
#include <mcl-debug.h>

#ifndef MCL_TEST_FPRINTF
#  define MCL_TEST_FPRINTF		fprintf
#endif

#ifndef MCL_TEST_VFPRINTF
#  define MCL_TEST_VFPRINTF		vfprintf
#endif



/*
  "test_result == 1": an error occurred
  "test_result == 0": success
 */
int		mcl_test_result	= 0;
const char *	mcl_test_name	= NULL;

bool		mcl_test_doit	= true;

/* ------------------------------------------------------------ */

#define mcl_test_report(...)	MCL_TEST_FPRINTF(stderr,__VA_ARGS__)

static void MCL_UNUSED
MCL_TEST_TITLE (const char * template, ...)
{
  va_list	ap;
  va_start(ap, template);
  MCL_TEST_FPRINTF(stderr, "\n****** ");
  MCL_TEST_VFPRINTF(stderr, template, ap);
  MCL_TEST_FPRINTF(stderr, "\n");
  va_end(ap);
}
#define mcl_test_title(...)	MCL_TEST_TITLE(__VA_ARGS__)

static void MCL_UNUSED
MCL_TEST_SUBTITLE (const char * template, ...)
{
  va_list	ap;
  va_start(ap, template);
  MCL_TEST_FPRINTF(stderr, "\n*** ");
  MCL_TEST_VFPRINTF(stderr, template, ap);
  MCL_TEST_FPRINTF(stderr, "\n");
  va_end(ap);
}
#define mcl_test_subtitle(...)	MCL_TEST_SUBTITLE(__VA_ARGS__)

/* ------------------------------------------------------------------ */

static bool MCL_UNUSED
MCL_TEST_BEGIN (const char * name, const char * description)
{
  const char *	selected_name	= getenv("name");
  if ((NULL == selected_name) || strstr(name, selected_name)) {
    mcl_test_doit	= true;
    mcl_test_result	= 0;
    mcl_test_name	= name;
    if (MCL_DEBUGGING)
      mcl_test_report("%s: %-50s...\n", mcl_test_name, description);
    else
      mcl_test_report("%s: %-50s... ", mcl_test_name, description);
  } else
    mcl_test_doit	= false;
  return mcl_test_doit;
}
#define mcl_test_begin(NAME,DESCR)	if (MCL_TEST_BEGIN(NAME,DESCR))

static void MCL_UNUSED
MCL_TEST_END (const char * funcname)
{
  if (mcl_test_doit) {
    if (mcl_test_result)
      mcl_test_report("%s (%s): %s\n", mcl_test_name, funcname, "*** FAILED");
    else
      mcl_test_report("good\n");
  }
  mcl_test_doit = true;
}
#define mcl_test_end()			MCL_TEST_END(__func__)

/* ------------------------------------------------------------------ */

static void MCL_UNUSED
mcl_test_error (const char * template, ...)
{
  va_list	ap;
  va_start(ap, template);
  MCL_TEST_VFPRINTF(stderr, template, ap);
  mcl_test_result=1;
  va_end(ap);
}

static void MCL_UNUSED
MCL_TEST_ERROR_IF_TRUE (bool expr, const char * template, ...)
{
  if (expr) {
    va_list	ap;
    va_start(ap, template);
    {
      mcl_test_report("\n\terror: ");
      MCL_TEST_VFPRINTF(stderr, template, ap);
      mcl_test_report("\n");
      mcl_test_result=1;
    }
    va_end(ap);
  }
}

#define mcl_test_error_if_true(EXPR,...)		\
							\
  MCL_TEST_ERROR_IF_TRUE((EXPR), __VA_ARGS__)

static void MCL_UNUSED
MCL_TEST_ERROR_IF_FALSE (bool expr, const char * template, ...)
{
  if (!expr) {
    va_list	ap;
    va_start(ap, template);
    {
      mcl_test_report("\n\terror: ");
      MCL_TEST_VFPRINTF(stderr, template, ap);
      mcl_test_report("\n");
      mcl_test_result=1;
    }
    va_end(ap);
  }
}

#define mcl_test_error_if_false(EXPR,...)		\
							\
  MCL_TEST_ERROR_IF_FALSE((EXPR), __VA_ARGS__)



#endif /* MCL_TEST_H */

/* end of file */
