#ifndef DEQUE_H
#define DEQUE_H
#include"alloc.h"
#include"construct.h"
namespace mystl {
//deque的迭代器
template <class T , class Ref , class Ptr , size_t BufSize>
struct __deque_iterator
{
    inline size_t __deque_buf_size(size_t n , size_t sz)
    {
        return n!=0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
    }
    typedef __deque_iterator<T,T&,T*,BufSize> iterator;
    typedef __deque_iterator<T,const T&,T*,BufSize> const_iterator;
    static size_t buff_size(){return __deque_buf_size(BufSize , sizeof (T));}

    ?? typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    //重点
    typedef   T** map_pointer;
    //指向本身
    typedef __deque_iterator self;

    T* cur;//迭代器所指的缓冲区的现行元素
    T* first;//迭代器所指的缓冲区的头
    T* last;//迭代器所指的缓冲区的尾
    map_pointer node;//管理中心

    //迭代器的一些操作
    //跳过一个缓冲区
    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buff_size());
    }

    //以下都是运算符重载
    reference operator*()const{return *cur;}
    pointer operator->()const{return &(operator*());}
    difference_type operator-(const self& x)const{return difference_type(buff_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);}
    self& operator++()
    {
        ++cur;
        if(cur==last){//如果到达缓冲区的最后时
            set_node(node+1);
            cur=first;
        }
        return *this;
    }
    self& operator++(int)
    {
        self tmp=*this;
        ++*this;
        return tmp;
    }
    self& operator--()
    {
        if(cur==first)//如果cur为缓冲区第一个
        {
            set_node(node-1);
            cur=last;
        }
        --cur;
        return *this;
    }
    self& operator--(int)
    {
        self tmp=*this;
        --*this;
        return tmp;
    }
    self& operator+=(difference_type n)
    {
        difference_type offset=n+(cur-first);
        if(offset>=0&&offset<buff_size())//如果目标位置在同一个缓冲区内
            cur+=n;
        else
        {
            //计算目标缓冲区与现在缓冲区的相对位置
            difference_type node_offset = offset>0 ? offset / difference_type(buff_size()) : -difference_type((-offset-1) / buff_size()) -1;
            //切换到争正确的位置
            set_node(node + node_offset);


            cur=first+(offset-node_offset*difference_type(buff_size()));
        }
        return *this;
    }
    self& operator+(difference_type n)const
    {
        self tmp=*this;//要用临时变量，因+不能直接运算
        return tmp+=n;
    }
    self& operator-=(difference_type n){return *this += -n;}
    reference operator[](difference_type n)const{return *(*this+n);}
    bool operator==(const self& x)const{return cur==x.cur;}
    bool operator!=(const self& x)const{return !(*this==x);}
    bool operator<(const self& x)const{return (node==x.node)?(cur<x.cur):(node<x.node);}
};

template <class T,class Alloc=alloc,size_t BufSize=0>
class deque
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
public://迭代器
    typedef __deque_iterator<T,T&,T*,BufSize> iterator;
protected://指针的指针
    typedef pointer* map_pointer;
protected:
    iterator start;//第一个节点
    iterator finish;//最后一个节点
    map_pointer map;//指向一个装指针的数组，指针指缓冲区
    size_type map_size;//map内有多少指针

public:
    iterator begin(){return start;}
    iterator end(){return finish;}

    reference operator[](size_type n)
    {
        //调用迭代器的[]
        return start[difference_type(n)];
    }

    reference front(){return *start;}
    reference back()
    {
        return *(finish-1);
        //原版代码
        /*iterator tmp=*this;
        --tmp;
        return *tmp;*/
    }
    size_type size() const{return finish - start;}
    size_type max_size()const{return size_type(-1);}//??
    bool empty()const {return finish==start;}
};
}//namespace mystl
#endif // DEQUE_H
