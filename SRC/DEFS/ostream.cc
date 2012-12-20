/***************************************************************************
 *
 * ostream.cc - Definitions for the Standard Library ostream classes
 *
 * $Id: ostream.cc,v 1.78 1996/09/24 00:41:58 philippe Exp $
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

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

/*
 * basic_ostream(basic_streambuf *)
 */

template<class charT, class traits> 
basic_ostream<charT, traits>::basic_ostream(basic_streambuf<charT, traits> *sb)
{
  if ( sb )
   {
     if ( sb->which_open_mode() & ios_base::out ) 
       init(sb);
     else
       init(0);
   } 
  else
   init(0);
}

/*
 * basic_ostream( )
 */

template<class charT, class traits>
basic_ostream<charT, traits>::basic_ostream( )
{
}


/*
 * ~basic_ostream()
 */

template<class charT, class traits>
basic_ostream<charT, traits>::~basic_ostream()
{

}

/*
 * basic_ostream& flush()  
 */
 
template<class charT, class traits>
basic_ostream<charT, traits>&
basic_ostream<charT, traits>::flush()
{
  
 if(rdbuf()) {

  #ifdef _RWSTD_MULTI_THREAD
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif


  	 if(rdbuf()->pubsync() == -1)
		this->setstate(ios_base::badbit);
  }

  return *this;
}
  
/*
 * basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&))
 *
 * these are the ostream manipulators (endl, ends, flush)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
basic_ostream<charT, traits>::
operator<<(basic_ostream<charT, traits>& (*pf)(basic_ostream<charT, traits>& ))
{
  return (*pf)(*this);
}



/*
 * basic_ostream& operator<<(ios_base& (*pf)(ios_base&))
 *
 * outputs the ios_base manipulators
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
basic_ostream<charT, traits>::
operator<<(ios_base& (*pf)(ios_base&))
{
  (*pf)(*this);

  return *this;
}

/*
 * basic_ostream& operator<<(basic_ios& (*pf)(basic_ios& ))
 *
 * these are the ios manipulators (dec, hex, ...)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
basic_ostream<charT, traits>::
operator<<(basic_ios<charT, traits>& (*pf)(basic_ios<charT, traits>&))
{
  (*pf)(*this);

  return *this;
}

/*
 * basic_ostream& operator<< (basic_ostream<charT, traits>& os, const charT *)
 *
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport operator<< ( basic_ostream<charT, traits>& os, const charT *s)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif
  
   if ( s ) {   
    _TYPENAME basic_ostream<charT, traits>::sentry opfx(os);
    if (opfx)
     {
       int   dlen = traits::length(s);
       int   pad = os.width() - dlen;

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good() && dlen) {
          if(os.rdbuf() && (os.rdbuf()->sputn(s, dlen) != dlen))
           err = ios_base::badbit;
          }

         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);
     }
    }
   else
    err = ios_base::badbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err);  

  return os;
}


/*
 * basic_ostream& operator<< (basic_ostream<charT, traits>& os, const char *)
 *
 */
#ifndef _RWSTD_NO_OVERLOAD_OF_TEMPLATE_FUNCTION

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport operator<< ( basic_ostream<charT, traits>& os, const char *s)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif
  
   if ( s ) {   
    _TYPENAME basic_ostream<charT, traits>::sentry opfx(os);
    if (opfx)
     {
       int   dlen = char_traits<char>::length(s);
       int   pad = os.width() - dlen;

       #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
        const ctype<charT>& ct = use_facet< ctype<charT> >(os.getloc());
       #else
        const ctype<charT>& ct = use_facet(os.getloc(),(ctype<charT>*)0);
       #endif

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good() && dlen) {
           while ( dlen )
            {
             if( traits::eq_int_type(
                    os.rdbuf()->sputc(ct.widen(*s)),
                    traits::eof())) 
               { 
                 err = ios_base::badbit;    
               }
              s++;
              dlen --;
            }
          }

         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);
     }
    }
   else
    err = ios_base::badbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err);  

  return os;
}
#endif

/*
 * basic_ostream& operator<< (basic_ostream<char, traits>& os, const char *)
 *
 */

#ifndef _RWSTD_NO_OVERLOAD_OF_TEMPLATE_FUNCTION

#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC
template<class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<< ( basic_ostream<char, traits>& os, const char *s)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif
  
   if ( s ) {   
    _TYPENAME basic_ostream<char, traits>::sentry opfx(os);
    if (opfx)
     {
       int   dlen = traits::length(s);
       int   pad = os.width() - dlen;

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good() && dlen) {
          if(os.rdbuf() && (os.rdbuf()->sputn(s, dlen) != dlen))
           err = ios_base::badbit;
          }

         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);
     }
    }
   else
    err = ios_base::badbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err);  

  return os;
}
#endif
#endif

/*
 * basic_ostream& operator<<( basic_ostream<charT, traits>&, charT )
 *
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport operator<< (basic_ostream<charT, traits>& os, charT c)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

    _TYPENAME basic_ostream<charT, traits>::sentry opfx(os); 

    if(opfx) 
    {
       int   pad = os.width() - 1;

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good()) {
           if( traits::eq_int_type(os.rdbuf()->sputc(c),traits::eof())) 
            err = ios_base::badbit;    
         }
 
         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);
     
     }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::failbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err); 

  return os;
}

/*
 * basic_ostream& operator<<( basic_ostream<charT, traits>&, char )
 *
 */

#ifndef _RWSTD_NO_OVERLOAD_OF_TEMPLATE_FUNCTION

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport operator<< (basic_ostream<charT, traits>& os, char c)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

    _TYPENAME basic_ostream<charT, traits>::sentry opfx(os); 

    if(opfx) 
    {
     #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      const ctype<charT>& ct = use_facet< ctype<charT> >(os.getloc());
     #else
      const ctype<charT>& ct = use_facet(os.getloc(),(ctype<charT>*)0);
     #endif

     int   pad = os.width() - 1;

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good()) {
           if( traits::eq_int_type(os.rdbuf()->sputc(ct.widen(c)),traits::eof())) 
            err = ios_base::badbit;      
         }
 
         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);    
  
     }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::failbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err); 

  return os;
}
#endif

/*
 * basic_ostream& operator<<( basic_ostream<char, traits>&, char )
 *
 */

#ifndef _RWSTD_NO_OVERLOAD_OF_TEMPLATE_FUNCTION

#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC
template<class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<< (basic_ostream<char, traits>& os, char c)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

    _TYPENAME basic_ostream<char, traits>::sentry opfx(os); 

    if(opfx) 
    {
       int   pad = os.width() - 1;

       // place right padding
       if( os.flags() & ios_base::right ) {
        while(--pad >= 0) {
          if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }
	 
	 // output internal padding
	 if(os.good() && (os.flags() & ios_base::internal)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
            err = ios_base::badbit;
            break;
           }
          }
         }

	 if(os.good()) {
            if( traits::eq_int_type(os.rdbuf()->sputc(c),traits::eof())) 
             err = ios_base::badbit;       
         }
 
         // output left padding. 
         if(os.good() && (os.flags() & ios_base::left)) {
          while(--pad >= 0) {
           if( traits::eq_int_type(os.rdbuf()->sputc(os.fill()),traits::eof())) {
             err = ios_base::badbit;
             break;
            }
           }
          }

       os.width(0);    
      
     }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           os.setstate(ios_base::failbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) os.setstate(err); 

  return os;
}
#endif
#endif
 
#ifndef _RWSTD_NO_SIGNED_CHAR_IN_STREAMS
/*
 * ostream& operator<<(basic_ostream<char,traits>&,unsigned char )
 *
 */

template <class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<<(basic_ostream<char, traits>& os,unsigned char uc)
{

  return (os << (char)uc);
}

/*
 * ostream& operator<<(basic_ostream<char, traits>&,signed char )
 *
 */

template <class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<<(basic_ostream<char, traits>& os,signed char sc)
{

  return (os << (char)sc);
}

/*
 * ostream& operator<<(basic_ostream<char, traits>&,const unsigned char* )
 *
 */

template <class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<<(basic_ostream<char, traits>& os,const unsigned char* uc)
{

  return (os << (char *)uc);
}

/*
 * ostream& operator<<(basic_ostream<char, traits>&,const signed char* )
 *
 */

template <class traits>
basic_ostream<char, traits>&
_RWSTDExport operator<<(basic_ostream<char, traits>& os,const signed char* sc)
{

  return (os << (char *)sc);
}
#endif


/*
 * basic_ostream& operator<<(bool n)
 */

#ifndef _RWSTD_NO_BOOL

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(bool n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
    if (
    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
    use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
    #else
    use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
    #endif
      .put(*this,*this,this->fill(),n).failed() )
      err = ios_base::badbit;
    this->width(0);
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

#endif

/*
 * basic_ostream& operator<<(short)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(short n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
      err = ios_base::badbit;

  this->width(0);

  }
 
  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& operator<<(unsigned short)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(unsigned short n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
    .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
      err = ios_base::badbit;
 
  this->width(0);

  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this; 
}

/*
 * basic_ostream& operator<<(int)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(int n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if ( 
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;
   
   this->width(0);
  
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);  

  return *this; 
}
/*
 * basic_ostream& operator<<(unsigned int)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(unsigned int n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;
    
  this->width(0);

  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this; 
}


/*
 * basic_ostream& operator<<(long)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(long n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;
   
   width(0);

  } 

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this; 
}

/*
 * basic_ostream& operator<<(unsigned long)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(unsigned long n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;
   
   this->width(0);

  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

   if ( err ) this->setstate(err);

  return *this; 
}


/*
 * basic_ostream& operator<<(float)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(float n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),(double)n).failed() )
    err = ios_base::badbit;
   this->width(0);
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& operator<<(double)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(double n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
   if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;

  this->width(0);

  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& operator<<(long double)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(long double n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
    if (
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
   #else
   use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
   #endif
     .put(*this,*this,basic_ios<charT,traits>::fill(),n).failed() )
       err = ios_base::badbit;
   
  this->width(0);

  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& operator<<(void *)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::operator<<(void *p)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);
  if (opfx)
  {
    if (
    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
    use_facet<num_put<charT,ostreambuf_iterator<charT,traits> > >(this->getloc())
    #else
    use_facet(this->getloc(),(num_put<charT,ostreambuf_iterator<charT,traits> >*)0)
    #endif
      .put(*this,*this,this->fill(),p).failed() )
      err = ios_base::badbit;
    this->width(0);
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::
operator<<(basic_streambuf<charT, traits>& sb)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  if ( !(sb.which_open_mode() & ios_base::in) )
   err = ios_base::failbit;
  else
  {

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);

  if(opfx) {
             int_type    c;
             while( !traits::eq_int_type( (c = sb.sbumpc()),traits::eof()) ) {
              if( traits::eq_int_type(rdbuf()->sputc(c),traits::eof()) ) {
               err = ios_base::failbit;
               break;
              }
             }
            }
   }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::
operator<<(basic_streambuf<charT, traits> *sb)
{
  ios_base::iostate err = 0; 

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif
 
if (sb)
 {

  if ( !(sb->which_open_mode() & ios_base::in) )
   err = ios_base::failbit;
  else
  {

   _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);

   if(opfx) {
              int_type    c;
              while( !traits::eq_int_type( (c = sb->sbumpc()),traits::eof())) {
              if( traits::eq_int_type(rdbuf()->sputc(c),traits::eof())) {
               err = ios_base::failbit;
               break;
             }
            }
          }
   }
 }
else 
   err = ios_base::badbit;  

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
          this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& put(char_type)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::put(char_type c)
{
  ios_base::iostate err = 0; 

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);

  if(opfx)
     {
         if( traits::eq_int_type(rdbuf()->sputc(c),traits::eof()) )  
	  err = ios_base::badbit;    
     }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * basic_ostream& write(const char_type *, streamsize)
 */

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::write(const char_type *s, streamsize n)
{
  ios_base::iostate err = 0; 

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 

 if(s)
  {
    _TYPENAME basic_ostream<charT, traits>::sentry opfx(*this);

    if(opfx)
     {
       if(rdbuf()->sputn(s, n)!=n) 
        err = ios_base::badbit;
     }
  }
 else
  err = ios_base::badbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           this->setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::
seekp(pos_type pos)
{
  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif

  if ( this->bad() ) return *this;

  if ( this->rdstate() & ios_base::eofbit )
   clear( this->rdstate() & ~ios_base::eofbit );
   
  if( rdbuf()->pubseekpos(pos, ios_base::out) == pos_type(off_type(-1)) )
   this->setstate(ios_base::failbit);

  return *this;
}

template<class charT, class traits>
basic_ostream<charT, traits>&
_RWSTDExport basic_ostream<charT, traits>::
seekp(off_type off, ios_base::seekdir dir)
{
  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif

  if ( this->bad() ) return *this;

  if(rdbuf()->pubseekoff(off, dir, ios_base::out) == pos_type(off_type(-1)))
   this->setstate(ios_base::failbit);
  else
   clear(this->rdstate() & ~(ios_base::eofbit | ios_base::failbit));

  return *this;
}

template<class charT, class traits>
_TYPENAME basic_ostream<charT,traits>::pos_type
_RWSTDExport basic_ostream<charT, traits>::tellp()
{
  pos_type    p;

  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif
 
 if ( this->bad() ) return pos_type(-1);

  if((p = rdbuf()->pubseekoff(0, ios_base::cur,ios_base::out)) ==
     pos_type(off_type(-1)))
    this->setstate(ios_base::failbit);

  return p;
}

// String insertor 

template<class charT, class traits, class Allocator>
basic_ostream<charT, traits>&
_RWSTDExport operator<< (basic_ostream<charT,traits>& os,
            const basic_string<charT,traits,Allocator>& s)
{
    os.write(s.data(), s.size() );
    return os;
}



#ifndef _RWSTD_NO_NAMESPACE
}
#endif






