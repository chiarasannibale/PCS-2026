#include <vector>
#include <optional>
#include <algorithm> //funzioni per manipolazione di dati
#include <utility>   //per usare le swap
#include <limits>

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


//Merge sort
template<typename T>
// la procedura merge (FUNZIONE FUSION) riceve in input un array A 
//e tre indici p(sx), q(metà), r(dx)
void merge(std::vector<T>& A, int p, int q, int r ) {

	//calcolo lunghezze
	int n1 = q - p + 1; //lunghezza del vettore a sx
	int n2 = r - q;     // lenghezza del vettore a dx: da q+1 a r
	
	//creo due array temporanei trà la metà di dx e la metà di sx
	//Considero che nello pseudocodice gli indicipartono da 1 = posizione 0 su c++
	std::vector<T> L(n1); 
	std::vector<T> R(n2); 
	
	//copio dati
	for (int i = 0; i < n1; i++) {
		L[i] = A[p + i]; //per i = 0 ---> p è l'inizio del blocco
	}
	for (int j = 0; j < n2; j++){
		R[j] = A[q + 1 + j]; //+1 per non includere il punto medio stesso
	}
	
	int i = 0; //indice per L
	int j = 0; //indice per R
	int k = p; //indice per A
	//confronta e  fonde finchè uno dei due vettori temporanei finisce
	//ciclo continua fintanto che ci sono elementi del vettore da confrontare 
	//funzione FUSION per fondere i due vettori ordinati in ordine crescente 
	//confronta ogni elemento nelle posizioni dei vettori di destra e quello di sinistra
	//i scorre sul vettore a sx
	//j scorre sul vettore di dx
	// confronta i due elementi
	//prende quello più piccolo e lo inserisce nell'array A in posizione k
		
	// 1. CICLO DI CONFRONTO
	while (i < n1 && j < n2){
		if (L[i] <= R[j]){
			A[k] = L[i]; 
			i++; 
		}else {
			A[k] = R[j];
			j++; 
		}
		k++; 
	}
	
	// 2. GESTIONE ELEMENTI RIMANENTI
	
	//Copia elementi restanti di L, a condizioni che ce ne siano
	// (ovvero se l'indice i è minore della lunghezza del vettore)
	while (i < n1){
		A[k] = L[i]; 
		i++; 
		k++; 
	}
	//Copia elementi rimanenti di R
	while (j < n2){
		A[k] = R[j]; 
		j++; 
		k++; 
	}
	
}

//effettiva funzione mergesort
template<typename T>	
	void merge_sort(std::vector<T>& A, int p, int r)
{
	if (p < r) {
		//punto medio per la divisione dell'array
		int q = (p+r)/ 2; 
		
		//FUNZIONE RICORSIVA: ordino la prima metà
		merge_sort(A, p, q); 
		
		//FUNZIONE RICORSIVA: ordino la seconda metà
		merge_sort( A, q+1, r); 
		
		//FUNZIONE FUSION
		merge( A, p, q, r); 
	}
}	
	
//quick sort
// A: vettore da partizionare
// p: indice iniziale
// r: indice finale(pivot)
template<typename T>
int partition(std::vector<T>& A, int p, int r){
	T x = A[r];  //pivot è l'ultimo elemento
	int i = p - 1; 
	
	for (int j = p; j <= r - 1; j++){
		if (A[j] <= x){
			i++;
			std::swap(A[i], A[j]); 
		}
	}
	std::swap(A[i+1], A[r]); 
	return i + 1;  //rango
}

template<typename T>
void quick_sort(std::vector<T>& A, int p, int r) 
{

	if ( p < r){
		int q = partition(A, p, r); 
		quick_sort(A, p, q - 1); 
		quick_sort(A, q + 1, r);
	}
}
