# Testing Rationale

## Constructors

- I tested that I could create a valid graph which was empty
- I tested for the initializer list and iterator constructor that all nodes were inserted properly
- I tested for the move constructor and operator that the new objects have all the same values but the old object is invalidated (no nodes)
- I tested for the copy constructor that both objects have the same value but do not have the same memory address

## Modifiers

- I tested that every function threw the appropriate error throws
- For insert_node, if a node is inserted properly then is_node should return true
- For insert_edge, if an edge is inserted properly then is_connected should return true
- For replace_node, if the old node is replaced properly then is_node should return false
- For replace_node, if the new node is inserted properly then all the inlinks and outlinks should be rewired appropriately which can be tested with is_connected
- For merge_replace_node, if the nodes are merged properly then the old node should return false when passed into is_node and the new node should have all the inlinks and outlinks of the old node which can be tested with is_connected
- For erase_node, if it is erased properly then is_node should return false for it, and any node which had an edge with a destination of the erased node should have its connections reduced
- For erase_edge, connections should show less connections than before
- The iterator versions of erase_edge check for the same things plus ensuring that the iterator returned points to the correct edge
- For clear, the graph should become empty

## Accessors

- I tested that every function threw the appropriate error throws
- I also tested that all functions work with const graphs
- For is_node, all nodes initialised should return true while nodes that aren't there should return false. Any inserted node should return true while erased nodes should return false
- For empty, an empty graph should return true and a populated graph should also return true when cleared
- For is_connected, an edge should return true, however as this is a directed graph switched arguments should not return true unless there is an edge there
- For nodes, all nodes should be visible, even when inserted and erased
- For weights, all nodes should be visible, even when inserted and erased
- For find, when an edge is found it should return a valid iterator, but when the find fails it should return the end
- For connections, all nodes should be visible, even when inserted and erased

## Comparisons

- For == operator I just gave it a variety of same and different graphs as well as testing it against itself which should return true

## Extractors

- For << operator I used the same test that was given in the spec as it tested a large graph with many nodes and edges

## Iterators

- I weaved begin() and end() into testing other iterator related functions
- For ++ I ensure that the iterator steps forward appropriately and can reach the end(), after this is undefined behaviour
- For -- I similarly ensure that the iterator can move all the way from the end() to begin() after which it is once again undefined behaviour
