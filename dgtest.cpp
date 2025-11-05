
#include "dg.h"
#include <stdio.h>

struct dgnode
{
    intrusive::dg_inject<dgnode> dg;

    int i = 0;
};

int main(void)
{
    dgnode nodes[5];
    for (int i = 0; i < 5; ++i)
    {
        nodes[i].i = i;
    }

    intrusive::directed_graph<dgnode, &dgnode::dg> dg;

    for (int i = 0; i < 5; ++i)
    {
        dg.add(&nodes[i]);
        printf("Node %d: %p\n", i, &nodes[i]);
    }

    intrusive::dg_edge<dgnode> edges[7];
    
    edges[0].from = nodes + 0;
    edges[0].to = nodes + 4;

    edges[1].from = nodes + 1;
    edges[1].to = nodes + 2;
    
    edges[2].from = nodes + 0;
    edges[2].to = nodes + 2;

    edges[3].from = nodes + 1;
    edges[3].to = nodes + 4;

    edges[4].from = nodes + 2;
    edges[4].to = nodes + 4;
    
    edges[5].from = nodes + 0;
    edges[5].to = nodes + 3;
    
    edges[6].from = nodes + 2;
    edges[6].to = nodes + 3;

    for (int i = 0; i < sizeof(edges) / sizeof(edges[0]); ++i)
    {
        dg.addEdge(edges + i);
        printf("Edge %d: %p        %p -> %p\n", i, &edges[i], edges[i].from, edges[i].to);
    }

    decltype(dg)::node_list list;
    bool r = dg.topo_sort(list);

    dgnode* n = list.head;
    while (n)
    {
        printf("%d\n", n->i);
        n = list.resolve(n)->right;
    }
}
