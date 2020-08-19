/*
 * 本文件包括以下内容
 *iterator
*/

#ifndef ITERATOR_H
#define ITERATOR_H

#include<cstddef>

namespace mystl {
//五个迭代器类型标签
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};
//迭代器的类别
//Input Iterator：只读
//Output Iterator：只写
//Fordward Iteratror:允许“写入型”算法对该类迭代器所形成区间进行读写操作
//Bidirectional Iterator：可双向移动
//Ramdom Acess Iterator：覆盖指针的所有算术能力

template <class Category , class T , class Distance = ptrdiff_t , class Pointer = T* , class Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

}//namespace mystl
#endif // ITERATOR_H
