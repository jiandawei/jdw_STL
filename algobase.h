
#ifndef ALGOBASE_H
#define ALGOBASE_H
#include "traits.h"
#include <string.h>
#include <utility>
using namespace std;
/*--------------------------------equal----------------------------------------*/
namespace mystl {
template <class InputIterator1 , class InputIterator2>
inline bool equal (InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2) {
    for( ; first1 != last1 ; ++first1 , ++first2) {
        if(*first1 != *first2)
            return false;
    }
    return true;
}

template <class InputIterator1 , class InputIterator2 , class BinaryPredicate>
inline bool equal (InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , BinaryPredicate binary_pred) {
    for( ; first1 != last1 ; ++first1 , ++first2) {
        if(!binary_pred(*first1 , *first2))
            return false;
    }
    return true;
}


/*--------------------------------fill----------------------------------------*/
template <class ForwardIterator , class T>
void fill(ForwardIterator first , ForwardIterator last , const T& value) {
    for ( ; first != last ; ++first)
        *first = value;
}

/*--------------------------------fill_n----------------------------------------*/
template <class OutputIterator , class Size , class T>
OutputIterator fill_n(OutputIterator first , Size n , const T& value) {
    for(; n>0 ; --n , ++first)
        *first = value;
    return first;
}

/*-------------------------------iter_swap-------------------------------------------*/
template <class ForwardIterator1 , class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a , ForwardIterator2 b) {
    typename iterator_traits<ForwardIterator1>::value_type tmp = *a;
    *a = *b;
    *b = tmp;
}

/*---------------------------------------------max-------------------------------------------------------*/
template <class T>
inline const T& max(const T& a , const T& b) {
    return a > b ? a : b;
}

template <class T , class Compare>
inline const T& max(const T& a , const T& b , Compare comp) {
    return comp(a , b) ? b : a;
}

/*---------------------------------------------min-------------------------------------------------------*/
template <class T>
inline const T& min(const T& a , const T& b) {
    return a < b ? a : b;
}

template <class T , class Compare>
inline const T& min(const T& a , const T& b , Compare comp) {
    return comp(a , b) ? a : b;
}


/*-----------------------------lexicographical_compare-----------------------*/
template <class InputIterator1 , class InputIterator2>
bool lexicographical_compare(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2) {
    for( ; first1 != last1 && first2 != last2 ; ++first1 , ++first2) {
        if(*first1 < *first2)
            return true;
        if(*first1 > *first2)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1 , class InputIterator2 , class Compare>
bool lexicographical_compare(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2 , Compare comp) {
    for( ; first1 != last1 && first2 != last2 ; ++first1 , ++first2) {
        if(comp(*first1 , *first2))
            return true;
        if(comp(*first2 , *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

inline bool lexicographical_compare(const unsigned char* first1 , const unsigned char* last1 , const unsigned char* first2 , const unsigned char* last2 ) {
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    //先比较长度相同的一段，memcmp是c标准函数，用于比较范围内字节的异同
    const int res = memcmp(first1 , first2 , min(len1 , len2));
    return res !=0 ? res < 0 : len1 < len2;
}

inline bool lexicographical_compare(const char* first1 , const char* last1 , const char* first2 , const char* last2 ) {
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    //先比较长度相同的一段，memcmp是c标准函数，用于比较范围内字节的异同
    const int res = memcmp(first1 , first2 , min(len1 , len2));
    return res !=0 ? res < 0 : len1 < len2;
}

/*---------------------------------------------------mismatch-----------------------------------------*/
template < class InputIterator1 , class InputIterator2>
pair<InputIterator1 , InputIterator2> mismatch(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2) {
    while(first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1 , InputIterator2 >(first1 , first2);
}

template < class InputIterator1 , class InputIterator2 , class BinarPredicate >
pair<InputIterator1 , InputIterator2> mismatch(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , BinarPredicate binary_pred) {
    while(first1 != last1 && binary_pred(*first1 , *first2)) {
        ++first1;
        ++first2;
    }
    return pair<InputIterator1 , InputIterator2 >(first1 , first2);
}

/*---------------------------------------------------swap-----------------------------------------------*/
template <class T>
inline void swap(T& a , T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

/*------------------------------------------------copy--------------------------------------------------*/

//完全泛化版本
template <class InputIterator , class OutputIterator>
struct __copy_dispatch {
    OutputIterator operator()(InputIterator first , InputIterator last , OutputIterator result) {
        return __copy(first , last , result , iterator_category(first));
    }
};

//偏特化版本1
template <class T>
struct __copy_dispatch<T* , T*> {
    T* operator()(T* first , T* last , T* result) {
        typedef typename __type_traits<T>::has_trivial_assign_operator t;
        return __copy_t(first , last , result , t());
    }
};

//偏特化版本2
template <class T>
struct __copy_dispatch<const T* , T*> {
    T* operator()(const T* first , const T* last , T* result) {
        typedef typename __type_traits<T>::has_trivial_assign_operator t;
        return __copy_t(first , last , result , t());
    }
};

//InputIterator版本
template <class InputIterator , class OutputIterator>
inline OutputIterator __copy(InputIterator first , InputIterator last , OutputIterator result , input_iterator_tag) {
    for( ; first != last ; ++first , ++result)//以迭代器是否等同来判断是否继续执行，速度慢
        *result = *first;
    return result;
}

//RandomAccessIterator版本
template <class RandomAccessIterator , class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first , RandomAccessIterator last , OutputIterator result , random_access_iterator_tag) {
    return __copy_d(first , last , result , distance_type(first));
}

template <class RandomAccessIterator , class OutputIterator , class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first , RandomAccessIterator last , OutputIterator result , Distance*) {
    for(Distance n = last - first ; n > 0 ; --n , ++first , ++result)
        *result = *first;
    return result;
}



//完全泛化版本
template <class InputIterator , class OutputIterator>
inline OutputIterator copy(InputIterator first , InputIterator last , OutputIterator result){
    return __copy_dispatch<InputIterator , OutputIterator>() (first , last , result);
}

//char* 特化版本
inline char* copy(const char* first , const char* last , char* result) {
    memmove(result , first , last - first);
    return result + (last - first);
}

//wchar_t*特化版本
inline wchar_t* copy(const wchar_t* first , const wchar_t* last , wchar_t* result) {
    memmove(result , first , sizeof(wchar_t) * (last - first));
    return result + (last - first);
}


//以下版本适合指针所指之物有trivial_assign_operator
template <class T>
inline T* __copy_t(const T* first , const T* last , T* result , __true_type) {
    memmove(result , first , sizeof(T) * (last - first));
    return result + (last - first);
}

//以下版本适合指针所指之物有non-trivial_assign_operator
template <class T>
inline T* __copy_t(const T* first , const T* last , T* result , __false_type) {
    return __copy_d(first , last , result , (ptrdiff_t*)0);
}
}//namespace mystl
#endif // ALGOBASE_H
