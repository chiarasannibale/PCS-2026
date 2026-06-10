#include <algorithm>
#include <vector> 
#include <iostream>
#pragma once

//rappresenta la strutture del componente R o V
struct componente{
private: 
	std::string tipo;  
	double valore; 
	int n_from; 
	int n_to; 
	bool segno_positivo; 
	int indice; // indice del resistore (-1 se non è un resistore)
	
	
public:
	
    // Costruzione: from < to. Modifica da tipo, valore --> _tipo, _valore causa shadowing
	componente(std::string _tipo, double _valore, int u, int v,  int _indice) {
        this->tipo   = _tipo;
		this->valore = _valore;
		n_from = std::min(u, v); 
		n_to = std::max(u, v); 
		
		//true se percorro la fem dal nodo minore al nodo maggiore: segno + sul nodo u
		segno_positivo=(u<v);
		indice = _indice;
    }
	
	//costruttore di default
	componente(){
		tipo = ""; 
		valore = 0.0; 
		n_from = 0; 
		n_to = 0; 
		indice = -1; 
	}
	
	// OSS: marchiamo gli operatori come const perchè 
	// non modificano lo  stato dell'oggetto	
	//metodi from e to
	
	std::string get_tipo() const { return tipo; }
    double get_valore() const { return valore; }
	int get_indice() const { return indice; }
	int from() const{return n_from; }
	int to() const{return n_to; }
	bool get_segno_positivo() const{ return segno_positivo;}
	
	//operator <(di ordinamento): rende gli archi ordinabili
	bool operator<(const componente& other) const{
		// se il nodo di partenza è diverso dell'altro nodo di partenza
		// li ordino in modo tale che il primo sia minore del secondo, se sono 
		//se sono uguali ordino unicamente il to (non entra nel ciclo if)
		if (n_from != other.n_from ){
			return n_from < other.n_from;
		}
		return n_to < other.n_to; 
	}
	
	//operator ==: (di ugualianza) stabilisce la logica di confronto 
	
	bool operator==(const componente& other) const{
		return n_from == other.n_from && n_to == other.n_to; 
	}
}; 	
	//operator << (operatore di stampa)


//std::ostream indica che stiamo passando l'oggetto per riferimento
	//std::cout << a esegue la stampa di a e restituisce un riferimento a std::cout.
	//A quel punto, il risultato diventa (riferimento a cout) << b, e così via.
	//Se non usassimo & nel tipo di ritorno, il concatenamento si romperebbe dopo il primo elemento.
	
	
//usiamo le funzioni getter : servono a "leggere" il valore di una variabile privata dall'esterno della classe,
// senza però permettere a nessuno di modificarla direttamente.
std::ostream& operator<< (std::ostream& os, const componente& c){
	os << c.get_tipo() << " [ " << c.get_valore() << " ] Nodi: {" << c.from() << "," << c.to() << "}";
return os; 
}; 

 
