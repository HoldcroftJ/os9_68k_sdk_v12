/***************************************************************************
 *
 * string.cc - Definitions for the Standard Library string classes
 *
 * $Id: string.cc,v 1.138 1996/09/24 21:54:25 smithey Exp $
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
#include <stdcomp.h>
#include <limits>

//
// Members for class basic_string
//

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

// basic_string<...>::nullref
#ifndef _RWSTD_NO_STATIC_DEF3
template <class charT, class traits, class Allocator >
const __RWSTD::null_string_ref_rep<charT, traits, Allocator>
basic_string<charT, traits, Allocator>::nullref 
  = __RWSTD::null_string_ref_rep<charT, traits, Allocator>();
#endif


// basic_string<...>::npos
#ifdef _RWSTD_MSC22_STATIC_INIT_BUG
#define npos INT_MAX;
#else
template <class charT, class traits, class Allocator >
const _TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::npos
#ifdef _RWSTD_NO_STATIC_CAST
  = (_TYPENAME basic_string<charT, traits, 
              Allocator>::size_type) INT_MAX
#else
  = static_cast<basic_string<charT, traits, 
              Allocator>::size_type>(INT_MAX)
#endif
;
#endif /*_RWSTD_MSC22_STATIC_INIT_BUG*/

template <class charT, class traits, class Allocator >
__RWSTD::string_ref<charT, traits, Allocator> *
basic_string<charT, traits, Allocator>::getRep (size_type capac, 
                                                size_type nchar)
                                              
{ 							
#ifndef _RWSTD_NO_NAMESPACE
  using namespace __rwstd;
#endif

  if ((capac | nchar) == 0)
  {
    getNullRep()->addReference();
    return getNullRep();
  }

  //
  // Allocate, then initialize the string_ref, then
  // initialize each character in the buffer. 
  //
  value_alloc_type va(alloc_);
#ifdef _RWSTD_NO_STATIC_CAST
  string_ref<charT, traits, Allocator> * ret = 
    (string_ref<charT, traits, Allocator> *)
    va.allocate(capac + sizeof(string_ref_rep<Allocator>)/sizeof(charT) + 2,0);
#else
  string_ref<charT, traits, Allocator> * ret = 
    reinterpret_cast<string_ref<charT, traits, Allocator> *>
    (va.allocate(capac + sizeof(string_ref_rep<Allocator>)/sizeof(charT) + 2,0));
#endif

  ref_alloc_type(alloc_).construct(ret, string_ref<charT,traits,Allocator>());
  charT * d = ret->data();
  va.construct(d,charT());

  ret->capacity_ = capac;
  ret->setRefCount(1);
  ret->data()[ret->nchars_ = nchar] = charT(0); // Terminating null
  return ret;
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  const basic_string<charT, traits, Allocator> & s,
  size_type pos, size_type  n)
{
    _RWSTD_THROW(pos > s.length(), out_of_range, 
                 __RWSTD::rwse_PosBeyondEndOfString);
    alloc_ = s.get_allocator();
    size_type rlen   = min(n, s.length() - pos);
    size_type maxlen = max(n == npos ? 0 : n, rlen);
    data_            = getRep(maxlen, rlen)->data();

    traits::copy(data_, &s.data_[pos], rlen);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  const charT*     s,
  size_type        n,
  const Allocator& alloc)
    : alloc_(alloc)
{	
    _RWSTD_THROW(s == 0, logic_error, __RWSTD::rwse_UnexpectedNullPtr);
    _RWSTD_THROW(n == npos, length_error, __RWSTD::rwse_ResultLenInvalid);
    data_  = getRep(n, n)->data();

    traits::copy(data_, s, n);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  const charT*     s,
  const Allocator& alloc)
    : alloc_(alloc)
{	
    _RWSTD_THROW(s == 0, logic_error, __RWSTD::rwse_UnexpectedNullPtr);
    size_type len = traits::length(s);
    data_         = getRep(len, len)->data();
	
    traits::copy(data_, s, len);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  size_type        n,
  charT            c,
  const Allocator& alloc)
    : alloc_(alloc)
{
    _RWSTD_THROW(n == npos, length_error, __RWSTD::rwse_InvalidSizeParam);
    data_  = getRep(n, n)->data();

    while (n--) traits::assign(data_[n], c);
}

#ifdef _RWSTD_NO_DEFAULT_TEMPLATE_ARGS
template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  const charT*     s,
  size_type        n)
    : alloc_(Allocator())
{	
    _RWSTD_THROW(s == 0, logic_error, __RWSTD::rwse_UnexpectedNullPtr);
    _RWSTD_THROW(n == npos, length_error, __RWSTD::rwse_ResultLenInvalid);
    data_  = getRep(n, n)->data();

    traits::copy(data_, s, n);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  const charT*     s)
    : alloc_(Allocator())
{	
    _RWSTD_THROW(s == 0, logic_error, __RWSTD::rwse_UnexpectedNullPtr);
    size_type len = traits::length(s);
    data_         = getRep(len, len)->data();

    traits::copy(data_, s, len);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>::basic_string (
  size_type        n,
  charT            c)
    : alloc_(Allocator())
{
    _RWSTD_THROW(n == npos, length_error, __RWSTD::rwse_InvalidSizeParam);
    data_  = getRep(n, n)->data();

    while (n--) traits::assign(data_[n], c);
}
#endif


#ifndef _RWSTD_NO_MEMBER_TEMPLATES
template <class charT, class traits , class Allocator >
template <class InputIterator>
basic_string<charT, traits, Allocator>::basic_string (InputIterator first,
                     InputIterator last,
                     const Allocator &alloc)
  : alloc_(alloc)
{
    difference_type d = 0;
    distance(first,last,d);
    data_ = getRep()->data(d);
    for (difference_type i = 0; i < d; i++)
      data_[i] = *first++;
}
#else
template <class charT, class traits , class Allocator  >
basic_string<charT, traits, Allocator>::basic_string (const charT* first,
                      const charT* last,
                      const Allocator& alloc)
  : alloc_(alloc)
{
    data_ = getNullRep()->data();
    getNullRep()->addReference();
    replace(0,0,first,last-first,0,last-first);
}
#ifdef _RWSTD_NO_DEFAULT_TEMPLATE_ARGS
template <class charT, class traits , class Allocator  >
basic_string<charT, traits, Allocator>::basic_string (const charT* first,
                      const charT* last)
  : alloc_(Allocator())
{
    data_ = getNullRep()->data();
    getNullRep()->addReference();
    replace(0,0,first,last-first,0,last-first);
}
#endif

#endif


template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::operator= (
  const basic_string<charT, traits, Allocator>& str)
{
    str.pref()->addReference();
    unLink();
    data_ = str.data_;
    return *this;
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::operator= (const charT* s)
{
    _RWSTD_THROW(s == 0, logic_error, __RWSTD::rwse_UnexpectedNullPtr);
    if (traits::eq(*s, eos()))
    {
        if (pref()->references() == 1)
        {
            pref()->nchars_ = 0;
            traits::assign(data_[0], eos());
        }
        else
        {
            unLink();
            getNullRep()->addReference();
            data_ = getNullRep()->data();
        }
        return *this;
    }

    return replace(0, length(), s, traits::length(s));
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::append (
  const basic_string<charT, traits, Allocator>& str,
  size_type                                     pos,
  size_type                                     n)
{
    _RWSTD_THROW(pos > str.length(), out_of_range, 
                 __RWSTD::rwse_PosBeyondEndOfString);

    size_type rlen = min(n, str.length() - pos);

    _RWSTD_THROW(length() >= npos - rlen,
                length_error,
                __RWSTD::rwse_ResultLenInvalid);

    replace(length(), 0, str.data(), str.length(), pos, n);

    return *this;
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::append (
  const basic_string<charT, traits, Allocator>& str)
{
    _RWSTD_THROW(length() >= npos - str.length(),
                length_error,
                __RWSTD::rwse_ResultLenInvalid);
    replace(length(), 0, str.data(), str.length());

    return *this;
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::assign (
  const basic_string<charT, traits, Allocator>& str,
  size_type                                     pos,
  size_type                                     n)
{
    _RWSTD_THROW(pos > str.length(), out_of_range, 
                 __RWSTD::rwse_PosBeyondEndOfString);
    size_type rlen = min(n, str.length() - pos);
    return replace(0, length(), str, pos, rlen);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::assign (
  const basic_string<charT, traits, Allocator>& str)
{
    return replace(0, length(), str);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::insert (
  size_type                                     pos1,
  const basic_string<charT, traits, Allocator>& str,
  size_type                                     pos2,
  size_type                                     n)
{
  _RWSTD_THROW(pos1 > length() || pos2 > str.length(),
              out_of_range,
              __RWSTD::rwse_StringIndexOutOfRange);
  size_type rlen = min(n, str.length() - pos2);
  _RWSTD_THROW(length() >= npos - rlen, length_error, 
               __RWSTD::rwse_ResultLenInvalid);
  return replace(pos1, 0, str, pos2, n);
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator> &
basic_string<charT, traits, Allocator>::insert (
  size_type                                     pos1,
  const basic_string<charT, traits, Allocator>& str)
{
  _RWSTD_THROW(pos1 > length(), out_of_range, 
               __RWSTD::rwse_StringIndexOutOfRange);
  _RWSTD_THROW(length()  >= npos - str.length(), length_error, 
               __RWSTD::rwse_ResultLenInvalid);
  return replace(pos1, 0, str);
}

template <class charT, class traits, class Allocator >
_TYPENAME basic_string<charT, traits, Allocator>::iterator 
basic_string<charT, traits, Allocator>::replace (
  size_type    pos1,
  size_type    n1,
  const charT* cs,
  size_type    cslen,
  size_type    pos2,
  size_type    n2)
{
  _RWSTD_THROW(pos1 > length() || pos2 > cslen,
              out_of_range,
              __RWSTD::rwse_StringIndexOutOfRange);

  size_type xlen = min(n1, length() - pos1);
  size_type rlen = min(n2, cslen - pos2);

  _RWSTD_THROW(length() - xlen >= npos - rlen,
              length_error,
              __RWSTD::rwse_ResultLenInvalid );

  size_type tot = length() - xlen + rlen;  // Final string length.

  if (!tot)
  {
    //
    // Special case a substitution that leaves the string empty.
    //
    unLink();
    getNullRep()->addReference();
    data_ = getNullRep()->data();
  }
  else
  {
    size_type rem = length() - xlen - pos1;  // Length of bit at end of string
    //
    // Check for shared representation, insufficient capacity, or overlap copy.
    //
#ifdef _RWSTD_MULTI_THREAD
    pref()->mutex_.acquire();
#endif
    if ( (pref()->references() > 1)
      || (getCapac() < tot)
      || (cs && (cs >= data() && cs < data()+length())))
    {
#ifdef _RWSTD_MULTI_THREAD
        pref()->mutex_.release();
#endif
        //
        // Need to allocate a new reference.
        //
        __RWSTD::string_ref<charT, traits, Allocator> * temp
          = getRep(tot, tot);
        if (pos1) traits::copy(temp->data(), data_, pos1);
        if (rlen) traits::copy(temp->data()+pos1, cs+pos2, rlen);
        if (rem ) traits::copy(temp->data()+pos1+rlen, data_+pos1+n1, rem);
        unSafeunLink();
        data_ = temp->data();
      }
      else
      {
#ifdef _RWSTD_MULTI_THREAD
        pref()->mutex_.release();
#endif
        //
        // Current reference has enough room.
        //
        if (rem)  traits::move(data_+pos1+rlen, data_+pos1+n1, rem);
        if (rlen) traits::move(data_+pos1, cs+pos2, rlen);
        data_[pref()->nchars_ = tot] = eos();	// Add terminating null
      }
  }
  return data_ + pos1;
}

template <class charT, class traits, class Allocator >
void
basic_string<charT, traits, Allocator>::resize (size_type n, charT c)
{
    _RWSTD_THROW(n > max_size(), length_error, __RWSTD::rwse_InvalidSizeParam);

    if (n < length())
        erase(n);
    else if (n > length())
        append(n - length(), c);
}

template <class charT, class traits, class Allocator >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::copy (charT* s,
                                              size_type n,
                                              size_type pos)
{
    _RWSTD_THROW(pos > length(), out_of_range, 
                 __RWSTD::rwse_PosBeyondEndOfString);
    size_type rlen = min(n, length() - pos);
    traits::copy(s, data_+pos, rlen);
    return rlen;
}


template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::find (const charT* s,
                                              size_type pos,
                                              size_type n) const
{

    for (size_type xpos = pos; (xpos + n) <= length() ; xpos++)
    {
        bool found = true;
        for (size_type i = 0; i < n ; i++)
        {
            if (!traits::eq(data_[xpos + i], s[i]))
            {
                found = false;
                break;
            }
        }
        if (found)
            return xpos;
    }

    return npos;
}

template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::rfind (const charT* s,
                                               size_type pos,
                                               size_type n) const
{
    size_type xpos_start = length() < n ?
                           npos :
                           min(length() - n, pos);

    for (size_type xpos = xpos_start+1; xpos != 0 ; xpos--)
    {
        bool found = true;
        for (size_type i = 0; i < n ; i++)
        {
            if (!traits::eq(data_[xpos + i-1], s[i]))
            {
                found = false;
                break;
            }
        }
        if (found)
            return xpos-1;
    }

    return npos;
}

template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::find_first_of (const charT* s,
                                                       size_type pos,
                                                       size_type n) const
{
    for (size_type xpos = pos; xpos < length() ; xpos++)
    {
        for (size_type i = 0; i < n ; i++)
            if (traits::eq(data_[xpos], s[i]))
                return xpos;
    }

    return npos;
}


template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::find_last_of (const charT* s,
                                                      size_type pos,
                                                      size_type n) const
{
    if (length())
    {
      size_type xpos_start = min(pos, length() - 1);
      for (size_type xpos = xpos_start+1; xpos != 0 ; xpos--)
      {
        for(size_type i = 0; i < n ; i++)
            if (traits::eq(data_[xpos-1], s[i]))
                return xpos-1;
      } 
    }
    return npos;
}
	
template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::find_first_not_of (const charT* s,
                                                           size_type pos,
                                                           size_type n) const
{
    for (size_type xpos = pos; xpos < length() ; xpos++)
    {
        bool found = false;
        for (size_type i = 0; i < n ; i++)
        {
            if (traits::eq(data_[xpos], s[i]))
            {
                found = true;
                break;
            }
        }
        if (!found)
            return xpos;
    }

    return npos;
}

template <class charT, class traits , class Allocator  >
_TYPENAME basic_string<charT, traits, Allocator>::size_type
basic_string<charT, traits, Allocator>::find_last_not_of(const charT* s,
                                                         size_type pos,
                                                         size_type n) const
{
    if (length())
    {
      size_type xpos_start = min(pos, length() - 1);
      for (size_type xpos = xpos_start+1; xpos != 0 ; xpos--)
      {
        bool found = false;
        for (size_type i = 0; i < n ; i++)
        {
            if (traits::eq(data_[xpos-1], s[i]))
            {
                found = true;
                break;
            }
        }
        if (!found)
            return xpos-1;
      }
    }

    return npos;
}

template <class charT, class traits, class Allocator >
basic_string<charT, traits, Allocator>
basic_string<charT, traits, Allocator>::substr (size_type pos,
                                                size_type n) const
{
    _RWSTD_THROW(pos > length(), out_of_range, 
                 __RWSTD::rwse_PosBeyondEndOfString);
    size_type rlen = min(n, length() - pos);
    return basic_string<charT, traits, Allocator>(data_+pos, rlen);
}

template <class charT, class traits, class Allocator >
int
basic_string<charT, traits, Allocator>::compare (
  size_type pos1, size_type n1,
  const basic_string<charT, traits, Allocator>& str,
  size_type pos2, size_type n2) const
{
    return compare(pos1,n1,basic_string<charT,traits,Allocator>(str,pos2,n2));
}

template <class charT, class traits, class Allocator >
int
basic_string<charT, traits, Allocator>::compare (
  size_type pos1, size_type n1,
  const basic_string<charT, traits, Allocator>& str) const
{
    return compare(pos1,n1,str.c_str(),str.length());
}

template <class charT, class traits , class Allocator  >
int
basic_string<charT, traits, Allocator>::compare (size_type pos,
                                                 size_type n1,
                                                 const charT* s,
                                                 size_type n2) const
{
    size_type rlen = min(n1,n2);
    int result = traits::compare(data_+pos, s, rlen);

    if (result == 0)
        result = (n1 < n2) ? -1 : (n1 != n2);

    return result;
}

template <class charT, class traits, class Allocator >
void basic_string<charT, traits, Allocator>::clobber (size_type nc)
{
    if (pref()->references() > 1 || (getCapac() < nc))
    {
        unLink();
        data_ = getRep(nc, 0)->data();
    }
    else
        data_[pref()->nchars_ = 0] = 0;
}

template <class charT, class traits, class Allocator >
void basic_string<charT, traits, Allocator>::unSafeClone ()
{
    __RWSTD::string_ref<charT, traits, Allocator> * temp =
        getRep(length(), length());

    traits::copy(temp->data(), data(), length());
    unSafeunLink();
    data_ = temp->data();
}

template <class charT, class traits, class Allocator >
void basic_string<charT, traits, Allocator>::unSafeClone (size_type nc)
{
    size_type len = length();
    if (len > nc) len = nc;
    __RWSTD::string_ref<charT, traits, Allocator> * temp = getRep(nc, len);
    traits::copy(temp->data(), data(), length());
    unSafeunLink();
    data_ = temp->data();
}

#ifdef _RWSTD_MSC22_STATIC_INIT_BUG
#undef npos
#endif

//
// Inserters and Extractors
//

#ifndef _RW_STD_IOSTREAM

template<class charT, class traits, class Allocator>
istream & _RWSTDExportTemplate operator>> (istream & is,
                      basic_string<charT, traits, Allocator > & s)
{
    charT c;

    if (!is.ipfx())
     return is;

    s.erase();
    c = is.get();

#ifdef _RWSTD_MS40_ISTREAM_BUG
    _TYPENAME size_t i =0;
    _TYPENAME size_t end = s.max_size();
#else
    _TYPENAME Allocator::size_type i   = 0;
    _TYPENAME Allocator::size_type end = s.max_size();
#endif
    if (is.width())
      end =  min((int)end,is.width());

    while (is.good() && i < end && !isspace(c))
    {
        s.append(1,c);
        c = is.get();
        i++;
    }
    if (!i)
       is.clear(ios::failbit);

#ifndef macintosh    
    is.isfx();
#endif
    
    return is;
}


template<class charT, class traits, class Allocator>
ostream & _RWSTDExportTemplate operator<< (ostream & os,
                      const basic_string<charT, traits, Allocator > & s)
{
    os << s.data();
    return os;
}

template<class Stream, class charT, class traits, class Allocator>
Stream& _RWSTDExportTemplate
getline (Stream& is, basic_string<charT, traits, Allocator>& str, charT delim)
{
    charT c;

    str.erase();
    c = is.get();

#ifdef _RWSTD_MS40_ISTREAM_BUG
    size_t i = 0;
    size_t end = str.max_size();
#else
    _TYPENAME Allocator::size_type i = 0;
    _TYPENAME Allocator::size_type end = str.max_size();
#endif

    while (is.good())
    {
        if (c == (char)EOF)
            is.clear(ios::eofbit);
        if (c == delim)
            break;
        if (i == end)
        {
            is.clear(ios::failbit);
            break;
        }
        str.append(1,c);
        c = is.get();
        i++;
    }
    if (!i)
       is.clear(ios::failbit);

    return is;
}  


#endif /* not defined _RW_STD_IOSTREAM */

#ifndef _RWSTD_NO_NAMESPACE
}
#endif
