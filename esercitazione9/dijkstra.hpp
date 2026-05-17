#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include "unidirected_graph1.hpp"
#include "unidirected_edge1.hpp"

//struttura della coda di priorita
template <typename T>


unidirected_graph<T>  dijkstra (const unidirected_graph<T>& G, T s){

	//inizializziamo pred e distance
	std::map<T, T> pred; 
	std::map<T, int> dist;
	std::map<T, bool> visitato; 
	
		
	//recuperiamo con le funzioni implementate la lisat degli archi per inizializzare i nodi
	//infinito
	const auto& lista_archi = G.all_edges();
	for (const auto& edge : lista_archi){
		T u = edge.from(); 
		T v = edge.to(); 
		if (dist.find(u)== dist.end()){
			dist[u] = INT_MAX; 
			visitato[u] = false; 
		}
		if (dist.find(v)== dist.end()){
			dist[v] = INT_MAX; 
			visitato[v] = false; 
		}
	}
	
	//inizializziamo il nodo sorgente marcato come visitato
	pred[s] = s; 
	dist[s] = 0; 
	visitato[s] =true;
	
	//coda che memorizza i nodi e inseriamo il nodo sorgente
	std::queue<T> q;  
	q.push(s); 

	
	while(!q.empty()){
		
		//estraggo un nodo dalla coda (in testa alla cosa)
		T u = q.front();  
		q.pop(); 
		
		//per ogni nodo vicino al nodo u se non è stato visitato allora lo visito
		for (T w : G.neighbours(u)){
			
			if (!visitato[w]){
				visitato[w] = true; 
				dist[w] = dist[u]+1; //non considerando i pesi ogi arco vale 1 per convenzione
				pred[w] = u;         //impostiamo il predecessore per tenere traccia del percorso
			
			
			//NOTA : non inseriamo (if dist[w] > dist[u] + W ) perchè 
			//la prima volta che incontriamo un nodo ci siamo arrivati sicuramente con il 
			//minor numero di archi pertanto non abbiamo neanche bisogno di aggiornare la distanza di un nodo visitato
			
				q.push(w); //inserisco il vicino in fondo alla coda
			}
		}
	}
	
	//COSTRUZIONE ALBERO DA RESTITUIRE 
	unidirected_graph<T> albero; 
	for (const auto& coppia : pred){
		T figlio = coppia.first; 
		T padre = coppia.second; 
		if(figlio!= padre){
			albero.add_edge(padre,figlio); 
		}
	}
	return albero; 
}