#include <map>
#include <vector>
#include <set>
#include "unidirected_edge1.hpp"
#pragma once
//includo la classe implemenytata per costruire gli archi


template <typename T>
class unidirected_graph {
private: 
	//costruiamo il grafo come mappa implementando la lista di adiacenza
	// Mappa : nodo --->  lista di noodi vicini
	// (chiave) ---> valore(lista dei nodi collegati al nodo chiave)
	//Indichiamo con LA la lista di adiacenza; 
	// lista_archi: lista ordinata degli oggetti unidirected_edge
	std::map<T, std::vector<T>> LA; 
	std::vector<unidirected_edge<T>> edges_list; 
public:

	//COSTRUTTORE DI DEFAULT: quando creo un nuovo grafo la funzione 
	//unidirected_graph() vienen chiamata e inizializza LA() = mappa vuota e 
	// archi = viene craeto un vettore vuoto
	
	unidirected_graph() {}; 
	
	//COSTRUTTORE DI COPIA: creo un grafo a partoire da un grafo esistente 
	//passo il grafo che andrò a copiare per rifermimento, il costruttore
	//copierà la sua lista di adiacenza e il vettore di archi
	// usiamo const--> la funzione può leggere i valori ma non modificarli
	
	unidirected_graph(const unidirected_graph<T>& other ) {
		LA = other.LA, 
		edges_list = other.edges_list; 
	};
	
	
	// 1.  ADD_EDGE: creo collegamento tra due nod u e v 
	// - creo arco (oggetto della classe implementata) 
	// - aggiorno la sua lista di adiacenza 
	void add_edge(T u, T v){
		
		//creo l'oggetto grazie al costruttore che abbiamo creato prima che
		//garantisce che from < to
		unidirected_edge e(u, v); 
		
		//aggiungo l'arco e in fondo al vettore con push_back, 
		//controllando che non sia già presente nella lista LA
		
		if(std::find(edges_list.begin(), edges_list.end(), e ) == edges_list.end()){
			//se non viene trovato allora lo inserisco (se find non trova l'elemento e restituisce il puntatore alla fine della lista)
			// inserisco e nella lista di adiacenza 
			edges_list.push_back(e); 
			//aggiungo la LA in entrambi i sensi della mappa perchè il grafo non è orinetato
			LA[u].push_back(v);
			LA[v].push_back(u); 
		}
	}
	
	
	// 2. NEIGHBOURS(): dato un nodo restituisce i suoi vicini
	std::vector<T> neighbours(T node) const{
		//il metodo count restituisce un valore booleano
		// se il nodo è presente nella LA-->1 sennò 0
		if (LA.count(node)){
			// se il nodo esiste prendo il vettore dei suoi vicini
			return LA.at(node); 
			// .at è metodo per accedere ai dati unicamente in modalità lettura
		}
		return{};  //se il nodo non ha vicini restituisce la lista vuota 
	}

	
	// 3. ALL_EDGES(): restituisce tutti gli archi di un grafo
	std::vector<unidirected_edge<T>> all_edges() const{
		return edges_list; 
	}
	
	
	// 4. ALL_NODES(): restituisce tutti i nodi di un grafo
	//(guardo le chiavi della mappa di adiacenza)
	// usiamo il set che consente di gestire automaticamente i duplicati
	std::set<T> all_nodes() const{
		
		//inizializzo il set dove andrò a salvare i nodi
		std::set<T> nodes; 
		
		// per ogni coppia chiave - valore nella lista di adiacenza/
		// salvo unicamente il primo termine (first = chiave, second = nodi)
		for (auto const& coppia : LA){
			nodes.insert(coppia.first); 	
		}
		return nodes; 
	}
	
	// 5. EDGE_NUMBER :  dato un arco restituisce la sua numerazione
	// all'interno del grafo (int)
	// TROVA LA POSIZIONE DI UN ARCO ALL'INTERNO DI UN GRAFO
	//RISPETTO L'ORDINE CON CUI E' STATO INSERITO NELLA LISTA_ARCHI
	
	//prendo in input l'arco e che voglio cercare
	int edge_number(unidirected_edge<T>& e){
		
		int count = 0; 
		//itero nel registro degli archi dalla prima posizione fino all'ultima con l'iteratore itor
		for (auto itor = edges_list.begin();  itor!= edges_list.end(); itor++ ){
			//se trovo l'arco cercato  restituisco la posizione attuale (dereferenziando l'iteratore)
			if (*itor == e) {
				
				return count; 
			}
			//altrimenti incrementiamo l'iteratore e continuiamo a scorrere sulla lista
			count++; 
		}
			//se non trovo quell'arco all'interno della lista degli archi
			return -1;  //arco non trovato
	}
	
	
	// 6. EDGE_AT :  dato un numero d’arco, restituisce il corrispondente oggetto 
	//arco all'interno del grafo
	unidirected_edge<T> edge_at(T indice) const{
		return edges_list.at(indice); 
	}


	// 7. - DIFFERENZA : G -G'
	// operativamente il grafo differenza contiene tutti i nodi necessari ma 
	// arch del primo grafo e non archi del secondo 
	unidirected_graph operator- (const unidirected_graph& other) const{
		
		// creo con costruttore di default un grafo vuoto
		unidirected_graph differenza; 
		
		//recupero la lista degli archi del secondo grafo
		auto other_edges = other.all_edges(); 
		
		//itero su tutti gli archi del grafo corrente G
		for (const auto& e : this->edges_list){
			
			//cerco l'arco e del grafo G all'interno del grafo G' (other)
			if (std::find(other_edges.begin(), other_edges.end(), e) == other_edges.end()) {
				
				//entro nell'if se non trovo l'arco e all'interno della lisat degli archi di G'
				// ----> l'arco appartiene solo al grafo G allora fa parte del grafo differenza
				
				//aggiungo l'arco trovato al risultato
				differenza.add_edge(e.from(), e.to()); 
				
			}
		}
		return differenza; 
	}
}; 