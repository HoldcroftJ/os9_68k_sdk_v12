/****************************************************************************
 *
 * locale.cc - Definitions of the Standard Library locale classes
 *
 * $Id: locale.cc,v 1.66 1996/10/02 01:45:02 smithey Exp $
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
 ***************************************************************************/

#ifndef __STD_LOCALE_IMPL__
#define __STD_LOCALE_IMPL__

#include <rw/numbrw>
#include <iomanip>

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

// ------------------------------
// Class locale member templates.
// ------------------------------

// Operator () compares two strings according to the collate<charT> facet of
// the locale.  It lets you use a locale object directly as a comparator
// predicate for strings in various algorithms.  For instance, you can sort
// a vector<string> according to the collating sequence of a locale with an
// expression like: sort(v.begin(),v.end(),mylocale).

#ifndef _RWSTD_NO_MEMBER_TEMPLATES
template <class charT, class Traits, class Allocator>
inline bool locale::operator() (
    const basic_string<charT,Traits,Allocator>& s1,
    const basic_string<charT,Traits,Allocator>& s2) const
{
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
  return use_facet<_RWSTD_STD::collate<charT> >(*this).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#else
  return use_facet(*this,(_RWSTD_STD::collate<charT>*)0).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#endif
}

#else

// If your compiler does not support member templates, we supply _EXPLICIT
// versions of operator () for string and (if your compiler supports wide
// chars) wstring.

inline bool locale::operator() (const string &s1,const string &s2) const {
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
  return use_facet<_RWSTD_STD::collate<char> >(*this).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#else
  return use_facet(*this,(_RWSTD_STD::collate<char>*)0).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#endif
}

#ifndef _RWSTD_NO_WIDE_CHAR
inline bool locale::operator() (const wstring &s1,const wstring &s2) const {
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
  return use_facet<_RWSTD_STD::collate<wchar_t> >(*this).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#else
  return use_facet(*this,(_RWSTD_STD::collate<wchar_t>*)0).compare
      (s1.data(),s1.data()+s1.length(),s2.data(),s2.data()+s2.length()) < 0;
#endif
}
#endif // _RWSTD_NO_WIDE_CHAR
#endif // _RWSTD_NO_MEMBER_TEMPLATES

// ------------------------------------
// Facet ctype<charT> member templates.
// ------------------------------------

template <class charT> locale::id ctype<charT>::id;

template <class charT>
ctype<charT>::ctype (size_t refs):
    locale::facet(refs,locale::_rw_ctype_category)
{ }

// --------------------------------
// Implementation member templates.
// --------------------------------

#ifndef _RWSTD_NO_NAMESPACE
} // namespace std
#endif

#ifdef _RWSTD_COMPILE_INSTANTIATE
#include <rw/locimpl.cc>
#endif

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

// ----------------------------------------------------
// Facet num_get<charT,InputIterator> member templates.
// ----------------------------------------------------


template <class charT, class InputIterator>
locale::id num_get<charT, InputIterator>::id;

template <class charT, class InputIterator>
num_get<charT,InputIterator>::~num_get() { }

#ifndef _RWSTD_NO_BOOL

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, bool& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  long v=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit; // aka 0

  if (!reader.error)
    if (v==0)
      value=false;
    else if (v==1 && !reader.negative)
      value=true;
    else
      err=ios_base::failbit;
  else if (reader.advanced)
    err=ios_base::failbit;
  else {
    const numpunct<charT>& nump =
        _RWSTD_STATIC_CAST(const numpunct<charT>&,reader.punct);
    int k=reader.get_keyword(reader.get_tf_map(nump));
    if (k<0)
      err=ios_base::failbit;
    else
      value=k;
  }

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

#endif // _RWSTD_NO_BOOL

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, void*& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  void *v=reader.to_pointer(reader.get_pointer_digits());
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, long& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  long v=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit;

  if (!reader.error)
    if (reader.negative) {
      if ((v=-v)>0)
        reader.error=reader.overflow;
    } else
      if (v<0 && reader.radix==10)
        reader.error=reader.overflow;

  if (reader.error)
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;
    
  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, unsigned short& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  unsigned long v=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit;

  if (reader.error
      || v > (unsigned long) numeric_limits<unsigned short>::max()
      || (reader.negative && v!=0))
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, unsigned int& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  unsigned long v=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit;

  if (reader.error
      || v > (unsigned long) numeric_limits<unsigned int>::max()
      || (reader.negative && v!=0))
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, unsigned long& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  unsigned long v=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit;

  if (reader.error || (reader.negative && v!=0))
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, float& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  float v=reader.to_float(reader.get_float_digits());
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, double& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  double v=reader.to_double(reader.get_float_digits());
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator>
InputIterator num_get<charT,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io,
     ios_base::iostate& err, long double& value) const
{
  _RWSTD_IMP_NAME(digit_reader)<charT,InputIterator> reader(in,end,io);
  long double v=reader.to_long_double(reader.get_float_digits());
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    value=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

// -----------------------------------------------------
// Facet num_put<charT,OutputIterator> member templates.
// -----------------------------------------------------

template <class charT, class OutputIterator>
locale::id num_put<charT, OutputIterator>::id;

template <class charT, class OutputIterator>
num_put<charT,OutputIterator>::~num_put() { }

#ifndef _RWSTD_NO_BOOL

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, bool value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  if (io.flags() & ios_base::boolalpha) {
    const numpunct<charT>& nump =
        _RWSTD_STATIC_CAST(const numpunct<charT>&,writer.punct);
    writer.put_keyword(writer.get_tf_string(nump,value),fill);
  } else {
    writer.digitize((unsigned long) (value? 1 : 0));
    writer.put_digits(fill);
  }
  return out;
}

#endif // _RWSTD_NO_BOOL

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, void* value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize(value);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, long value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize((unsigned long) value,true);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, unsigned long value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize(value,false);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, double value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> digits(out,io);
  digits.digitize(value);
  digits.put_digits(fill);
  return out;
}

#ifndef _RWSTD_NO_LONG_DOUBLE
template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, long double value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> digits(out,io);
  digits.digitize(value);
  digits.put_digits(fill);
  return out;
}
#endif // _RWSTD_NO_LONG_DOUBLE

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, short value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize((unsigned short)value,true);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, unsigned short value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize(value,false);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, int value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize((unsigned int)value,true);
  writer.put_digits(fill);
  return out;
}

template <class charT, class OutputIterator>
OutputIterator num_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill, unsigned int value) const
{
  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.digitize(value,false);
  writer.put_digits(fill);
  return out;
}

// ---------------------------------------
// Facet numpunct<charT> member templates.
// ---------------------------------------

template <class charT>
locale::id numpunct<charT>::id;

template <class charT>
numpunct<charT>::~numpunct() { }

template <class charT>
charT numpunct<charT>::do_decimal_point () const { return this->dp_; }

template <class charT>
charT numpunct<charT>::do_thousands_sep () const { return this->ts_; }

template <class charT>
string numpunct<charT>::do_grouping () const { return this->gr_; }

template <class charT>
_TYPENAME numpunct<charT>::string_type
numpunct<charT>::do_falsename () const { return this->fn_; }

template <class charT>
_TYPENAME numpunct<charT>::string_type
numpunct<charT>::do_truename () const { return this->tn_; }

template <class charT>
void numpunct<charT>::rw_init () {
  this->dp_=do_decimal_point();
  this->ts_=do_thousands_sep();
  this->gr_=do_grouping();
  this->fn_=do_falsename();
  this->tn_=do_truename();

  _RWSTD_IMP_NAME(numpunct_impl)<charT>::rw_init();
}

// --------------------------------------
// Facet collate<charT> member templates.
// --------------------------------------

template <class charT>
locale::id collate<charT>::id;

template <class charT>
collate<charT>::~collate() { }

template <class charT>
int collate<charT>::do_compare
    (const charT* low1, const charT* high1,
     const charT* low2, const charT* high2) const
{
  size_t len1=high1-low1;
  size_t len2=high2-low2;
  size_t len=min(len1,len2);
  int comp;

  for (const charT *p=low1,*q=low2; len--; p++,q++)
    if ((comp=coll_order(*p)-coll_order(*q))!=0)
      return comp<0? -1 : 1;

  if (len1<len2)
    return -1;
  if (len2<len1)
    return 1;
  return 0;
}

template <class charT>
_TYPENAME collate<charT>::string_type
collate<charT>::do_transform
    (const charT* low, const charT* high) const
{
  string_type result(high-low,'\0');
  _TYPENAME string_type::iterator out=result.begin();
  for (const charT *p=low; p!=high; p++)
    *out++=coll_order(*p);
  return result;
}

template <class charT>
long collate<charT>::do_hash (const charT *start, const charT *end) const
{
  // Peter Weinberger's generic hashing algorithm, adapted by Andrew Binstock
  // from a version by Allen Holub (see Andrew Binstock, "Hashing Revisited",
  // Dr. Dobb's Journal, April 1996) and templatized by Rogue Wave.

  const int long_bits=CHAR_BIT*sizeof(long);
  const int one_eighth=long_bits/8;
  const int three_fourths=long_bits*3/4;
  const int high_bits=(~0L) << (long_bits-one_eighth);

  long result=0;
  for (const charT *p=start; start<end; start++) {
    result=(result << one_eighth) + *p;
    long temp=result & high_bits;
    if (temp)
      result=(result^(temp>>three_fourths)) &~ high_bits;
  }

  return result;
}

// -------------------------------------------------
// Facet codecvt<internT,externT,stateT> member templates.
// -------------------------------------------------

#ifndef _MSC_VER
template <class internT,class externT,class stateT>
locale::id codecvt<internT,externT,stateT>::id;

template <class internT,class externT,class stateT>
codecvt<internT,externT,stateT>::~codecvt() {}

template <class internT, class externT, class stateT>
codecvt_base::result
codecvt<internT,externT,stateT>::do_out (stateT&,
    const internT* from, const internT* from_end, const internT*& from_next
   , externT* to, externT* to_end, externT*& to_next) const
{
  return noconv;
}

template <class internT, class externT, class stateT>
codecvt_base::result
codecvt<internT,externT,stateT>::do_in (stateT&,
    const externT* from, const externT* from_end, const externT*& from_next
   , internT* to, internT* to_end, internT*& to_next) const
{
  return noconv;
}

template <class internT,class externT,class stateT>
bool codecvt<internT,externT,stateT>::do_always_noconv ()
    const _RWSTD_THROW_SPEC_NULL
{
  return true;
}

template <class internT,class externT,class stateT>
int codecvt<internT,externT,stateT>::do_encoding()
    const _RWSTD_THROW_SPEC_NULL
{
  return sizeof(internT) == sizeof(externT);
}

template <class internT,class externT,class stateT>
int codecvt<internT,externT,stateT>::do_length
    (const stateT&, const internT *from, const internT *from_end, size_t max)
    const
{
  return min (size_t(from_end-from), max);
}

template <class internT,class externT,class stateT>
int codecvt<internT,externT,stateT>::do_max_length ()
    const _RWSTD_THROW_SPEC_NULL
{
  return 1;
}

template <class internT,class externT,class stateT>
_TYPENAME codecvt<internT,externT,stateT>::internal_string_type
codecvt<internT,externT,stateT>::in (const external_string_type &s) const
{
  // Calculate the number of internT's that will be produced.  (I'm not sure
  // what to use for max in the do_length call; too bad char_traits can't tell
  // me.) (This needs to be revised to use separate in and out versions of
  // do_length once someone comes up with the right syntax.)
  int n=1000; // do_length(stateT(0),s.c_str(),s.c_str()+s.length(),
              // numeric_limits<size_t>::max() / do_max_length());

  const externT *unused_from;
  internT *unused_to;

  internT *conversion_buffer=new internT[n];
  stateT stt(0);
  n=do_in(stt,s.c_str(),s.c_str()+s.length(),unused_from,
      conversion_buffer,conversion_buffer+n,unused_to);
  internal_string_type result(conversion_buffer,conversion_buffer+n);
  delete[] conversion_buffer;
  return result;
}

#endif // _MSC_VER

// -----------------------------------------------------
// Facet time_get<charT,InputIterator> member templates.
// -----------------------------------------------------

template <class charT, class InputIterator>
locale::id time_get<charT,InputIterator>::id;

template <class charT, class InputIterator>
time_get<charT,InputIterator>::~time_get() { }

template <class charT, class InputIterator>
time_base::dateorder time_get<charT,InputIterator>::do_date_order () const
{
  // We would prefer to return a value that matches the date format defined
  // in the timepunct facet in the caller's locale, but we can't get there
  // from here ...
  return no_order;
}

template <class charT, class InputIterator>
_TYPENAME time_get<charT,InputIterator>::iter_type
time_get<charT,InputIterator>::do_get_time (InputIterator in,
    InputIterator end, ios_base& io, ios_base::iostate& err, tm* time) const
{
  _RWSTD_IMP_NAME(time_reader)<charT,InputIterator> reader(in,end,io);
  err=ios_base::goodbit;

  // Parse according to pattern 1 (strftime '%X' -- default time pattern)
  if (!reader.get_time_pattern(reader.get_patt_string(reader.timp,1),time))
    err=ios_base::failbit;

  if (reader.reached_end)
    err|=ios_base::eofbit;
    
  return in;
}

template <class charT, class InputIterator >
_TYPENAME time_get<charT,InputIterator>::iter_type
time_get<charT,InputIterator>::do_get_date (InputIterator in,
    InputIterator end, ios_base& io, ios_base::iostate& err, tm* time) const
{
  _RWSTD_IMP_NAME(time_reader)<charT,InputIterator> reader(in,end,io);
  err=ios_base::goodbit;

  // Parse according to pattern 0 (strftime '%x' -- default date pattern)
  if (!reader.get_time_pattern(reader.get_patt_string(reader.timp,0),time))
    err=ios_base::failbit;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator >
_TYPENAME time_get<charT,InputIterator>::iter_type
time_get<charT,InputIterator>::do_get_weekday (InputIterator in,
    InputIterator end, ios_base& io, ios_base::iostate& err, tm* time) const
{
  _RWSTD_IMP_NAME(time_reader)<charT,InputIterator> reader(in,end,io);
  int wd=reader.get_keyword(reader.get_day_map(reader.timp));
  err=ios_base::goodbit;

  if (wd<0)
    err=ios_base::failbit;
  else
    time->tm_wday=wd;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator >
_TYPENAME time_get<charT,InputIterator>::iter_type
time_get<charT,InputIterator>::do_get_monthname (InputIterator in,
    InputIterator end, ios_base& io, ios_base::iostate& err, tm* time) const
{
  _RWSTD_IMP_NAME(time_reader)<charT,InputIterator> reader(in,end,io);
  int mo=reader.get_keyword(reader.get_month_map(reader.timp));
  err=ios_base::goodbit;

  if (mo<0)
    err=ios_base::failbit;
  else
    time->tm_mon=mo;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

template <class charT, class InputIterator >
_TYPENAME time_get<charT,InputIterator>::iter_type
time_get<charT,InputIterator>::do_get_year (InputIterator in,
    InputIterator end, ios_base& io, ios_base::iostate& err, tm* time) const
{
  _RWSTD_IMP_NAME(time_reader)<charT,InputIterator> reader(in,end,io);
  int yr=reader.to_ulong(reader.get_int_digits());
  err=ios_base::goodbit;

  // 2-digit year numbers are accepted, but are not treated specially, and so
  // end up in the 1st century C.E.
  if (reader.error)
    err=ios_base::failbit;
  else
    time->tm_year=yr-1900;

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

// -----------------------------------------------------
// Facet time_put<charT,InputIterator> member templates.
// -----------------------------------------------------

template <class charT, class OutputIterator>
locale::id time_put<charT,OutputIterator>::id;

template <class charT, class OutputIterator>
time_put<charT,OutputIterator>::~time_put() { }

template <class charT, class OutputIterator>
OutputIterator time_put<charT,OutputIterator>::put
    (OutputIterator out, ios_base &io, charT fill, const tm *time,
     const charT* pattern, const charT* patt_end) const
{
  size_t patt_len=patt_end-pattern;
  char scratch[40];
  char *narrow_patt=(patt_len<=sizeof scratch)? scratch : new char[patt_len];
  
  #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
  use_facet<ctype<charT> >(io.getloc()).narrow(pattern,patt_end,' ',narrow_patt);
  #else
  use_facet(io.getloc(),(ctype<charT>*)0).narrow(pattern,patt_end,' ',narrow_patt);
  #endif

  char *np=narrow_patt;
  for (const charT* wp=pattern; wp<patt_end; wp++,np++)
    if (*np!='%')
      *out++=*wp;
    else if (++wp<patt_end)
      switch (*++np) {

       case 'O':
        // POSIX-style modifier
        if (++wp<patt_end)
          out=do_put(out,io,fill,time,*++np,'O');
        break;

       case '%':
        // Literal percent sign
        *out++=*wp;
        break;

       default:
        // Treat everything else as a time format specifier.
        out=do_put(out,io,fill,time,*np,' ');
      }

  if (narrow_patt!=scratch)
    delete[] narrow_patt;

  return out;
}

template <class charT, class OutputIterator>
OutputIterator time_put<charT,OutputIterator>::do_put
    (OutputIterator out, ios_base &io, charT fill, const tm *time,
     char format, char modifier) const
{
  const _RWSTD_IMP_NAME(timepunct)<charT>& timp =
  #ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
      use_facet<_RWSTD_IMP_NAME(timepunct)<charT> >(io.getloc());
  #else
      use_facet(io.getloc(),(_RWSTD_IMP_NAME(timepunct)<charT>*)0);
  #endif

  static const char do_as_pattern[]="xXcDrT";
  const char *p=strchr(do_as_pattern,format);
  if (p) {
    // Catch formats implemented as calls to the pattern-based method before
    // going to the expense of constructing a digit_writer.
    unsigned i=p-do_as_pattern;
    const _TYPENAME _RWSTD_IMP_NAME(timepunct)<charT>::string_type& s =
        _RWSTD_IMP_NAME(keyword_cracker)<charT>::get_patt_string(timp,i);
    return put(out,io,fill,time,s.c_str(),s.c_str()+s.length());
  }

  _RWSTD_IMP_NAME(digit_writer)<charT,OutputIterator> writer(out,io);
  writer.width=0;

  if (modifier=='O') {
    // Uppercase letter O (not zero) -- requests ordinal string if defined.
    int n,m;
    switch (format) {
     case 'C':
      // Exclusive Rogue Wave extension, let's you announce, "Welcome to the
      // %OC century!"  (See note on unmodified 'C' below.)
      n=(time->tm_year+1999)/100;
      break;
     case 'd':
     case 'e':
      n=time->tm_mday;
      break;
     case 'H':
      n=time->tm_hour+1;
      break;
     case 'I':
      n=time->tm_hour+1;
      if (n>12)
        n-=12;
      break;
     case 'm':
      n=time->tm_mon+1;
      break;
     case 'M':
      n=time->tm_min+1;
      break;
     case 'S':
      n=time->tm_sec+1;
      break;
     case 'u':
      n=time->tm_wday;
      if (n==0) n=7;
      break;
     case 'w':
      n=time->tm_wday;
      break;
     case 'U':
      n=(time->tm_yday-time->tm_wday)/7;
      break;
     case 'W':
      m=time->tm_wday-1; // Monday-based day of week
      if (m<0) m+=7;
      n=(time->tm_yday-m)/7;
      break;
     case 'y':
      n=(time->tm_year%100)+1;
      break;
     default:
      n=999; // Cause error output.
    }

    writer.put_keyword(writer.get_ord_string(timp,n),fill);

  } else {
    bool abbrev=false;
    const _TYPENAME _RWSTD_IMP_NAME(timepunct)<charT>::string_type *s=NULL;
    int n;

    writer.radix=10;
    writer.separable=false;

    switch (format) {
     case 'a':
      abbrev=true;
     case 'A':
      // Week day name or abbreviation.
      s=&writer.get_day_string(timp,time->tm_wday,abbrev);
      break;

     case 'b':
     case 'h':
      abbrev=true;
     case 'B':
      // Month name or abbreviation.
      s=&writer.get_month_string(timp,time->tm_mon,abbrev);
      break;

     case 'C':
      // Century.  Note that we begin the 20th century in 1901, not 1900.  The
      // draft standard does not seem to address this controversy.
      n=(time->tm_year+1999)/100;
      break;

     case 'd':
      // Day of month with leading zero.
      writer.iprecision=2;
     case 'e':
      // Day of month with leading blank.
      n=time->tm_mday;
      writer.width=2;
      break;

     case 'H':
      // Hour (24-hour clock).
      n=time->tm_hour;
      writer.iprecision=2;
      break;

     case 'I':
      // Hour (12-hour clock, caller must append am/pm to resolve ambiguity).
      n=time->tm_hour;
      if (n==0)
        n=12;
      else if (n>12)
        n-=12;
      writer.iprecision=2;
      break;

     case 'j':
      // 3-digit day of year.
      n=time->tm_yday+1;
      writer.iprecision=3;
      break;

     case 'm':
      // Month number.
      n=time->tm_mon+1;
      writer.iprecision=2;
      break;

     case 'M':
      // Minutes.
      n=time->tm_min;
      writer.iprecision=2;
      break;

     case 'n':
      // Newline character.
      *out++=writer.ctyp.widen('\n');
      break;

     case 'p':
      // ante/post meridian string.
      s=&writer.get_ampm_string(timp,time->tm_hour==0 || time->tm_hour>12);
      break;

     case 'S':
      // Seconds.
      n=time->tm_sec;
      writer.iprecision=2;
      break;

     case 't':
      // Tab character.
      *out++=writer.ctyp.widen('\t');
      break;

     case 'u':
      // Weekday (1-7, 1==Monday)
      n=time->tm_wday;
      if (n==0)
        n=7;
      break;

     case 'U':
      // Week number of year (assuming weeks start on Sunday).
      n=(time->tm_yday+time->tm_wday)/7;
      writer.iprecision=2;
      break;

     case 'w':
      // Weekday (0-6, 0==Sunday).
      n=time->tm_wday;
      break;

     case 'W':
      // Week number of year (assuming weeks start on Monday).
      n=(time->tm_yday+(time->tm_wday==0? 6 : (time->tm_wday-1)))/7;
      writer.iprecision=2;
      break;

     case 'y':
      // Year without century.
      n=time->tm_year%100;
      writer.iprecision=2;
      break;

     case 'Y':
      // Year with century.
      n=time->tm_year+1900;
      break;

     case 'Z':
      // Time zone name ... where do I get this?
      break;

     default:
      // Everything else is an error.
      s=&writer.get_day_string(timp,99,false);
    }

    if (s)
      writer.put_keyword(*s,fill);
    else {
      writer.digitize((unsigned long) n);
      writer.put_digits(fill);
    }
  }

  return out;
}

// -----------------------------------------------
// Facet rwstd::timepunct<charT> member templates.
// -----------------------------------------------

#ifndef _RWSTD_NO_NAMESPACE
} namespace __rwstd { // Leave std, enter __rwstd
#endif

template <class charT>
#if __EDG_VERSION__ >= 236
::std::locale::id _RWSTD_IMP_DECL(timepunct)<charT>::id;
#else
locale::id _RWSTD_IMP_DECL(timepunct)<charT>::id;
#endif

template <class charT>
void _RWSTD_IMP_DECL(timepunct)<charT>::rw_init () {
  int n;
  for (n=0; n<7; n++) {
    this->dn_[n][false]=do_dayname(n,false);
    this->dn_[n][true]=do_dayname(n,true);
  }
  for (n=0; n<12; n++) {
    this->mn_[n][false]=do_monthname(n,false);
    this->mn_[n][true]=do_monthname(n,true);
  }
  for (n=0; n<100; n++)
    this->ord_[n]=do_ordinal(n);

  _RWSTD_IMP_DECL(timepunct_impl)<charT>::rw_init();
}

template <class charT>
_TYPENAME _RWSTD_IMP_DECL(timepunct)<charT>::string_type
_RWSTD_IMP_DECL(timepunct)<charT>::do_dayname (int day, bool abbr) const {
  return this->dn_[day][abbr];
}

template <class charT>
_TYPENAME _RWSTD_IMP_DECL(timepunct)<charT>::string_type
_RWSTD_IMP_DECL(timepunct)<charT>::do_monthname (int mon, bool abbr) const {
  return this->mn_[mon][abbr];
}

template <class charT>
_TYPENAME _RWSTD_IMP_DECL(timepunct)<charT>::string_type
_RWSTD_IMP_DECL(timepunct)<charT>::do_ordinal (int number) const {
  return (number<0 || number>99) ? this->bad_ : this->ord_[number];
}

#ifndef _RWSTD_NO_NAMESPACE
} namespace std { // Leave __rwstd, back to std.
#endif

// -----------------------------------------------------------
// Facet money_get<charT,Intl,InputIterator> member templates.
// -----------------------------------------------------------

template <class charT, bool Intl, class InputIterator>
locale::id money_get<charT,Intl,InputIterator>::id;

#ifndef _RWSTD_NO_STI_SIMPLE
template <class charT, bool Intl, class InputIterator>
const bool money_get<charT,Intl,InputIterator>::intl;
#endif

template <class charT, bool Intl, class InputIterator>
money_get<charT,Intl,InputIterator>::~money_get() { }

// Warning -- these functions do not input actual monetary value; they just
// input numbers that represent monetary value.

template <class charT, bool Intl, class InputIterator>
InputIterator money_get<charT,Intl,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base& io, ios_base::iostate& err,
     long double& units) const

{
  _RWSTD_IMP_NAME(money_reader)<charT,Intl,InputIterator> reader(in,end,io);
  long double v=reader.to_long_double(reader.get_money_digits());
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    units=v;

  if (reader.reached_end)
    err|=ios_base::eofbit;
    
  return in;
}

template <class charT, bool Intl, class InputIterator>
InputIterator money_get<charT,Intl,InputIterator>::do_get
    (InputIterator in, InputIterator end, ios_base &io, ios_base::iostate &err,
     string_type &digit_string) const
{
  _RWSTD_IMP_NAME(money_reader)<charT,Intl,InputIterator> reader(in,end,io);
  const char *eod=reader.get_money_digits();
  err=ios_base::goodbit;

  if (reader.error)
    err=ios_base::failbit;
  else
    reader.get_money_string(digit_string,eod);

  if (reader.reached_end)
    err|=ios_base::eofbit;

  return in;
}

// ------------------------------------------------------------
// Facet money_put<charT,Intl,OutputIterator> member templates.
// ------------------------------------------------------------

template <class charT, bool Intl, class OutputIterator>
locale::id money_put<charT,Intl,OutputIterator>::id;

#ifndef _RWSTD_NO_STI_SIMPLE
template <class charT, bool Intl, class OutputIterator>
const bool money_put<charT,Intl,OutputIterator>::intl;
#endif

template <class charT, bool Intl, class OutputIterator>
money_put<charT,Intl,OutputIterator>::~money_put() { }

template <class charT, bool Intl, class OutputIterator>
OutputIterator money_put<charT,Intl,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill,
#ifndef _RWSTD_NO_LONG_DOUBLE
     long double quant) const
#else
     double quant) const
#endif

{
  _RWSTD_IMP_NAME(money_writer)<charT,Intl,OutputIterator> writer(out,io);
  writer.digitize(quant);
  writer.put_money(fill);
  return out;
}

template <class charT, bool Intl, class OutputIterator>
OutputIterator money_put<charT,Intl,OutputIterator>::do_put
    (OutputIterator out, ios_base& io, charT fill,
     const string_type& digits) const
{
  _RWSTD_IMP_NAME(money_writer)<charT,Intl,OutputIterator> writer(out,io);
  writer.put_money(digits,fill);
  return out;
}

// ----------------------------------------------
// Facet moneypunct<charT,Intl> member templates.
// ----------------------------------------------

template <class charT, bool Intl>
locale::id moneypunct<charT,Intl>::id;

#ifndef _RWSTD_NO_STI_SIMPLE
template <class charT, bool Intl>
const bool moneypunct<charT,Intl>::intl;
#endif

template <class charT, bool Intl>
moneypunct<charT,Intl>::~moneypunct () { }

template <class charT, bool Intl>
charT moneypunct<charT,Intl>::do_decimal_point () const
     { return this->dp_; }

template <class charT, bool Intl>
charT moneypunct<charT,Intl>::do_thousands_sep () const
     { return this->ts_; }

template <class charT, bool Intl>
string moneypunct<charT,Intl>::do_grouping () const
     { return this->gr_; }

template <class charT, bool Intl>
_TYPENAME moneypunct<charT,Intl>::string_type
moneypunct<charT,Intl>::do_curr_symbol () const
     { return this->cs_; }

template <class charT, bool Intl>
_TYPENAME moneypunct<charT,Intl>::string_type
moneypunct<charT,Intl>::do_positive_sign () const
     { return this->ps_; }

template <class charT, bool Intl>
_TYPENAME moneypunct<charT,Intl>::string_type
moneypunct<charT,Intl>::do_negative_sign () const
     { return this->ns_; }

template <class charT, bool Intl>
int moneypunct<charT,Intl>::do_frac_digits () const
     { return this->fd_; }

template <class charT, bool Intl>
money_base::pattern
moneypunct<charT,Intl>::do_pos_format () const
     { return this->pf_; }

template <class charT, bool Intl>
money_base::pattern
moneypunct<charT,Intl>::do_neg_format () const
     { return this->nf_; }

template <class charT, bool Intl>
void moneypunct<charT,Intl>::rw_init () {
  _RWSTD_IMP_NAME(punct_data)<charT>::dp_=do_decimal_point();
  _RWSTD_IMP_NAME(punct_data)<charT>::ts_=do_thousands_sep();
  _RWSTD_IMP_NAME(punct_data)<charT>::gr_=do_grouping();

  _RWSTD_IMP_NAME(moneypunct_data)<charT>::cs_=do_curr_symbol();
  _RWSTD_IMP_NAME(moneypunct_data)<charT>::ps_=do_positive_sign();
  _RWSTD_IMP_NAME(moneypunct_data)<charT>::ns_=do_negative_sign();
  _RWSTD_IMP_NAME(moneypunct_data)<charT>::fd_=do_frac_digits();
  _RWSTD_IMP_NAME(moneypunct_data)<charT>::pf_=do_pos_format();
  _RWSTD_IMP_NAME(moneypunct_data)<charT>::nf_=do_neg_format();

  _RWSTD_IMP_NAME(moneypunct_impl)<charT,Intl>::rw_init();
}

// ---------------------------------------
// Facet messages<charT> member templates.
// ---------------------------------------

template <class charT>
locale::id messages<charT>::id;

template <class charT>
messages<charT>::messages (size_t refs):
    locale::facet(refs,locale::messages), 
	_RWSTD_IMP_NAME(messages_impl)("C")
{ }

template <class charT>
messages<charT>::messages (size_t refs,string name):
    locale::facet(refs,locale::messages), _RWSTD_IMP_NAME(messages_impl)(name)
{ }

template <class charT>
messages<charT>::~messages() { }

template <class charT>
messages_base::catalog
messages<charT>::do_open (const string &cat_name,
#if __EDG_VERSION__ >= 236 // for OS9 (tag)
			  const ::std::locale& loc)
#else
			  const locale &loc) 
#endif
  const
{
  return open_cat_(cat_name,loc);
}

template <class charT>
_TYPENAME messages<charT>::string_type
messages<charT>::do_get (messages_base::catalog cat,
    int set,int msgid,const string_type &dflt) const
{
  char *text=get_mess_(cat,set,msgid);

  // The following is wrong -- it should be using codecvt instead of
  // ctype.widen, but this requires an effective codecvt implementation ...

  if (text) {
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
    return use_facet<ctype<charT> >(get_loc_(cat)).widen(string(text));
#else
    return use_facet(get_loc_(cat),(ctype<charT>*)0).widen(string(text));
#endif
  }
  return dflt;
}

// Specialize member function for char for performance.  (Is this really okay
// without specializing the whole class?  Borland fails on it -- if it is in
// fact legit, we need a config test.)

#if 0
string messages<char>::do_get
    (messages_base::catalog cat,int set,int msgid,const string &dflt) const
{
  char *text=get_mess_(cat,set,msgid);
  if (text)
    return text;
  return dflt;
}
#endif

template <class charT>
void messages<charT>::do_close (messages_base::catalog cat) const
{
  close_cat_(cat);
}

#ifndef _RWSTD_NO_NAMESPACE
} // namespace std
#endif

#ifndef __STD_LOCALE_BYNAME_IMPL__
#include <rw/byname.cc>
#endif

#endif // __STD_LOCALE_IMPL__

