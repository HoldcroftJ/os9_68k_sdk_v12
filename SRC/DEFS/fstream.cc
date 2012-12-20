/***************************************************************************
 *
 * fstream.cc - Definition for the Standard Library file streams
 *
 * $Id: fstream.cc,v 1.79 1996/09/27 07:04:46 philippe Exp $
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
#include <cstdio>
#else
#include <stdio.h>
#endif


#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

//
// Work-around for Borland problem.
//
#ifdef __BORLANDC__
template <class T>
void __io_initialize (T& t, T val) { t = val; }
#endif

/*
 *
 *
 * class basic_filebuf
 *
 *
 */

/*
 * basic_filebuf()
 */

template<class charT, class traits>
basic_filebuf<charT, traits>::basic_filebuf()
: basic_streambuf<charT, traits>()
, file_(-1)
, data_(0)
, read_buff_(FALSE)
, write_buff_(FALSE)
, last_numb_read_(0)
, __rwBufferSize(512)
, absolute_pos(0)
{
  state_beg = new state_t;
  state_end = new state_t;
}

/*
 * basic_filebuf(int)
 */

template<class charT, class traits>
basic_filebuf<charT, traits>::basic_filebuf(int fd)
: basic_streambuf<charT, traits>()
, file_(fd)
, read_buff_(FALSE)
, write_buff_(FALSE)
, last_numb_read_(0)
, __rwBufferSize(512)
, absolute_pos(0)
{
  data_ = new char_type[__rwBufferSize+1];
  state_beg = new state_t;
  state_end = new state_t;

  if ( fd == 0 )
   basic_streambuf<charT,traits>::mode_ = ios_base::in;
  else
  {
    if ( fd < 3 )
     basic_streambuf<charT,traits>::mode_ = ios_base::out;
  }

}


/*
 * ~basic_filebuf()
 */

template<class charT, class traits>
basic_filebuf<charT, traits>::~basic_filebuf()
{
  close();
  delete state_beg;
  delete state_end;
}

/*
 * bool is_open() const
 */

template<class charT, class traits>
bool basic_filebuf<charT, traits>::is_open() const
{
  return !(file_ == -1);
}

/*
 * basic_filebuf *open(int fd)
 *
 * fstream compatibility
 */


template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_filebuf<charT, traits>::
open(int fd )
{   
  if(file_ != -1)
	 return 0;

  this->streambuf_init(FALSE); 

  file_           = fd;
  read_buff_      = FALSE;
  write_buff_     = FALSE;
  last_numb_read_ = 0; 

  delete [] data_;

  data_ = new char_type[__rwBufferSize+1];

  if ( fd == 0 )
   basic_streambuf<charT,traits>::mode_ = ios_base::in;
  else
  {
    if ( fd < 3 )
     basic_streambuf<charT,traits>::mode_ = ios_base::out;
  }

  return this;
}



/*
 * basic_filebuf *open(const char *, ios_base::openmode, long )
 *
 * opens a file and allocates a buffer
 */


template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_filebuf<charT, traits>::
open(const char *s, ios_base::openmode mode, long protection)
{
  #if !defined(_RWSTD_NO_NAMESPACE) && !defined(_RWSTD_NO_NEW_HEADER)
//using std::open;
//we have open() in namespace __unix -- for OS9 (tag)
  using __unix::open;
  #endif

  long    m;

  if ( mode & ios_base::ate )
   m = mode & (~ios_base::ate) | ios_base::app;
  else
   m = mode;
  
  if(file_ != -1)
	 return 0;

  basic_streambuf<charT,traits>::mode_ = mode;

  this->streambuf_init(FALSE);

  if(__rwOpenTable[m] == -1) 
   return 0;

 #if !defined(_RWSTD_NO_NAMESPACE) && !defined(_RWSTD_NO_NEW_HEADER)
   if((file_ = open(s, __rwOpenTable[m], protection)) == -1)
 #else
   if((file_ = ::open(s, __rwOpenTable[m], protection)) == -1)
 #endif
   return 0;

  delete [] data_;

  data_ = new char_type[__rwBufferSize+1];

  if(mode & ios_base::ate) {
//	 if( (absolute_pos = lseek(file_, 0, SEEK_END))== -1  ) {
// use __unix::lseek instead of ::lseek for OS9 (tag)
     if( (absolute_pos = __unix::lseek(file_, 0, SEEK_END))== -1  ) { 
		close();
		delete [] data_;  
		data_ = 0;
                file_=-1;
		return 0;
	 }
  }

	
  return this;
}

/*
 * basic_filebuf *close()
 *
 * closes a file if one was already open.  Also deletes the
 * data, if it was allocated
 */

template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_filebuf<charT, traits>::close()
{
  #if !defined(_RWSTD_NO_NAMESPACE) && !defined(_RWSTD_NO_NEW_HEADER)
//  using std::close;
// we have __unix::close
	using __unix::close;
  #endif

delete state_beg;
delete state_end;
state_beg = new state_t;
state_end = new state_t;

absolute_pos = 0;

if ( (file_ < 3)  && (file_ != -1) )
 {
    if ( write_buff_ )
     overflow( traits::eof() );
    read_buff_=FALSE;
    write_buff_=FALSE;
    last_numb_read_=0;
    return this;
 }
   
  if(file_ != -1) {          
    if ( write_buff_ )
     overflow( traits::eof() );
    delete [] data_;
    data_ = 0;
    read_buff_=FALSE;
    write_buff_=FALSE;
    last_numb_read_=0;
 
  #if !defined(_RWSTD_NO_NAMESPACE) && !defined(_RWSTD_NO_NEW_HEADER)
    if(close(file_) == -1)
  #else
//    if(::close(file_) == -1)
    if(__unix::close(file_) == -1)	// for OS9 (tag)
  #endif
      return 0;

    file_ = -1;
 
    return this;
  }

  return 0;          
}

/*
 * int showmanyc()
 */

template<class charT, class traits>
int
basic_filebuf<charT, traits>::showmanyc()
{ 
   if ( gptr() )
    return (int)(egptr()-gptr());
   
   if ( pptr() && read_buff_ )
        return (int)(epptr()-pptr());   

   return 0;

}



/*
 * int_type overflow(int_type)
 *
 * this is called when the put pointer overruns the buffer.
 * it should flush what was in the put buffer, and move the pointer
 * back to the beginning
 */

template<class charT, class traits>
_TYPENAME basic_filebuf<charT, traits>::int_type
basic_filebuf<charT, traits>::overflow(int_type c)
{
  long  count = pptr() - pbase();
  bool   do_noconv;

  if(file_ == -1)        
   return traits::eof();

  if ( (pptr()==0) && gptr() )
   {
      setp(eback(),eback()+__rwBufferSize);
      pbump((int)(gptr()-eback()));
      setg(0,0,0);
      if ( pptr()<epptr() )
       { 
         if( !traits::eq_int_type(c,traits::eof()) ) 
          {
	   sputc( traits::to_char_type(c) );
	   this->gbump(1);
           write_buff_=TRUE;
           return traits::to_int_type(*pptr());
         }
       } 
      count=  pptr() - pbase();
   }

  if ( (pptr()==0) && (gptr()==0) )
    setp(data_,data_+ __rwBufferSize);  

  write_buff_= FALSE;

  if(count)
   {        
    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
     do_noconv = use_facet< codecvt<charT,char,state_t> >(this->getloc())
    #else
     do_noconv = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
    #endif
      .always_noconv();

    if ( do_noconv )
     {
        if ( read_buff_ ) 
         {
//           if ( lseek(file_, -last_numb_read_, SEEK_CUR) == -1 )
// use __unix::lseek instead of ::lseek for OS9 (tag)
           if ( __unix::lseek(file_, -last_numb_read_, SEEK_CUR) == -1 )   
            return traits::eof();
         }

//        if(write(file_,(char *)pbase(), count*sizeof(charT))!= 
// use __unix::write instead of ::write for OS9 (tag
        if(__unix::write(file_,(char *)pbase(), count*sizeof(charT))!= 
                 count*sizeof(charT))
         return traits::eof();
     }
    else
     {
      long size_to = count*sizeof(charT);
      const charT *from_next =0;
      char *to= new char[size_to];
      char *to_next = 0;
      codecvt_base::result conv_result;

      if ( read_buff_ )
       {
//         if ( lseek(file_, -last_numb_read_, SEEK_CUR) == -1 )
// use __unix::lseek instead of ::lseek for OS9 (tag)
         if ( __unix::lseek(file_, -last_numb_read_, SEEK_CUR) == -1 )
          return traits::eof();
         else
          {
            state_t *tmp = state_beg;
            state_beg = state_end;
            state_end = tmp;
          }
       }
 
       *state_beg = *state_end;

    do {

       #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
        conv_result = use_facet< codecvt<charT,char,state_t> >(this->getloc())
       #else
        conv_result = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
       #endif
        .out(*state_end,pbase(),pptr(),from_next,
                 to,to+size_to,to_next); 

      if ( conv_result == codecvt_base::noconv ) break;

      if (  (conv_result == codecvt_base::partial) || ( from_next != pptr() ) )
       {
         size_to *= 2;
         char *to_bis = new char[size_to];
         long diff_to = to_next - to;
         memcpy(to_bis,to,diff_to);
         delete [] to;
         to = to_bis;
         to_next = to + diff_to;
       }

      }while( (conv_result == codecvt_base::partial) || ( from_next != pptr()));

     if ( conv_result==codecvt_base::error )
      {
         delete [] to;
         return traits::eof();
      }

     if (conv_result == codecvt_base::noconv) 
      {
//       if(write(file_, (char*)pbase(), sizeof(charT)*count)
// use __unix::write instead of ::write for OS9 (tag)
       if(__unix::write(file_, (char*)pbase(), sizeof(charT)*count)
          != sizeof(charT) * count)
         {
          delete [] to;
          return traits::eof();              
         }
      } 
     else 
      {
//       if(write(file_, to, (to_next-to)) != (to_next-to))
// use __unix::write instead of ::write for OS9 (tag)
       if(__unix::write(file_, to, (to_next-to)) != (to_next-to))  
        {
          delete [] to;
          return traits::eof();                
        }
       }

      delete [] to;
    }
                
  }

  setp(data_, data_+__rwBufferSize);    
  setg(0,0,0);  			 
  read_buff_ = FALSE;		
  write_buff_= FALSE; 

//  absolute_pos = lseek(file_,0,SEEK_CUR);
// use __unix::lseek instead of ::lseek for OS9 (tag)
  absolute_pos = __unix::lseek(file_,0,SEEK_CUR);

  if( !traits::eq_int_type(c,traits::eof()) ) 
   {
     sputc(traits::to_char_type(c));
     this->gbump(1);
     write_buff_ = TRUE;
   }
  else
   setp(0,0);

  return traits::not_eof(c);
}

/*
 * int_type pbackfail(int_type)
 */


template<class charT, class traits>
_TYPENAME basic_filebuf<charT, traits>::int_type
basic_filebuf<charT, traits>::pbackfail(int_type c)
{
  if(file_ == -1)
	 return traits::eof();

  if ( (gptr()==0) && pptr() )
   {
      if ( read_buff_ )
       setg(pbase(),pptr(),epptr());
      else
       setg(pbase(),pptr(),pptr()); 
   }

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

  if ( (traits::eq_int_type(c,traits::eof())) && (gptr()>eback()) )  
   { 
     this->gbump(-1);
     return traits::not_eof(c);
   }
                                                
  return traits::eof();
 }

/*
 * basic_streambuf<charT,traits>* setbuf(char_type*,streamsize)
 */

template<class charT, class traits>
basic_streambuf<charT, traits>*
basic_filebuf<charT, traits>::setbuf(char_type *s, streamsize n)
{

 if (n>0)
 { 

  if ( file_ != -1 )
   {
     if ( !traits::eq_int_type(overflow( traits::eof() ),traits::eof()) )
       {
          if ( s )
           { 
              delete [] data_;
              __rwBufferSize = n-1; 
              data_=s;
           }
          else
           {
              charT *tmp;
              __rwBufferSize = n;  
              tmp = new char_type[__rwBufferSize+1];
              delete [] data_;
              data_ = tmp;
           }

          setp(0,0);    
          setg(0,0,0);                         
          read_buff_=FALSE;
          write_buff_=FALSE;
       }
   }
  else
    {
      if ( s )
       {
          __rwBufferSize = n-1;
          data_ =s;
          setp(0,0);    
          setg(0,0,0);                        
          write_buff_= FALSE;
          read_buff_ = FALSE; 
       }
      else
      __rwBufferSize = n;
    }  
 } 
  return (basic_streambuf<charT,traits>*)(this); 
  
}


/*
 * int_type underflow()
 *
 * this is called when the get pointer "underflows" the buffer,
 * or when there are no more "get" characters.
 */

template<class charT, class traits>
_TYPENAME basic_filebuf<charT, traits>::int_type
basic_filebuf<charT, traits>::underflow()
{
  bool do_noconv;
  long offset;
#ifndef __BORLANDC__
  offset = 0;
#else
  __io_initialize(offset,0L);
#endif

  if(file_ == -1)
    return traits::eof();

 
  if ( (gptr()==0) && pptr() )
   {
      if ( read_buff_ )
       setg(pbase(),pptr(),epptr());
      else
       setg(pbase(),pptr(),pptr());

      setp(0,0);
   }

  if(gptr() && (gptr()<egptr()) ) return traits::to_int_type(*gptr());     

  if( ((write_buff_) && gptr() ) || ((write_buff_) && pptr() ) )
    {
         if ( traits::eq_int_type(overflow( traits::eof() ),traits::eof()) ) 
           return traits::eof();
         setg(pbase(),pptr(),epptr());
         setp(0,0);
         write_buff_ = FALSE; 
    }                 

//    absolute_pos = lseek(file_,0,SEEK_CUR);
// use __unix::lseek instead of ::lseek for OS9 (tag)
    absolute_pos = __unix::lseek(file_,0,SEEK_CUR);

    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      do_noconv = use_facet< codecvt<charT,char,state_t> >(this->getloc())
    #else
      do_noconv = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
    #endif
      .always_noconv();

    if ( do_noconv )
     {
//      last_numb_read_ = read(file_, (char *)data_,__rwBufferSize*sizeof(charT));
// use __unix::read instead of ::read for OS9 (tag)
      last_numb_read_ = __unix::read(file_, (char *)data_,__rwBufferSize*sizeof(charT));

      if( last_numb_read_ <= 0 ) 
       { 
         if ( write_buff_ ) overflow( traits::eof() );
         write_buff_ = FALSE;
	 setp(0,0);       
         setg(0,0,0);
         read_buff_=FALSE;
	 return traits::eof();
       }
      else
       offset = last_numb_read_/sizeof(charT);

     }
    else
     {
      long from_size = __rwBufferSize*sizeof(charT);
      const char *from_next =0;
      char *from= new char[from_size];
      charT *to_next = 0;
      codecvt_base::result conv_result;

      last_numb_read_ = 0;
      long new_numb_read;


      do {
//           new_numb_read = read(file_, from+last_numb_read_
// use __unix::read instead of ::read for OS9 (tag)
           new_numb_read = __unix::read(file_, from+last_numb_read_
                                ,__rwBufferSize*sizeof(charT));

           last_numb_read_ += new_numb_read;

        if( last_numb_read_ <= 0 ) 
            { 
             if ( write_buff_ ) overflow( traits::eof() );
             write_buff_ = FALSE;
	     setp(0,0);       
             setg(0,0,0);
             read_buff_=FALSE;
	     return traits::eof();
           }

         if( new_numb_read <= 0 ) 
          break;

       *state_beg = *state_end;

       #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
        conv_result = use_facet< codecvt<charT,char,state_t> >(this->getloc())
       #else
        conv_result = use_facet(this->getloc(), (codecvt<charT,char,state_t>* )0)
       #endif
        .in(*state_end,from,from+last_numb_read_,from_next,data_,
            data_+__rwBufferSize,to_next); 
 
         if ( (conv_result == codecvt_base::error) ||
              (conv_result == codecvt_base::noconv) ) break;

         if (  (to_next != (data_ + __rwBufferSize) ) &&
                ( new_numb_read == __rwBufferSize*sizeof(charT) ) )
           {
             from_size *= 2;
             char *from_bis = new char[from_size];
             long diff_from = from_next - from;
             memcpy(from_bis,from,last_numb_read_);
             delete [] from;
             from = from_bis;
             from_next = from + diff_from;
           }

          } while ( (to_next != (data_ + __rwBufferSize) ) &&
                    ( new_numb_read == __rwBufferSize*sizeof(charT) ) ); 
               
       if ( (conv_result==codecvt_base::error) ||
            (conv_result==codecvt_base::partial) )
       {
         delete [] from;
         return traits::eof();
       } 
      else 
       {
         if (conv_result==codecvt_base::noconv)
          {
            memcpy((char*)data_,from,last_numb_read_);
            offset = last_numb_read_;
          }
         else
            {
               long diff_minus = last_numb_read_ - (from_next - from);
//               if ( lseek(file_, -diff_minus, SEEK_CUR) == -1 )
// use __unix::lseek instead of ::lseek for OS9 (tag)
               if ( __unix::lseek(file_, -diff_minus, SEEK_CUR) == -1 )
                return traits::eof();
               last_numb_read_ -= diff_minus;
               offset = to_next - data_;
            }
        }

      delete [] from;
    }
        
  if(last_numb_read_) 
   {   
     setg(data_, data_, data_+offset); 
     setp(0, 0);
     read_buff_=TRUE;
     return traits::to_int_type(*gptr());
   }

  return traits::eof();
}

/*
 * pos_type seekoff(off_type, ios_base::seekdir, ios_base::openmode)
 */

template<class charT, class traits>
_TYPENAME basic_filebuf<charT, traits>::pos_type
basic_filebuf<charT, traits>::seekoff(off_type off,
                                      ios_base::seekdir way,ios_base::openmode)
{
  off_type            loff = off;
  pos_type            newoff;

  if(file_ == -1)
    return pos_type(off_type(-1));

  if ( off == off_type(-1) )
    return pos_type(off_type(-1));

  if ( (way==ios_base::cur) && (off==off_type(0)) )
  {
     streamsize remain_in_buffer=0;
     
     if ( read_buff_ )
      {
        if ( gptr() )  
          remain_in_buffer= egptr()-gptr();
        else
         {
           if ( pptr() )  remain_in_buffer= epptr()-pptr();
         }
      }
     else
      if ( write_buff_ )
        {
          if ( pptr() )  
           remain_in_buffer= -(pptr()-pbase());
          else
          { 
           if ( gptr() )  remain_in_buffer= -(gptr()-eback());
          }
        }     

    int const_conv;

    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      const_conv = use_facet< codecvt<charT,char,state_t> >(this->getloc())
    #else
      const_conv = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
    #endif
      .encoding(); 

  if ( const_conv > 0 )
   {
//    return ( (lseek(file_,0,SEEK_CUR)-remain_in_buffer*const_conv)/const_conv);
// use __unix::lseek instead of ::lseek for OS9 (tag)
    return ( (__unix::lseek(file_,0,SEEK_CUR)-remain_in_buffer*const_conv)/const_conv);
   }
  else
   {  
      streamsize relative_pos = gptr() - eback() + pptr() - pbase();

      pos_type position(relative_pos,absolute_pos,*state_beg); 
      return position;       
   }
  } 

   if ( way==ios_base::cur )
  {
     int remain_in_buffer=0;
 
     if ( ((read_buff_) && !(write_buff_)) ||  ((read_buff_) && (write_buff_)) )
      {
        if ( pptr() )  remain_in_buffer= epptr()-pptr();
        if ( gptr() )  remain_in_buffer= egptr()-gptr();
      }
     
     if ( (write_buff_) && !(read_buff_) )
        {
          remain_in_buffer= 0;
        }   
 
     loff-= remain_in_buffer;

  } 
  
    int const_conv;

    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      const_conv = use_facet< codecvt<charT,char,state_t> >(this->getloc())
    #else
      const_conv = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
    #endif
      .encoding(); 

   if ( const_conv < 1 )
   {
     if ( (way==ios_base::beg) && (off==off_type(0)) )
     {
        seekpos(0);
     }
     else 
      return pos_type(off_type(-1));
   }


  if ( write_buff_)
   {  
      overflow( traits::eof() );
      write_buff_ =FALSE;  
   }

  
  int                 w = (way == ios_base::beg) ?
                           SEEK_SET : ((way == ios_base::cur) ? SEEK_CUR :
                                                          SEEK_END);  
//  newoff = lseek(file_, loff*const_conv, w);
// use __unix::lseek instead of ::lseek for OS9 (tag)
  newoff = __unix::lseek(file_, loff*const_conv, w);

  setp(0,0);
  setg(0,0,0);

  return newoff;
}

/*
 * pos_type seekpos(pos_type, ios_base::openmode)
 */

template<class charT, class traits>
_TYPENAME basic_filebuf<charT, traits>::pos_type
basic_filebuf<charT, traits>::
seekpos(pos_type sp, ios_base::openmode which)
{ 
   if(file_ == -1)
    return pos_type(off_type(-1));

   if( sp == pos_type(off_type(-1)) )
    return pos_type(off_type(-1)); 

   int const_conv;

    #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      const_conv = use_facet< codecvt<charT,char,state_t> >(this->getloc())
    #else
      const_conv = use_facet( this->getloc(), (codecvt<charT,char,state_t>* )0)
    #endif
      .encoding();

   if ( const_conv > 0 )
      return seekoff(off_type(sp.offset()), ios_base::beg, which);

//   _TYPENAME traits::off_type end_pos = lseek(file_,0,SEEK_END);
// use __unix::lseek instead of ::lseek for OS9 (tag)
   _TYPENAME traits::off_type end_pos = __unix::lseek(file_,0,SEEK_END);
   
// same here
//   if ( lseek(file_, sp.pos(), SEEK_SET) == -1 )
   if ( __unix::lseek(file_, sp.pos(), SEEK_SET) == -1 )

    return pos_type(off_type(-1));

   setp(0,0);
   setg(0,0,0); 

   *state_beg = *state_end = sp.state();

   if ( traits::eq_int_type(underflow(),traits::eof()) && (end_pos > sp.pos()))
     return pos_type(off_type(-1));

   this->gbump(sp.offset());

   return sp;   
}

/*
 * int sync()
 *
 * this synchronizes the buffers and their streams
 */

template<class charT, class traits>
int basic_filebuf<charT, traits>::sync()
{
 if ( this->which_open_mode( ) & ios_base::out )
{
 if ( file_ == 1 )
  {
    if( traits::eq_int_type(overflow(traits::eof()),traits::eof()) ) 
    return -1;
  }
 else
  { 
   pos_type p_out = seekoff(0,ios_base::cur,ios_base::out);
//   pos_type tmp_out = lseek(file_,0,SEEK_CUR);
//   pos_type end_out = lseek(file_,0,SEEK_END);
//   lseek(file_,tmp_out,SEEK_SET);
// use __unix::lseek instead of ::lseek for OS9 (tag)
   pos_type tmp_out = __unix::lseek(file_,0,SEEK_CUR);
   pos_type end_out = __unix::lseek(file_,0,SEEK_END);
   __unix::lseek(file_,tmp_out,SEEK_SET);
   if ( end_out > p_out )
    {
      if( seekpos(p_out) == pos_type(off_type(-1)))
       return -1;
    }
   else
    {
       if( traits::eq_int_type(overflow(traits::eof()),traits::eof()) )
        return -1;
    }
  }
}

 if ( this->which_open_mode( ) & ios_base::in )
 {
  if ( file_ != 0 )
   {
     pos_type p_in = seekoff(0,ios_base::cur,ios_base::in);
//     pos_type tmp_in = lseek(file_,0,SEEK_CUR);
//     pos_type end_in = lseek(file_,0,SEEK_END);
// use __unix::lseek instead of ::lseek for OS9 (tag)
     pos_type tmp_in = __unix::lseek(file_,0,SEEK_CUR);
     pos_type end_in = __unix::lseek(file_,0,SEEK_END);

// same here
//     lseek(file_,tmp_in,SEEK_SET);
     __unix::lseek(file_,tmp_in,SEEK_SET);
   if ( end_in > p_in )
    {
      if( seekpos(p_in) == pos_type(off_type(-1)))
       return -1;
    }
   }
 }

  return 0;
}


/*
 * streamsize xsputn(const char_type *, streamsize)
 */

template<class charT, class traits>
streamsize basic_filebuf<charT, traits>::
xsputn(const char_type *s, streamsize n)
{
  if ( !s || (n == 0) ) return 0;

  if ( n >  __rwBufferSize )
  {

   if ( traits::eq_int_type(overflow( traits::eof() ),traits::eof()) )
    return 0;

   char_type   *eback_s = eback();
   char_type   *gptr_s  = gptr();
   char_type   *egptr_s = egptr();

   char_type   *pbase_s = pbase();
   char_type   *pptr_s  = pptr();
   char_type   *epptr_s = epptr();

   setg(0,0,0);
   setp((char_type *)s,(char_type *)(s+n));
   basic_streambuf<charT, traits>::pbump(n);

   if ( traits::eq_int_type(overflow( traits::eof() ),traits::eof()) )
    return 0; 
  
   setg(eback_s,gptr_s,egptr_s);
   setp(pbase_s,epptr_s);

   pbump(pptr_s-pbase_s); 

   return n;
  }
  else
  {
    int         i=0;

    while((i < n) && ( !traits::eq_int_type(sputc(*s++),traits::eof())))
    i++;

    return i;
  }

 }


 
/*
 *
 * class basic_ifstream : public basic_istream
 *
 */

/*
 * basic_ifstream()
 */

template<class charT, class traits>
basic_ifstream<charT, traits>::basic_ifstream()
: basic_istream<charT, traits>( )
{
   init(&fb_);
}

/*
 * basic_ifstream(const char *, ios_base::openmode, long)
 *
 * opens a filebuf for (most likely) reading
 */

template<class charT, class traits>
basic_ifstream<charT, traits>::basic_ifstream(const char *s,
                                              ios_base::openmode mode,
                                              long protection )
: basic_istream<charT, traits>( )
{
  init(&fb_);
  open(s, mode, protection);
}

/*
 * basic_ifstream(int fd)
 *
 * opens a filebuf for reading
 */

template<class charT, class traits>
basic_ifstream<charT, traits>::
basic_ifstream(int fd)
: basic_istream<charT, traits>( )
{

  init(&fb_);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}

/*
 * basic_ifstream(int fd, char_type *buf, int len)
 *
 * opens a filebuf for reading
 */

template<class charT, class traits>
basic_ifstream<charT, traits>::
basic_ifstream(int fd, char_type *buf, int len)
: basic_istream<charT, traits>( )
{

  init(&fb_);

  if ( ( buf!=0 ) && ( len>0 ) )
  fb_.pubsetbuf(buf,len);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}


/*
 * ~basic_ifstream()
 */

template<class charT, class traits>
basic_ifstream<charT, traits>::~basic_ifstream()
{
}

/*
 * basic_filebuf *rdbuf() const
 *
 * returns the streambuf associated with this stream
 */

template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_ifstream<charT, traits>::rdbuf() const
{
  return (basic_filebuf<charT, traits> *)&fb_;
}

/*
 * bool is_open()
 */

template<class charT, class traits>
bool basic_ifstream<charT, traits>::is_open()
{
  return fb_.is_open();
}

/*
 * void open(const char *, ios_base::openmode, long )
 *
 * opens up a file
 */

template<class charT, class traits>
void basic_ifstream<charT, traits>::open(const char *s,
                                         ios_base::openmode mode,
                                         long protection )
{
  mode |= ios_base::in; 

  if ( !fb_.open(s, mode, protection) )  
   this->setstate(ios_base::failbit);
}

/* 
 * void close()
 */

template<class charT, class traits>
void basic_ifstream<charT, traits>::close()
{
  if(!is_open())
    return;
  
  if(!fb_.close())
    this->setstate(ios_base::failbit);
}


/*
 *
 * class basic_ofstream : public basic_ostream
 *
 */

/*
 * basic_ofstream()
 */

template<class charT, class traits>
basic_ofstream<charT, traits>::basic_ofstream()
: basic_ostream<charT, traits>( )
{
   init(&fb_);
}

/*
 * basic_ofstream(const char *, ios_base::openmode, long )
 */

template<class charT, class traits>
basic_ofstream<charT, traits>::
basic_ofstream(const char *s, ios_base::openmode mode, long protection)
: basic_ostream<charT, traits>()
{
       
  init(&fb_);
  open(s, mode, protection);
}

/*
 * basic_ofstream(int fd)
 *
 * opens a filebuf for writing
 */

template<class charT, class traits>
basic_ofstream<charT, traits>::
basic_ofstream(int fd)
: basic_ostream<charT, traits>( )
{

  init(&fb_);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}

/*
 * basic_ofstream(int fd, char_type *buf, int len)
 *
 * opens a filebuf for writing
 */

template<class charT, class traits>
basic_ofstream<charT, traits>::
basic_ofstream(int fd, char_type *buf, int len)
: basic_ostream<charT, traits>( )
{

  init(&fb_);

  if ( ( buf!=0 ) && ( len>0 ) )
  fb_.pubsetbuf(buf,len);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}


/*
 * ~basic_ofstream()
 */

template<class charT, class traits>
basic_ofstream<charT, traits>::~basic_ofstream()
{
}

/*
 * basic_filebuf *rdbuf() const
 */

template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_ofstream<charT, traits>::rdbuf() const
{
  return (basic_filebuf<charT, traits> *)&fb_;
}

/*
 * bool is_open()
 */

template<class charT, class traits>
bool basic_ofstream<charT, traits>::is_open()
{
  return fb_.is_open();
}

/*
 * void open(const char *, ios_base::openmode)
 */

template<class charT, class traits>
void basic_ofstream<charT, traits>::open(const char *s,
                                         ios_base::openmode mode,
                                         long protection )
{
  mode |= ios_base::out;

  if ( !fb_.open(s, mode, protection) )
   this->setstate(ios_base::failbit);
}

/*
 * void close()
 */

template<class charT, class traits>
void basic_ofstream<charT, traits>::close()
{
  if(!is_open())
    return;

  if(!fb_.close())
    this->setstate(ios_base::failbit);
}

/*
 *
 * class basic_fstream : public basic_iostream
 *
 */

/*
 * basic_fstream()
 */

template<class charT, class traits>
basic_fstream<charT, traits>::basic_fstream()
: basic_iostream<charT, traits>( )
{
   init(&fb_);
}

/*
 * basic_fstream(const char *, ios_base::openmode, long)
 *
 * opens a filebuf for reading and writing
 */

template<class charT, class traits>
basic_fstream<charT, traits>::
basic_fstream(const char *s, ios_base::openmode mode, long protection)
: basic_iostream<charT, traits>( )
{
  init(&fb_);
  open(s, mode, protection);
}

/*
 * basic_fstream(int fd)
 *
 * opens a filebuf for reading and writing
 */

template<class charT, class traits>
basic_fstream<charT, traits>::
basic_fstream(int fd)
: basic_iostream<charT, traits>( )
{

  init(&fb_);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}

/*
 * basic_fstream(int fd, char_type *buf, int len)
 *
 * opens a filebuf for reading and writing
 */

template<class charT, class traits>
basic_fstream<charT, traits>::
basic_fstream(int fd, char_type *buf, int len)
: basic_iostream<charT, traits>( )
{

  init(&fb_);

  if ( ( buf!=0 ) && ( len>0 ) )
  fb_.pubsetbuf(buf,len);

  if ( !fb_.open(fd) ) 
   this->setstate(ios_base::failbit);
}


/*
 * ~basic_fstream()
 */

template<class charT, class traits>
basic_fstream<charT, traits>::~basic_fstream()
{
}

/*
 * basic_filebuf *rdbuf() const
 *
 * returns the streambuf associated with this stream
 */

template<class charT, class traits>
basic_filebuf<charT, traits> *
basic_fstream<charT, traits>::rdbuf() const
{
  return (basic_filebuf<charT, traits> *)&fb_;
}

/*
 * bool is_open()
 */

template<class charT, class traits>
bool basic_fstream<charT, traits>::is_open()
{
  return fb_.is_open();
}

/*
 * void open(const char *, ios_base::openmode, long)
 *
 * opens up a file
 */

template<class charT, class traits>
void basic_fstream<charT, traits>::
open(const char *s, ios_base::openmode mode, long protection)
{
  mode = mode | ios_base::in | ios_base::out;
  if ( !fb_.open(s, mode, protection) ) 
   this->setstate(ios_base::failbit);
}

/* 
 * void close()
 */

template<class charT, class traits>
void basic_fstream<charT, traits>::close()
{
  if(!is_open())
    return;
  
  if(!fb_.close())
    this->setstate(ios_base::failbit);
}

#ifndef _RWSTD_NO_NAMESPACE
}
#endif
