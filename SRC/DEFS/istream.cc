/***************************************************************************
 *
 * istream.cc - istream definitions
 *
 * $Id: istream.cc,v 1.96 1996/09/19 23:21:52 smithey Exp $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
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

#ifndef _RWSTD_NO_NEW_HEADER
#include <cctype>
#else
#include <ctype.h>
#endif

#include <ios>

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

/*
 * Standard basic_istream manipulators
 *
 * skips the next available white spaces
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport ws(basic_istream<charT, traits>& is)
{
   _TYPENAME traits::int_type        c;

   #ifdef _RWSTD_MULTI_THREAD
    #ifndef _RWSTD_NO_EXCEPTIONS
     try {
    #endif
     if ( is.rdbuf() )
     {
       _RWSTDGuard* tmp = new _RWSTDGuard(is.rdbuf()->buffer_mutex_);
       if ( tmp )
              is.istream_sentry_guard = tmp;
             else
              is.istream_sentry_guard = 0;
     }
   #endif


   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
     const ctype<charT>& ct = use_facet< ctype<charT> >(is.getloc());
   #else
     const ctype<charT>& ct = use_facet(is.getloc(),(ctype<charT>*)0);
   #endif
         
   while((c = is.rdbuf()->sgetc()),
         ( !traits::eq_int_type(c,traits::eof()) && ct.is(ct.space,c) )) 
    is.rdbuf()->snextc();
 
   if( traits::eq_int_type(c,traits::eof()) )
     is.setstate(ios_base::eofbit);
       
  #ifdef _RWSTD_MULTI_THREAD
   if ( is.istream_sentry_guard )
    {
      delete is.istream_sentry_guard;
      is.istream_sentry_guard = 0;
    }
  #endif

#ifdef _RWSTD_MULTI_THREAD
 #ifndef _RWSTD_NO_EXCEPTIONS
  }

  catch(...)
  {
    if ( is.istream_sentry_guard )
    {
      delete is.istream_sentry_guard;
      is.istream_sentry_guard = 0;
      throw;
    }
  }
 #endif
#endif



  return is;
}



/*
 * basic_istream(basic_streambuf *)
 */

template<class charT, class traits>
basic_istream<charT, traits>::
basic_istream(basic_streambuf<charT, traits> *sb)
: chcount_(0)
{
  if ( sb )
   {
     if ( sb->which_open_mode() & ios_base::in )
       init(sb);
     else
       init(0);
   } 
  else
   init(0); 
}

/*
 * basic_istream( )
 */

template<class charT, class traits>
basic_istream<charT, traits>::
basic_istream( )
: chcount_(0)
{ 
}


/*
 * ~basic_istream();
 */

template<class charT, class traits>
basic_istream<charT, traits>::~basic_istream()
{

}

/*
 * istream_type& operator>>(istream_type& (*pf)(istream_type&))
 *
 * for functions relating to istreams
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport basic_istream<charT, traits>::
operator>>(basic_istream<charT,traits>& (*pf)(basic_istream<charT,traits>&))
{
  (*pf)(*this);
  return *this;
}


/*
 * istream_type& operator>>(ios_base& (*pf)(ios_base&))
 *
 * for manipulators relating to the ios_base class
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport basic_istream<charT, traits>::
operator>>(ios_base& (*pf)(ios_base&))
{
  (*pf)(*this);

  return *this;
}

/*
 * istream_type& operator>>(ios_type& (*pf)(ios_type&))
 *
 * used to set one of the ios states (ios manipulator)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport basic_istream<charT, traits>::
operator>>(basic_ios<charT, traits>& (*pf)(basic_ios<charT, traits>&))
{
  (*pf)(*this);

  return *this;
}

/*
 * basic_istream<charT,traits>& operator>>(basic_istream<charT,traits>, charT *)
 *
 * read in a charT *. 
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport operator>>(basic_istream<charT, traits>& is, charT *p)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

 if ( p!=0 )
 {

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(is);

  if(ipfx) { 
	 charT                      *op = p;
	 _TYPENAME traits::int_type  c = 0;
	 int                        len; 

  if ( is.width() == 0 ) len=0; 
  else len=is.width()+1; 

         #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
          const ctype<charT>& ct = use_facet< ctype<charT> >(is.getloc());
         #else
          const ctype<charT>& ct = use_facet(is.getloc(), (ctype<charT>*)0);
         #endif

	 while(--len &&
               (c = is.rdbuf()->sgetc(),!( traits::eq_int_type(c,traits::eof()) ||
                ct.is(ct.space,c))))
        {
	        *p++ = c;
		is.rdbuf()->sbumpc();
	 }
	
	 if( traits::eq_int_type(c,traits::eof()) )
		err = ((p == op) ? 
                  (ios_base::eofbit | ios_base::failbit) : ios_base::eofbit);
	 *p = charT ('\0');  
	 is.width(0); 
  }

}
else
  err = ios_base::failbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           is.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) is.setstate(err);

  return is;
}

/*
 * basic_istream<charT,traits>& operator>>(basic_istream<charT,traits>&, charT& )
 *
 * read in a character
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport operator>>(basic_istream<charT,traits>& is, charT& c)
{
 ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(is);
 
  if(ipfx) { 
             _TYPENAME traits::int_type tmp = is.rdbuf()->sbumpc();
             if ( traits::eq_int_type(traits::eof(),tmp) ) 
               err = ios_base::failbit | ios_base::eofbit;
             else
               c = traits::to_char_type(tmp);
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  
  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           is.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) is.setstate(err);

  return is;
}


#ifndef _RWSTD_NO_SIGNED_CHAR_IN_STREAMS
/*
 * istream& operator>>(basic_istream<char,traits>&,unsigned char& )
 *
 */

template <class traits>
basic_istream<char, traits>&
_RWSTDExport operator>>(basic_istream<char, traits>& is, unsigned char& uc)
{
  is >> (char &)uc;

  return is;
}

/*
 * istream& operator>>(basic_istream<char, traits>&, signed char& )
 *
 */

template <class traits>
basic_istream<char, traits>&
_RWSTDExport operator>>(basic_istream<char, traits>& is, signed char& sc)
{
  is >> (char &)sc;

  return is;
}


/*
 * istream& operator>>(basic_istream<char, traits>&, unsigned char* )
 *
 */

template <class traits>
basic_istream<char, traits>&
_RWSTDExport operator>>(basic_istream<char, traits>& is,unsigned char* uc)
{
  is >> (char *)uc;

  return is;
}

/*
 * istream& operator>>(basic_istream<char, traits>&,signed char* )
 *
 */

template <class traits>
basic_istream<char, traits>&
_RWSTDExport operator>>(basic_istream<char, traits>& is, signed char* sc)
{
   is >> (char *)sc;

   return is;
}

#endif

#ifndef _RWSTD_NO_BOOL
/*
 * istream_type& operator>>(bool&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(bool& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

  if(ipfx) 
 {
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
    use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
   #else
    use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
   #endif
     .get(*this,istreambuf_iterator<charT,traits>(),*this,err,n);
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

#endif /* _RWSTD_NO_BOOL */

/*
 * istream_type& operator>>(short&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(short& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif 

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) 
 {
  long l;
  #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
   use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
  #else
   use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
  #endif
  .get(*this,istreambuf_iterator<charT,traits>(),*this,err,l);
  n=(short)l;
  if ( (n!=l) && ((unsigned short)n != l) )
   err |= ios_base::failbit;
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
 * istream_type& operator>>(unsigned short&)
 */
template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(unsigned short& n)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);


  if(ipfx) {
            #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
             use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
            #else
             use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
            #endif
             .get(*this,istreambuf_iterator<charT,traits>(),*this,err,n);
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
 * istream_type& operator>>(int&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(int& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) {

          long l;
          #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
           use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
          #else
           use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
          #endif
           .get(*this,istreambuf_iterator<charT,traits>(),*this,err,l);
           n=(int)l;
           if ( (n!=l) && ((unsigned int)n!=l) )
           err |= ios_base::failbit;
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
 * istream_type& operator>>(unsigned int&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(unsigned int& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) {
            #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
             use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
            #else
             use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
            #endif
            .get(*this,istreambuf_iterator<charT,traits>(),*this,err,n);
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
 * istream_type& operator>>(long&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(long& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) {

           #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
            use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
           #else
            use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
           #endif
           .get(*this,istreambuf_iterator<charT,traits>(),*this,err,n);
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
 * istream_type& operator>>(unsigned long&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(unsigned long& n)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

_TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) {
            #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
             use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
            #else
             use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
            #endif
            .get(*this,istreambuf_iterator<charT,traits>(),*this,err,n);
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
 * istream_type& operator>>(float&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(float& f)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) { 
            #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
             use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
            #else
             use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
            #endif
            .get(*this,istreambuf_iterator<charT,traits>(),*this,err,f);
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
 * istream_type& operator>>(double&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(double& f)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif  

_TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

if(ipfx) {
           #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
            use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
           #else
            use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
           #endif
            .get(*this,istreambuf_iterator<charT,traits>(),*this,err,f);
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
 * istream_type& operator>>(long double&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(long double& f)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif  

 _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

 if(ipfx) {  
             #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
              use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
             #else   
             use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
             #endif
              .get(*this,istreambuf_iterator<charT,traits>(),*this,err,f);
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
 * istream_type& operator>>(void*&)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(void*& p)
{
  ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

  if(ipfx) 
 {
   #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
    use_facet<num_get<charT,istreambuf_iterator<charT,traits> > >(this->getloc())
   #else
    use_facet(this->getloc(),(num_get<charT,istreambuf_iterator<charT,traits> >*)0)
   #endif
     .get(*this,istreambuf_iterator<charT,traits>(),*this,err,p);
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
 * istream_type& operator>>(basic_streambuf&)
 *
 * reads characters from the stream and inserts them into 'sb'
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(streambuf_type& sb)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif  

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

  if(ipfx) {
    int_type         c;

    if ( traits::eq_int_type(rdbuf()->sgetc(),traits::eof()) ) 
     err = ios_base::failbit; 
         
    while( !traits::eq_int_type( (c = rdbuf()->sgetc()),traits::eof()) ) {
		if( traits::eq_int_type(sb.sputc(c),traits::eof()) ) { 
        err = ios_base::failbit;
        break;
      }
      rdbuf()->sbumpc();  
    }

    if( traits::eq_int_type(c,traits::eof()) ) 
      err |= ios_base::eofbit;  
  }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
            this->setstate(ios_base::failbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * istream_type& operator>>(basic_streambuf *)
 *
 * reads characters from the stream and inserts them into 'sb'
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::operator>>(streambuf_type *sb)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 

 if ( sb!=0 )
 {
  

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this);

  if(ipfx) {
    int_type         c;
    
    if ( traits::eq_int_type(rdbuf()->sgetc(),traits::eof()) ) 
     err = ios_base::failbit; 
     
    while( !traits::eq_int_type( (c = rdbuf()->sgetc()),traits::eof()) ) {
      if( traits::eq_int_type(sb->sputc(c),traits::eof()) ) { 
        err = ios_base::failbit;
        break;
      }
      rdbuf()->sbumpc(); 
    }

       
    if( traits::eq_int_type(c,traits::eof()) )
      err |= ios_base::eofbit;
   }
 }
else
  err = ios_base::failbit;

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
            this->setstate(ios_base::failbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) this->setstate(err);

  return *this;
}

/*
 * int_type get()
 *
 * get a single character
 */

template<class charT, class traits>
_TYPENAME basic_istream<charT, traits>::int_type
basic_istream<charT, traits>::get()
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  chcount_ = 0;

  if(ipfx) { 
    int_type      c = rdbuf()->sbumpc();

    if( traits::eq_int_type(c,traits::eof()) )  
     {
      err = ios_base::eofbit | ios_base::failbit;
     }
    else
      {
       chcount_ = 1;
       return c;
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

  return traits::eof();
}

/*
 * istream_type& get(char_type *, streamsize, char_type)
 *
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::
get(char_type *s, streamsize n, char_type delim)
{
 ios_base::iostate err = 0; 

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif  

 chcount_ = 0; 

 if (s!=0)
 {

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  if(ipfx) { 
    char_type  *op = s;
    int_type   c = 0;

    while(--n > 0 && (c = rdbuf()->sgetc()) != delim && 
     ( !traits::eq_int_type(c,traits::eof()) )) {
      *s++ = c; 
      ++chcount_;
      rdbuf()->snextc(); 
    }

    if( traits::eq_int_type(c,traits::eof()) )  
      err = ((s == op) ? (ios_base::eofbit | ios_base::failbit) :
                           ios_base::eofbit);
  }

  *s = 0;  
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
 * istream_type& get(char_type&)
 *
 * gets a single character, first skipping white space.
 * see also: int_type get();
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::get(char_type& s)
{
 ios_base::iostate err = 0;  

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  chcount_ = 0;

  if(ipfx) { 
             int_type tmp = rdbuf()->sbumpc();
             if ( traits::eq_int_type(tmp,traits::eof()) ) 
              {
                err = ios_base::failbit | ios_base::eofbit;
              }
             else
              {
                s = traits::to_char_type(tmp);
                chcount_ = 1;
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

/*
 * istream_type& get(basic_streambuf&, char_type)
 *
 * insert characters into sb upto delim
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::
get(streambuf_type& sb, char_type delim)
{
 ios_base::iostate err = 0;   

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  chcount_ = 0;

  if(ipfx) { 
    int_type        c;

    while(((c = rdbuf()->sgetc()) != delim) && 
     ( !traits::eq_int_type(c,traits::eof()) )) {
      if( traits::eq_int_type(sb.sputc(c),traits::eof()) ) {  
        err = ios_base::failbit;  
        break;
      }
      ++chcount_;
      rdbuf()->sbumpc();  
    }

    if(c == traits::eof())  
      err |= ios_base::eofbit;
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
 * istream_type& getline(char_type *, streamsize, char_type)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::
getline(char_type *s, streamsize n, char_type delim)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif

 chcount_ = 0; 

 if (s!=0)
 {

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  if(ipfx) {  
    char_type    *op = s;
    int_type     c = 0;

 while( --n > 0 && !traits::eq_int_type( (c = rdbuf()->sgetc()),traits::eof()) ) {
      ++chcount_;

   
    if(c == delim)
      {
        #ifndef _RWSTD_NO_WIDE_CHAR
         if ( (((void *)this)!=((void *)&cin)) && 
              (((void *)this)!=((void *)&wcin)) ) 
        #else
         if ( ((void *)this)!=((void *)&cin) ) 
        #endif

              rdbuf()->snextc();  
      
         break;
      }

    rdbuf()->snextc();
            *s++ = c;          
    }
    *s = 0; 
    if( traits::eq_int_type(c,traits::eof()) )  
  err = (s == op) ? (ios_base::eofbit | ios_base::failbit) : ios_base::eofbit;

    if ( n == 0 ) 
     {
       if ( rdbuf()->sgetc() != delim )
        err |= ios_base::failbit;
       else
        rdbuf()->snextc();
     }

  }
  
  *s = 0; 
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
 * istream_type& ignore(streamsize, int_type)
 *
 * this ignores characters n characters or until delim
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::ignore(streamsize n, int_type delim)
{
  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);
  
  chcount_ = 0;

  if(ipfx) {  
    int_type      c = 0;

 while(--n >= 0 && !traits::eq_int_type( (c = rdbuf()->sgetc()),traits::eof()) ) {
      ++chcount_;
      rdbuf()->sbumpc();
      if(c == delim)   
        break;
    }
    if( traits::eq_int_type(c,traits::eof()) )
      this->setstate(ios_base::eofbit);
  }

  return *this;
}

/*
 * istream_type& read(char_type *, streamsize)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
_RWSTDExport basic_istream<charT, traits>::read(char_type *s, streamsize n)
{
 ios_base::iostate err = 0; 

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif 

 chcount_ = 0;

 if (s!=0)
 { 

  _TYPENAME basic_istream<charT, traits>::sentry ipfx(*this,1);

  if(ipfx) {  
    int_type       c = 0;

while((--n >= 0) && !traits::eq_int_type( (c = rdbuf()->sgetc()),traits::eof()))
   {
      *s++ = c; 
      ++chcount_;
      rdbuf()->sbumpc();  
    }
   *s=0;

    if( traits::eq_int_type(c,traits::eof()) )  
    err = (n >= 0) ? (ios_base::eofbit | ios_base::failbit) : ios_base::eofbit;
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
 * streamsize readsome(char_type *, streamsize)
 */

template<class charT, class traits>
streamsize 
basic_istream<charT, traits>::readsome(char_type *s, streamsize n)
{
  int navail = rdbuf()->in_avail();

   
  if(navail == -1) {   
    this->setstate(ios_base::eofbit);
    return 0;
  }
  if(navail == 0 ) return 0;

 if ( this->good() )
 {
  if(n < navail) 
    { 
      read(s, n);
      return (n);
    }

  read(s, navail);
  return (streamsize)(navail);
 }
 else
  { 
    if ( !(this->rdstate() & ios_base::failbit) )
     this->setstate(ios_base::failbit);
  }
  
  return 0;
}

/*
 * int peek()
 */

template<class charT, class traits>
int basic_istream<charT, traits>::peek()
{
  chcount_ = 0;

  if(this->good())
   {
     #ifdef _RWSTD_MULTI_THREAD
      if ( rdbuf() )
       _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
     #endif

     _TYPENAME traits::int_type tmp = rdbuf()->sgetc();
     if ( !traits::eq_int_type( tmp,traits::eof() ) )
      chcount_ = 1;
     return tmp;
   }

  return traits::eof();
}

/*
 * pos_type tellg()
 */

template<class charT, class traits>
_TYPENAME basic_istream<charT, traits>::pos_type
basic_istream<charT, traits>::tellg()
{
 ios_base::iostate err = 0; 
 pos_type         p;

  #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif 

  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif

  if ( this->bad() ) return pos_type(off_type(-1));   

  if((p = rdbuf()->pubseekoff(0, ios_base::cur, ios_base::in))
   == pos_type(off_type(-1)))
    err = ios_base::failbit;

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

  return p;
}

/*
 * basic_istream& seekg(pos_type)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::seekg(pos_type pos)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif 

 if ( this->rdstate() & ios_base::eofbit )
   clear( this->rdstate() & ~ios_base::eofbit );

 #ifdef _RWSTD_MULTI_THREAD
  if ( rdbuf() )
   _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
 #endif 

if ( !this->bad() )
  if(rdbuf()->pubseekpos(pos, ios_base::in) == pos_type(off_type(-1)))
   err = ios_base::failbit;

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
 * basic_istream& seekg(off_type, ios_base::seekdir)
 */

#ifndef _RWSTD_NO_INT_TYPEDEF
template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::
seekg(off_type off, ios_base::seekdir dir)
{
 ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif 

   #ifdef _RWSTD_MULTI_THREAD
    if ( rdbuf() )
     _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
   #endif 

if ( !this->bad() )
 {
  if (rdbuf())
  {
   if( rdbuf()->pubseekoff(off, dir, ios_base::in) == pos_type(off_type(-1)) )
    err = ios_base::failbit;
  }
  else
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
  else
   if ( !this->bad() )
    clear(this->rdstate() & ~(ios_base::eofbit | ios_base::failbit));

  return *this;
}
#endif

/*
 * istream_type& putback(char_type)
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::putback(char_type c)
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 
  
  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif 

 if (this->good())
   {
     if( traits::eq_int_type(rdbuf()->sputbackc(c),traits::eof()) ) 
      err = ios_base::badbit;      
  }
 else
  { 
    if ( !(this->rdstate() & ios_base::failbit) )
     this->setstate(ios_base::failbit);
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
 * istream_type& unget()
 */

template<class charT, class traits>
basic_istream<charT, traits>&
basic_istream<charT, traits>::unget()
{
  ios_base::iostate err = 0;

  #ifndef _RWSTD_NO_EXCEPTIONS
  try {
  #endif 

  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif

  if (this->good())
   {
     if( traits::eq_int_type(rdbuf()->sungetc(),traits::eof()) )
      err = ios_base::badbit;  
   }
 else
  { 
    if ( !(this->rdstate() & ios_base::failbit) )
     this->setstate(ios_base::failbit);
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
 * streamsize gcount() const
 */

template<class charT, class traits>
streamsize basic_istream<charT, traits>::gcount() const
{
  return chcount_;
}


/*
 * int sync()
 */

template<class charT, class traits>
int basic_istream<charT, traits>::sync()
{
   ios_base::iostate err = 0;

   #ifndef _RWSTD_NO_EXCEPTIONS
   try {
   #endif 
  
  #ifdef _RWSTD_MULTI_THREAD
   if ( rdbuf() )
    _RWSTDGuard guard(this->rdbuf()->buffer_mutex_);
  #endif

  if(rdbuf()) 
{  

    if(rdbuf()->pubsync() == -1)
      err = ios_base::badbit;  
    else
      {
        if ( this->rdstate() & ios_base::eofbit )
         clear( this->rdstate() & ~(ios_base::eofbit | ios_base::failbit));
        return 0;
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

  return -1;
}


// string extractor and getline

template<class charT, class traits, class Allocator>
basic_istream<charT, traits>&
_RWSTDExport operator>> (basic_istream<charT,traits>& is,
            basic_string<charT,traits,Allocator>& s)
{
    _TYPENAME traits::int_type c;
    ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif  

    _TYPENAME basic_istream<charT,traits>::sentry ipfx(is);

    if(ipfx) { 

      s.erase();
      c = is.rdbuf()->sbumpc();

      const ctype<charT>& ctype_facet =
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
        use_facet<ctype<charT> >(is.getloc());
#else
        use_facet(is.getloc(),(ctype<charT>*)0);
#endif

      _TYPENAME Allocator::size_type i   = 0;

      _TYPENAME Allocator::size_type end = s.max_size();
      if (is.width())
        end = (end > is.width()) ? is.width() : end;

      while ( !traits::eq_int_type(c,traits::eof()) &&  !ctype_facet.is(ctype_facet.space,c) )
      {
        s.append(1,traits::to_char_type(c));
        i++;
        if ( i == end ) break;
        c = is.rdbuf()->sbumpc();
      }
    if ( i == 0 ) err |= ios_base::failbit;
    if ( traits::eq_int_type(c,traits::eof()) ) 
     err |= ios_base::eofbit;

    }

  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           is.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) is.setstate(err);

  return is;
}


template<class charT, class traits, class Allocator>
basic_istream<charT,traits>&
_RWSTDExport getline (basic_istream<charT,traits>& is,
         basic_string<charT,traits,Allocator>& str,
         charT delim )
{
    _TYPENAME traits::int_type c;
    ios_base::iostate err = 0;

 #ifndef _RWSTD_NO_EXCEPTIONS
 try {
 #endif  

    _TYPENAME basic_istream<charT,traits>::sentry ipfx(is,1);

    if(ipfx) { 

    str.erase();
    
    c = is.rdbuf()->sbumpc();
    _TYPENAME Allocator::size_type i = 0;

      while ( !traits::eq_int_type(c,traits::eof()) )
      {
        i++;
        if ( traits::eq(traits::to_char_type(c),delim) )
            break;
        if (i == str.max_size())
        {
          err |= ios_base::failbit;
          break;
        }
        str.append(1,traits::to_char_type(c));
        c = is.rdbuf()->sbumpc();
      }

     if ( traits::eq_int_type(c,traits::eof()) )
      err |= ios_base::eofbit;

    if ( i==0 )
     err |= ios_base::failbit;

    }


  #ifndef _RWSTD_NO_EXCEPTIONS
  }
  #endif

  #ifndef _RWSTD_NO_EXCEPTIONS
  catch(...)
  {
    bool flag = FALSE;
    try {
           is.setstate(ios_base::badbit);
        }
    catch( ios_base::failure ) { flag= TRUE; }
    if ( flag ) throw;
  }
  #endif

  if ( err ) is.setstate(err);

  return is;
}  


/*
 *
 * class basic_iostream
 *
 */

/*
 * basic_iostream(basic_streambuf *)
 */

template<class charT, class traits>
basic_iostream<charT, traits>::
basic_iostream(basic_streambuf<charT, traits> *sb)
:basic_istream<charT, traits>(sb)
,basic_ostream<charT, traits>(sb)
{
}

/*
 * basic_iostream( )
 */

template<class charT, class traits>
basic_iostream<charT, traits>::
basic_iostream( )
{
}


/*
 * ~basic_iostream()
 */

template<class charT, class traits>
basic_iostream<charT, traits>::
~basic_iostream()
{

}

#ifndef _RWSTD_NO_NAMESPACE
}
#endif







