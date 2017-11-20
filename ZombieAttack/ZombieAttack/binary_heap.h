#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "eecs281priority_queue.h"

//A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class binary_heap: public eecs281priority_queue<TYPE, COMP> {
	public:
		typedef unsigned size_type;

		//Description: Construct a heap out of an iterator range with an optional
		//             comparison functor.
		//Runtime: O(n) where n is number of elements in range.
		template<typename InputIterator>
		binary_heap(InputIterator start, InputIterator end, COMP comp = COMP());

		//Description: Construct an empty heap with an optional comparison functor.
		//Runtime: O(1)
		binary_heap(COMP comp = COMP());

		//Description: Assumes that all elements inside the heap are out of order and
		//             'rebuilds' the heap by fixing the heap invariant.
		//Runtime: O(n)
		virtual void update_priorities();

		//Description: Add a new element to the heap.
		//Runtime: O(log(n))
		virtual void push(const TYPE& val);

		//Description: Remove the most extreme (defined by 'compare') element from
		//             the heap.
		//Note: We will not run tests on your code that would require it to pop an
		//element when the heap is empty. Though you are welcome to if you are
		//familiar with them, you do not need to use exceptions in this project.
		//Runtime: O(log(n))
		virtual void pop();

		//Description: Return the most extreme (defined by 'compare') element of
		//             the heap.
		//Runtime: O(1)
		virtual const TYPE& top() const;

		//Description: Get the number of elements in the heap.
		//Runtime: O(1)
		virtual size_type size() const { return data.size(); }

		//Description: Return true if the heap is empty.
		//Runtime: O(1)
		virtual bool empty() const { return data.empty(); }

		virtual ~binary_heap() {}

	private:
		void fixUp(int index);
		void fixDown(int index);
		
		//Note: This vector *must* be used your heap implementation.
		std::vector<TYPE> data;

        // declare any helper functions/variables you may need here
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
binary_heap<TYPE, COMP>::binary_heap(InputIterator start, InputIterator end,
		COMP comp) 
{
    this->compare = comp;
    TYPE trivial;
    data.push_back(trivial);
    while(start != end) 
    {
        data.push_back(*start++);
    }

    update_priorities();
}

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp) 
{
    this->compare = comp;
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::update_priorities() 
{
    uint index = size()-1;
    while (index > 0)
    {
    	fixDown(index--);
    }
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::push(const TYPE& val) 
{
    data.push_back(val);
    fixUp(size()-1);
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::pop() 
{
    data[1] = data[size()-1];
    data.pop_back();
    fixDown(1);
}

template<typename TYPE, typename COMP>
const TYPE& binary_heap<TYPE, COMP>::top() const 
{
    return data[1];
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::fixUp(int index)
{
	while(index > 1 && this->compare(data[index/2],data[index])) 
	{
	    std::swap(data[index], data[index/2]);
		index /= 2;
	}
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::fixDown(int index)
{
	int data_size = size();

	while(2*index <= data_size-1) 
	{
	    int j = 2*index;
	    if (j<data_size-1 && this->compare(data[j],data[j+1])){j++;}
	    if (this->compare(data[j],data[index])) {break;}
	    std::swap(data[index],data[j]);
	    index = j;
	}
}


#endif //BINARY_HEAP_H

