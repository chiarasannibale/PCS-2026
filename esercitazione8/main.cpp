#include <iostream>
#include "unidirected_graph.hpp"

int main(){
	
	// TEST 1. archi invarianti ((1, 2) == (2, 1))
	unidirected_edge e1(2, 1); 
	
	std::cout << "L'arco {2, 1}: " << e1 << std::endl; 
	//stampa (from -- to ) con from < to
	
	// TEST 2.: Costruzione grafo G1 per aggiunta di archi
	
	unidirected_graph<int> G1; 
	
	G1.add_edge(1, 2); 
	G1.add_edge(2, 3); 
	G1.add_edge(3, 1); 
	G1.add_edge(2, 3); //essendo un duplicato non dovrebbe essere aggiunto
	
	std::cout << "Numero archi del grafo G1 (atteso: 3):  "<< G1.all_edges().size()<< std::endl; 
	
	// TEST 3. : funzione neighbours
	std::cout << "Nodi vicini del nodo 3: [ ";
	// ":" ---> per ogni v nei nodi adiacenti del nodo 3 nel grafo G1
	for (int v : G1.neighbours(3)) {
		std::cout << v << " "; 
	}
	std::cout << "]" << std::endl; 	
	
	
	//TEST 4. : EDGE_NUMBER
	
	//inizializzo un arco da cercare (da numerare)
	unidirected_edge e(2, 1);  //equivalente all'arco (2, 3)
	int i = G1.edge_number(e); 
	std::cout << "L'arco (1, 2) è il numero: " << i << std::endl; 
	std::cout << "L'arco alla posizione " << i <<" "<<  " è " << G1.edge_at(i) << std::endl; 
	
	//TEST 5. : differenza grafi 
	
	// inizializzo grafo G'=G2
	unidirected_graph<int> G2; 
	G2.add_edge(1, 2); 
	G2.add_edge(2, 3);
	
	//usooperatore - definito sui grafi
	unidirected_graph diff = G1 - G2;
	// diff dovrebbe contenere soloun arco (1, 3)
	std::cout << "La differenza G1 - G2 è il grafo: ";
	for (const auto& edge : diff.all_edges()) {
		std::cout << edge <<" "; 
	}
	std::cout << std::endl; 
	
	return 0; 
	
	
}; 