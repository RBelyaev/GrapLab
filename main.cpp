
#include <iostream>
#include "Graph.h"

int main()
{

    std::string path = "../graph/11.txt";

    Graph G(path);
    path = "../prints/graph11.bmp";
    G.Print(500, 500, 10005, path);

    return 0;
}
