#include <vector>
#include <iostream>
#include <optional>
#include <algorithm> //funzioni per manipolazione di dati
#include <utility>   //per usare le swap
#include <limits>	 //usiamo inf dalla libreria standard

template<typename T>
bool is_sorted(const std::vector<T>& A)
{	
	//caso base: un vettore con 0 elementi o 1 elemento è ordinato per def
	if (A.size() <=1 ){ 
		return true; 
	}
	
	//iterazione sul vettore considerando el adiacenti
	//partiamo da zero e arriviamo fino alla lungjezza di x -1 poichè nel ciclo
	//usiamo i+1(altrimenti andremmo fuori dal limite
	for ( int i= 0; i < A.size() -1; i++){
		if (A[i] > A[i+1]) {
			return false; 
		}
	}
	
	//se il proggramma non ritona false allora il vettore è ordinato
	return true; 
}

// Bubble Sort
template<typename T>
void bubble_sort(std::vector<T>& A) {
	
	if (is_sorted(A)) {
		return;
	}
	
	for (int i = 0; i < A.size()-1; i++)  {   // i va da 0 a N-2
		for (int j = A.size() -1; j > i; j-=1){ //j scende verso i+1
			if(A[j] < A[j -1]) {               //confronto tra adiacenti
				std::swap(A[j] , A[j - 1]);    //scambio i deu elementi 
			}
			
		}
	}
}

//Insertion sort
template<typename T>
void insertion_sort(std::vector<T>& A) {
		
	if (is_sorted(A)) {
		return;
	}
	
	for (int j = 1; j < A.size(); ++j) { //partiamo dal secondo elemento
		T key = A[j]; //elemento da inserire
		int i = j - 1; //indice dell'elemento precedente a quello sui cui scorre il for
		
		//sposta elementi maggiori della chiave a destra
		while (i >= 0 && A[i] > key) {
			A[i + 1] = A[i]; 
			i = i -1;  
		}
		A[i + 1] = key; 
	}
}


//Selection sort
template<typename T>
void selection_sort(std::vector<T>& A) {
		
	if (is_sorted(A)) {
		return;
	}
	
	for (int i = 0; i < A.size()-1; i++) {
		int min = i; //primo elemento sia il minimo
		for (int j = i +1; j < A.size(); j++){
			if (A[j] < A[min]) {
				min = j; 
			}
		}
		std::swap(A[i], A[min]); 
	}
}

