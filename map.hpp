    /**
     * implement a container like std::map
     */
    #ifndef SJTU_MAP_HPP
    #define SJTU_MAP_HPP

        // only for std::less<T>
    #include <functional>
    #include <cstddef>
    #include "utility.hpp"
    #include "exceptions.hpp"
using namespace std;
    namespace sjtu {
    
        template<
        class Key,
        class T,
        class Compare = std::less<Key>
        > class map {
            /**
             * the internal type of data.
             * it should have a default constructor, a copy constructor.
             * You can use sjtu::map as value_type by typedef.
             */
            struct AVL_node{
                pair<const Key, T> *node;
                AVL_node *leftson,*rightson;
                AVL_node *prev,*next;
                int height;//储存高度，用于计算平衡因子
                AVL_node(pair<const Key, T> *&Node):node(Node),leftson(nullptr),rightson(nullptr),prev(nullptr),next(nullptr),height(1){}
            AVL_node():node(nullptr),leftson(nullptr),rightson(nullptr),prev(nullptr),next(nullptr),height(1){}
            };
            Compare compare;//用来比较关键字大小 compare(k1,k2)<=>k1<k2
            int sum;//计算size
            AVL_node *root;
            //四个调整函数的实现
            int max(int a,int b){return (a>b)?a:b;}
            int height(AVL_node *node)
            {
                if(node==nullptr) return 0;
                return node ->height;
            }
            void makeEmpty(AVL_node *&t)
            {
                if(t==nullptr) return;
                if(t->leftson) makeEmpty(t->leftson);
                if(t->rightson) makeEmpty(t->rightson);
                delete t->node;
                delete t;
            }
            AVL_node* AVL_LL(AVL_node * & t)
            {
                AVL_node *t1=t->leftson;
                t->leftson=t1->rightson;
                t1->rightson=t;
                t->height=max(height(t->leftson),height(t->rightson))+1;
                t1->height=max(height(t1->leftson),height(t1->rightson))+1;
                t=t1;
                return t;
            }
            AVL_node * AVL_RR(AVL_node*  &t)
            {
                AVL_node *t1=t->rightson;
                t->rightson=t1->leftson;
                t1->leftson=t;
                t->height=max(height(t->leftson),height(t->rightson))+1;
                t1->height=max(height(t1->rightson),height(t1->leftson))+1;
                t=t1;
                return t;
            }
            AVL_node* AVL_LR(AVL_node *&t)
            {
                t->leftson=AVL_RR(t->leftson);
                t=AVL_LL(t);
                return t;
            }
            AVL_node* AVL_RL(AVL_node *&t)
            {
                t->rightson=AVL_LL(t->rightson);
                t=AVL_RR(t);
                return t;
            }
            AVL_node * AVL_Find(const Key &key,AVL_node *t) const
            {
                if(t==nullptr) return nullptr;
                if(compare(key,t->node->first))
                    return  AVL_Find(key, t->leftson);
                else if(compare(t->node->first,key))
                    return AVL_Find(key, t->rightson);
                else return t;
            }
            void AVL_find_prev_next(const Key &key,AVL_node *rt,AVL_node *&Prev,AVL_node *&Next)
            {
                if(rt==nullptr) return ;
                if(compare(key,rt->node->first)&&(Next==nullptr || compare(rt->node->first,Next->node->first)))
                    Next=rt;
                if(compare(rt->node->first,key)&&(Prev==nullptr || compare(Prev->node->first,rt->node->first)))
                    Prev=rt;
                if(compare(key,rt->node->first)) AVL_find_prev_next(key, rt->leftson, Prev, Next);
                else AVL_find_prev_next(key, rt->rightson, Prev, Next);
            }
            AVL_node * AVL_Insert(const pair<const Key,T> &X,AVL_node *&t)
            {
                AVL_node *op=nullptr;
                if(t==nullptr)
                {
                    AVL_node *Prev=nullptr,*Next=nullptr;
                    AVL_find_prev_next(X.first, root, Prev, Next);
                    t=new AVL_node;
                    t->node=new pair<const Key, T>(X.first,X.second);
                    t->prev=Prev;
                    t->next=Next;
                    if(Prev) Prev->next=t;
                    if(Next) Next->prev=t;
                    return t;
                }
               else
               {if(compare(X.first,t->node->first))
               {
                   op=AVL_Insert(X, t->leftson);
                   if(height(t->leftson)-height(t->rightson)==2)
                   {
                       if(compare(X.first,t->leftson->node->first))
                           t=AVL_LL(t);
                       else if(compare(t->leftson->node->first,X.first))
                           t=AVL_LR(t);
                   }
               }
                else
                {
                    if(compare(t->node->first,X.first))
                    {
                    op=AVL_Insert(X, t->rightson);
                    if(height(t->rightson)-height(t->leftson)==2)
                    {
                    if(compare(t->rightson->node->first,X.first))
                    {
                        t=AVL_RR(t);
                    }
                    else if(compare(X.first,t->rightson->node->first))
                        t=AVL_RL(t);
                }
                }
                }
               }
                t->height=max(height(t->leftson), height(t->rightson))+1;
                return op;
            }
            
            AVL_node  * Insert(const pair<const Key, T> &X)
            {
                AVL_node *Operate=AVL_Insert(X, root);
                return Operate;
            }
            void AVL_Remove(const pair<const Key, T> &X,AVL_node *& t)
            {
                if(t==nullptr) return ;
                if(compare(X.first,t->node->first))
                {
                    AVL_Remove(X, t->leftson);
                    if(height(t->rightson)-height(t->leftson)==2)
                    {
                        AVL_node *r=t->rightson;
                        if(height(r->leftson)>height(r->rightson))
                            t=AVL_RL(t);
                        else 
                            t=AVL_RR(t);
                    }
                }
                else
                {
                    if(compare(t->node->first,X.first))
                {
                    AVL_Remove(X, t->rightson);
                    if(height(t->leftson)-height(t->rightson)==2)
                    {
                        AVL_node *l=t->leftson;
                        if(height(l->rightson)>height(l->leftson))
                            t=AVL_LR(t);
                        else
                            t=AVL_LL(t);
                    }
                }
                else
                {
                    if(t->leftson&&t->rightson)
                    {
                        if(height(t->leftson)>height(t->rightson))
                        {
                            AVL_node *max=t->leftson;
                            while(max->rightson) max=max->rightson;
                            delete t->node;
                            t->node=new pair<const Key, T>(*max->node);
                            t->prev=max->prev;
                            if(t->prev) t->prev->next=t;
                            AVL_Remove(*t->node, t->leftson);
                        }
                        else{
                            AVL_node *min=t->rightson;
                            while(min->leftson) min=min->leftson;
                            delete t->node;
                            t->node=new pair<const Key, T>(*min->node);
                            t->next=min->next;
                            if(t->next) t->next->prev=t;
                            AVL_Remove(*t->node, t->rightson);
                        }
                    }
                    else{
                        if(!t->leftson&&!t->rightson)
                        {
                            AVL_node *Prev=nullptr,*Next=nullptr;
                            Key tnode(t->node->first);
                            delete t->node;
                            delete t;
                            t=nullptr;
                            if(AVL_Find(tnode, root)) return;
                            AVL_find_prev_next(tnode, root, Prev, Next);
                            if(Prev) Prev->next=Next;
                            if(Next) Next->prev=Prev;
                            return ;
                        }
                        else{
                            AVL_node *existson=nullptr;
                            if(t->leftson)
                            {
                                existson=t->leftson;
                                existson->next=t->next;
                                if(existson->next) existson->next->prev=existson;
                            }
                            else{
                                existson=t->rightson;
                                existson->prev=t->prev;
                                if(existson->prev) existson->prev->next=existson;
                            }
                            delete t->node;
                            t=existson;
                        }
                    }
                }
                }
                 t->height=max(height(t->leftson),height(t->rightson))+1;
                return ;
            }
            void Remove(const pair<const Key, T> &X )
            {
                AVL_Remove(X, root);
            }
            AVL_node *AVL_copy(AVL_node *rt,AVL_node *&rtmin,AVL_node *&rtmax)
            {
                AVL_node *leftmin=nullptr,*leftmax=nullptr,*rightmin=nullptr,*rightmax=nullptr;
                AVL_node *p;
                p=new AVL_node();
                p->node=new pair<const Key, T>(*(rt->node));
                p->height=rt->height;
                if(rt->leftson!=nullptr) p->leftson=AVL_copy(rt->leftson, leftmin, leftmax);
                if(rt->rightson!=nullptr) p->rightson=AVL_copy(rt->rightson, rightmin, rightmax);
                if(leftmin==nullptr) leftmin=p;
                if(rightmax==nullptr) rightmax=p;
                if(leftmax!=nullptr)
                {
                    p->prev=leftmax;
                    leftmax->next=p;
                }
                if(rightmin!=nullptr)
                {
                    p->next=rightmin;
                    rightmin->prev=p;
                }
                rtmin=leftmin;
                rtmax=rightmax;
                return p;
            }
            public:
            typedef pair<const Key, T> value_type;
            /**
             * see BidirectionalIterator at CppReference for help.
             *
             * if there is anything wrong throw invalid_iterator.
             *     like it = map.begin(); --it;
             *       or it = map.end(); ++end();
             */
            class const_iterator;
            class iterator
            {
            private:
                AVL_node *current_node;
                AVL_node *current_root;
            public:
                AVL_node *GetNode() const {return current_node;}
                AVL_node *GetRoot() const {return current_root;}
                iterator()
                {
                    current_node=nullptr;
                    current_root=nullptr;
                }
                iterator(AVL_node *rt,AVL_node *pp)
                {
                    current_root=rt;
                    current_node=pp;
                }
                iterator(const iterator &other)
                {
                    current_root=other.current_root;
                    current_node=other.current_node;
                }
                iterator &operator=(const iterator &other)
                {
                    current_node=other.current_node;
                    current_root=other.current_root;
                    return *this;
                }
                /**
                 * return a new iterator which pointer n-next elements
                 *   even if there are not enough elements, just return the answer.
                 * as well as operator-
                 */
                /**
                 * TODO iter++
                 */
                iterator operator++(int)
                {
                    //if(current_root==nullptr) throw invalid_iterator{};
                    if(current_node==nullptr) throw invalid_iterator{};
                    AVL_node *oldp=current_node;
                    current_node=current_node->next;
                    iterator *it=new iterator(current_root,oldp);
                    return *it;
                }
                /**
                 * TODO ++iter
                 */
                iterator & operator++()
                {
                    //if(current_root==nullptr) throw invalid_iterator{};
                    if(current_node==nullptr) throw invalid_iterator{};
                    current_node=current_node->next;
                    return *this;
                }
                /**
                 * TODO iter--
                 */
                iterator operator--(int)
                {
                    AVL_node *p=current_node;
                    if(current_node!=nullptr)
                    {
                        if(current_node->prev==nullptr) throw invalid_iterator{};
                        current_node=current_node->prev;
                    }
                    else{
                        if(current_root==nullptr) throw invalid_iterator{};
                        AVL_node *k=current_root;
                        while(k->rightson) k=k->rightson;
                        current_node=k;
                    }
                    return iterator(current_root,p);
                }
                /**
                 * TODO --iter
                 */
                iterator & operator--()
                {
                    if(current_node!=nullptr)
                    {
                        if(current_node->prev==nullptr) throw invalid_iterator{};
                        current_node=current_node->prev;
                    }
                    else {
                        if(current_root==nullptr) throw invalid_iterator{};
                        AVL_node *k=current_root;
                        while(k->rightson) k=k->rightson;
                        current_node=k;
                    }
                    return *this;
                }
                /**
                 * a operator to check whether two iterators are same (pointing to the same memory).
                 */
                value_type & operator*() const
                {
                    if(current_root==nullptr) throw invalid_iterator{};
                    if(current_node==nullptr) throw invalid_iterator{};
                    return *(current_node->node);
                }
                value_type* operator->() const noexcept
                {
                    return current_node->node;
                }
                bool operator==(const iterator &rhs) const
                {
                    return (current_root==rhs.current_root)&&(current_node==rhs.current_node);
                }
                bool operator==(const const_iterator &rhs) const
                {
                    return (current_root==rhs.GetRoot())&&(current_node==rhs.GetNode());
                }
                /**
                 * some other operator for iterator.
                 */
                bool operator!=(const iterator &rhs) const
                {
                    return !(*this==rhs);
                }
                bool operator!=(const const_iterator &rhs) const
                {
                    return !(*this==rhs);
                }
    
                /**
                 * for the support of it->first.
                 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
                 */
            };
            class const_iterator {
                    // it should has similar member method as iterator.
                    //  and it should be able to construct from an iterator.
            private:
                    // data members.
                AVL_node *current_root;
                AVL_node *current_node;
            public:
                AVL_node *GetRoot()const {return current_root;}
                AVL_node *GetNode()const{return current_node;}
                const_iterator():current_root(nullptr),current_node(nullptr){}
                const_iterator(AVL_node *M, AVL_node *p):current_root(M),current_node(p){}
                const_iterator(const const_iterator &other)
                {
                    current_root=other.current_root;
                    current_node=other.current_node;
                }
                const_iterator(const iterator &other)
                {
                    current_root=other.GetRoot();
                    current_node=other.GetNode();
                }
    
                    // And other methods in iterator.
                    // And other methods in iterator.
                    // And other methods in iterator.
                const_iterator operator++(int)
                {
                    if(current_node==nullptr) throw invalid_iterator{};
                    AVL_node *oldp=current_node;
                    current_node=current_node->next;
                    return const_iterator(current_root,oldp);
                }
                /**
                 * TODO ++iter
                 */
                const_iterator & operator++()
                {
                    if(current_node==nullptr) throw invalid_iterator{};
                    current_node=current_node->next;
                    return *this;
                }
                /**
                 * TODO iter--
                 */
                const_iterator operator--(int)
                {
                    AVL_node *p=current_node;
                    if(current_node!=nullptr)
                    {
                        if(current_node->prev==nullptr) throw invalid_iterator{};
                        current_node=current_node->prev;
                    }
                    else{
                        if(current_root==nullptr) throw invalid_iterator{};
                        AVL_node *k=current_root;
                        while(k->rightson) k=k->rightson;
                        current_node=k;
                    }
                    return const_iterator(current_root,p);
                }
                /**
                 * TODO --iter
                 */
                const_iterator & operator--()
                {
                    if(current_node!=nullptr)
                    {
                        if(current_node->prev==nullptr) throw invalid_iterator{};
                        current_node=current_node->prev;
                    }
                    else {
                        if(current_root==nullptr) throw invalid_iterator{};
                        AVL_node *k=current_root;
                        while(k->rightson) k=k->rightson;
                        current_node=k;
                    }
                    return *this;
                }
                /**
                 * a operator to check whether two iterators are same (pointing to the same memory).
                 */
                value_type & operator*() const
                {
                    if(current_root==nullptr) throw invalid_iterator{};
                    if(current_node==nullptr) throw invalid_iterator{};
                    return *(current_node->node);
                }
                value_type* operator->() const noexcept
                {
                    if(current_root==nullptr) throw invalid_iterator{};
                    if(current_node==nullptr) throw invalid_iterator{};
                    return current_node->node;
                }
                bool operator==(const iterator &rhs) const
                {
                    return (current_root==rhs.GetRoot())&&(current_node==rhs.GetNode());
                }
                bool operator==(const const_iterator &rhs) const
                {
                    return (current_root==rhs.current_root)&&(current_node==rhs.current_node);
                }
                /**
                 * some other operator for iterator.
                 */
                bool operator!=(const iterator &rhs) const
                {
                    return !(*this==rhs);
                }
                bool operator!=(const const_iterator &rhs) const
                {
                    return !(*this==rhs);
                }
            };
            /**
             * TODO two constructors
             */
            map():sum(0),root(nullptr) {}
            map(const map &other)
            {
                sum=other.sum;
                AVL_node *rtmax=nullptr,*rtmin=nullptr;
                if(other.root==nullptr) root=nullptr;
                else{
                    root=AVL_copy(other.root, rtmin,rtmax);
                }
            }
            /**
             * TODO assignment operator
             */
            map & operator=(const map &other)
            {
                if(this==&other) return *this;
                if(root!=nullptr) makeEmpty(root);
                sum=other.sum;
                AVL_node *rtmax=nullptr,*rtmin=nullptr;
                if(other.root!=nullptr) root=AVL_copy(other.root, rtmin, rtmax);
                else root=nullptr;
                return *this;
            }
            /**
             * TODO Destructors
             */
            ~map()
            {
                if(root) makeEmpty(root);
            }
            /**
             * TODO
             * access specified element with bounds checking
             * Returns a reference to the mapped value of the element with key equivalent to key.
             * If no such element exists, an exception of type `index_out_of_bound'
             */
            T & at(const Key &key)
            {
                AVL_node *p=AVL_Find(key, root);
                if(p==nullptr) throw index_out_of_bound{};
                return p->node->second;
            }
            const T & at(const Key &key) const
            {
                AVL_node *p=AVL_Find(key, root);
                if(p==nullptr) throw index_out_of_bound{};
                return p->node->second;
            }
            /**
             * TODO
             * access specified element
             * Returns a reference to the value that is mapped to a key equivalent to key,
             *   performing an insertion if such key does not already exist.
             */
            T & operator[](const Key &key)
            {
                AVL_node *p=AVL_Find(key, root);
                if(p==nullptr)
                {
                    pair<const Key, T> newnode(key,T());
                    p=Insert(newnode);
                    sum++;
                }
                return p->node->second;
            }
            /**
             * behave like at() throw index_out_of_bound if such key does not exist.
             */
            const T & operator[](const Key &key) const
            {
                AVL_node *p=AVL_Find(key, root);
                if(p==nullptr) throw index_out_of_bound{};
                return p->node->second;
            }
            /**
             * return a iterator to the beginning
             */
            iterator begin()
            {
                if(root==nullptr) return iterator(root,nullptr);
                AVL_node *p=root;
                while(p->leftson) p=p->leftson;
                iterator *it=new iterator(root,p);
                return *it;
            }
            const_iterator cbegin() const
            {
                if(root==nullptr) return const_iterator(root,nullptr);
                AVL_node *p=root;
                while(p->leftson) p=p->leftson;
                return const_iterator(root,p);
            }
            /**
             * return a iterator to the end
             * in fact, it returns past-the-end.
             */
            iterator end()
            {
                return iterator(root,nullptr);
            }
            const_iterator cend() const
            {
                return const_iterator(root,nullptr);
            }
            /**
             * checks whether the container is empty
             * return true if empty, otherwise false.
             */
            bool empty() const {return sum==0;}
            /**
             * returns the number of elements.
             */
            size_t size() const {return sum;}
            /**
             * clears the contents
             */
            void clear()
            {
                if(root) makeEmpty(root);
                root=nullptr;sum=0;
            }
            /**
             * insert an element.
             * return a pair, the first of the pair is
             *   the iterator to the new element (or the element that prevented the insertion),
             *   the second one is true if insert successfully, or false.
             */
            pair<iterator, bool> insert(const value_type &value)
            {
                AVL_node *p=AVL_Find(value.first, root);
                if(p!=nullptr) return pair<iterator,bool>(iterator(root,p),0);
                p=Insert(value);
                sum++;
                return pair<iterator, bool>(iterator(root,p),1);
            }
            /**
             * erase the element at pos.
             *
             * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
             */
            void erase(iterator pos)
            {
                AVL_node *p_root=pos.GetRoot();
                AVL_node *p=pos.GetNode();
                if(p_root==nullptr || p==nullptr)
                    throw invalid_iterator{};
                if(root!=p_root) throw invalid_iterator{};
                pair<const Key, T> *KT=new pair<const Key, T>(p->node->first,p->node->second);
                Remove(*KT);
                delete KT;
                sum--;
            }
            /**
             * Returns the number of elements with key
             *   that compares equivalent to the specified argument,
             *   which is either 1 or 0
             *     since this container does not allow duplicates.
             * The default method of check the equivalence is !(a < b || b > a)
             */
            size_t count(const Key &key) const
            {
                AVL_node *p=AVL_Find(key, root);
                if(p==nullptr) return 0;
                else return 1;
            }
            /**
             * Finds an element with key equivalent to key.
             * key value of the element to search for.
             * Iterator to an element with key equivalent to key.
             *   If no such element is found, past-the-end (see end()) iterator is returned.
             */
            iterator find(const Key &key)
            {
                if(root==nullptr) return iterator(nullptr,nullptr);
                AVL_node *p=AVL_Find(key, root);
                return iterator(root,p);
            }
            const_iterator find(const Key &key) const
            {
                if(root==nullptr) return const_iterator(nullptr,nullptr);
                AVL_node *p=AVL_Find(key, root);
                return const_iterator(root,p);
            }
            
        };
    
    }

    #endif
