/***************************************************************************
 *
 * sstream.cc - Declarations for the Standard Library basic strings
 *
 * $Id: sstream.cc,v 1.54 1996/09/11 00:28:19 smithey Exp $
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
 * basic_stringbuf(basic_ios::openmode)
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator>::
basic_stringbuf( ios_base::openmode which)
: basic_streambuf<charT, traits>()
, data_(0)
, length_(0)
, end_pos(0)
{
  setp(0,0);
  setg(0,0,0);
  basic_streambuf<charT,traits>::mode_ = which;
}

/*
 * basic_stringbuf(const basic_string, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator>::
basic_stringbuf(const string_type& s, ios_base::openmode which)
: basic_streambuf<charT, traits>()
, length_(s.length())
{
  basic_streambuf<charT,traits>::mode_ = which;

  data_ = new charT[length_];

  if(s.data())
    traits::copy(data_, s.data(), length_);

  if(which & ios_base::in)
    setg(data_, data_, data_+length_);

  if(which & ios_base::out)
    setp(data_, data_+length_);

  if(which & ( ios_base::app | ios_base::ate ) )
    pbump(length_);

  end_pos = length_;

}

/*
 * virtual ~basic_stringbuf()
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator>::~basic_stringbuf()
{
  if ( data_ )
   delete [] data_;
}


/*
 * basic_string str() const
 */

template<class charT, class traits, class Allocator>
basic_string<charT, traits, Allocator>
basic_stringbuf<charT, traits, Allocator>::str() const
{

  if ( end_pos == 0 )  return string_type();

  if ( (end_pos > ( pptr() - pbase() )) && (end_pos > ( egptr() - eback() )) )
   return string_type(data_, end_pos);
  else 
   {
      if ( ( pptr() - pbase() ) > ( egptr() - eback() ) )
       return string_type(data_, pptr() - pbase() );
      else
       return string_type(data_, egptr() - eback() );
   }
   
}

/*
 * void str(const basic_string&)
 */

template<class charT, class traits, class Allocator>
void 
basic_stringbuf<charT, traits, Allocator>::
str(const string_type& s)
{
  if ( data_ )
   delete [] data_;
  length_ = s.length();

  if(length_ == 0) {
    setg(0,0,0);
    setp(0,0);
    return;
  }

  data_ = new charT[length_];

  if(s.data())
    traits::copy(data_, s.data(), length_);
    
  if(basic_streambuf<charT,traits>::mode_ & ios_base::in)
    setg(data_, data_, data_+length_);

  if(basic_streambuf<charT,traits>::mode_ & ios_base::out)
   {
    setp(data_, data_+length_);
    if( (basic_streambuf<charT,traits>::mode_ & ios_base::app) || (basic_streambuf<charT,traits>::mode_ & ios_base::ate ) )
    this->pbump(length_); 
   } 

  end_pos = pptr() - pbase();
}


/*
 * int_type overflow(int_type)
 */

template<class charT, class traits, class Allocator>
_TYPENAME basic_stringbuf<charT, traits, Allocator>::int_type
basic_stringbuf<charT, traits, Allocator>::overflow(int_type c)
{
  if((basic_streambuf<charT,traits>::mode_ & ios_base::out) == 0) {
    return traits::eof();
  }
  
  charT        *temp;
  int          old_numb_of_elements,new_numb_of_elements;
  const int    increment=128;
  int_type     var;

  if (pptr())
    {
       old_numb_of_elements = pptr() - data_;
       
       new_numb_of_elements = old_numb_of_elements + increment;

       temp = new charT[new_numb_of_elements];
        
       traits::copy(temp, data_, old_numb_of_elements);   

       setp (temp,temp+new_numb_of_elements);

       this->pbump(old_numb_of_elements);  

     
       charT *tmp=temp+(gptr()-eback()); 
       setg(temp, tmp, pptr()+1);

       delete [] data_;
       
       data_ = temp;
     }
   else
     {
        new_numb_of_elements=increment;

        temp = new charT[new_numb_of_elements];
         
        setp(temp,temp+new_numb_of_elements);

        if((basic_streambuf<charT,traits>::mode_ & ios_base::in) != 0)
           setg(temp, temp , temp);

        data_ =temp; 
     }
  
  if ( traits::eq_int_type(c,traits::eof()) ) var = traits::not_eof(c);
  else
   var = sputc(c);

  if ( (pptr() - pbase()) > end_pos )
   end_pos = pptr() - pbase(); 

  return var;
}

/*
 * int_type pbackfail(int_type)
 */

template<class charT, class traits, class Allocator>
_TYPENAME basic_stringbuf<charT, traits, Allocator>::int_type
basic_stringbuf<charT, traits, Allocator>::pbackfail(int_type c)
{
  
   if ( (!traits::eq_int_type(c,traits::eof())) && (gptr()>eback()) )  {

                         if ( traits::eq(*(gptr()-1),traits::to_char_type(c)) ) 
                                 {
                                   this->gbump(-1);
                                   return c;
                                 }
                                else
                                 {
                                   if( basic_streambuf<charT,traits>::mode_ & ios_base::out )
                                     {
                                       this->gbump(-1);
                                       *gptr()=traits::to_char_type(c);
                                       return c;
                                     }
                                 }
			     }	

if ( (traits::eq_int_type(c,traits::eof())) && (gptr()>eback()) )  { 
						   this->gbump(-1);
                                                   return traits::not_eof(c);
						}
                                                

  return traits::eof();
}


/*
 * basic_streambuf<charT,traits>* setbuf(char_type* s, streamsize n)
 */

template<class charT, class traits, class Allocator>
basic_streambuf<charT, traits>*
basic_stringbuf<charT, traits, Allocator>::setbuf(char_type* s, streamsize n)
{
 if((basic_streambuf<charT,traits>::mode_ & ios_base::out) != 0)
  {
    if ( n > ( pptr() - pbase() ) )
     {
       if ( s == 0 ) s = new charT[n];

         if ( s )
          {
            int          old_numb_of_elements;

            if (pptr())
             {
               old_numb_of_elements = pptr() - data_;
        
               traits::copy(s, data_, old_numb_of_elements);   

               setp (s,s+n-1);

               this->pbump(old_numb_of_elements);  

              charT *tmp=s+(gptr()-eback()); 
              setg(s, tmp, pptr()+1);

              delete [] data_;
       
              data_ = s;
             }
            else
             {
         
               setp(s,s+n-1);

               if((basic_streambuf<charT,traits>::mode_ & ios_base::in) != 0)
                setg(s, s , s);

               data_ =s; 
              }
           }
          else
           return (basic_streambuf<charT,traits>*)(0);


     }
    else
     return (basic_streambuf<charT,traits>*)(0);
  }

 return (basic_streambuf<charT,traits>*)(this);
}


/*
 * int_type underflow()
 */

template<class charT, class traits, class Allocator>
_TYPENAME basic_stringbuf<charT, traits, Allocator>::int_type
basic_stringbuf<charT, traits, Allocator>::underflow()
{
   if(gptr() && (gptr()<egptr()) ) return traits::to_int_type(*gptr());


    if(((pptr() != 0) && ( (pptr() > egptr()) || ( end_pos > (egptr() - eback()) ) ) ) && (!gptr())) 
      {
        if ( end_pos > ( pptr() - pbase() ) )
         setg(pbase(), pbase(), pbase()+end_pos );
        else     
         setg(pbase(), pbase(), pptr());
        return traits::to_int_type(*gptr());
      }
     


     if((pptr() != 0) && ( (pptr() > egptr()) || ( end_pos > ( egptr() - eback() )) ) ) 
     {
       if ( end_pos > ( pptr() - pbase() ) )
        setg(eback(),gptr(),eback()+end_pos);
       else
        setg(eback(), gptr(), pptr());
       return traits::to_int_type(*gptr());
     }
                                  
  return traits::eof();
}

/*
 * pos_type seekoff(off_type, ios_base::seekdir, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
_TYPENAME basic_stringbuf<charT, traits, Allocator>::pos_type
basic_stringbuf<charT, traits, Allocator>::
seekoff(off_type off, ios_base::seekdir way, ios_base::openmode which)
{
  streamsize       newoff;
  
  if((which & ios_base::in) && (which & ios_base::out)) {
     if ( (way==ios_base::beg) || (way==ios_base::end) )
       {
         if ( seekoff(off,way,ios_base::out) == pos_type(off_type(-1)) )
          return pos_type(off_type(-1));
         return seekoff(off,way,ios_base::in);
       }
     else
       return pos_type(off_type(-1)); 
  }
  
  if((which & ios_base::in) && (gptr()!=0)) {

    if ( (egptr() - eback()) > end_pos )
     end_pos = egptr() - eback(); 

    if ( (pptr() - pbase()) > end_pos )
     end_pos = pptr() - pbase(); 

    if(way == ios_base::beg)
      newoff = 0;
    if(way == ios_base::cur)
      newoff = gptr() - eback();
    if(way == ios_base::end)
      newoff = end_pos;

    if ( newoff<0 )  return pos_type(off_type(-1)); 

     if ( ((eback()+long(newoff)+long(off))> egptr()) || ((newoff+off)< 0) )
    return pos_type(-1);

      setg(eback(), eback() + long(newoff) + long(off), egptr());

    return pos_type(newoff+off);
  }

 if((which & ios_base::in) && (gptr()==0) && (egptr()==0) && (eback()==0) ) 
   return pos_type(0);

  if((which & ios_base::out) && (pptr()!=0) && !(basic_streambuf<charT,traits>::mode_ & ios_base::app) ) {

    if ( (egptr() - eback()) > end_pos )
     end_pos = egptr() - eback(); 

    if ( (pptr() - pbase()) > end_pos )
     end_pos = pptr() - pbase(); 

    if(way == ios_base::beg)
      newoff = 0;
    if(way == ios_base::cur)
      newoff = pptr() - pbase();
    if(way == ios_base::end)
      newoff = end_pos;

    if ( (pptr() - pbase()) > end_pos ) end_pos = pptr() - pbase();

    if ( ((newoff+off)<0) || ((pbase()+long(newoff)+long(off))> epptr()) )
    return pos_type(off_type(-1));   

    this->pbump( newoff+off-(pptr()-pbase())  );

    if ( gptr()<=pptr() )
     setg(eback(),gptr(),pptr());
    else
     {
       if ( (basic_streambuf<charT,traits>::mode_ & ios_base::out  ) &&
          !(basic_streambuf<charT,traits>::mode_ & ios_base::in ) )
        setg(pbase(),pptr(),epptr());
     } 

    return pos_type(newoff+off);
        
   }
   else 
    {

     if ( basic_streambuf<charT,traits>::mode_ & ios_base::app )
      return pos_type(pptr()-pbase());

     if((which & ios_base::out) && (pptr()==0) && (epptr()==0) && (pbase()==0) )
      return pos_type(0);
    }


  return pos_type(off_type(-1));
}

/*
 * pos_type seekpos(pos_type, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
_TYPENAME basic_stringbuf<charT, traits, Allocator>::pos_type
basic_stringbuf<charT, traits, Allocator>::
seekpos(pos_type sp, ios_base::openmode which)
{
  streamsize    newoff = sp.offset();

   
   if((which & ios_base::in) && (which & ios_base::out)) {
         if ( seekpos(sp,ios_base::out) == pos_type(off_type(-1)) )
          return pos_type(off_type(-1));
         return seekpos(sp,ios_base::in);
  }

  if((which & ios_base::in) && (gptr()!=0)) {
    
    if ( newoff<0 )  return pos_type(off_type(-1));
    
    if ( (eback()+long(newoff))> egptr() ) return pos_type(off_type(-1));

      setg(eback(), eback() + long(newoff), egptr());

    return pos_type(newoff);
  }


  if((which & ios_base::out) && (pptr()!=0) && !(basic_streambuf<charT,traits>::mode_ & ios_base::app) ) {

    if ( (newoff<0) || ((pbase()+long(newoff))> epptr()) )
    return pos_type(off_type(-1)); 

    if ( (pptr() - pbase()) > end_pos ) end_pos = pptr() - pbase();

    this->pbump( newoff-(pptr()-pbase())  );

    if ( gptr()<=pptr() )
     setg(eback(),gptr(),pptr());
    else
     {
       if ( (basic_streambuf<charT,traits>::mode_ & ios_base::out  ) &&
          !(basic_streambuf<charT,traits>::mode_ & ios_base::in ) )
        setg(pbase(),pptr(),epptr());
     }

    return pos_type(newoff);

   }
  else 
    if ( basic_streambuf<charT,traits>::mode_ & ios_base::app )
     return pos_type(pptr()-pbase());

  return pos_type(off_type(-1));

}

/*
 * streamsize xsputn(const char_type *, streamsize)
 */

template<class charT, class traits, class Allocator>
streamsize basic_stringbuf<charT, traits, Allocator>::
xsputn(const char_type *s, streamsize n)
{
  if ( !s || (n == 0) ) return 0;

 if ( n > ( epptr()-pptr()+128 ) )
 { 
   if ( setbuf(0, pptr()-pbase()+n+128)== 0)
    {
      return 0;
    }

  traits::copy(pptr(), s, n);
  this->pbump(n);
 }
 else
 {
    int         i=0;

    while((i < n) && ( !traits::eq_int_type(sputc(*s++),traits::eof())))
    i++;

    return i;
 }

  return n;    
}


 
/*
 *
 * class basic_istringstream
 *
 */

/*
 * basic_istringstream(ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_istringstream<charT, traits, Allocator>::
basic_istringstream(ios_base::openmode m)
: basic_istream<charT, traits>( )
, sb_(m | ios_base::in)
{
  init(&sb_);
}

/*
 * basic_istringstream(const basic_string<charT>&, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_istringstream<charT, traits, Allocator>::
basic_istringstream(const string_type& s, ios_base::openmode which)
: basic_istream<charT, traits>( )
, sb_(s, which | ios_base::in)
{
  init(&sb_);
}

/*
 * virtual ~basic_istringstream()
 */

template<class charT, class traits, class Allocator>
basic_istringstream<charT, traits, Allocator>::~basic_istringstream()
{

}

/*
 * basic_stringbuf *rdbuf() const
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator> *
basic_istringstream<charT, traits, Allocator>::rdbuf() const
{
  return (basic_stringbuf<charT, traits, Allocator> *)&sb_;
}

/*
 * basic_string<charT> str() const
 */

template<class charT, class traits, class Allocator>
basic_string<charT, traits, Allocator>
basic_istringstream<charT, traits, Allocator>::str() const
{
  return sb_.str();
}

/*
 * void str(const basic_string<charT>& )
 */

template<class charT, class traits, class Allocator>
void basic_istringstream<charT, traits, Allocator>::str(const string_type& s)
{
  sb_.str(s);
}

/*
 *
 *
 * class basic_ostringstring
 *
 */

/*
 * basic_ostringstream(ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_ostringstream<charT, traits, Allocator>::
basic_ostringstream(ios_base::openmode w)
: basic_ostream<charT, traits>( )		
, sb_(w | ios_base::out )
{
   init(&sb_);
}

/*
 * basic_ostringstream(const basic_string&, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_ostringstream<charT, traits, Allocator>::
basic_ostringstream(const string_type& s, ios_base::openmode which)
: basic_ostream<charT, traits>( )
, sb_(s, which | ios_base::out )
{
  init(&sb_);
}

/*
 * virtual ~basic_ostringstream()
 */

template<class charT, class traits, class Allocator>
basic_ostringstream<charT, traits, Allocator>::~basic_ostringstream()
{

}

/*
 * basic_stringbuf *rdbuf() const
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator> *
basic_ostringstream<charT, traits, Allocator>::rdbuf() const
{
    return (basic_stringbuf<charT, traits, Allocator> *)&sb_;
}

/*
 * basic_string str() const
 */

template<class charT, class traits, class Allocator>
basic_string<charT, traits, Allocator>
basic_ostringstream<charT, traits, Allocator>::str() const
{
  return sb_.str();
}

/*
 * void str(const basic_string& s)
 */

template<class charT, class traits, class Allocator>
void basic_ostringstream<charT, traits, Allocator>::
str(const string_type& s)
{
  sb_.str(s);
}

/*
 *
 *
 * class basic_stringstream
 *
 */

/*
 * basic_stringstream(ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_stringstream<charT, traits, Allocator>::
basic_stringstream(ios_base::openmode w)
: basic_iostream<charT, traits>( )		
, sb_(w)
{
   init(&sb_);
}

/*
 * basic_stringstream(const basic_string&, ios_base::openmode)
 */

template<class charT, class traits, class Allocator>
basic_stringstream<charT, traits, Allocator>::
basic_stringstream(const string_type& s, ios_base::openmode which)
: basic_iostream<charT, traits>( )
, sb_(s, which)
{
  init(&sb_);
}

/*
 * virtual ~basic_stringstream()
 */

template<class charT, class traits, class Allocator>
basic_stringstream<charT, traits, Allocator>::~basic_stringstream()
{

}

/*
 * basic_stringbuf *rdbuf() const
 */

template<class charT, class traits, class Allocator>
basic_stringbuf<charT, traits, Allocator> *
basic_stringstream<charT, traits, Allocator>::rdbuf() const
{
    return (basic_stringbuf<charT, traits, Allocator> *)&sb_;
}

/*
 * basic_string str() const
 */

template<class charT, class traits, class Allocator>
basic_string<charT, traits, Allocator>
basic_stringstream<charT, traits, Allocator>::str() const
{
  return sb_.str();
}

/*
 * void str(const basic_string& s)
 */

template<class charT, class traits, class Allocator>
void basic_stringstream<charT, traits, Allocator>::
str(const string_type& s)
{
  sb_.str(s);
}


#ifndef _RWSTD_NO_NAMESPACE
}
#endif

