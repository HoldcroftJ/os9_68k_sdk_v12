/***************************************************************************
 *
 * bitset - class bitset declaration
 *
 * $Id: bitset.cc,v 1.2 1996/08/28 01:28:21 smithey Exp $
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

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

#ifndef NELEMENTS
#  ifndef _RWSTD_BC5_ENUM_BUG
#define NELEMENTS NumOfElems
#  else
#define NELEMENTS NumOfElems()
#  endif /*_RWSTD_BC5_ENUM_BUG*/
#endif /* NELEMENTS */

#ifndef _RWSTD_MSC22_STATIC_INIT_BUG
template <size_t N>
const size_t bitset<N>::bitset_size 
#ifdef _RWSTD_NO_STI_TEMPLATE
= N
#endif
;
#endif /* _RWSTD_MSC22_STATIC_INIT_BUG */

template <size_t N>
bitset<N>::bitset (const string& str,
                   size_t pos,
                   size_t n) _RWSTD_THROW_SPEC((out_of_range, invalid_argument))
#ifdef _RWSTD_MSC22_STATIC_INIT_BUG
  : bitset_size(N)
#endif
{
    size_t slen = str.size();

    _RWSTD_THROW(pos > slen,
                out_of_range,
                __rw_bitset_InvalidPosition);

    size_t rlen = n < (slen - pos) ? n : slen - pos;
    size_t M = N >= rlen ? rlen : N;
#ifndef _RWSTD_BC5_ENUM_BUG
    memset(bits, 0, sizeof(bits));
#else
    bits = new VectorType[NELEMENTS];
    //
    // TODO -- check for bits == 0 here?
    //
    memset(bits, 0, NELEMENTS*sizeof(VectorType));
#endif /*_RWSTD_BC5_ENUM_BUG*/
    for (size_t i = pos; i < M + pos; i++)
    {
        char c = str[slen - i - 1];

        _RWSTD_THROW(!(c == '0' || c == '1'),
                    invalid_argument,
                    __rw_bitset_InvalidCtorArgument);

        if (c == '1') set(i - pos);
    }
}

//
// Constructs an object of type string and initializes it
// to a string of length N characters. Each character is
// determined by the value of its corresponding bit position
// in *this. Character position N-1 corresponds to bit
// position zero. Subsequent decreasing character positions
// correspond to increasing bit positions. Bit value zero becomes
// the character 0, bit value one becomes the character 1.
//

template <size_t N>
string 
bitset<N>::to_string () const 
{
    string s;
    for (long i = N - 1; i >= 0; --i)
        s.append(1, test(i) ? '1' : '0');
    return s;
}

//
// If the integral value x corresponding to the bitset in *this
// cannot be represented as type unsigned long, throws overflow_error.
//

template <size_t N>
unsigned long
bitset<N>::to_ulong () const _RWSTD_THROW_SPEC((overflow_error))
{
    const size_t size_long = sizeof(unsigned long);

    for (size_t i = NELEMENTS-1; size_long/sizeof(VectorType) <= i; --i)
        _RWSTD_THROW(bits[i],
                    overflow_error,
                    __rw_bitset_ConversionOverflow);

    unsigned long result = 0;

    for (size_t pos = 0; pos < N; pos++)
        if (test(pos))
            result |= 1UL << pos;

    return result;
} 

//
// Returns the count of the number of set bits.
//

template <size_t N>
size_t
bitset<N>::count () const _RWSTD_THROW_SPEC_NULL
{
    size_t sum = 0;

#if UINT_MAX <= 4294967295
    //
    // A sophisticated implementaton that works if BitsPerChunk < 63
    //
    for (size_t i = 0; i < NELEMENTS; i++)
    {
        unsigned long n = bits[i];
        unsigned long t = n - ((n>>1) & 033333333333) - ((n>>2) & 011111111111);
        t = ((t + (t >> 3)) & 030707070707);

        unsigned long x = t & 07700770077;
        unsigned long y = (t >> 6) & 07700770077;

        t = x + y;
        t = ((t >> 12) + (t >> 24) + t) & 0777;
        t = (t >> 6) + (t & 077);
        t = t + 1;

        sum += (t >> 6) + (t & 077) - 1;
    }
#else
    //
    // The more naive implementation that always works.
    //
    for (size_t i = 0; i < NELEMENTS; i++)
    {
        unsigned long n = bits[i];
        while (n)
        {
            n &= n-1;
            sum++;
        }
    }
#endif
    return sum;
}


#ifndef _RWSTD_NO_NONTYPE_ARGS
template <size_t N>
istream&
 operator>> (istream& is, bitset<N>& x)
{
    string str(N, '0');

    for (size_t count = 0; count < N && !is.eof(); )
    {
        char c = 0;
        is >> c;
        if (c == '1' || c == '0')
        { 
            str.append(1, c);
            count++; 
        }
        else
        {
            is.putback(c);
            break;
        }
    }

    if (str.size() == 0)
#ifdef _RW_STD_IOSTREAM
        is.setstate(ios::failbit);
#else
        is.clear(ios::failbit);
#endif

    x = bitset<N>(str);

    return is;
}
#endif /*_RWSTD_NO_NONTYPE_ARGS*/


#ifndef _RWSTD_NO_NAMESPACE
}
#endif

