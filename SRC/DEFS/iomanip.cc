/***************************************************************************
 *
 * iomanip.cc - Definitions for the Standard Library iomanip
 *
 * $Id: iomanip.cc,v 1.5 1995/04/21 22:12:16 smithey Exp $
 *
 ***************************************************************************
 *
 * (c) Copyright 1994, 1995 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 **************************************************************************/

#ifndef RW_COMPILE_INSTANTIATE
#ifndef RW_NO_NAMESPACE
namespace std {
#endif
#endif

/*
 * class imanip
 */

template<class T>
imanip<T>::imanip(istream& (*pf)(istream&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class iapp
 */

template<class T>
iapp<T>::iapp(istream& (*pf)(istream&, T))
: pf_(pf)
{

}

template<class T>
imanip<T> iapp<T>::operator()(T t)
{
  return imanip<T>(pf_, t);
}


/*
 * class omanip
 */
#if 0
template<class T>
omanip<T>::omanip(ios& (*pf)(ios&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}
#endif
template<class T>
omanip<T>::omanip(ostream& (*pf)(ostream&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class oapp
 */
#if 0
template<class T>
oapp<T>::oapp(ios& (*pf)(ios&, T))
: pf_(pf)
{

}
#endif
template<class T>
oapp<T>::oapp(ostream& (*pf)(ostream&, T))
: pf_(pf)
{

}

template<class T>
omanip<T> oapp<T>::operator()(T t)
{
  return omanip<T>(pf_, t);
}

/*
 * class smanip
 */

template<class T>
smanip<T>::smanip(ios& (*pf)(ios&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class sapp
 */

template<class T>
sapp<T>::sapp(ios& (*pf)(ios&, T))
: pf_(pf)
{

}

template<class T>
smanip<T> sapp<T>::operator()(T t)
{
  return smanip<T>(pf_, t);
}

/*
 * global shift operators for manipulators
 */

template<class T>
ostream& operator<<(ostream& os, const omanip<T>& a)
{
  try {
    (*a.pf_)(os, a.manarg_);
  }
  catch(...) {
    os.setstate(ios::failbit);
  }

  return os;
}

template<class T>
istream& operator>>(istream& is, const imanip<T>& a)
{
  try {
    (*a.pf_)(is, a.manarg_);
  }
  catch(...) {
    is.setstate(ios::failbit);
  }
  
  return is;
}

template<class T>
istream& operator>>(istream& is, const smanip<T>& a)
{
  try {
    (*a.pf_)(is, a.manarg_);
  }
  catch(...) {
    is.setstate(ios::failbit);
  }
  
  return is;
}

template<class T>
ostream& operator<<(ostream& os, const smanip<T>& a)
{
  try {
    (*a.pf_)(os, a.manarg_);
  }
  catch(...) {
    os.setstate(ios::failbit);
  }
  
  return os;
}

/*
 * global manipulators
 */

ios& rsios(ios& str, ios::fmtflags mask)
{
  str.setf((ios::fmtflags)0, mask);

  return str;
}

smanip<ios::fmtflags> resetiosflags(ios::fmtflags mask)
{
  return smanip<ios::fmtflags>(rsios, mask);
}

ios& sios(ios& str, ios::fmtflags mask)
{
  str.setf(mask);

  return str;
}

smanip<ios::fmtflags> setiosflags(ios::fmtflags mask)
{
  return smanip<ios::fmtflags>(sios, mask);
}

ios& sbase(ios& str, int base)
{
  str.setf(base == 8 ? ios::oct :
	   base == 10 ? ios::dec :
	   base == 16 ? ios::hex :
	   ios::fmtflags(0), ios::basefield);

  return str;
}

smanip<int> setbase(int base)
{
  return smanip<int>(sbase, base);
}

ios& sfill(ios& str, int c)
{
  str.fill(c);

  return str;
}

smanip<int> setfill(int c)
{
  return smanip<int>(sfill, c);
}

ios& sprec(ios& str, int n)
{
  str.precision(n);

  return str;
}

smanip<int> setprecision(int n)
{
  return smanip<int>(sprec, n);
}

ios& swidth(ios& str, int n)
{
  str.width(n);

  return str;
}

smanip<int> setw(int n)
{
  return smanip<int>(swidth, n);
}


#ifndef RW_NO_WIDE_CHAR

/*
 * class wimanip
 */

template<class T>
wimanip<T>::wimanip(wistream& (*pf)(wistream&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class wiapp
 */

template<class T>
wiapp<T>::wiapp(wistream& (*pf)(wistream&, T))
: pf_(pf)
{

}

template<class T>
wimanip<T> wiapp<T>::operator()(T t)
{
  return wimanip<T>(pf_, t);
}


/*
 * class womanip
 */
#if 0
template<class T>
womanip<T>::womanip(wios& (*pf)(wios&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}
#endif
template<class T>
womanip<T>::womanip(wostream& (*pf)(wostream&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class woapp
 */
#if 0
template<class T>
woapp<T>::woapp(ios& (*pf)(wios&, T))
: pf_(pf)
{

}
#endif
template<class T>
woapp<T>::woapp(wostream& (*pf)(wostream&, T))
: pf_(pf)
{

}

template<class T>
womanip<T> woapp<T>::operator()(T t)
{
  return womanip<T>(pf_, t);
}

/*
 * class wsmanip
 */

template<class T>
wsmanip<T>::wsmanip(wios& (*pf)(wios&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class wsapp
 */

template<class T>
wsapp<T>::wsapp(wios& (*pf)(wios&, T))
: pf_(pf)
{

}

template<class T>
wsmanip<T> wsapp<T>::operator()(T t)
{
  return wsmanip<T>(pf_, t);
}

/*
 * global shift operators for manipulators
 */

template<class T>
wostream& operator<<(wostream& os, const womanip<T>& a)
{
  try {
	 (*a.pf_)(os, a.manarg_);
  }
  catch(...) {
	 os.setstate(ios::failbit);
  }

  return os;
}

template<class T>
wistream& operator>>(wistream& is, const wimanip<T>& a)
{
  try {
	 (*a.pf_)(is, a.manarg_);
  }
  catch(...) {
	 is.setstate(ios::failbit);
  }

  return is;
}

template<class T>
wistream& operator>>(wistream& is, const wsmanip<T>& a)
{
  try {
	 (*a.pf_)(is, a.manarg_);
  }
  catch(...) {
	 is.setstate(ios::failbit);
  }

  return is;
}

template<class T>
wostream& operator<<(wostream& os, const wsmanip<T>& a)
{
  try {
	 (*a.pf_)(os, a.manarg_);
  }
  catch(...) {
	 os.setstate(ios::failbit);
  }

  return os;
}

/*
 * global manipulators
 */

wios& rsios(wios& str, wios::fmtflags mask)
{
  str.setf((wios::fmtflags)0, mask);

  return str;
}

wsmanip<wios::fmtflags> wresetiosflags(wios::fmtflags mask)
{
  return wsmanip<wios::fmtflags>(rsios, mask);
}

wios& sios(wios& str, wios::fmtflags mask)
{
  str.setf(mask);

  return str;
}

wsmanip<wios::fmtflags> wsetiosflags(wios::fmtflags mask)
{
  return wsmanip<wios::fmtflags>(sios, mask);
}

wios& sbase(wios& str, int base)
{
  str.setf(base == 8 ? wios::oct :
		base == 10 ? wios::dec :
		base == 16 ? wios::hex :
		ios::fmtflags(0), wios::basefield);

  return str;
}

wsmanip<int> wsetbase(int base)
{
  return wsmanip<int>(sbase, base);
}

wios& sfill(wios& str, int c)
{
  str.fill(c);

  return str;
}

wsmanip<int> wsetfill(int c)
{
  return wsmanip<int>(sfill, c);
}

wios& sprec(wios& str, int n)
{
  str.precision(n);

  return str;
}

wsmanip<int> wsetprecision(int n)
{
  return wsmanip<int>(sprec, n);
}

wios& swidth(wios& str, int n)
{
  str.width(n);

  return str;
}

wsmanip<int> wsetw(int n)
{
  return wsmanip<int>(swidth, n);
}

#endif


#if 0
template<class T, class charT, class traits>
basic_istream<charT, traits>&
  operator>>(basic_istream<charT, traits>& is,
             const basic_imanip<T, charT, traits>& a)
{
  (*a.pf)(is, a.manarg_);

    // need to catch exceptions

  return is;
}



template<class T, class charT, class traits>
basic_ostream<charT, traits>&
  operator<<(basic_ostream<charT, traits>& os,
             const basic_omanip<T, charT, traits>& a)
{
  (*a.pf)(os, a.manarg_);

    // catch any exception thrown

  return os;
}


template<class T, class charT, class traits>
basic_istream<charT, traits>&
  operator>>(basic_istream<charT, traits>& is,
             const basic_smanip<T, charT, traits>& a)
{
  (*a.pf)(is, a.manarg_);

  return is;
}

template<class T, class charT, class traits>
basic_ostream<charT, traits>&
  operator<<(basic_ostream<charT, traits>& os,
             const basic_smanip<T, charT, traits>& a)
{
  (*a.pf)(os, a.manarg_);

  return os;
}

/*
 * standard manipulators
 *
 */

/*
 * basic_resetiosflags
 */
#endif
#if 0

template<class charT, class traits>
basic_smanip<basic_ios<charT, traits>::fmtflags, charT, traits>
  basic_resetiosflags(basic_ios<charT, traits>::fmtflags mask);

/*
 * basic_setiosflags
 */

template<class charT, class traits>
basic_smanip<basic_ios<charT, traits>::fmtflags, charT, traits>
  basic_setiosflags(basic_ios<charT, traits>::fmtflags mask);


/*
 * basic_setbase
 */

template<class charT, class traits>
basic_smanip<int, charT, traits> basic_setbase(int base);

/*
 * basic_setfill
 */

template<class charT, class traits>
basic_smanip<int, charT, traits> basic_setfill(int c);

/*
 * basic_setprecision
 */

template<class charT, class traits>
basic_smanip<int, charT, traits> basic_setprecision(int n);

/*
 * basic_setw
 */

template<class charT, class traits>
basic_smanip<int, charT, traits> basic_setw(int n);

#endif



/*
 * class basic_imanip
 *
 */


/*
 * basic_imanip(basic_ios& (*pf)(basic_ios&, T), T)
 */
#if 0
template<class T, class charT, class traits>
basic_imanip<T, charT, traits>::basic_imanip(
             ios_type& (*pf)(ios_type&, T), T manarg)
: pf_(pf)
, manarg_(manarg)
{

}

/*
 * class basic_omanip
 *
 */


/*
 * basic_omanip(basic_ios& (*pf)(basic_ios&, T), T)
 */

template<class T, class charT, class traits>
basic_omanip<T, charT, traits>::basic_omanip(
            ios_type& (*pf)(ios_type&, T), T manarg)
: pf_(pf)
, manarg_(manarg)_
{

}

/*
 *
 */
#endif

#ifndef RW_COMPILE_INSTANTIATE
#ifndef RW_NO_NAMESPACE
}
#endif
#endif

