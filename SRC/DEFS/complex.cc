
/***************************************************************************
 *
 * complex - Declaration for the Standard Library complex class
 *
 * $Id: complex.cc,v 1.3 1996/09/24 19:16:52 smithey Exp $
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

#ifdef _RW_STD_IOSTREAM
template <class T,class charT, class traits>
basic_istream<charT, traits >&  
_RWSTDExportTemplate operator>> (basic_istream<charT, traits >& is,complex<T>& x)
#else
template <class T>
istream& _RWSTDExportTemplate operator>> (istream& is, complex<T>& x)
#endif
{
    //
    // operator >> reads a complex number x in the form
    // u
    // (u)
    // (u, v)
    //
    T u = 0, v = 0;
    char c;

    is >> c;
    if (c == '(')
    {
        is >> u >> c;
        if (c == ',') { is >> v  >> c;}
        if (c != ')' )
        {
#ifdef _RW_STD_IOSTREAM
            is.setstate(ios::failbit);
#else
            is.clear(ios::failbit);
#endif
        }
    } 
    else
    {
        is.putback(c);
        is >> u;
    }

    if (is)
        x = complex<T>(u,v);

    return is;
}

#ifdef _RW_STD_IOSTREAM
template <class T,class charT,class traits>
basic_ostream<charT,traits >& _RWSTDExportTemplate operator<< (basic_ostream<charT, traits >& os,const complex<T>& x)
#else
template <class T>
ostream& _RWSTDExportTemplate operator<< (ostream& os, const complex<T>& x)
#endif
{
    return os << "(" << x.real() << "," << x.imag() << ")";
}

#ifndef _RWSTD_NO_NAMESPACE
}
#endif
