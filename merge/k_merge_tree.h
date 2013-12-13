/*

  K-Way Merge Template
  By Jordan Zimmerman
  
*/

#ifndef KMERGE_TREE_H
#define KMERGE_TREE_H

/*

K-Way Merge

An implementation of "k-Way Merging" as described in "Fundamentals of Data 
Structures" by Horowitz/Sahni.

The idea is to merge k sorted arrays limiting the number of comparisons. A tree 
is built containing the results of comparing the heads of each array. The top 
most node is always the smallest entry. Then, its corresponding leaf in the tree 
is refilled and the tree is processed again. It's easier to see in the following 
example:

Imagine 4 sorted arrays:
{5, 10, 15, 20}
{10, 13, 16, 19}
{2, 19, 26, 40}
{18, 22, 23, 24}

The initial tree looks like this:


           2
       /       \
     2           5
   /   \       /   \
 18     2    10     5
 
The '/' and '\' represent links. The bottom row has the leaves and they contain 
the heads of the arrays. The rows above the leaves represent the smaller of the 
two child nodes. Thus, the top node is the smallest. To process the next 
iteration, the top node gets popped and its leaf gets refilled. Then, the new 
leaf's associated nodes are processed. So, after the 2 is taken, it is filled 
with 19 (the next head of its array). After processing, the tree looks like this:

           5
       /       \
     18          5
   /   \       /   \
 18    19    10     5
 
So, you can see how the number of comparisons is reduced. 

A good use of this is when you have a very large array that needs to be sorted. 
Break it up into n small arrays and sort those. Then use this merge sort for the 
final sort. This can also be done with files. If you have n sorted files, you 
can merge them into one sorted file. K Way Merging works best when comparing is 
somewhat expensive.

*/

#include <functional>

template <typename T, typename iter_t, typename cmp_t = std::less<T> > 
class kmerge_tree_c
{

public:
  // create the tree with the given number of buckets. Call add() for each of 
  // the buckets and then call execute() to build things. Call get_top() then 
  // next() until get_top returns false.
  kmerge_tree_c(long bucket_qty);
  ~kmerge_tree_c();
public:
  void  add(iter_t begin, iter_t end); // add a sorted collection to the tree
                                       // begin/end - start end of a collection
  void  execute(void);                 // process the first sort
  void  next(void);                    // advance to the next entry
  bool  get_top(iter_t& iterator)
  {
    // return the next entry without re-processing the tree
    // if false is returned, the merge is complete

    iterator = top_node_ptr_mbr->current_iterator;
    return iterator != top_node_ptr_mbr->end_iterator;
  }
  
private:
  class node_rec
  {
  public:
    node_rec(void) :
      left_child_ptr(NULL),
      right_child_ptr(NULL),
      parent_ptr(NULL),
      next_ptr(NULL),
      previous_ptr(NULL),
      next_leaf_ptr(NULL),
      source_node_ptr(NULL),
      has_iterator(false){
    }
    ~node_rec(){
      delete left_child_ptr;
      delete right_child_ptr;
    }
    
    node_rec*  left_child_ptr;  // owned the left child of this node
    node_rec*  right_child_ptr; // owned the right child of this node
    node_rec*  parent_ptr;      // copy  the parent of this node
    node_rec*  next_ptr;        // copy  the next sibling
    node_rec*  previous_ptr;    // copy  the previous sibling
    node_rec*  next_leaf_ptr;   // copy  only for the bottom rows, a linked list 
                                // of the buckets
    node_rec*  source_node_ptr; // copy  ptr back to the node from whence this 
                                // iterator came
    int        has_iterator;
    iter_t current_iterator;
    iter_t end_iterator;

  private:
    node_rec(const node_rec&);
    node_rec&  operator=(const node_rec&);
  };
  
  void        build_tree(void);  
  node_rec*   build_levels(long number_of_levels);
  void        build_left_siblings(node_rec* node_ptr);
  void        build_right_siblings(node_rec* node_ptr);
  void        compare_nodes(node_rec* node_ptr);

  cmp_t       comparitor_mbr;
  long        bucket_qty_mbr;
  long        number_of_levels_mbr;
  node_rec*   top_node_ptr_mbr;     // owned
  node_rec*   first_leaf_ptr;       // copy
  node_rec*   last_leaf_ptr;        // copy

};

//~~~~~~~~~~class kmerge_tree_c

// brute force log2
/* I've read that log2 of something is defined as:
  log2(x) is the power you'd have to raise 2 to get x
  Thus, log2(4) is 2, log2(16) is 4, etc.
*/
inline long kmerge_tree_brute_log2(long value)
{
  long  square = 2;
  long  count = 1;
  while ( square < value ){
    square *= 2;
    ++count;
  }
  return count;
} // kmerge_tree_brute_log2

template <class T, class iter_t, class cmp_t> 
kmerge_tree_c<T, iter_t, cmp_t>::kmerge_tree_c(long bucket_qty) :
  bucket_qty_mbr(bucket_qty),
  top_node_ptr_mbr(NULL),
  first_leaf_ptr(NULL),
  last_leaf_ptr(NULL),
  number_of_levels_mbr(::kmerge_tree_brute_log2(bucket_qty))  // don't add one - build_levels is zero based
{
  build_tree();
}

template <class T, class iter_t, class cmp_t> 
kmerge_tree_c<T, iter_t, cmp_t>::~kmerge_tree_c()
{
  delete top_node_ptr_mbr;
}

/*
  Unlike the book, I'm going to make my life easy
  by maintaining every possible link to each node that I might need
*/
template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::build_tree(void)
{
  // the book says that the number of levels is (log2 * k) + 1
  top_node_ptr_mbr = build_levels(number_of_levels_mbr);
  if ( top_node_ptr_mbr ){
    build_left_siblings(top_node_ptr_mbr);
    build_right_siblings(top_node_ptr_mbr);
  }
}

/*
  highly recursive tree builder
  as long as number_of_levels isn't zero, each node builds its own children
  and updates the parent link for them. 
  
  If no children are to be built (i.e. number_of_levels is 0), then the leaf 
  linked list is created
*/
template <class T, class iter_t, class cmp_t> 
typename kmerge_tree_c<T, iter_t, cmp_t>::node_rec* 
kmerge_tree_c<T, iter_t, cmp_t>::build_levels(long number_of_levels)
{
  node_rec* node_ptr = new node_rec;

  if ( number_of_levels ){
    node_ptr->left_child_ptr = build_levels(number_of_levels - 1);
    if ( node_ptr->left_child_ptr ){
      node_ptr->left_child_ptr->parent_ptr = node_ptr;
      node_ptr->right_child_ptr = build_levels(number_of_levels - 1);
      if ( node_ptr->right_child_ptr )
        node_ptr->right_child_ptr->parent_ptr = node_ptr;
    }
  }else{
    if ( last_leaf_ptr ){
      last_leaf_ptr->next_leaf_ptr = node_ptr;
      last_leaf_ptr = node_ptr;
    }else
      first_leaf_ptr = last_leaf_ptr = node_ptr;
  }

  return node_ptr;
}

/*
  when we process a comparison, we'll have to compare two siblings
  this code matches each link with its sibling
  
  To get every node correct, I had to write two routines: one which works
  with left nodes and one which works with right nodes. build_tree() starts it
  off with the top node's children and then these two swing back and forth
  between each other.
*/

template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::
build_left_siblings(typename kmerge_tree_c<T, iter_t, cmp_t>::node_rec* pnode)
{
  if ( pnode->parent_ptr ){
    if ( pnode->parent_ptr->right_child_ptr ){
      pnode->next_ptr = pnode->parent_ptr->right_child_ptr;
      pnode->parent_ptr->right_child_ptr->previous_ptr = pnode;
    }
  }
  
  if ( pnode->left_child_ptr )
    build_left_siblings(pnode->left_child_ptr);
  if ( pnode->right_child_ptr )
    build_right_siblings(pnode->right_child_ptr);
}

template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::
build_right_siblings(typename kmerge_tree_c<T, iter_t, cmp_t>::node_rec* pnode)
{
  if ( pnode->parent_ptr ){
    if ( pnode->parent_ptr->left_child_ptr ){
      pnode->previous_ptr = pnode->parent_ptr->left_child_ptr;
      pnode->parent_ptr->left_child_ptr->next_ptr = pnode;
    }
  }
  
  if ( pnode->right_child_ptr )
    build_right_siblings(pnode->right_child_ptr);
  if ( pnode->left_child_ptr )
    build_left_siblings(pnode->left_child_ptr);
}

// fill the leaf linked list
template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::
add(iter_t begin, iter_t end)
{
  if ( begin == end )
    return;

  node_rec*    node_ptr = first_leaf_ptr;
  while ( node_ptr ){
    if ( node_ptr->has_iterator )
      node_ptr = node_ptr->next_leaf_ptr;
    else{
      node_ptr->has_iterator = true;
      node_ptr->current_iterator = begin;
      node_ptr->end_iterator = end;
      break;
    }
  }
}

/*
  fill the initial tree by comparing each sibling in the
  leaf linked list and then factoring that up to the parents
  This is only done once so it doesn't have to be that efficient
*/
template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::execute(void)
{
  for (long parent_level=0; parent_level < number_of_levels_mbr; ++parent_level)
  {
    // the number of nodes to skip is (parent level + 1) ^ 2 - 1
    long      skip_nodes = 2;
    for ( long skip = 0; skip < parent_level; ++skip )
      skip_nodes *= 2;

    --skip_nodes;

    node_rec*    node_ptr = first_leaf_ptr;
    while ( node_ptr ){
      node_rec*  parent_ptr = node_ptr; 

      for (long i = 0; i < parent_level; ++i )
        parent_ptr = parent_ptr->parent_ptr;    
      compare_nodes(parent_ptr);      
      for (long i = 0; i < skip_nodes; ++i )
        node_ptr = node_ptr->next_leaf_ptr;
      
      node_ptr = node_ptr->next_leaf_ptr;
    }
  }
}

// compare the given node and its sibling and bubble the result up to the parent
template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::
compare_nodes(typename kmerge_tree_c<T, iter_t, cmp_t>::node_rec* pnode)
{
  //  assert(node_ptr->parent_ptr);
  node_rec*  node1_ptr = NULL;
  node_rec*  node2_ptr = NULL;
  if ( pnode->next_ptr ){
    node1_ptr = pnode;
    node2_ptr = pnode->next_ptr;
  }else{
    node1_ptr = pnode->previous_ptr;
    node2_ptr = pnode;
  }
  
  long result;
  if ( (node1_ptr->current_iterator != node1_ptr->end_iterator) && 
       (node2_ptr->current_iterator != node2_ptr->end_iterator) ){
    // no need to check for exact equality - we just want the lesser of the two
    result = comparitor_mbr(*node1_ptr->current_iterator, 
                            *node2_ptr->current_iterator) ? -1 : 1;
  }else if ( node1_ptr->current_iterator != node1_ptr->end_iterator )
    result = -1;
  else // if ( node2_ptr->current_iterator != node2_ptr->end_iterator )
    result = 1;

  node_rec*  winner_ptr = (result <= 0) ? node1_ptr : node2_ptr; 
  node_rec*  parent_ptr = pnode->parent_ptr;
  
  parent_ptr->current_iterator = winner_ptr->current_iterator;
  parent_ptr->end_iterator = winner_ptr->end_iterator;
  parent_ptr->source_node_ptr = winner_ptr;

}

/*
  pop the top node, factor it down the list to find its
  leaf, replace its leaf and then factor that back up
*/
template <class T, class iter_t, class cmp_t> 
void kmerge_tree_c<T, iter_t, cmp_t>::next(void)
{
  if ( top_node_ptr_mbr->current_iterator == top_node_ptr_mbr->end_iterator )
    return;

  // find the source leaf ptr
  node_rec*  node_ptr = top_node_ptr_mbr;
  while ( node_ptr->source_node_ptr )
    node_ptr = node_ptr->source_node_ptr;
  
//  assert(!node_ptr->left_child_ptr && !node_ptr->right_child_ptr);
//  assert(top_node_ptr_mbr->current_iterator == node_ptr->current_iterator);
  
  ++node_ptr->current_iterator;

  while ( node_ptr->parent_ptr ){
    compare_nodes(node_ptr);
    node_ptr = node_ptr->parent_ptr;
  }
}

#endif  // KMERGE_TREE_H

