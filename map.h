#ifndef MAP_H
#define MAP_H
#include"rb_tree.h"
namespace mystl {
template <class Key , class T , class Compare = less<Key> , class Alloc = alloc>
class map{
public:
    //typedefs:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key , T> value_type;
    typedef Compare key_compare;

    class  value_compare : public binary_function<value_type , value_type , bool>{
        friend class map<Key , T , Compare , Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c){}
    public:
        bool operator()(const value_type& x , const value_type& y) const {return comp(x , y);}
    };

private:
    typedef rb_tree<key_type , value_type , select1st<value_type> , key_compare , Alloc > rep_type;
    rep_type t;

public:
    //typedefs:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    //allocation/deallocation
    map() : t(Compare()){}
    explicit map(const Compare& comp) : t(comp){}
    template<class InputIterator>
    map(InputIterator first , InputIterator last) : t(Compare()){t.insert_unique(first , last);}
    template<class InputIterator>
    map(InputIterator first , InputIterator last , Compare& comp) : t(comp){t.insert_unique(first , last);}
    map(const map<Key , T , Compare , Alloc>& x) : t(x.t){}
    map<Key , T , Compare , Alloc>& operator = (const map<Key , T , Compare , Alloc>& x){
        t = x.t;
        return *this;
    }

    //accessors
    key_compare key_comp()const{return t.key_comp();}
    value_compare value_comp()const{return value_compare(t.value_comp());}
    iterator begin(){return t.begin();}
    const_iterator begin() const{return t.begin();}
    iterator end(){return t.end();}
    const_iterator end()const{return t.end();}
    reverse_iterator rbegin(){return t.rebgin();}
    const_reverse_iterator rbegin()const{return t.rbegin();}
    reverse_iterator rend(){return t.rend();}
    const_reverse_iterator rend()const {return t.rend();}
    bool empty() const {return t.empty();}
    size_type size() const {return t.size();}
    size_type max_size() const {return t.max_size();}
    T& operator [] (const key_type& k){
        return (*((inserte(value_type(k , T()))).first)).second;
    }
    void swap(const map<Key , T , Compare , Alloc>& x){t.swap(x.t);}


    //insert/erase
    pair<iterator , bool> insert(const value_type& x){
        return t.insert_unique(x);
    }

    iterator insert(iterator position , const value_type& x){
        return t.insert_unique(position , x);
    }

    template<class InputIterator>
    void insert(InputIterator first , InputIterator last){
        t.insert_unique(first , last);
    }

    void erase(iterator position){t.erase(position);}
    size_type erase(const key_type& x){return t.erase(x);}
    void erase(iterator first , iterator last){t.erase(first , last);}
    void clear(){t.clear();}

    //map operations
    iterator find(const key_type& x){return t.find(x);}
    //const_iterator(const key_type& x) const{return t.find(x);}
    size_type count(const key_type& x)const{t.count(x);}
    iterator lowerbound(const key_type& x){return t.lowerbound(x);}
    const_iterator lowerbound(const key_type& x)const{return t.lowerbound(x);}
    iterator upperbound(const key_type& x){return t.upperbound(x);}
    const_iterator upperbound(const key_type& x)const{return t.upperbound(x);}

    pair<iterator , iterator> equal_range(const key_type& x){return t.equal_range(x);}
    pair<const_iterator , const_iterator> equal_range(const key_type& x)const{return t.equal_range(x);}

    friend bool operator == (const map& , const map&);
    friend bool operator < (const map& , const map&);
};
template <class Key , class T , class Compare , class Alloc >
inline bool operator == (const map<Key , T , Compare , Alloc>& x , const map<Key , T , Compare , Alloc>& y){return x.t == y.t;}

template <class Key , class T , class Compare , class Alloc >
inline bool operator < (const map<Key , T , Compare , Alloc>& x , const map<Key , T , Compare , Alloc>& y){return x.t < y.t;}
}
#endif // MAP_H
