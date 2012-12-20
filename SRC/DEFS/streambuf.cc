/***************************************************************************
 *
 * streambuf.cc - Definitions for the Standard Library stream buffers
 *
 * $Id: streambuf.cc,v 1.39 1996/09/13 23:39:36 smithey Exp $
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

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

/*
 * class basic_streambuf<charT, traits>
 *
 */

/*
 * ~basic_streambuf()
 */

template<class charT, class traits>
basic_streambuf<charT, traits>::~basic_streambuf()
{

}

/*
 * basic_streambuf()
 */

template<class charT, class traits>
basic_streambuf<charT, traits>::basic_streambuf()
{
  streambuf_init();
}

/*
 * void streambuf_init()
 */

template<class charT, class traits>
void _RWSTDExport basic_streambuf<charT, traits>::streambuf_init(bool set_mode)
{
  if ( set_mode )
  mode_ = ( ios_base::in | ios_base::out );

  gbeg_ = 0;
  gnext_ = 0;
  gend_ = 0;

  pbeg_ = 0;
  pnext_ = 0;
  pend_ = 0;
}


/*
 * int_type overflow(int_type)
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::int_type
_RWSTDExport basic_streambuf<charT, traits>::overflow(int_type )
{
  return traits::eof();  
}

/*
 * int_type pbackfail(int_type)
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::int_type
_RWSTDExport basic_streambuf<charT, traits>::pbackfail(int_type )
{
  return traits::eof();
}

/*
 * int showmanyc()
 */

template<class charT, class traits>
int _RWSTDExport basic_streambuf<charT, traits>::showmanyc()
{
  if ( gptr() )
   {
      if ( pptr()>egptr() ) setg(eback(),gptr(),pptr());
   }
  else
    if ( pptr() ) setg(pbase(),pbase(),pptr());

  return (int)(egptr()-gptr());
}

/*
 * int_type underflow()
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::int_type
_RWSTDExport basic_streambuf<charT, traits>::underflow()
{
  return traits::eof();
}

/*
 * int_type uflow()
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::int_type
_RWSTDExport basic_streambuf<charT, traits>::uflow()
{
  if( traits::eq_int_type(underflow(),traits::eof()) )
    return traits::eof();


   return sbumpc();
}

/*
 * streamsize xsgetn(char_type *, streamsize)
 */

template<class charT, class traits>
streamsize _RWSTDExport basic_streambuf<charT, traits>::
xsgetn(char_type *s, streamsize n)
{
  if ( !s || (n==0) ) return 0;

  streamsize i = ( in_avail() > n ) ? n : in_avail();
  int_type   c;

  if(i > 0) {
    s = traits::copy(s, gptr(), i);
    s += i;
    gbump(i);
  }

  while((i < n) && ( !traits::eq_int_type( (c = sbumpc()),traits::eof()))) {
    *s++ = traits::to_char_type(c);
    ++i;
  }

  return i;
}

/*
 * streamsize xsputn(const char_type *, streamsize)
 */

template<class charT, class traits>
streamsize _RWSTDExport basic_streambuf<charT, traits>::
xsputn(const char_type *s, streamsize n)
{
  if ( !s || (n == 0) ) return 0;

  int         i=0;

  while((i < n) && ( !traits::eq_int_type(sputc(*s++),traits::eof()))) {
    i++;
  }

  return i;
 }

/*
 * pos_type seekoff(off_type, ios_base::seekdir, ios_base::openmode)
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::pos_type
_RWSTDExport basic_streambuf<charT, traits>::
seekoff(off_type , ios_base::seekdir , ios_base::openmode )
{
  return pos_type(off_type(-1));
}

/*
 * pos_type seekpos(pos_type, ios_base::openmode)
 */

template<class charT, class traits>
_TYPENAME basic_streambuf<charT, traits>::pos_type
_RWSTDExport basic_streambuf<charT, traits>::
seekpos(pos_type , ios_base::openmode )
{
  return pos_type(off_type(-1));
}

/*
 * basic_streambuf *setbuf(char_type *, streamsize)
 */

template<class charT, class traits>
basic_streambuf<charT, traits> *
_RWSTDExport basic_streambuf<charT, traits>::setbuf(char_type*, streamsize )
{
  return this;
}

/*
 * int sync()
 */

template<class charT, class traits>
int _RWSTDExport basic_streambuf<charT, traits>::sync()
{
  return 0;
}


/*
 * locale pubimbue(const locale& loc)
 */

template<class charT, class traits>
locale _RWSTDExport _RWSTDExport basic_streambuf<charT, traits>::pubimbue(const locale& loc)
{
  locale tmp = getloc();
  imbue(loc);
  return tmp; 
}

/*
 * locale getloc() const
 */

template<class charT, class traits>
locale _RWSTDExport basic_streambuf<charT, traits>::getloc() const
{
  return loc_buf;
}

/*
 * virtual void imbue(const locale& loc)
 */

template<class charT, class traits>
void _RWSTDExport basic_streambuf<charT, traits>::imbue(const locale& loc)
{
  loc_buf = loc;

}

#ifndef _RWSTD_NO_NAMESPACE
}
#endif



