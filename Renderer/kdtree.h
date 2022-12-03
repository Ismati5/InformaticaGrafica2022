#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <list>

#include "camera.hpp"
#include "light.hpp"
#include "matrix4.hpp"
#include "vect3.hpp"

using namespace std;

namespace nn {
    
namespace {
    class RandomAccess {
    public:
        template<typename T>
        constexpr auto operator()(const T& t, size_t i) const { return t[i]; } 
    };
    class RandomAccessTupleFirst {
    public:
        template<typename T>
        constexpr auto operator()(const T& t, size_t i) const { return get<0>(t)[i]; } 
    };
    template <typename> struct is_tuple: false_type {};
    template <typename ...T> struct is_tuple<tuple<T...>>: true_type {};
};
    
/**
 * T - Data type contained in the KD Tree
 * N - Number of dimensions of the KD Tree (1 == binary tree)
 * A - Axis function to the position
            A axis_position; T t;
            axis_position(t,d) returns the numerical position for the d dimension. If possible it is deduced automatically for random access vectors and the like.
**/
template<typename T, size_t N, typename A>
class KDTree {
public:
    using real = decltype(declval<A>()(declval<T>(),size_t(0)));
    static constexpr size_t dimensions = N;
   
private:
    A axis_position;
    using axis_type = size_t; //The axis type is size_t but we might want to reduce its size in compile time when N<256...
    //elements and nodes are sorted equally so the element pointed by the ith node in node is the ith element in elements 
    vector<axis_type> nodes;
    vector<T> elements;
    //Given a node / element in position $i$, its left child is in position 2i+1 and its right child 2i+2

    array<real,N>& assign(array<real,N>& a, const T& t) const {
        for (size_t i = 0; i<N; ++i) a[i]=axis_position(t,i);
        return a;
    }
    
    array<real,N>& if_less_assign(array<real,N>& a, const T& t) const {
        for (size_t i = 0; i<N; ++i) if (a[i]>axis_position(t,i)) a[i]=axis_position(t,i);
        return a;        
    }
    
    array<real,N>& if_greater_assign(array<real,N>& a, const T& t) const {
        for (size_t i = 0; i<N; ++i) if (a[i]<axis_position(t,i)) a[i]=axis_position(t,i);
        return a;        
    }
    
    template<typename T2>
    array<real,N> difference(const array<real,N>& a, const T2& t) const {
        array<real,N> sol;
        if constexpr (is_same_v<T,T2>) {
            for (size_t i = 0; i<N; ++i) sol[i] = (a[i] - axis_position(t,i));
        } else { //It is an array
            for (size_t i = 0; i<N; ++i) sol[i] = (a[i] - t[i]);
        }
        return sol;
    }
    
    void build_tree(size_t left, size_t right) {
        if ((right-left) > 1) {
            //We build the bounding box each subdivision because even if it is slow it leaves a better kdtree balance
            array<real,N> bbmin, bbmax;
            assign(bbmin,elements[left]); assign(bbmax,elements[left]);
            for (size_t i = (left+1); i < right; ++i) {
                if_less_assign(bbmin,elements[i]);
                if_greater_assign(bbmax,elements[i]);
            }
            size_t median = (right+left)/2;
            //We find the larger axis
            size_t axis = 0; real max_bound = bbmax[0]-bbmin[0];
            for (size_t i = 1;i<N;++i) if ((bbmax[i]-bbmin[i])<max_bound) {
                axis = i; max_bound = bbmax[i]-bbmin[i];
            }
            //Partial ordering over that axis (median contains the median, to the left are smaller, to the right are greater)
            nth_element(elements.begin()+left,elements.begin()+median,elements.begin()+right,
                [&] (const T& a, const T& b) { return axis_position(a,axis)<axis_position(b,axis); });
            //The median stays in the median, so if in one dimension the vector is ordered (but not the case)
            //We setup the node as well (we just need the axis)
            nodes[median] = axis;
            //Recursive calls for the subtrees.
            build_tree(left,median);
            build_tree(median+1,right); 
        }
    }
    
    void build_tree() {
        nodes.resize(elements.size());
        build_tree(0,elements.size());
    }
    
    template<typename Norm> //Norm is a norm of a vector (euclidean or any other one, even a weighted one) for array<real,N>
    void nearest_neighbors_impl(vector<const T*>& values, size_t left, size_t right, const array<real,N>& p, size_t number, float& max_distance, const Norm& norm) const {
        if (right > left) {
            size_t median = (right+left)/2; //Points to the actual node which is always in the median
            auto distance_comparison = [&] (const T* a, const T* b) { return norm(difference(p,*a))<norm(difference(p,*b)); };
            if (norm(difference(p,elements[median]))<max_distance) {
                values.push_back(&elements[median]);
                if (values.size() == number) { //We reach the number so we make this a heap
                    make_heap(values.begin(),values.end(),distance_comparison);
                } else if (values.size() > number) { //We reached the number a while ago, so we push heap and pop heap
                    push_heap(values.begin(),values.end(),distance_comparison);
                    pop_heap(values.begin(),values.end(),distance_comparison);
                    values.pop_back();
                    max_distance = norm(difference(p,*values.front())); //We update max_distance so elements further away are just ignored
                }
            }
            //We have to explore the children, so we choose according to the node
            if ((right-left)>1) {
                //This is for distance measurement to check if we need to explore the other node
                array<real,N> pplane = p; 
                pplane[nodes[median]] = axis_position(elements[median],nodes[median]);
                if (p[nodes[median]] < axis_position(elements[median],nodes[median])) {//First left node and then, if needed, right node
                    nearest_neighbors_impl(values,left,median,p,number,max_distance,norm);
                    if (norm(difference(p,pplane)) < max_distance) //We still need to explore the other node
                        nearest_neighbors_impl(values,median+1,right,p,number,max_distance,norm);
                } else { //First right node and then, if needed, left node
                    nearest_neighbors_impl(values,median+1,right,p,number,max_distance,norm);
                    if (norm(difference(p,pplane)) < max_distance) //We still need to explore the other node
                        nearest_neighbors_impl(values,left,median,p,number,max_distance,norm);                      
                }
            }
        }
    }     
    
public:
    KDTree(vector<T>&& elements, const A& axis_position = A()) : elements(move(elements)), axis_position(axis_position) { build_tree(); }
    KDTree() {}
    template<typename C> //Constructing from a general collection if possible
    KDTree(const C& c, const A& axis_position = A(), typename enable_if<is_same<T,typename C::value_type>::value>::type* sfinae = nullptr) : axis_position(axis_position), elements(c.begin(),c.end()) { build_tree(); }
    
    template<typename Norm>
    vector<const T*> nearest_neighbors(const array<real,N>& p, size_t number, float max_distance, const Norm& norm) const {
        vector<const T*> sol;
        nearest_neighbors_impl(sol,0,elements.size(),p,number,max_distance,norm);
        return sol;
    }
    

    vector<const T*> nearest_neighbors(const array<real,N>& p, size_t number = 1, float max_distance = numeric_limits<float>::infinity()) const {
        return nearest_neighbors(p,number,max_distance,
            [] (const array<real,N>& v) {
                real s(0); for (real r : v) s+=r*r; return sqrt(s);
            });            
    }

    template<typename P, typename Norm> //P -> position N dimensional, should have random access
    vector<const T*> nearest_neighbors(const P& p, size_t number, float max_distance, const Norm& norm) const {
        array<real,N> p_impl;
        for (size_t i = 0; i<N; ++i) p_impl[i] = p[i];
        return nearest_neighbors(p_impl,number,max_distance,norm);
    }
    
    template<typename P> //P -> position N dimensional, should have random access
    vector<const T*> nearest_neighbors(const P& p, size_t number = 1, float max_distance = numeric_limits<float>::infinity()) const {
        return nearest_neighbors(p,number,max_distance,
            [] (const array<real,N>& v) {
                real s(0); for (real r : v) s+=r*r; return sqrt(s);
            });            
    }
};

template<size_t N,typename C,typename A>
auto kdtree(const C& c, const A& ap) { 
    return KDTree<decay_t<typename C::value_type>,N,decay_t<A>>(c,ap); 
} 


template<size_t N, typename C>
auto kdtree(const C& c, enable_if_t<is_arithmetic_v<decay_t<decltype(declval<typename C::value_type>()[0])>>>* sfinae = nullptr) {   
    return kdtree<N>(c,RandomAccess());   
} 

template<typename C>
auto kdtree(const C& c, enable_if_t<is_tuple<typename C::value_type>::value && 
            is_arithmetic_v<decay_t<decltype(get<0>(declval<typename C::value_type>()))>>>* sfinae = nullptr) {
    return kdtree<tuple_size_v<typename C::value_type>>(c);
}

template<size_t N, typename C>
auto kdtree(const C& c, enable_if_t<is_tuple<typename C::value_type>::value &&
            (tuple_size_v<typename C::value_type> > 1) && 
            is_arithmetic_v<decay_t<decltype(get<0>(declval<typename C::value_type>())[0])>>>* sfinae = nullptr) {
    return kdtree<N>(c, RandomAccessTupleFirst());
}

template<typename C>
auto kdtree(const C& c, enable_if_t<is_tuple<typename C::value_type>::value &&
            (tuple_size_v<typename C::value_type> > 1) && 
            is_arithmetic_v<decay_t<decltype(get<0>(get<0>(declval<typename C::value_type>())))>>>* sfinae = nullptr) {
    return kdtree<tuple_size_v<decay_t<decltype(get<0>(declval<typename C::value_type>()))>>>(c);
}
};