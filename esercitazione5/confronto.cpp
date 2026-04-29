#include <vector>
#include <optional>
#include <algorithm> //funzioni per manipolazione di dati
#include <utility>   //per usare le swap
#include <limits>
#include <thread>
#include <chrono>
#include "randfiller.h"
#include "algorithms.hpp"
#include "timecounter.h"

//Da questa impplementazione trovo sperimentalmente la soglia
// andando a studiare l'andamento del tempo di esecuzione dei 
//vari algoritmi

int main(){
	//1. Preallocazione di vettori
	
	randfiller rf; //inizializzo oggetto per riempimento casualedei vettori
	timecounter tc; //inizializzo timecounter
	
	std::cout<< "Dimensione, InsertionSort, QuickSort"<< "\n";
	
	//testiamo su dimensione ceh va da 2 a 100 per trovare l'incrocio 
	for (int n = 2; n <= 100; n+=1) {
		
		//genero una lista di vettori:100 campioni per avere una media stabile
		std::vector<std::vector<int>> vettori(100); 
		
		for (int i = 0; i < 100; i++) {	
			vettori[i].resize(n); 
			rf.fill(vettori[i], -1000, 1000); //riempimento casuale dei vettori ridimensionati
		}

	std::cout<< n << ":";
	//---misura quick sort---
	std::vector<std::vector<int>> set1 = vettori;	
	tc.tic(); 
	//faccio la media sul tempo di ordinamento di 100 vettori
	for (int i = 0; i < 100; i++){
		quick_sort(set1[i], 0, set1[i].size() -1); 
	}
		
	double tempo_tot1 = tc.toc(); //stop
	double media1 = tempo_tot1 / 100.0; 
	std::cout << media1<< ",";
	//---misura insertion sort---
	std::vector<std::vector<int>> set2 = vettori; 
		
	tc.tic(); 
		
	//faccio la media sul tempo di ordinamento di 100 vettori
	for (int i = 0; i < 100; i++){
		insertion_sort(set2[i]); 
	}
		
	double tempo_tot2 = tc.toc(); //stop
	double media2 = tempo_tot2 / 100.0; 


   
    std::cout << media2 << std::endl;
	
	}
	return 0; 
}