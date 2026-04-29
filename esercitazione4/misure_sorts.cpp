#include <iostream>
#include <vector>
#include <string>
#include <algorithm> //per std::sort
#include <thread>
#include <chrono>
#include "randfiller.h"
#include "algorithms.hpp"
#include "timecounter.h"


// Confronto algoritmi sort con quelli della libreria standard

int main() {
	randfiller rf; //inizializzo oggetto per riempimento casualedei vettori
	timecounter tc; //inizializzo timecounter
	
	std::cout<< "Dimensione, BubbleSort, InsertionSort, SelectionSort, MergeSort, QuickSort, StdSort"<< "\n";
	
	//dimensione dei vari vettori raddoppia ad ongi passo
	for (int n = 2; n <= 8192; n*=2) {
		
		std::vector<std::vector<int>> vettori(100); 
		
		for (int i = 0; i < 100; i++) {	
			vettori[i].resize(n); 
			rf.fill(vettori[i], -1000, 1000); //riempimento casuale dei vettori ridimensionati
		}
		//stampo dimensione n considerata 
		std::cout<< n;  
		
		//---misura bubble sort---
		std::vector<std::vector<int>> set1 = vettori; 
		
		tc.tic(); //start
		
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			bubble_sort(set1[i]); 
		}                   
		
		double tempo_tot1 = tc.toc(); //stop
		double media1 = tempo_tot1 / 100.0; 
		std::cout<<","<< media1 ; 
		
		//---misura insertion sort---
		std::vector<std::vector<int>> set2 = vettori; 
		
		tc.tic(); 
		
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			insertion_sort(set2[i]); 
		}
		
		double tempo_tot2 = tc.toc(); //stop
		double media2 = tempo_tot2 / 100.0; 
		std::cout<<","<< media2 ; 

		
		//---misura selection sort---
		std::vector<std::vector<int>> set3 = vettori; 
		
		tc.tic(); 
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			selection_sort(set3[i]); 
		}
		
		double tempo_tot3 = tc.toc(); //stop
		double media3 = tempo_tot3 / 100.0; 
		std::cout<<","<< media3 ; 
		
		
		//---misura merge sort---
		std::vector<std::vector<int>> set4 = vettori; 
		
		tc.tic(); 
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			merge_sort(set4[i], 0, set4[i].size() - 1); 
		}
		
		double tempo_tot4 = tc.toc(); //stop
		double media4 = tempo_tot4 / 100.0; 
		std::cout<<","<< media4 ; 
		
		//---misura quick sort---
		std::vector<std::vector<int>> set5 = vettori; 
		
		tc.tic(); 
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			quick_sort(set5[i], 0, set5[i].size() -1); 
		}
		
		double tempo_tot5 = tc.toc(); //stop
		double media5 = tempo_tot5 / 100.0; 
		std::cout<<","<< media5 ; 
	
		//---misura standard sort---
		std::vector<std::vector<int>> set6 = vettori;  
		
		tc.tic(); 
		//faccio la media sul tempo di ordinamento di 100 vettori
		for (int i = 0; i < 100; i++){
			std::sort(set6[i].begin(), set6[i].end()); 
		}
		
		double tempo_tot6 = tc.toc(); //stop
		double media6 = tempo_tot6 / 100.0; 
		std::cout<<","<< media6 ; 

		
		//vado a capo per l'iterazione successiva
		std::cout<< "\n"; 
	}
	return EXIT_SUCCESS; 
}