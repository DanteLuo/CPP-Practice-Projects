#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include <deque>
#include "eecs281priority_queue.h"

//A specialized version of the 'heap' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class pairing_heap : public eecs281priority_queue<TYPE, COMP> {
    public:
        typedef unsigned size_type;

        //Description: Construct a heap out of an iterator range 
        //              with an optional comparison functor.
        //Runtime: O(n) where n is number of elements in range.
        template<typename InputIterator>
        pairing_heap(InputIterator start, InputIterator end, 
                        COMP comp = COMP());

        //Description: Construct an empty heap with an optional 
        //              comparison functor.
        //Runtime: O(1)
        pairing_heap(COMP comp = COMP());

        //Description: Assumes that all elements inside the heap are 
        //              out of order and 'rebuilds' the heap by fixing 
        //              the heap invariant.
        //Runtime: O(n)
        virtual void update_priorities();

        //Description: Add a new element to the heap. This has been 
        //              provided for you, in that you should 
        //              implement push functionality in the add_node function.
        //Runtime: Amortized O(1)
        virtual void push(const TYPE& val) override { add_node(val); }

        //Description: Remove the most extreme (defined by 'compare') 
        //              element from the heap.
        //Runtime: Amortized O(log(n))
        virtual void pop() override;

        //Description: Return the most extreme (defined by 'compare') element of
        //             the heap.
        //Runtime: O(1)
        virtual const TYPE& top() const override;

        //Description: Get the number of elements in the heap.
        //Runtime: O(1)
        virtual size_type size() const { return size_elt; }

        //Description: Return true if the heap is empty.
        //Runtime: O(1)
        virtual bool empty() const override { return size_elt== 0;  }

        //Description: Copy constructor.
        //Runtime: O(n)
        pairing_heap(const pairing_heap& other);

        //Description: Copy assignment operator.
        //Runtime: O(n)
        pairing_heap& operator=(const pairing_heap& rhs);

        //Description: Destructor
        //Runtime: O(n)
        virtual ~pairing_heap();

        class Node {
            //This node class will allow external users to update the 
            //priority of
            //elements that are already inside the heap.
            public:
                // Add any constructors and destructors you need here
                Node(TYPE val):
                elt(val),
                previous(nullptr),
                next(nullptr),
                child(nullptr)
                {}

                //Description: Allows access to the element at that Node's 
                //position.
                //Runtime: O(1) - this has been provided for you.
                const TYPE &operator*() const { return elt; }

                //The following line allows you to access any private data 
                //members of this
                //Node class from within the pairing_heap class. 
                //(ie: myNode.elt is a legal
                //statement in pairing_heap's add_node() function).
                friend pairing_heap;

            private:
                TYPE elt;
                Node* previous;
                Node* next;
                Node* child;
                // Add any addtional member functions or data you require here
        };

        //Description: Add a new element to the heap. Returns a 
        //              Node* corresponding to the newly added element.
        //Runtime: Amortized O(1)
        Node* add_node(const TYPE& val);

        // Description: Updates the priority of an element already 
        //              in the heap by replacing the element referred 
        //              to the Node with new_value.
        //              Must maintain heap invariants.
        // PRECONDITION: The new priority, given by 'new_value' must 
        //                  be more extreme (as defined by comp)
        //                  than the old priority.
        //
        // Runtime: As discussed in reading material.
        void updateElt(Node * node, TYPE new_value);

    private:
        // Add any additional member functions or data you require here.
        // We recommend creating a 'meld' function (see th readings).
        Node* root;
        int   size_elt;
        
        void meld(Node*& root, Node* sub_root);
        Node* deepCopy(Node* theNode);
        void deleteNode(Node* theNode);
        void rebuild(Node* theNode);
        Node* findParent(Node* theNode);
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
pairing_heap<TYPE, COMP>::pairing_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
)  
{
    this->compare = comp;
    
    size_elt= 0;
    root = nullptr;
    while(start != end) 
    {
        add_node(*start++);
    }
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(COMP comp) 
{
    this->compare = comp;
    size_elt= 0;
    root = nullptr;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(const pairing_heap& other) 
{
    size_elt= other.size();
    root = deepCopy(other.root);
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>&
pairing_heap<TYPE, COMP>::operator=(const pairing_heap& rhs) 
{
    if (this != &rhs)
    {
        deleteNode(root);
        size_elt= rhs.size;
        root = rhs.deepCopy(rhs.root);
    }

    return *this;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::~pairing_heap() 
{
    deleteNode(root);
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::update_priorities() 
{
    Node* root_copy = root;
    root = nullptr;
    int size_buf = size_elt;

    rebuild(root_copy);
    size_elt= size_buf;
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::pop() 
{
    Node* former_root = root;

    if (root->child == nullptr)
    {
        root = nullptr;
        size_elt--;
        return;
    }

    if (root->child->next == nullptr) 
    {
        root = root->child; // No next sibling; No need to cut.
        former_root->child = nullptr;
        delete former_root;
        size_elt--;
        return;
    }

    Node *indexNode = root->child;
    std::deque<Node*> tree_queue;
    
    while (indexNode!=nullptr) 
    {
        Node *tmpNode = indexNode->next;
        indexNode->next = nullptr;
        tree_queue.push_back(indexNode);
        indexNode = tmpNode;
    }

    while(tree_queue.size() > 1)
    {
        Node* node_one = tree_queue.front();
        tree_queue.pop_front();
        Node* node_two = tree_queue.front();
        tree_queue.pop_front();
        meld(node_one, node_two);
        tree_queue.push_back(node_one);
    }

    former_root->child = nullptr;
    delete former_root;
    root = tree_queue.front();
    size_elt--;
}


template<typename TYPE, typename COMP>
const TYPE& pairing_heap<TYPE, COMP>::top() const 
{
    return root->elt;
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::add_node(const TYPE& val) 
{
    Node* theNode = new Node(val);

    if (root == nullptr)
    {
        root = theNode;    
    }else
    {
        meld(root, theNode);
    }

    size_elt++;
    return theNode;
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::updateElt(Node* node, TYPE new_value) 
{
    node->elt = new_value;
    if (node == root){return;}
    else
    {
        Node* parent = findParent(node);
        if (this->compare(*node,*parent)){return;}
        
        if (node->next != nullptr){node->next->previous = node->previous;}

        if (parent->child == node){node->previous->child = node->next;}// cut the tree off
        else {node->previous->next = node->next;}
         
        node->next = nullptr;
        node->previous = nullptr;
        meld(root, node);
    }
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::meld(Node*& root_node, Node* sub_root_node)
{
    if(this->compare(root_node->elt, sub_root_node->elt)) 
    {
        root_node->previous = sub_root_node;
        root_node->next = sub_root_node->child;
        sub_root_node->child = root_node;

        if (root_node->next != nullptr)
        {
            root_node->next->previous = root_node;
        }

        root_node = sub_root_node;
    }
    else
    {
        sub_root_node->previous = root_node;
        sub_root_node->next = root_node->child;
        root_node->child = sub_root_node;

        if (sub_root_node->next != nullptr)
        {
            sub_root_node->next->previous = sub_root_node;
        }
    }
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::deepCopy(Node* theNode)
{
    if(theNode == nullptr) // base case
    {
        return nullptr;
    }
    else
    {
        Node* new_node = new Node(theNode->elt);
        new_node->next = deepCopy(theNode->next);
        new_node->child = deepCopy(theNode->child);
        if(new_node->child != nullptr){new_node->child->previous = new_node;}
        if(new_node->next != nullptr){new_node->next->previous = new_node;} 

        return new_node;
    }
}
        
template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::deleteNode(Node* theNode)
{
    // if (theNode != nullptr)
    // {
    //     std::vector<Node*> theNodes;
    //     theNodes.push_back(theNode);
    //     Node* victim;

    //     while(!theNodes.empty())
    //     {
    //         victim = theNodes.back();
    //         theNodes.pop_back();
    //         if(victim->child != nullptr){theNodes.push_back(victim->child);}
    //         if(victim->next != nullptr){theNodes.push_back(victim->next);}
    //         delete victim;
    //     }
    // }
    
    if (theNode == nullptr)
    {
        return;
    }
    else if (theNode->next == nullptr && theNode->child == nullptr){delete theNode;}
    else
    {
        deleteNode(theNode->next);
        deleteNode(theNode->child);
    }
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::rebuild(Node* theNode)
{
    if(theNode == nullptr){return;}
    else
    {
        rebuild(theNode->next);
        rebuild(theNode->child);
        add_node(theNode->elt);
        delete theNode;
    }
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::findParent(Node* theNode)
{
    while(theNode->child->previous != theNode) 
    {
        theNode = theNode->previous;
    }

    return theNode;
}

#endif //PAIRING_HEAP_H

