#include <vector>
#include <optional>
#include <algorithm> //funzioni per manipolazione di dati
#include <utility>   //per usare le swap
#include <limits>
#include <thread>
#include <chrono>
#include "randfiller.h"
#include "confronto.cpp"
#include "timecounter.h"
#include "algorithms.hpp"

// OSS: mandiamo in input il riferimento di A in modo tale da 
// riprendere sempre il vettore che man mano stiamo aggiornando.
// OGNI MODIFICA FATTA DA UNA CHIAMATA RICORSIVA E VISIBILE A TUTTE LE CHIAMATE

// fissiamo la soglia trovata sperimentalmente nel file di confronto
const int soglia = 53;

template<typename T>
void quick_sort_aggiornato(std::vector<T>& A, int p, int r, int soglia) {
	
	if ( p < r){
		if ((r - p + 1) < soglia){
			insertion_sort(A, p, r); 
		} 
		else {
		int q = partition(A, p, r);
		// la funzione partition va a dividere il vettore in due parti in manìdo tale da
		// considerare il problema di ordinamento a dx e sx dell'elementopivot
		quick_sort_aggiornato(A, p, q - 1, soglia); 
		quick_sort_aggiornato(A, q + 1, r, soglia);
		}
	} 
}