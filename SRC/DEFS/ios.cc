/***************************************************************************
 *
 * ios.cc - Definition for the Standard Library iostreams
 *
 * $Id: ios.cc,v 1.51 1996/09/13 23:39:36 smithey Exp $
 *
 ***************************************************************************
 *
 * (c) Copyright 1994-1996 Rogue Wave Software, Inc.
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

#include <streambuf>
#include <iosfwd>


#ifndef _RWSTD_NO_NAMESPACE
namespace std { 
#endif
  
extern istream _RWSTDExport cin;

#ifndef _RWSTD_NO_WIDE_CHAR
extern wistream _RWSTDExport wcin;
#endif


/*
 * class basic_ios<charT, traits>
 *
 */


/*
 *
 *    		class basic_ios<charT,traits> member functions   
 *
 */


/*
 * basic_ios(basic_streambuf *)
 */

template<class charT, class traits>
basic_ios<charT, traits>::
basic_ios(basic_streambuf<charT, traits> *sb_arg)
{
  init(sb_arg);
}


/*
 * ~basic_ios()
 */

template<class charT, class traits>
basic_ios<charT, traits>::~basic_ios()
{

}

/*
 * char_type fill() const
 */

template<class charT, class traits>
_TYPENAME basic_ios<charT,traits>::char_type 
_RWSTDExport basic_ios<charT,traits>::fill() const
{
  return fillch_;
}

/*
 * int_type fill(char_type)
 */

template<class charT, class traits> 
_TYPENAME basic_ios<charT,traits>::char_type
_RWSTDExport basic_ios<charT,traits>::fill(char_type ch)
{
  #ifdef _RWSTD_MULTI_THREAD
   _RWSTDGuard guard(this->stream_mutex_);
  #endif

  char_type    temp = fillch_;

  fillch_ = ch;

  return temp;
}


/*
 * ios_type& copyfmt(const ios_type& )
 */

template<class charT, class traits>
basic_ios<charT, traits>&
_RWSTDExport basic_ios<charT, traits>::
copyfmt(const basic_ios<charT, traits>& rhs)
{
  #ifdef _RWSTD_MULTI_THREAD
   _RWSTDGuard guard(this->stream_mutex_);
  #endif

  tiestr_ = rhs.tiestr_;
   
  fillch_ = rhs.fillch_;

  ios_base::_RW_UNSAFE_copyfmt( rhs );

  except_ = rhs.except_;

  return *this;
}


/*
 * basic_ostream *tie(basic_ostream *)
 */

template<class charT, class traits>
basic_ostream<charT, traits> *
_RWSTDExport basic_ios<charT, traits>::
tie(basic_ostream<charT, traits> *tie_arg)
{
  #ifdef _RWSTD_MULTI_THREAD
   _RWSTDGuard guard(this->stream_mutex_);
  #endif

  basic_ostream<charT, traits>   *temp = tiestr_;
  tiestr_ = tie_arg;
  return temp;
}
 

/*
 * basic_streambuf *rdbuf(basic_streambuf *)
 */

template<class charT, class traits>
_TYPENAME basic_ios<charT, traits>::streambuf_type*	
_RWSTDExport basic_ios<charT, traits>::                     
rdbuf( basic_streambuf<charT, traits> *sb)
{
  #ifdef _RWSTD_MULTI_THREAD
   _RWSTDGuard guard(this->stream_mutex_);
  #endif

  basic_streambuf<charT, traits>   *temp = sb_;

  sb_ = sb;

  if ( sb==0 ) 
    state_ |= ios_base::badbit;
 
  _RW_UNSAFE_clear();

  return temp;
}

/*
 * basic_ios()
 */

template<class charT, class traits>
basic_ios<charT, traits>::basic_ios()
{
  init(0);
}

/*
 * void imbue(const locale& )
 */

template<class charT, class traits>
locale _RWSTDExport basic_ios<charT, traits>::
imbue(const locale& loc)
{
   locale tmp = getloc();
   
   ((ios_base *)(this))->imbue(loc);
   
   if ( rdbuf() )
     rdbuf()->pubimbue(loc);

   return tmp;  
}


/*
 * void init(basic_streambuf *)
 */

template<class charT, class traits>
void _RWSTDExport basic_ios<charT, traits>::
init(basic_streambuf<charT, traits> *sb)
{
  #ifdef _RWSTD_MULTI_THREAD
   _RWSTDGuard guard(this->stream_mutex_);
  #endif

  sb_ = sb;
  tiestr_ = 0;

  if(sb_)
    state_ = goodbit;
  else
    state_ = badbit;

  except_ = goodbit;
  fmtfl_ = skipws | dec;
  wide_ = 0;
  prec_ = 6;

  fillch_ = widen(' ');

  fmtfl_ &= ~ios_base::adjustfield;

  fmtfl_ |= (ios_base::right & ios_base::adjustfield);

}

#ifdef _RWSTD_MULTI_THREAD

/*
 * lock the associated buffer
 */


template <class charT, class traits>
basic_ios<charT,traits>& _RWSTDExport lock(basic_ios<charT,traits>& strm)
{
  if ( strm.rdbuf() )
   strm.rdbuf()->_RW_lock_buffer();

  return strm;
}

template <class charT, class traits>
basic_ios<charT,traits>& _RWSTDExport unlock(basic_ios<charT,traits>& strm)
{
  if ( strm.rdbuf() )
   strm.rdbuf()->_RW_unlock_buffer();

  return strm;
}

#endif


#ifndef _RWSTD_NO_NAMESPACE
}
#endif







