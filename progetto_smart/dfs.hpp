#include <stack> 
#pragma once

class lifo{
private:
	std::stack<int> s; //inizializzo la struttura stack
public: 
	//costruttore di default vuoto
	lifo (){}
	
	// PUT: inserisce in cima 
	void put(const int& val) {
		s.push(val); 
	}
	
	//GET: prende l'elemento in cima 
	int get() {
		int v = s.top(); //legge elemento in cima
		s.pop();         //lo rimuove
		return v; 
	}
	
	
	//EMPTY: verfica se il contenitore è vuoto
	bool empty(){
		return s.empty(); 
	}
};

// NOTA: non implementtiamo un ciclo if in quanto i  metodi hanno lo stesso nome, 
//dunque rispetto al contenitore che vienne passato usa il metodo di quel rispettivo container
unidirected_graph<int> dfs(unidirected_graph<int>& G, int s, lifo& c ){
	
	unidirected_graph<int> albero;  //inizializzo grafo da restituire di default messo a vuoto
	
	// MAPPA VISITATI: nodo---> {0, 1}
	std::map<int, bool> visitati; 
	
	
	//START: visito il nodo sorgente del grafo
	c.put(s); 
	visitati[s]= true; //inserisco il vertice nel sett di vertici visitati
	
	// finchè il contenitore non è vuoto, 
	while (!c.empty()){
		
		// DEQUEUE
		int u = c.get();
		
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
