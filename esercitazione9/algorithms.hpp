#include "containers.hpp"
#include "unidirected_graph1.hpp"
#include<map>
#pragma once
// funzione template i base al contenitore che accetto (coda, stack)
template <typename T, typename contenitore>

//funzione template (per ogni tipo di nodo)

// NOTA: non implementtiamo un ciclo if in quanto i  metodi hanno lo stesso nome, 
//dunque rispetto al contenitore che vienne passato usa il metodo di quel rispettivo container
unidirected_graph<T> graph_visit(unidirected_graph<T>& G, T s, contenitore& c ){
	unidirected_graph<T> albero;  //inizializzo grafo da restituire di default messo a vuoto
	
	// MAPPA VISITATI: nodo---> {0, 1}
	std::map<T, bool> visitati; 
	
	
	//START: visito il nodo sorgente del grafo
	c.put(s); 
	visitati[s]= true; //inserisco il vertice nel sett di vertici visitati
	
	// finchè il contenitore non è vuoto, 
	while (!c.empty()){
		
		// DEQUEUE
		T u = c.get();
		
		//LA =  LISTA DI ADIACENZA
		//foreach (w in LA[u])--> trovo i nodi vicini a u,
		//se non li ho visitati li visito e creo l'arco
		
		for (auto v : G.neighbours(u)){
			
			// se non ho ancora visitato il nodo v, lo visito e creo l'arco
			if(!visitati[v]){
				visitati[v] = true; 
				albero.add_edge(u, v); 
				
				// per la coda sarebbe q.enqueue dello pseudocodice
				c.put(v); 
				
			}
		}
	}
	return albero; 
}


//funzione dfs ricorsivo: passiamo il grafo per riferimento in modo tale che 
//tutte le chiamate ricorsive modifichino la stessa mappa
template <typename T> 
void  dfs(unidirected_graph<T>& G, T u, std::map<T, bool>& visitati, unidirected_graph<T>& albero ){
	
	//segnamo il nodo corrente come visitato
	visitati[u]= true; 
	
	//esploro tutti i vicini del nodo u
	for (T v : G.neighbours(u)){
		
		//se il vicino v non l'ho ancora visitato allora lo visito
		if (!visitati[v]){
			
			//aggiungo l'arco (u, v) alla visita
			albero.add_edge(u, v); 
			
			//faccio lo stesso per il nodo v ricorsivamente
			dfs(G, v, visitati, albero); 
			
		}
	}
}

//scriviamo ora la vera e propria funzione
// prende in input il grafo da visitare G e il nodo sorgente 
template <typename T> 
unidirected_graph<T> recursive_dfs( unidirected_graph<T>& G, T s){
	
	unidirected_graph<T> albero;  //inizializzo grafo da restituire di default messo a vuoto
	
	// MAPPA VISITATI: nodo---> {0, 1}
	std::map<T, bool> visitati; 
	
	//inizializziamo tutti i nodi del grafo come non visitati
	for (T nodo : G.all_nodes()){
		visitati[nodo] = false; 
	}
	
	//effettuiamo la ricorsione a partire dal nodo sorgente
	dfs(G, s, visitati, albero); 
	
	//restituisco albero popolato dalla dfs
	return albero; 
}; 



	
