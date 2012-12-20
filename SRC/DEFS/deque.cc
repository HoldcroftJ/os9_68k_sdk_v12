/***************************************************************************
 *
 * deque.cc - Non-iniline definitions for the Standard Library deque class
 *
 * $Id: deque.cc,v 1.4 1996/08/28 18:41:29 smithey Exp $
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
#include <stdcomp.h>

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif


template <class T, class Allocator>
deque<T,Allocator>::~deque()
{
    while (!empty()) pop_front();
}

template <class T, class Allocator>
void deque<T,Allocator>::allocate_at_begin ()
{
    pointer p = value_alloc_type(the_allocator).allocate(buffer_size(),start.current);
    if (!empty())
    {
        if (start.node == map)
        {
            map_alloc_type ma(the_allocator);
            difference_type i = finish.node - start.node;
            map_size = (i + 1) * 2;
            map_pointer tmp = ma.allocate(map_size,map);
            copy(start.node, finish.node + 1, tmp + map_size / 4 + 1);
            ma.deallocate(map);
            map = tmp;
            map[map_size / 4] = p;
            start = iterator(p + buffer_size(), map + map_size / 4);
            finish = iterator(finish.current, map + map_size / 4 + i + 1);
        }
        else
        {
            *--start.node = p;
            start = iterator(p + buffer_size(), start.node);
        }
    }
    else
    {
        map_size = buffer_size();
        map = map_alloc_type(the_allocator).allocate(map_size,map);
        map[map_size / 2] = p;
        start = iterator(p + buffer_size() / 2 + 1, map + map_size / 2);
        finish = start;
    }
}


template <class T, class Allocator>
void deque<T,Allocator>::allocate_at_end ()
{
    pointer p = value_alloc_type(the_allocator).allocate(buffer_size(),start.current);

    if (!empty())
    {
        if (finish.node == map + map_size - 1)
        {
            map_alloc_type ma(the_allocator);
            difference_type i = finish.node - start.node;
            map_size = (i + 1) * 2;
            map_pointer tmp = map_alloc_type(the_allocator).allocate(map_size,map);
            copy(start.node, finish.node + 1, tmp + map_size / 4);
            map_alloc_type(the_allocator).deallocate(map);
            map = tmp;
            map[map_size / 4 + i + 1] = p;
            start = iterator(start.current, map + map_size / 4);
            finish = iterator(p, map + map_size / 4 + i + 1);
        }
        else
        {
            *++finish.node = p;
            finish = iterator(p, finish.node);
        }
    }
    else
    {
        map_size = buffer_size();
        map = map_alloc_type(the_allocator).allocate(map_size,map);
        map[map_size / 2] = p;
        start = iterator(p + buffer_size() / 2, map + map_size / 2);
        finish = start;
    }
}

template <class T, class Allocator>
void deque<T,Allocator>::deallocate_at_begin ()
{
    value_alloc_type(the_allocator).deallocate(*start.node++);
    if (empty())
    {
        start = iterator();
        finish = start;
        map_alloc_type(the_allocator).deallocate(map);
    }
    else
        start = iterator(*start.node, start.node);
}

template <class T, class Allocator>
void deque<T,Allocator>::deallocate_at_end ()
{
    value_alloc_type(the_allocator).deallocate(*finish.node--);
    if (empty())
    {
        start = iterator();
        finish = start;
        map_alloc_type(the_allocator).deallocate(map);
    }
    else
        finish = iterator(*finish.node + buffer_size(), finish.node);
}

template <class T, class Allocator>
void deque<T,Allocator>::resize (size_type new_size)
{
    T value;
    if (new_size > size())
        insert(end(), new_size - size(), value);
    else if (new_size < size())
        erase(begin() + new_size,end());
}

template <class T, class Allocator>
void deque<T,Allocator>::resize (size_type new_size, T value)
{
    if (new_size > size())
        insert(end(), new_size - size(), value);
    else if (new_size < size())
        erase(begin() + new_size,end());
}

template <class T, class Allocator>
_TYPENAME deque<T,Allocator>::iterator 
deque<T,Allocator>::insert (deque<T,Allocator>::iterator position)
{
    T x;

    if (position == begin())
    {
        push_front(x); return begin();
    }
    else if (position == end())
    {
        push_back(x); return end() - 1;
    }
    else
    {
        difference_type index = position - begin();
        if (index < length)
        {
            push_front(*begin());
            copy(begin() + 2, begin() + index + 1, begin() + 1);
        }
        else
        {
            push_back(*(end() - 1));
            copy_backward(begin() + index, end() - 2, end() - 1);
        }
        *(begin() + index) = x;
        return begin() + index;
    }
}

template <class T, class Allocator>
_TYPENAME deque<T,Allocator>::iterator 
deque<T,Allocator>::insert (deque<T,Allocator>::iterator position, const T& x)
{
    if (position == begin())
    {
        push_front(x); return begin();
    }
    else if (position == end())
    {
        push_back(x); return end() - 1;
    }
    else
    {
        difference_type index = position - begin();
        if (index < length)
        {
            push_front(*begin());
            copy(begin() + 2, begin() + index + 1, begin() + 1);
        }
        else
        {
            push_back(*(end() - 1));
            copy_backward(begin() + index, end() - 2, end() - 1);
        }
        *(begin() + index) = x;
        return begin() + index;
    }
}

template <class T, class Allocator>
void deque<T,Allocator>::insert (deque<T,Allocator>::iterator position,      
                                 size_type n, const T& x)
{
    difference_type index     = position - begin();
    difference_type remainder = length   - index;

    if (remainder > index)
    {
        if (n > index)
        {
            difference_type m = n - index;
            while (m-- > 0) push_front(x);
            difference_type i = index;
            while (i--) push_front(*(begin() + n - 1));
#ifdef _RWSTD_FILL_NAME_CLASH
            std_fill(begin() + n, begin() + n + index, x);
#else
            fill(begin() + n, begin() + n + index, x);
#endif
        }
        else
        {
            difference_type i = n;
            while (i--) push_front(*(begin() + n - 1));
            copy(begin() + n + n, begin() + n + index, begin() + n);
#ifdef _RWSTD_FILL_NAME_CLASH
            std_fill(begin() + index, begin() + n + index, x);
#else
            fill(begin() + index, begin() + n + index, x);
#endif
        }
    }
    else
    {
        difference_type orig_len = index + remainder;
        if (n > remainder)
        {
            difference_type m = n - remainder;
            while (m-- > 0) push_back(x);
            difference_type i = 0;
            while (i < remainder) push_back(*(begin() + index + i++));
#ifdef _RWSTD_FILL_NAME_CLASH
            std_fill(begin() + index, begin() + orig_len, x);
#else
            fill(begin() + index, begin() + orig_len, x);
#endif
        }
        else
        {
            difference_type i = 0;
            while (i < n) push_back(*(begin() + orig_len - n + i++));
            copy_backward(begin() + index, begin() + orig_len - n,
                          begin() + orig_len);
#ifdef _RWSTD_FILL_NAME_CLASH
            std_fill(begin() + index, begin() + index + n, x);
#else
            fill(begin() + index, begin() + index + n, x);
#endif
        }
    }
}


#ifndef _RWSTD_NO_MEMBER_TEMPLATES
template <class T, class Allocator>
template <class Iterator>
void deque<T,Allocator>::insert (deque<T,Allocator>::iterator position, 
                                 Iterator first, Iterator last)
{
    difference_type index     = position - begin();
    difference_type remainder = length   - index;
    size_type n;
    __initialize(n, size_type(0));
    distance(first, last, n);

    if (remainder > index)
    {
        if (n > index)
        {
            Iterator m = last - index;
            while (m != first) push_front(*--m);
            difference_type i = index;
            while (i--) push_front(*(begin() + n - 1));
            copy(last - index, last, begin() + n);
        }
        else
        {
            difference_type i = n;
            while (i--) push_front(*(begin() + n - 1));
            copy(begin() + n + n, begin() + n + index, begin() + n);
            copy(first, last, begin() + index);
        }
    }
    else
    {
        difference_type orig_len = index + remainder;
        if (n > remainder)
        {
            Iterator m = first + remainder;
            while (m != last) push_back(*m++);
            difference_type i = 0;
            while (i < remainder) push_back(*(begin() + index + i++));
            copy(first, first + remainder, begin() + index);
        }
        else
        {
            difference_type i = 0;
            while (i < n) push_back(*(begin() + orig_len - n + i++));
            copy_backward(begin() + index, begin() + orig_len - n,
                          begin() + orig_len);
            copy(first, last, begin() + index);
        }
    }
}

#else

template<class T, class Allocator>
void deque<T,Allocator>::insert (deque<T,Allocator>::iterator position, 
                                 deque<T,Allocator>::const_iterator first, 
                                 deque<T,Allocator>::const_iterator last)
{
    difference_type index     = position - begin();
    difference_type remainder = length   - index;
    size_type n;
    __initialize(n, size_type(0));
    distance(first, last, n);

    if (remainder > index)
    {
        if (n > index)
        {
            const_iterator m = last - index;
            while (m != first) push_front(*--m);
            difference_type i = index;
            while (i--) push_front(*(begin() + n - 1));
            copy(last - index, last, begin() + n);
        }
        else
        {
            difference_type i = n;
            while (i--) push_front(*(begin() + n - 1));
            copy(begin() + n + n, begin() + n + index, begin() + n);
            copy(first, last, begin() + index);
        }
    }
    else
    {
        difference_type orig_len = index + remainder;
        if (n > remainder)
        {
            const_iterator m = first + remainder;
            while (m != last) push_back(*m++);
            difference_type i = 0;
            while (i < remainder) push_back(*(begin() + index + i++));
            copy(first, first + remainder, begin() + index);
        }
        else
        {
            difference_type i = 0;
            while (i < n) push_back(*(begin() + orig_len - n + i++));
            copy_backward(begin() + index, begin() + orig_len - n,
                          begin() + orig_len);
            copy(first, last, begin() + index);
        }
    }
}

template<class T, class Allocator>
void deque<T,Allocator>::insert (deque<T,Allocator>::iterator position, 
                                 const T* first, const T* last)
{
    difference_type index     = position - begin();
    difference_type remainder = length   - index;
    size_type n;
    __initialize(n, size_type(0));
    distance(first, last, n);

    if (remainder > index)
    {
        if (n > index)
        {
            const T* m = last - index;
            while (m != first) push_front(*--m);
            difference_type i = index;
            while (i--) push_front(*(begin() + n - 1));
            copy(last - index, last, begin() + n);
        }
        else
        {
            difference_type i = n;
            while (i--) push_front(*(begin() + n - 1));
            copy(begin() + n + n, begin() + n + index, begin() + n);
            copy(first, last, begin() + index);
        }
    }
    else
    {
        difference_type orig_len = index + remainder;
        if (n > remainder)
        {
            const T* m = first + remainder;
            while (m != last) push_back(*m++);
            difference_type i = 0;
            while (i < remainder) push_back(*(begin() + index + i++));
            copy(first, first + remainder, begin() + index);
        }
        else
        {
            difference_type i = 0;
            while (i < n) push_back(*(begin() + orig_len - n + i++));
            copy_backward(begin() + index, begin() + orig_len - n,
                          begin() + orig_len);
            copy(first, last, begin() + index);
        }
    }
}

#endif /*_RWSTD_NO_MEMBER_TEMPLATES*/

template <class T, class Allocator>
_TYPENAME deque<T,Allocator>::iterator 
deque<T,Allocator>::erase (deque<T,Allocator>::iterator position)
{
    if (end() - position > position - begin())
    {
        copy_backward(begin(), position, position + 1); 
        pop_front();
        return position + 1;
    }
    else
    {
        copy(position + 1, end(), position); 
        pop_back();
        return position;
    }
}
    
template <class T, class Allocator>
_TYPENAME deque<T,Allocator>::iterator 
deque<T,Allocator>::erase (deque<T,Allocator>::iterator first, 
                           deque<T,Allocator>::iterator last)
{
    difference_type n = last - first;
    if (end() - last > first - begin())
    {
        copy_backward(begin(), first, last);
        while(n-- > 0) pop_front();
        return last;
    }
    else
    {
        copy(last, end(), first);
        while(n-- > 0) pop_back();
        return first;
    }
}



#ifndef _RWSTD_NO_NAMESPACE
}
#endif
