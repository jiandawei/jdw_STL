#ifndef RB_TREE_H
#define RB_TREE_H
#include<iostream>
#include"iterator.h"
#include"functional.h"
#include"alloc.h"
namespace mystl {
//红黑树节点颜色
typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;//红色 false
const __rb_tree_color_type __rb_tree_black = true;//黑色 true

//基层节点
struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef  __rb_tree_node_base* base_ptr;

    color_type color;//节点颜色
    base_ptr parent;//节点的父节点
    base_ptr left;//左孩子
    base_ptr right;//右孩子

    static base_ptr minimum(base_ptr x) {//一直往左走，找到最小值
        while (x->left != 0) {
            x = x->left;
        }
        return x;
    }

    static base_ptr maximum(base_ptr x) {//一直往右走，找到最大值
        while (x->right != 0) {
            x = x->right;
        }
        return x;
    }
};

//基层迭代器
struct __rb_tree_node_base_iterator {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef  bidirectional_iterator_tag iterator_category;//双向迭代器，但不能随机定位
    typedef ptrdiff_t difference_type;

    base_ptr node;

    //迭代器operator++就调用这个函数
    //迭代器前进
    void increment() {
        if(node->right != 0) {
            node = node->right;
            while (node->left != 0) {
                node = node->left;
            }
        }
        else{
            base_ptr y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if(node->right != y)
                node = y;
        }
    }

    //迭代器operator--就调用这个函数
    //迭代器后退
    void decrement() {
        //如果node为红色，且其父节点的父节点是自己
        if(node->color == __rb_tree_red && node->parent->parent == node)
            node = node->right;
        else if(node->left != 0){
            base_ptr y = node->left;
            while (y->right != 0)
                y = y->right;
            node = y;
        }
        else{
            base_ptr y = node->parent;
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

//红黑树节点
template <class Value>
struct __rb_tree_node : public __rb_tree_node_base {
    typedef __rb_tree_node<Value>* link_type;
    Value value_field;//节点值
};

//红黑树迭代器
template <class Value , class Ref , class Ptr>
struct __rb_tree_iterator : public __rb_tree_node_base_iterator {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value , Value& , Value*> iterator;
    typedef __rb_tree_iterator<Value , const Value& , const Value*> const_iterator;
    typedef __rb_tree_iterator<Value , Ref , Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    reference operator* () {return link_type(node)->value_field;}
    pointer operator->() {return &(operator*());}
    self& operator++() {increment(); return *this;}
    self operator++(int) {self tmp = *this ; increment(); return tmp;}
    self& operator--() {decrement(); return *this;}
    self operator--(int) {self tmp = *this ; decrement(); return tmp;}
};


template <class Key , class Value , class KeyOfValue , class Compare , class Alloc = alloc>
class rb_tree {
protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef simple_alloc<rb_tree_node , Alloc> rb_tree_node_allocator;
    typedef __rb_tree_color_type color_type;

protected:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    link_type get_node() {return rb_tree_node_allocator::allocate();}

    void put_node(link_type p){rb_tree_node_allocator::deallocate(p);}

    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        construct(&tmp->value_field , x);
        return tmp;
    }

    link_type clone_node(link_type x){
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = tmp->right = 0;
        return tmp;
    }

    void destory_node(link_type p){
        destory(&p->value_field);
        put_node(p);
    }

protected:
    //红黑树只以以下三笔数据体现
    size_type node_count;
    link_type header;
    Compare key_compare;

    //以下3个函数方便获得header的成员
    link_type& root() const {return (link_type&)header->parent;}
    link_type& leftmost() const {return (link_type&)header->left;}
    link_type& rightmost() const {return (link_type&)header->right;}

    //以下6个函数方便获得节点x的成员
    static link_type& left(link_type x)
    {return (link_type&)(x->left);}
    static link_type& right(link_type x)
    {return (link_type&)(x->right);}
    static link_type& parent(link_type x)
    {return (link_type&)(x->parent);}
    static reference value(link_type x)
    {return x->value_field;}
    static const Key& key(link_type x)
    {return KeyOfValue()(value(x));}
    static color_type& color(link_type x)
    {return (color_type&)(x->color);}

    //以下6个函数方便获得节点x的成员
    static link_type& left(base_ptr x)
    {return (link_type&)(x->left);}
    static link_type& right(base_ptr x)
    {return (link_type&)(x->right);}
    static link_type& parent(base_ptr x)
    {return (link_type&)(x->parent);}
    static reference value(base_ptr x)
    {return ((link_type)x)->value_field;}
    static const Key& key(base_ptr x)
    {return KeyOfValue()(value(x));}
    static color_type& color(base_ptr x)
    {return (color_type&)(link_type(x)->color);}

    static link_type minimum(link_type x){
        return (link_type) __rb_tree_node_base::minimum(x);
    }

    static link_type maximum(link_type x){
        return (link_type) __rb_tree_node_base::maximum(x);
    }

public:
    typedef __rb_tree_iterator<value_type , reference , pointer> iterator;
private:
    inline void __rb_tree_rotate_left(__rb_tree_node_base*  x , __rb_tree_node_base*& root){
        __rb_tree_node_base* y = x->right;
        x->right = y->left;
        if(y->left != 0)
            y->left->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        x->left = x;
        x->parent = y;
    }
    inline void __rb_tree_rotate_right(__rb_tree_node_base*  x , __rb_tree_node_base*& root){
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if(y->right != 0)
            y->right->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if(x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        x->right = x;
        x->parent = y;
    }
    inline void __rb_tree_insert_rebalance(__rb_tree_node_base* x , __rb_tree_node_base*& root){
        x->color = __rb_tree_red;
        while (x != root && x->parent->color == __rb_tree_red) {
            if(x->parent == x->parent->parent->left){
                __rb_tree_node_base* y = x->parent->parent->right;
                if(y && y->color == __rb_tree_red){
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    if( x == x->parent->right){
                        x = x->parent;
                        __rb_tree_rotate_left(x , root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent , root);
                }
            }
            else {
                __rb_tree_node_base* y = x->parent->parent->left;
                if(y && y->color == __rb_tree_red){
                    x->parent->parent->color = __rb_tree_red;
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x = x->parent->parent;
                }
                else {
                    if(x == x->parent->left){
                        x = x->parent;
                        __rb_tree_rotate_right(x , root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent , root);
                }
            }
        }
        root->color = __rb_tree_black;
    }
    inline void __rb_tree_erase_rebalance(__rb_tree_node_base* x , __rb_tree_node_base*& root){

    }
    iterator __insert(base_ptr x_ , base_ptr y_ , const value_type& v){
        link_type x = (link_type )x_;
        link_type y = (link_type)y_;
        link_type z;

        if(y == header || x != 0 || key_compare(KeyOfValue()(v) , key(y))){
            z = create_node(v);
            left(y) = z;
            if(y == header){
                root() = z;
                rightmost() = z;
            }
            else {
                z = create_node(v);
                right(y) = z;
                if(y == rightmost())
                    rightmost() = z;
            }
            parent(z) = y;
            left(z) = 0;
            right(z) = 0;

            __rb_tree_insert_rebalance(z , header->parent);
            ++node_count;
            return iterator(z);
        }
    }
    link_type __copy(link_type  x , link_type p);
    void __erase(link_type x);
    void init(){
        header = get_node();
        color(header) = __rb_tree_red;

        root() = 0;
        leftmost() = header;
        rightmost() = header;
    }
public:
    rb_tree(const Compare& comp = Compare()) : node_count(0) , key_compare(comp) {init();}
    ~rb_tree(){
        //clear();
        put_node(header);
    }
    rb_tree<Key , Value , Compare , Alloc>& operator = (const rb_tree<Key , Value , Compare , Alloc>& x);

public:
    Compare key_comp() const {return key_compare;}
    iterator begin(){return leftmost();}
    iterator end(){return header;}
    bool empty() const {return node_count == 0;}
    size_type size() const {return node_count;}
    size_type max_size()const {return size_type(-1);}

public:
    pair<iterator , bool> insert_unique(const value_type& v){
        link_type y =header;
        link_type x = root();
        bool comp = true;
        while (x != 0) {
            y = x;
            comp = key_compare(KeyOfValue()(v) , key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j = iterator(y);
        if(comp){
            if(j == begin())
                return pair<iterator , bool>(__insert(x , y , v) , true);
        }
        else
                --j;
        if(key_compare(key(j.node) , KeyOfValue()(v) ))
            return pair<iterator , bool>(__insert(x , y ,v) , true);

        return pair<iterator , bool>(j , false);
    }
    iterator insert_equal(const value_type& v){
        link_type y = header;
        link_type x = root();
        while (x != 0) {
            y = x;
            x = key_compare(KeyOfValue()(v) , key(x)) ? left(x) : right(x);
        }
        return __insert(x , y ,v);
    }

};
}//namespace mystl
#endif // RB_TREE_H
