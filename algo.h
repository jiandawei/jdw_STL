#ifndef ALGO_H
#define ALGO_H
#include "traits.h"
namespace mystl {
/*------------------------------------adjacent_find-------------------------------*/
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first , ForwardIterator last ) {
    if(first == last) return  last;
    ForwardIterator next = first;
    while(++next != last) {
        if(*first == *next)return first;
        first = next;
    }
    return last;
}

template <class ForwardIterator , class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first , ForwardIterator last , BinaryPredicate binary_pred ) {
    if(first == last) return  last;
    ForwardIterator next = first;
    while(++next != last) {
        if(binary_pred(*first ,*next))return first;
        first = next;
    }
    return last;
}


/*------------------------------------count---------------------------------------*/
template <class InputIterator , class T>
typename iterator_traits<InputIterator>::difference_type count(InputIterator first , InputIterator last , const T& value) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(; first != last ; ++first) {
        if(*first == value)
            ++n;
    }
    return n;
}

template <class InputIterator , class T , class Size>
void count(InputIterator first , InputIterator last , const T& value , Size& n) {
    for(; first != last ; ++first) {
        if(*first == value)
            ++n;
    }
}


/*---------------------------------count_if-------------------------------------------*/
template <class InputIterator , class Predicate>
typename iterator_traits<InputIterator>::difference_type count(InputIterator first , InputIterator last , Predicate pred) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(; first != last ; ++first) {
        if(pred(*first))
            ++n;
    }
    return n;
}

template <class InputIterator , class Predicate , class Size>
void count(InputIterator first , InputIterator last , Predicate pred , Size& n) {
    for(; first != last ; ++first) {
        if(pred(*first))
            ++n;
    }
}


/*-------------------------------------find-------------------------------------*/
template <class InputIterator , class T>
InputIterator find(InputIterator first , InputIterator last , const T& value) {
    while (first != last) {
        if(*first == value)
            return first;
        ++first;
    }
    return first;
}


/*-----------------------------------------find_if-------------------------------*/

template <class InputIterator , class Predicate>
InputIterator find_if(InputIterator first , InputIterator last , Predicate pred) {
    while (first != last) {
        if(pred(*first))
            return first;
        ++first;
    }
    return first;
}

/*-----------------------------------------find_end-------------------------------*/

/*-----------------------------------------find_first_of-------------------------------*/

/*-----------------------------------------for_each-------------------------------*/
template <class InputIterator , class Function>
Function for_each(InputIterator first , InputIterator last , Function f) {
    for(; first != last ; ++first) {
        f(*first);
    }
    return f;//忽略返回值
}

/*-------------------------------------generate-------------------------------*/
template <class ForwardIterator , class Generator>
void generate(ForwardIterator first , ForwardIterator last , Generator gen) {
    for ( ; first != last ; ++first) {
        *first = gen();
    }
}

/*-------------------------------------generate_n-------------------------------*/
template <class ForwardIterator , class Size , class Generator>
void generate_n(ForwardIterator first , Size n , Generator gen) {
    while (n--) {
        *first = gen();
        ++first;
    }
}

/*-------------------------------------includes----------------------------------*/
template <class InputIterator1 , class InputIterator2>
bool includes(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if(*first2 < *first1)
            return false;
        else if(*first1 < *first2)
            ++first1;
        else
            ++first1,++first2;
    }
    return first2 == last2;
}


template <class InputIterator1 , class InputIterator2 , class Compare>
bool includes(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2 , Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if(comp(*first2 , *first1))
            return false;
        else if(comp(*first1 , *first2))
            ++first1;
        else
            ++first1,++first2;
    }
    return first2 == last2;
}

/*---------------------------------------------------max_element------------------------------------*/
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first , ForwardIterator last ) {
    ForwardIterator result = first;
    while (++first != last) {
        if(*result < *first)
            result = first;
    }
    return  result;
}

template <class ForwardIterator , class Compare>
ForwardIterator max_element(ForwardIterator first , ForwardIterator last , Compare comp) {
    ForwardIterator result = first;
    while (++first != last) {
        if(comp(*result , *first))
            result = first;
    }
    return result;
}


/*---------------------------------------------------min_element------------------------------------*/
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first , ForwardIterator last ) {
    ForwardIterator result = first;
    while (++first != last) {
        if(*first < *result)
            result = first;
    }
    return  result;
}

template <class ForwardIterator , class Compare>
ForwardIterator min_element(ForwardIterator first , ForwardIterator last , Compare comp) {
    ForwardIterator result = first;
    while (++first != last) {
        if(comp(*first , *result))
            result = first;
    }
    return result;
}

/*--------------------------------------------merge--------------------------------------------------*/
template <class InputIterator1 , class InputIterator2 , class OutputIterator>
OutputIterator merge(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2 , OutputIterator result ) {
    while (first1 != last1 && first2 != last2) {
        if(*first2 < *first1) {
            *result = *first2;
            ++first2;
        }
        else{
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2 , last2 , copy(first1 , last1 ,result));
}

template <class InputIterator1 , class InputIterator2 , class OutputIterator , class Compare>
OutputIterator merge(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , InputIterator2 last2 , OutputIterator result , Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if(comp(*first2 , *first1)) {
            *result = *first2;
            ++first2;
        }
        else{
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2 , last2 , copy(first1 , last1 ,result));
}

/*--------------------------------------partion--------------------------------------*/
template <class BidirectionalIterator , class Predicate>
BidirectionalIterator partion(BidirectionalIterator first , BidirectionalIterator last , Predicate pred) {
    while (true) {
        while (true) {
            if(first == last)return first;
            else if(pred(*first))++first;
            else break;
        }
        --last;//要将last-1才可以正常比较
        while (true) {
            if(first == last)return first;
            else if(!pred(*last))--last;
            else break;
        }
        iter_swap(first , last);
        ++first;//交换后要+1
    }
    return first;
}

/*--------------------------------------remove-----------------------------------*/
template <class InputIterator , class OutputIterator , class T>
OutputIterator remove_copy(InputIterator first , InputIterator last , OutputIterator result , const T& value);

template <class ForwardIterator , class T>
ForwardIterator remove(ForwardIterator first , ForwardIterator last , const T& value) {
    first = find(first , last , value);//找到value第一次出现的位置
    ForwardIterator next = first;
    return first == last ? first : remove_copy(++next , last , first , value);
}

/*--------------------------------------remove_copy-----------------------------------*/
template <class InputIterator , class OutputIterator , class T>
OutputIterator remove_copy(InputIterator first , InputIterator last , OutputIterator result , const T& value) {
    for( ; first != last ; ++first) {
        if(*first != value){
            *result = *first;
            ++result;
        }
    }
    return result;
}

/*----------------------------------remove_if--------------------------------*/
template <class InputIterator , class OutputIterator , class Predicate>
OutputIterator remove_copy_if(InputIterator first , InputIterator last , OutputIterator result , Predicate pred);

template <class ForwardIterator , class OutputIterator , class Predicate>
OutputIterator remove_if(ForwardIterator first , ForwardIterator last , Predicate pred) {
    first = find_if(first , last , pred);
    ForwardIterator next = first;
    return remove_copy_if(++next , last , last , pred);
}

/*----------------------------------remove_copy_if--------------------------------*/
template <class InputIterator , class OutputIterator , class Predicate>
OutputIterator remove_copy_if(InputIterator first , InputIterator last , OutputIterator result , Predicate pred) {
    for( ; first != last ; ++first) {
        if(!pred(*first)){
            *result = *first;
            ++result;
        }
    }
    return result;
}


/*-------------------------------replace---------------------------------*/
template <class ForwardIterator , class T>
void replace(ForwardIterator first , ForwardIterator last , const T& old_value , const T& new_value) {
    for ( ; first != last ; ++first) {
        if(*first == old_value)
            *first = new_value;
    }
}

/*------------------------replace_copy--------------------------------*/
template <class InputIterator , class OutputIterator , class T>
OutputIterator replace(InputIterator first , InputIterator last , OutputIterator result , const T& old_value , const T& new_value) {
    for ( ; first != last ; ++first , ++result)
        *result = *first == old_value ? new_value : *first;
    return result;
}

/*-------------------------------replace_if---------------------------------*/
template <class ForwardIterator , class T , class Predicate>
void replace(ForwardIterator first , ForwardIterator last , Predicate pred , const T& new_value) {
    for ( ; first != last ; ++first) {
        if(pred(*first))*first = new_value;
    }
}

/*-----------------------------replace_copy_if-----------------------------*/
template <class InputIterator , class OutputIterator , class T , class Predicate>
OutputIterator replace(InputIterator first , InputIterator last , OutputIterator result , Predicate pred , const T& new_value) {
    for ( ; first != last ; ++first , ++result)
        *result = pred(*first) ? new_value : *first;
    return result;
}


/*---------------------------------------reverse-------------------------------*/
template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first , BidirectionalIterator last , bidirectional_iterator_tag);
template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first , RandomAccessIterator last , random_access_iterator_tag);

template <class BidirectionalIterator>
inline void reverse(BidirectionalIterator first , BidirectionalIterator last) {
    __reverse(first , last , iterator_category(first));
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first , BidirectionalIterator last , bidirectional_iterator_tag) {
    while (true) {
        if(first == last || first == --last)//区间元素小于等于1
            return;
        else
            iter_swap(first++ , last);
    }
}

template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first , RandomAccessIterator last , random_access_iterator_tag) {
    while (first < last) iter_swap(first++ , --last);
}


/*-------------------------------reverse_copy------------------------------------*/
template <class BidirectionalIterator , class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first , BidirectionalIterator last , OutputIterator result) {
    while(first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}


/*-------------------------rotate------------------------------------------------*/
template <class ForwardIterator>
inline void rotate(ForwardIterator first , ForwardIterator middle , ForwardIterator last) {
    __rotate(first , middle , last , distance_type(first) , iterator_category(first));
}

//forward iterator版本
template <class ForwardIterator , class Distance>
void __rotate(ForwardIterator first , ForwardIterator middle , ForwardIterator last , Distance* , forward_iterator_tag) {
    for (ForwardIterator i = middle ; ;) {
        iter_swap(first , i);
        ++first;
        ++i;
        if(first == middle){
            if(i == last)return ;//[first,middle)的长度 = [middle,last)的区间长度
            middle = i;//[first,middle)的长度 < [middle,last)的区间长度
        }
        else if(i == last)//[first,middle)的长度 > [middle,last)的区间长度
            i = middle;
    }
}


//bidirectional iterator版本
template <class BidirectionalIterator , class Distance>
void __rotate(BidirectionalIterator first , BidirectionalIterator middle , BidirectionalIterator last , Distance* , bidirectional_iterator_tag) {
    reverse(first , middle);
    reverse(middle , last);
    reverse(first , last);
}

//random access iterator版本
template <class RandomAccessIterator , class Distance , class T>
void __rotate_cycle(RandomAccessIterator first  , RandomAccessIterator last , RandomAccessIterator initial , Distance shift , T*);

template <class RandomAccessIterator , class Distance>
void __rotate(RandomAccessIterator first , RandomAccessIterator middle , RandomAccessIterator last , Distance* , random_access_iterator_tag) {
    Distance n = __gcd(last - first , middle - first);
    while (n--) {
        __rotate_cycle(first , last , first + n , middle - first , value_type(first));
    }
}

template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m , EuclideanRingElement n) {
    while (n) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

template <class RandomAccessIterator , class Distance , class T>
void __rotate_cycle(RandomAccessIterator first  , RandomAccessIterator last , RandomAccessIterator initial , Distance shift , T*) {
    T value = *initial;
    RandomAccessIterator ptr1 = initial;
    RandomAccessIterator ptr2 = ptr1 + shift;
    while (ptr2 != initial) {
        *ptr1 = *ptr2;
        ptr1 = ptr2;
        if(last - ptr2 > shift)
            ptr2 += shift;
        else
            ptr2 = first + (shift - (last - ptr2));
    }
    *ptr1 = value;
}


/*------------------------------rotate_copy------------------------------------*/
template <class ForwardIterator , class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first , ForwardIterator middle , ForwardIterator last , OutputIterator result) {
    return copy(first , middle , cop(middle , last , result));
}

/*--------------------------search-----------------------------------------------*/

/*--------------------------search_n-----------------------------------------------*/

/*--------------------------swap_ranges-----------------------------------------------*/
template <class ForwardIterator1 , class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1 , ForwardIterator1 last1 , ForwardIterator2 first2) {
    while (first1 != last1) {
        iter_swap(first1 , first2);
        ++first2;
    }
    return first2;
}

/*------------------------------transform----------------------------------------------*/
template <class InputIterator , class OutputIterator , class UnaryOperation>
OutputIterator transform(InputIterator first1 , InputIterator last1 ,  OutputIterator result , UnaryOperation op) {
    for(; first1 != last1 ; ++first1 , ++result) {
        *result = op(*first1);
    }
    return result;
}

template <class InputIterator1 , class InputIterator2 , class OutputIterator , class BinaryOperation>
OutputIterator transform(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , OutputIterator result , BinaryOperation binary_op) {
    for(; first1 != last1 ; ++first1 , ++first2 , ++result) {
        *result = binary_op(*first1 , *first2);
    }
    return result;
}

/*------------------------unique----------------------------------------------*/
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first , ForwardIterator last);
template <class InputIterator , class OutputIterator>
OutputIterator unique_copy(InputIterator first , InputIterator last , OutputIterator result);
template <class InputIterator , class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first , InputIterator last , ForwardIterator result , forward_iterator_tag);
template <class InputIterator , class OutputIterator>
OutputIterator __unique_copy(InputIterator first , InputIterator last ,OutputIterator result , output_iterator_tag) ;
template <class InputIterator , class OutputIterator , class T>
OutputIterator __unique_copy(InputIterator first , InputIterator last ,OutputIterator result , T*);

template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first , ForwardIterator last) {
    first = adjacent_find(first , last);
    return unique_copy(first , last , first);
}


/*------------------------unique_copy----------------------------------------------*/
template <class InputIterator , class OutputIterator>
OutputIterator unique_copy(InputIterator first , InputIterator last , OutputIterator result) {
    if(first == last) return;
    return __unique_copy(first , last , result , iterator_category(first));
}

//forward_iterator_tag版本
template <class InputIterator , class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first , InputIterator last , ForwardIterator result , forward_iterator_tag) {
    *result = *first;
    while (++first != last) {
        if(*result != *first)*++result = *first;
    }
    return ++result;
}
//output_iterator_tag版本

template <class InputIterator , class OutputIterator>
OutputIterator __unique_copy(InputIterator first , InputIterator last ,OutputIterator result , output_iterator_tag) {
    return __unique_copy(first , last , result , value_type(first));
}

template <class InputIterator , class OutputIterator , class T>
OutputIterator __unique_copy(InputIterator first , InputIterator last ,OutputIterator result , T*) {
    T value = *first;
    *result = value;
    while (++first != last) {
        if(value != *first){
            value = *first;
            *++result = *first;
        }
    }
    return ++result;
}


/*-------------------------------------lower_bound-----------------------------------------*/

template <class RandomAccessIterator , class T , class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Distance* ,random_access_iterator_tag);
template <class ForwardIterator , class T , class Distance>
ForwardIterator __lower_bound(ForwardIterator first , ForwardIterator last , const T& value , Distance* , forward_iterator_tag);
//版本1
template <class ForwardIterator , class T>
inline ForwardIterator lower_bound(ForwardIterator first , ForwardIterator last , const T& value) {
    return __lower_bound(first , last , value , distance_type(first) , iterator_category(first));
}

template <class ForwardIterator , class T , class Distance , class Compare>
ForwardIterator __lower_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp , Distance* , forward_iterator_tag);
template <class RandomAccessIterator , class T , class Distance , class Compare>
RandomAccessIterator __lower_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Compare comp , Distance* ,random_access_iterator_tag) ;
//版本2
template <class ForwardIterator , class T , class Compare>
inline ForwardIterator lower_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp) {
    return __lower_bound(first , last , value , comp ,  distance_type(first) , iterator_category(first));
}

//版本1：forward_iterator 版本
template <class ForwardIterator , class T , class Distance>
ForwardIterator __lower_bound(ForwardIterator first , ForwardIterator last , const T& value , Distance* , forward_iterator_tag) {
    Distance len = 0;
    distance(first , last , len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;//使middle指向范围中间
        advance(middle , half);
        if(*middle < value) {
            first = middle;
            ++first;
            len = len - half -1 ;
        }
        else
            len = half;
    }
    return first;
}

//版本1：random_access_iterator版本
template <class RandomAccessIterator , class T , class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Distance* ,random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if(*middle < value){
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}


//版本2：forward_iterator 版本
template <class ForwardIterator , class T , class Distance , class Compare>
ForwardIterator __lower_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp , Distance* , forward_iterator_tag) {
    Distance len = 0;
    distance(first , last , len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;//使middle指向范围中间
        advance(middle , half);
        if(comp(*middle , value)) {
            first = middle;
            ++first;
            len = len - half -1 ;
        }
        else
            len = half;
    }
    return first;
}

//版本2：random_access_iterator版本
template <class RandomAccessIterator , class T , class Distance , class Compare>
RandomAccessIterator __lower_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Compare comp , Distance* ,random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if(comp(*middle , value)){
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}


/*-------------------------------------upper_bound-----------------------------------------*/

template <class RandomAccessIterator , class T , class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Distance* ,random_access_iterator_tag);
template <class ForwardIterator , class T , class Distance>
ForwardIterator __upper_bound(ForwardIterator first , ForwardIterator last , const T& value , Distance* , forward_iterator_tag);
//版本1
template <class ForwardIterator , class T>
inline ForwardIterator upper_bound(ForwardIterator first , ForwardIterator last , const T& value) {
    return __upper_bound(first , last , value , distance_type(first) , iterator_category(first));
}


template <class ForwardIterator , class T , class Distance , class Compare>
ForwardIterator __upper_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp , Distance* , forward_iterator_tag);
template <class RandomAccessIterator , class T , class Distance , class Compare>
RandomAccessIterator __upper_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Compare comp , Distance* ,random_access_iterator_tag) ;
//版本2
template <class ForwardIterator , class T , class Compare>
inline ForwardIterator upper_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp) {
    return __upper_bound(first , last , value , comp ,  distance_type(first) , iterator_category(first));
}

//版本1：forward_iterator 版本
template <class ForwardIterator , class T , class Distance>
ForwardIterator __upper_bound(ForwardIterator first , ForwardIterator last , const T& value , Distance* , forward_iterator_tag) {
    Distance len = 0;
    distance(first , last , len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;//使middle指向范围中间
        advance(middle , half);
        if(value < *middle ) {
            len = half ;
        }
        else{
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

//版本1：random_access_iterator版本
template <class RandomAccessIterator , class T , class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value , Distance* ,random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if(value < *middle){
            len = half;
        }
        else{
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

//版本2：forward_iterator 版本
template <class ForwardIterator , class T , class Distance , class Compare >
ForwardIterator __upper_bound(ForwardIterator first , ForwardIterator last , const T& value , Compare comp ,Distance* , forward_iterator_tag) {
    Distance len = 0;
    distance(first , last , len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;//使middle指向范围中间
        advance(middle , half);
        if(comp(value , *middle) ) {
            len = half ;
        }
        else{
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

//版本2：random_access_iterator版本
template <class RandomAccessIterator , class T , class Distance , class Compare>
RandomAccessIterator __upper_bound(RandomAccessIterator first , RandomAccessIterator last , const T& value ,Compare comp , Distance* ,random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first + half;
        if(comp(value , *middle)){
            len = half;
        }
        else{
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}


/*-----------------------------binary_search------------------------------------------*/
//版本1
template <class ForwardIterator , class T>
bool binary_search(ForwardIterator first , ForwardIterator last , const T& value) {
    ForwardIterator i =lower_bound(first , last , value);
    return i != last && !(value < *i);
}
//版本2
template <class ForwardIterator , class T , class Compare>
bool binary_search(ForwardIterator first , ForwardIterator last , const T& value , Compare comp) {
    ForwardIterator i =lower_bound(first , last , value , comp);
    return i != last && !(value < *i);
}

/*------------------------------next_permutation------------------------------------*/
//版本1
template <class BidirectionalIterator>
bool next_permuation(BidirectionalIterator first , BidirectionalIterator last) {
    if(first == last) return false;
    BidirectionalIterator i = first;
    ++i;
    if(i == last)return false;
    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;
        if(*i < *ii){
            BidirectionalIterator j = last;
            while (!(*i < *--j)) {
                ;
            }
            iter_swap(i , j);
            reverse(ii , last);
            return true;
        }
        if(i == last){
            reverse(first , last);
            return false;
        }
    }
}

//版本2
template <class BidirectionalIterator , class Compare>
bool next_permuation(BidirectionalIterator first , BidirectionalIterator last , Compare comp) {
    if(first == last) return false;
    BidirectionalIterator i = first;
    ++i;
    if(i == last)return false;
    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;
        if(comp(*i , *ii)){
            BidirectionalIterator j = last;
            while (!comp(*i , *--j)) {
                ;
            }
            iter_swap(i , j);
            reverse(ii , last);
            return true;
        }
        if(i == last){
            reverse(first , last);
            return false;
        }
    }
}


/*------------------------------prev_permutation------------------------------------*/
//版本1
template <class BidirectionalIterator>
bool prev_permuation(BidirectionalIterator first , BidirectionalIterator last) {
    if(first == last) return false;
    BidirectionalIterator i = first;
    ++i;
    if(i == last)return false;
    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;
        if(*ii < *i){
            BidirectionalIterator j = last;
            while (!(*--j < *i)) {
                ;
            }
            iter_swap(i , j);
            reverse(ii , last);
            return true;
        }
        if(i == last){
            reverse(first , last);
            return false;
        }
    }
}

//版本2
template <class BidirectionalIterator , class Compare>
bool prev_permuation(BidirectionalIterator first , BidirectionalIterator last , Compare comp) {
    if(first == last) return false;
    BidirectionalIterator i = first;
    ++i;
    if(i == last)return false;
    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;
        if(comp(*ii , *i)){
            BidirectionalIterator j = last;
            while (!comp(*--j , *i)) {
                ;
            }
            iter_swap(i , j);
            reverse(ii , last);
            return true;
        }
        if(i == last){
            reverse(first , last);
            return false;
        }
    }
}

/*------------------------------------------random_shuffle---------------------------------------------*/

}//namespace mystl
#endif // ALGO_H

