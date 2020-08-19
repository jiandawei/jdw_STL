#ifndef LIST_H
#define LIST_H
#include"alloc.h"
#include"construct.h"

namespace mystl {
template <class T>
//list节点
struct __list_node
{
    typedef __list_node<T>* node_pointer;
    node_pointer prev;
    node_pointer next;
    T data;
};
//list的迭代器
template <class T, class Ref , class Ptr>
struct __list_iterator
{
    typedef __list_iterator<T,T&,T*> iterator;
    typedef __list_iterator<T,T&,T*> self;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;

    //constructor
    __list_iterator(link_type x):node(x){}
    __list_iterator(){}
    __list_iterator(const iterator& x):node(x.node){}


    //操作符重载
    bool operator == (const self& x){return node == x.node;}
    bool operator != (const self& x){return node != x.node;}
    reference operator *(){return node->data;}
    pointer operator -> (){return &(operator*());}
    self& operator ++()
    {
        node = (link_type)(node->next);
        return *this;
    }
    self& operator ++(int)
    {
        self tmp= node;
        node = (link_type)(node->next);
        return tmp;
    }
    self& operator --()
    {
        node = (link_type)(node->prev);
        return *this;
    }
    self& operator--(int)
    {
        self tmp = node;
        node = (link_type)(node->prev);
        return tmp;
    }
};

template <class T , class Alloc >
class list
{
protected:
    typedef __list_node<T> list_node;
    typedef list_node* link_type;
    typedef simple_alloc<list_node , Alloc> list_node_allocator;
    typedef __list_iterator<T,T&,T*> iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T& reference;
    typedef ptrdiff_t difference_type;

protected:
    //以下是关于配置、释放、构造、销毁一个节点
    //配置
    link_type get_node(){return list_node_allocator::allocte();}
    //释放(获得空间)
    void put_node(link_type p){list_node_allocator::deallocate(p);}
    //构造
    link_type creat_node(const T& x)
    {
        link_type p = get_node();
        construct(&(p->data),x);
        return p;
    }
    //销毁
    void destory_node(link_type p)
    {
        destory_node(&(p->data));
        put_node(p);
    }
protected:
    link_type node;//指向list的一个指针
public:
    iterator begin(){return (link_type)(node->prev);}
    iterator end(){return node;}
    bool empty(){return node->next == node;}
    size_type size()const
    {
        size_type result = 0;
        distance(begin() , end() , result);
        return result;
    }
    reference front(){return *begin();}
    reference back(){return *(--end());}
protected:
    void empty_initialize()
    {
        node=get_node();
        node->next=node;
        node->prev=node;
    }
public:
    list(){empty_initialize();}
public:
    //元素操作

    //在position位置插入一个节点
    iterator insert(iterator position , const T&x)
    {
        link_type tmp = creat_node(x);
        tmp->next = position.node;
        tmp->prev = (position.node)->prev;
        (link_type(position.node->prev))->next = tmp;
        (position.node)->prev = tmp;
        return tmp;
    }
    //删除position所指节点
    iterator erase(iterator position)
    {
        link_type next_node = (position.node)->next;
        link_type prev_node = (position.node)->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(position.node);
        return iterator(next_node);
    }
    void push_front(const T& x){insert(begin() , x);}
    void push_back(const T& x){insert(end() , x);}
    void pop_front(){erase(begin());}
    void pop_back(){iterator tmp = end(); erease(--tmp);}
    //删除整个list
    void clear()
    {
        link_type cur = (link_type)begin();
        while(cur != node)
        {
            link_type tmp = cur;
            cur = cur->next;
            destory_node(tmp);
        }
        node->next = node;
        node->prev =node;
    }
    //将所有数值为value的节点删除
    void remove(const T& value)
    {
        iterator cur = begin();
        iterator last = end();
        while(cur != last)
        {
            iterator tmp = cur++;
            if(*tmp == value)erase(tmp);//找到就删除
        }
    }
    //连续相同的元素只留下一个
    void unique()
    {
        iterator cur = begin();
        iterator last = end();
        if(cur == last)return;
        iterator next = cur;
        while(++next != last)
        {
            if(*cur == *next)
                erase(next);
            else
                cur = next;

            next = cur;
        }
    }
    //将[first,last)内的所有元素移到position之前
    void transfer(iterator position , iterator first , iterator last)
    {
        if(position == last)return ;
        (last.node)->prev->next = position.node;
        (first.node)->prev->next = last.node;
        (position.node)->prev->next = first.node;

        iterator tmp = position.node->prev;

        (position.node)->prev = (last.node)->prev;
        (last.node)->prev = (first.node)->prev;
        (first.node)->prev = tmp;
    }

    //将list x接入到position之前(x必须不是*this)
    void splice(iterator position, list& x)
    {
        if(!x.empty())
            transfer(position , x.begin() , x.end());
    }
    //将i所指节点接入到position之前
    void splice(iterator position , list& , iterator i)
    {
        iterator j = i;
        ++j;
        if(position == i || position == j)return;
        transfer(position , i ,j);
    }
    //将[first,last)内的所有元素移到position之前
    //position和[first,last)可指向同一个list
    //但是position不能在[first,last)之间
    void splice(iterator position , list& , iterator first , iterator last)
    {
        if(first != last)//非空
            transfer(position , first , last);
    }
    //将x合并到*this。两个list必须经过递增排序
    void merge(list& x)
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while(first1 != last1 && first2 != last2)
        {
            if(*first2 < *first1)
            {
                iterator next = first2;
                ++next;
                transfer(first1 , first2 , next);
                first2 = next;
            }
            else
                ++first1;
        }

        if(first2 != last2)
            transfer(last1 , first2 , last2);
    }
    //将list倒置
    void reverse()
    {
        if(node->next == node || node->next->next == node)
            return ;
        iterator first = begin();++first;
        iterator last = end();
        while(first != last)
        {
            iterator old = first;
            ++first;
            transfer(begin() , old, first);
        }
    }
    //排序
    void sort()
    {
        if(node->next == node || node->next->next ==node)
            return ;
        list carry;
        list counter[64];
        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin() , *this , begin());
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if(i == fill)
                ++fill;
        }

        for(int i =1 ; i < fill ;++i)
            counter[i].merge(counter[i-1]);
        swap(counter[fill - 1]);
    }

};

}//namespace mystl
#endif // LIST_H
