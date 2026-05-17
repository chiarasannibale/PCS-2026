#include <iostream>
#include <string>
#include <fstream>
#include "containers.hpp"
#include "dijkstra.hpp"
#include "algorithms.hpp"

template <typename T>
void save_to_dot(const unidirected_graph<T>& G, const std::string& filename) {
	std::ofstream file(filename); // apre un file con il nome scelto
	
	//scrive sul file gli archi del grafo
	file << "graph" << "{"<< std::endl; 
	for (const auto& edge : G.all_edges()) {
		file << " " << edge.from() << "--" << edge.to()<< ";" << std::endl; 	
		}
	file <<"}"<< std::endl; 
}

int main(){
	unidirected_graph<int> G; 
	G.add_edge(1,2); 
	G.add_edge(1,3);
	G.add_edge(1,4);
	G.add_edge(1,6);
	G.add_edge(2,4);
	G.add_edge(2,5);
	G.add_edge(2,7);
	G.add_edge(3,6);
	G.add_edge(4,7);
	G.add_edge(4,6);
	G.add_edge(5,7);
	G.add_edge(6,7);
	G.add_edge(6,8);
	G.add_edge(9,8);
	G.add_edge(7,9);
	
	save_to_dot(G, "G_tree.dot"); 
	
	fifo<int> s; 
	auto bfsG=graph_visit(G, 1, s); 
	save_to_dot(bfsG, "bfs.dot" );
	
	lifo<int> q; 
	auto dfsG=graph_visit(G, 1, q); 
	save_to_dot(dfsG, "dfs.dot" );
	 
	auto dfsG_r=recursive_dfs(G, 1); 
	save_to_dot(dfsG_r, "recursive_dfs.dot" );
	
	//algoritmo dijkstra
	auto dijkstraG = dijkstra(G, 1); 
	
	//salvo l'albero nel file
	save_to_dot(dijkstraG, "dijkstra.dot"); 
	
	// Stampa conclusiva degli archi dell'albero di Dijkstra nel terminale
    std::cout << "graph  {\n";
    for (const auto& edge : dijkstraG.all_edges()) {
        std::cout << "  " << edge.from() << " -- " << edge.to() << ";" << std::endl;
    }
    std::cout << "}" << std::endl;

    return 0;
}