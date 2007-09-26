@c ------------------------------------------------------------
@node Graph
@section The graph structure


@noindent
@strong{THE GRAPH STRUCTURE IS NOT IMPLEMENTED YET}.


@deftp {Struct Typedef} ucl_graph_node_t
The type of the node structure. Holds the pointer to the first outgoing
link.
@end deftp


@deftp {Struct Typedef} ucl_graph_link_t
The type of the link structure. Links originating from the same node are
organised in a linked list. Each one references the destination node.
@end deftp



@menu
* Graph Implementation::        How it's done.
* Graph Creation::              Creating and destroying graphs.
* Graph Adding::                Adding elements to a graph.
* Graph Removing::              Removing elements from a graph.
* Graph Ops::                   Various operations on a graph.
@end menu


@c ------------------------------------------------------------
@node Graph Implementation
@subsection How it's done


@noindent
A graph is a network of little structures holding pointers.


@example   
 ----     ----     ----     ----
|nod1|-->|lnk1|-->|nod2|-->|lnk4|
 ----     ----     ----     ----
            |
            v
          ----     ----
         |lnk2|-->|nod3|
          ----     ----
            |
            v 
          ----     ----
         |lnk3|-->|nod4|
          ----     ----
@end example

@c ------------------------------------------------------------
@node Graph Creation
@subsection Creating and destroying graphs


@deftypefun void ucl_graph_initnode (ucl_graph_node_t * @var{nodPtr})
A wrapper for @code{memset()}. Initialises the node structure.
@end deftypefun


@deftypefun void ucl_graph_initlink (UCL_Graph_Link * @var{lnkPtr})
A wrapper for @code{memset()}. Initialises the link structure.
@end deftypefun



@c ------------------------------------------------------------
@node Graph Adding
@subsection Adding elements to a graph


@deftypefun void ucl_graph_link	(@var{lnkPtr}, @var{fromPtr}, @var{toPtr})
Links together two nodes with a link. Arguments: 

@table @code
@item ucl_graph_link_t * @var{lnkPtr}
pointer to the link structure;

@item ucl_graph_node_t * @var{fromPtr}
pointer to the source node structure;

@item ucl_graph_node_t * @var{toPtr}
pointer to the destination node structure.
@end table
@end deftypefun


@defmac ucl_graph_setlink (ucl_graph_node_t *@var{nodPtr}, ucl_graph_link_t *@var{newPtr})
Sets the link structure referenced by @var{newPtr} as first outgoing
link for the node structure referenced by @var{nodPtr}. The pointer to
the old link (if present) is overwritten.
@end defmac


@defmac ucl_graph_getlink (ucl_graph_node_t *@var{nodPtr})
Returns a @code{UCL_Graph_Link} pointer to the first outgoing link from
the node referenced by @var{nodPtr}.
@end defmac


@defmac ucl_graph_setnext (ucl_graph_link_t *@var{lnkPtr}, ucl_graph_link_t *@var{newPtr})
Sets the link structure referenced by @var{newPtr} as next link for the
link structure referenced by @var{lnkPtr}. The pointer to the old link
(if present) is overwritten. The link referenced by @var{newPtr} becomes
an outgoing link from the same node as @var{lnkPtr} is.
@end defmac


@defmac ucl_graph_getnext (ucl_graph_link_t *@var{lnkPtr})
Returns a @code{UCL_Graph_Link} pointer referencing the next link in the
chain from the link referenced by @var{lnkPtr}.
@end defmac


@defmac ucl_graph_setnode (ucl_graph_link_t *@var{lnkPtr}, ucl_graph_node_t *@var{newPtr})
Sets the node referenced by @var{newPtr} as the destination node for the
link referenced by @var{lnkPtr}.
@end defmac


@defmac ucl_graph_getnode (lnkPtr)
Return a @code{UCL_Graph_Node} pointer referencing the destination node
of the link referenced by @var{lnkPtr}.
@end defmac



@c ------------------------------------------------------------
@node Graph Removing
@subsection Removing elements from a graph


@noindent
The extraction of links and nodes from a graph is impossible.


@c ------------------------------------------------------------
@node Graph Ops
@subsection Various operations on a graph


@defmac ucl_graph_setval (@var{ptr}, ucl_value_t @var{newval})
Stored in the link or node structure referenced by @var{ptr} a new
value.
@end defmac


@defmac ucl_graph_getval (@var{ptr})
Returns a copy of the @code{ucl_value_t} data stored in the node or link
structure referenced by @var{ptr}.
@end defmac


@c end of file
@c Local Variables:
@c mode: texinfo
@c End:
