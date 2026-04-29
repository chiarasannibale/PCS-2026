#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "algorithms.hpp" //sorted e algoritmi sort
#include "randfiller.h"   //per riempire vettori
#include <random>

int main() {
	std::random_device rd;
	randfiller rf; //iniziallizzo oggetto fornito
	
	//1. test su 100 vettori DIMENSIONE CASUALE
	for (int i=0; i < 100; i+=1){
		std::vector<int> v; 
		int dim = rd() % 1000; //in tal modo diamo limite alla dim
		
		
		v.resize(dim); 
		//riempimenti casuale del vettore
		rf.fill(v, -1000, 1000); 
		
		//eseguo algoritmo
		quick_sort(v, 0, dim -1); 
		
		
		//verifico che sia ordinato
		if (!is_sorted(v)) {
			std::cout << "Errore: l'algoritmo quick sort ha fallito al test numero" << i << "\n"; 
			return EXIT_FAILURE; 
		}
	}
	
		// 2. test su un vettore di stringhe
		std::vector<std::string> s = {"bottiglia", "acqua", "aula", "studio", "uno", "rosso", "verde", "giallo", "blu", "biennale", "evoluzione"}; 
		
		//eseguo algoritmo
		quick_sort(s, 0, 10); 
		
		//verifico che sia ordinato
		if (!is_sorted(s)) {
			std::cout << "Errore: l'algoritmo quick sort ha fallito al test delle stringhe " << "\n"; 
			return EXIT_FAILURE; 
		}

		//se non vengono segnalati problemi alsistema operativo allora i test sono avvenuti con successo
	
		std::cout << "Tutti i vettori e le stringhe testati sono stati ordinati con successo con Quick Sort"<< "\n";
		return EXIT_SUCCESS; 
	
}