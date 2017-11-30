#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>

#define inf 9999999

using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;

AdjacencyMatrix read_graph(char filename[]);
AdjacencyMatrix pred_matrix(AdjacencyMatrix G);
AdjacencyMatrix floyd_warshal(AdjacencyMatrix G);
void path(AdjacencyMatrix preds, int i, int j);
void print_adj_matrix(AdjacencyMatrix G);

int main(int argc, char* argv[])
{
     AdjacencyMatrix G = read_graph(argv[1]);
     AdjacencyMatrix preds = floyd_warshal(G);
     for (int i = 0; i < G.size(); i++)
     {
          for (int j = i; j < G.size(); j++)
          {
               cout<< "(" << i << ", " << j << "):";
               path(preds, i, j);
               cout<<endl;
          }
     }

}

void print_adj_matrix(AdjacencyMatrix G)
{
     for (auto const& row: G)
     {
          for (auto const& item: row)
          {
               cout << item << " ";
          }
          cout << endl;
     }
}


AdjacencyMatrix floyd_warshal(AdjacencyMatrix G)
{
     AdjacencyMatrix preds = pred_matrix(G);
     int n = G.size();
     for (int k = 0; k < n; k++)
     {
          for (int i = 0; i < n; i++)
          {
               for (int j = 0; j < n; j++)
               {
                    if (G[i][k] != inf && G[k][j] != inf)
                    {
                         int temp = G[i][k] + G[k][j];
                         if (G[i][j] > temp)
                         {
                              G[i][j] = temp;
                              preds[i][j] = preds[k][j];
                         }
                    }
               }
          }
     }
     return preds;
}

AdjacencyMatrix pred_matrix(AdjacencyMatrix G)
{
     int size = G.size();
     AdjacencyMatrix preds = AdjacencyMatrix(size, vector<int>(size));
     for (int i = 0; i < size; i++)
     {
          for (int j = 0; j < size; j++)
          {
               if (G[i][j] == inf)
               {
                    preds[i][j] = -1;
               } else {
                    preds[i][j] = i;
               }
          }
     }
     return preds;
}

void path(AdjacencyMatrix preds, int i, int j)
{
     if (i == j)
     {
          cout << " " << i;
     }
     else if (preds[i][j] == -1)
     {
          cout << "No path";
     }
     else
     {
          path(preds, i, preds[i][j]);
          cout << " " << j;
     }

}

AdjacencyMatrix read_graph(char filename[])
{
     fstream f(filename, std::ios_base::in);
     int u,v;
     vector<pair<int,int> > all_edges;
     int max_node = 0;
     while (f >> u >> v)
     {
          all_edges.push_back(make_pair(u,v));
          if (u > max_node)
          {
               max_node = u;
          }
          if (v > max_node)
          {
               max_node = v;
          }
     }
     f.close();
     int num_nodes = max_node + 1;
     AdjacencyMatrix graph = AdjacencyMatrix(num_nodes, vector<int>(num_nodes));
     for (int i = 0; i < all_edges.size(); i++)
     {
          u = all_edges[i].first;
          v = all_edges[i].second;
          graph[u][v] = 1;
          graph[v][u] = 1;
     }
     for (int i = 0; i < graph.size(); i++)
     {
          graph[i][i] = 0;
     }
     return graph;
}
