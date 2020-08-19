#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include<cstddef>
#include<iostream>
#include<new>
#include<stdlib.h>
#include<limits.h>
using namespace std;
namespace mystl {

template <class T>
inline T* allocate(ptrdiff_t size,T*){
    T* tmp=(T*)(::operator new((size_t)(size*sizeof(T))));
    if(tmp==0){
        std::cerr<<"out of memory"<<std::endl;
        exit(1);
    }
    return tmp;
}


template <class T>
inline void deallocate(T* buffer){
    ::operator delete(buffer);
}



//空间置配器
//模板类：allocator
template  <class T>
class allocator
{
public:
    typedef T  value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

public:
    pointer allocate(size_type n){
        return allocate((difference_type)n,(pointer)0);
    }

    void deallocate(pointer p){
        deallocate(p);
    }

    pointer address(reference x){return (pointer)x;}

    const_pointer const_address(const_reference x){return (const_pointer)x;}

    size_type init_page_size()
    {
        return max(size_type(1) , size_type(4096 / sizeof(T)));
    }

    size_type max_size( ) const
    {
        return max(size_type(1) , size_type(UINT_MAX / sizeof(T)));
    }
};
}//namespace mystl
/*
class allocator
{
   public:
    typedef void* pointer;
};
*/
#endif // ALLOCATOR_H
