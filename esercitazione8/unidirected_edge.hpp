#include <algorithm>
#include <vector> 
#include <iostream>

template <typename T>

class unidirected_edge {
private: 
    T n_from;
    T n_to;

public:
    // Costruzione: from < to
    unidirected_edge(T u, T v) {
        n_from = std::min(u, v); 
		n_to = std::max(u, v); 
    }
	
	// OSS: marchiamo gli operatori come const perchè 
	// non modificano lo  stato dell'oggetto	
	//metodi from e to
	T from() const{return n_from; }
	T to() const{return n_to; }
	
	//operator <(di ordinamento): rende gli archi ordinabili
	bool operator<(const unidirected_edge& other) const{
		// se il nodo di partenza è diverso dell'altro nodo di partenza
		// li ordino in modo tale che il primo sia minore del secondo, se sono 
		//se sono uguali ordino unicamente il to (non entra nel ciclo if)
		if (n_from != other.n_from ){
			return n_from < other.m_from;
		}
		return n_to < other.n_to; 
	}
	
	//operator ==: (di ugualianza) stabilisce la logica di confronto 
	
	bool operator==(const unidirected_edge& other) const{
		return n_from == other.n_from && n_to == other.n_to; 
	}
}; 	
	//operator << (operatore di stampa)
template <typename T>
//operator << (operatore di stampa)

//std::ostream indica che stiamo passando l'oggetto per riferimento
	//std::cout << a esegue la stampa di a e restituisce un riferimento a std::cout.
	//A quel punto, il risultato diventa (riferimento a cout) << b, e così via.
	//Se non usassimo & nel tipo di ritorno, il concatenamento si romperebbe dopo il primo elemento.
	
	
//usiamo le funzioni getter : servono a "leggere" il valore di una variabile privata dall'esterno della classe,
// senza però permettere a nessuno di modificarla direttamente.
std::ostream& operator<< (std::ostream& os, const unidirected_edge<T>& e){
	os<< "{"<< e.from() << ","<< e.to() << "}"; 
return os; 
}; 

 

	