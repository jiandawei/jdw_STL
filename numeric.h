#ifndef NUMERIC_H
#define NUMERIC_H
#include"traits.h"
namespace mystl {
/*-----------------------------accumulate----------------------------*/
//版本1
template <class InputIterator , class T>
T accumulate(InputIterator first , InputIterator last , T init) {
    while (first != last) {
        init = init + *first;
        ++first;
    }
    return init;
}
//版本2
template <class InputIterator , class T , class BinaryOperation>
T accumulate(InputIterator first , InputIterator last , T init , BinaryOperation binary_op) {
    while (first != last) {
        init = binary_op(init , *first);
        ++first;
    }
    return init;
}

/*--------------------------adjacent_difference------------------------*/
template <class InputIterator , class OutputIterator , class T>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result , T*);
//版本1
template <class InputIterator , class OutputIterator>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result) {
    if(first == last) return  result;
    *result = *first;
    return __adjacent_difference(first , last , result , value_type(first));
}

template <class InputIterator , class OutputIterator , class T>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result , T*) {
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = tmp - value;
        value = tmp;
    }
    return  ++result;
}


template <class InputIterator , class OutputIterator , class T>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result , T*);
//版本2
template <class InputIterator , class OutputIterator , class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result , BinaryOperation binary_op) {
    if(first == last) return  result;
    *result = *first;
    return __adjacent_difference(first , last , result , value_type(first) , binary_op);
}

template <class InputIterator , class OutputIterator , class T , class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first , InputIterator last , OutputIterator result , T* , BinaryOperation binary_op) {
    T value = *first;
    while (++first != last) {
        T tmp = *first;
        *++result = binary_op(tmp , value);
        value = tmp;
    }
    return  ++result;
}

/*----------------------inner_product------------------------------*/
//版本1
template <class InputIterator1 , class InputIterator2 , class T>
T inner_product(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , T init) {
    for( ; first1 != last1 ; ++first1 , ++first2) {
        init = init + ((*first1) * (*first2));
    }
    return init;
}

//版本2
template <class InputIterator1 , class InputIterator2 , class T , class BinaryOperation1 , class BInaryOperation2>
T inner_product(InputIterator1 first1 , InputIterator1 last1 , InputIterator2 first2 , T init , BinaryOperation1 binary_op1 , BInaryOperation2 binary_op2) {
    for( ; first1 != last1 ; ++first1 , ++first2) {
        init = binary_op1(init ,  binary_op2((*first1) , (*first2)));//op1 ----> + , op2 ------> *
    }
    return init;
}

/*----------------------------------partial_sum--------------------------------*/
//版本1
template <class InputIterator , class OutputIterator , class T>
OutputIterator partial_sum(InputIterator first , InputIterator last , OutputIterator result , T* );
template <class InputIterator , class OutputIterator>
OutputIterator partial_sum(InputIterator first , InputIterator last , OutputIterator result) {
    if(first == last)return result;
    *result = *first;
    return __partial_sum(first , last , result , value_type(first));
}

template <class InputIterator , class OutputIterator , class T>
OutputIterator __partial_sum(InputIterator first , InputIterator last , OutputIterator result , T* ) {
    T value = *first;
    while (first != last) {
        value = value + *first;
        *++result = value;
    }
    return ++result;
}

//版本2
template <class InputIterator , class OutputIterator , class BinaryOperation>
OutputIterator partial_sum(InputIterator first , InputIterator last , OutputIterator result ,  BinaryOperation binary_op) {
    if(first == last)return result;
    return __partial_sum(first , last , result , value_type(first) , binary_op);
}

template <class InputIterator , class OutputIterator , class T , class BinaryOperation>
OutputIterator __partial_sum(InputIterator first , InputIterator last , OutputIterator result , T* ,  BinaryOperation binary_op){
    T value = *first;
    while (first != last) {
        value = binary_op(value , *first);
        *++result = value;
    }
    return ++result;
}

/*----------------------------------power--------------------------------------*/

/*----------------------------------iota----------------------------------------*/
template <class ForwardIterator , class T>
void iota(ForwardIterator first , ForwardIterator last , T value) {
    while (first != last) {
        *first++ = value++;
    }
}
};
#endif // NUMERIC_H
