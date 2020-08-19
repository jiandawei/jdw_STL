/*本文件包括以下内容
 *iterator_traits
 * __type_traits
*/
#ifndef TRAITS_H
#define TRAITS_H

#include"iterator.h"

namespace mystl {//要用namespace，否则会与原来的命名冲突


//特性萃取机
template <class Iterator>
struct iterator_traits
{

    typedef typename Iterator::itearator_category iterator_category;

    //迭代器所指对象的型别
    typedef typename Iterator::value_type value_type;

    //用来表示两个迭代器的距离
    typedef typename Iterator::difference_type difference_type;

    //一个pointer：指向迭代器所指之对象
    typedef typename Iterator::pointer pointer;

    //一个引用：引用迭代器所指之对象
    typedef typename Iterator::reference reference;
};

//以下都是偏特化版本


//针对原生指针的版本
template <class T>
struct iterator_traits<T*>
{
    //迭代器的类别
    typedef random_access_iterator_tag iterator_category;

    //迭代器所指对象的型别
    typedef  T value_type;

    //用来表示两个迭代器的距离
    typedef ptrdiff_t difference_type;

    //一个pointer：指向迭代器所指之对象
    typedef T* pointer;

    //一个引用：引用迭代器所指之对象
    typedef T& reference;
};


//针对pointer-to-const的版本
template <class T>
struct iterator_traits<const T*>
{
    //迭代器的类别
    typedef random_access_iterator_tag iterator_category;

    //迭代器所指对象的型别
    typedef  T value_type;

    //用来表示两个迭代器的距离
    typedef ptrdiff_t difference_type;

    //一个pointer：指向迭代器所指之对象
    typedef const T* pointer;

    //一个引用：引用迭代器所指之对象
    typedef const T& reference;
};




//以下是对Iterator操作的一些函数

//返回某个迭代器的category
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}


//返回某个迭代器的value_type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    //强制转换
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
  return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//以下是整组distance函数

//random access iterator 版本
template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first , RandomAccessIterator last , random_access_iterator_tag)
{
    return last - first;
}

//input iterator 版本
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first , InputIterator last , input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while(first != last)
    {
        ++first;
        ++n;
    }
    return n;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first , InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    __distance(first , last , category());
}


//以下是整组advance函数
//注意迭代都是通过引用传入

//input Iterator 版本
template <class InputIterator , class Distance>
inline void __advance(InputIterator& i , Distance n , input_iterator_tag)
{
    while(n--)
    {
        ++i;
    }
}

//bidirectional Iterator 版本
template <class Bidirectional , class Distance>
inline void __advance(Bidirectional& i , Distance n , bidirectional_iterator_tag)
{
    if(n>0)
        while(n--)++i;
    else
        while(n++)--i;
}

//random access iterator 版本
template <class RandomAccessIterator , class Distance>
inline void __advance(RandomAccessIterator& i, Distance n , random_access_iterator_tag)
{
    i+=n;
}

template <class InputIterator , class Distance>
inline void advance(InputIterator& i , Distance n)
{
    __advance(i,n,iterator_category(i));
}











//type_traits
//型别特性萃取
//此处我们关注的型别特性是指：这个型别是否有non-trivial default ctor、non-trivial copy ctor、non-trivial  assignment operator、non-trivial dtor
//如果没有，我们在对这些型别构造、析构、拷贝、赋值等操作时，采用更有效的措施，不去调用那些constructor、destructor，而直接才有内存直接处理操作如malloc、memcpy等等以提高效率
struct __true_type{};
struct __false_type{};

template <class type>
struct __type_traits
{
    //通知有能力自动将__type_traits特化的编译器，我们现在这个__type_traits是特殊的。
    //避免编译器也使用一个名为__type_traits的template而导致的错误
    typedef __true_type this_dummy_member_must_be_first;

    //这里给出最保守的值，全都置为__false_type
    //然后再给出特化版本
    typedef __false_type has_trivial_default_constructor;
    typedef __false_type has_trivial_copy_constructor;
    typedef __false_type has_trivial_assign_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};


//以下是__type_traits的特化

template<>
struct __type_traits<char>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<signed char>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned char>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<short>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned short>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<int>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned int>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<unsigned long>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<double>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<long double>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template<>
struct __type_traits<float>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

//以下是针对原生指针的特化版本
template<class T>
struct __type_traits<T*>
{
    typedef __true_type has_trivial_default_constructor;
    typedef __true_type has_trivial_copy_constructor;
    typedef __true_type has_trivial_assign_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

}//namespace mystl
#endif // TRAITS_H
