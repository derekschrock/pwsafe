/*
* Copyright (c) 2003-2014 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/

#include "../logit.h"
#include "../../core/PWSLog.h"
#include "../../core/Util.h"
#include "../fmtspecs_cvt.h"

#include <stdio.h>
#include <stdarg.h>


void pws_os::Logit(LPCTSTR lpszHdrFormat, ...)
{
  va_list args;
  va_start(args, lpszHdrFormat);

  int num_required, num_written;

#ifdef UNICODE
  stringT format(lpszHdrFormat); // header format is not converted
  const stringT format2(FormatStr(va_arg(args, LPCTSTR)));
  format += format2;

  num_required = GetStringBufSize(format.c_str(), args);
  va_end(args);  // after using args we should reset list
  va_start(args, lpszHdrFormat);
  va_arg(args, LPCTSTR); // throw away - we alread have it.

  wchar_t *szBuffer = new wchar_t[num_required];
  num_written = vswprintf(szBuffer, num_required, format.c_str(), args);
  assert(num_required == num_written + 1);
  szBuffer[num_required - 1] = L'\0';
#else
  num_required = GetStringBufSize(lpszFormat, args);
  va_end(args);  // after using args we should reset list
  va_start(args, lpszFormat);

  char *szBuffer = new char[num_required];
  num_written = vsnprintf(szBuffer, num_required, lpszFormat, args);
  assert(num_required == num_written+1);
  szBuffer[num_required - 1] = '\0';
#endif /* UNICODE */
  UNREFERENCED_PARAMETER(num_written); // used only in assert
  const stringT s(szBuffer);
  PWSLog::GetLog()->Add(s);
  delete[] szBuffer;

  va_end(args);
}
