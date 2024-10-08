
#include <iostream>
#include "Graph.h"

int main()
{
    std::string path;

    std::cin >> path;
    Graph G(path);
    std::cin >> path;
    int w, h, i;
    std::cin >> w >> h >> i;
    G.Print(w, h, i, path);

    return 0;
}
