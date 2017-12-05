#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>


using namespace std;

const int inf = numeric_limits<int>::max();

typedef vector<vector<int> > Graph;

Graph read_graph(char filename[]);
Graph initialize_pred_matrix(const Graph &G);
void floyd_warshall(Graph &G, Graph &preds);
void shortest_path(const Graph &preds, const int i, const int j, vector<int> &path);
void print_adj_matrix(const Graph &G);

int main(int argc, char* argv[])
{
     Graph G = read_graph(argv[1]);
     Graph preds = initialize_pred_matrix(G);
     floyd_warshall(G, preds);

     // print shortest paths.
     for (int i = 0; i < G.size(); i++)
     {
          for (int j = i; j < G.size(); j++)
          {
               vector<int> path = vector<int>();
               shortest_path(preds, i, j, path);
               if (!path.empty())
               {
                    cout<< "(" << i << ", " << j << "):";
                    for (auto const& x: path)
                    {
                         cout << ' ' << x;
                    }
                    cout << endl;
               }
          }
     }
}

/*
  Prints an adjacency matrix to stdout.
 */
void print_adj_matrix(const Graph &G)
{
     for (auto const& row: G)
     {
          for (auto const& item: row)
          {
               cout << item << ' ';
          }
          cout << endl;
     }
}

/*
  Finds APSP with path reconstruction using the Floyd Warshall algorithm.

  After completion, G[i][j] will be the distance of the shortest i -> j path.
  preds should be a graph of the same size as G, initialized with G[i][j] = i if
  there is an edge between i and j in G, otherwise -1 as a sentinel. After
  completion, preds[i][j] will be the node that comes before i on the shortest
  path from i to j.
*/
void floyd_warshall(Graph &G, Graph &preds)
{
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
                         if (temp < G[i][j])
                         {
                              G[i][j] = temp;
                              preds[i][j] = preds[k][j];
                         }
                    }
               }
          }
     }
}

/*
  Initializes a predecessor matrix of a Graph.

  G[i][j] is set to i if there is an edge G[i][j], otherwise -1 as a sentinel
  value.
 */
Graph initialize_pred_matrix(const Graph &G)
{
     int size = G.size();
     Graph preds = Graph(size, vector<int>(size, -1));
     for (int i = 0; i < size; i++)
     {
          for (int j = 0; j < size; j++)
          {
               if (G[i][j] != inf)
               {
                    preds[i][j] = i;
               }
          }
     }
     return preds;
}


/*
  Finds the shortest path from 'i' to 'j' in 'preds' and saves it in 'path'.
 */
void shortest_path(const Graph &preds, const int i, const int j, vector<int> &path)
{
     if (i == j)
     {
          path.push_back(i);
     }
     else if (preds[i][j] == -1)
     {
          path.clear();
          return;
     }
     else
     {
          shortest_path(preds, i, preds[i][j], path);
          path.push_back(j);
     }
}

/*
  Read in a file and return an adjacency matrix.

  Contents of the file should be of the form:

  0 0
  0 1
  2 3

  Graph is assumed to be undirected and unweighted.
 */
Graph read_graph(char filename[])
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
     Graph graph = Graph(num_nodes, vector<int>(num_nodes, inf));
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
