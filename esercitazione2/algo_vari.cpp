#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std; 

int main()
{
	static const int N = 10; 
	double arr [N];
	double minimo, massimo, somma = 0, num=0, dev; 
		for ( int i= 0; i< N; i++) {
			arr[i]= rand();
			somma+= arr[i]; 
		
			if (i == 0) {
				minimo = arr[i]; 
				massimo = arr[i]; 
			}
			else {
			
				minimo = min( minimo, arr[i]); 
				massimo= max( massimo, arr[i]);
			}
		} 
	
	
	double media = somma/N ;
	
	for  ( int i= 0; i< N; i +=1) {
		double diff = arr[i]-media; 
		num += diff*diff; 
	}
	dev = sqrt(num)/N; 
	
		
		
	cout <<"Dato il vettore: ["; 
		for (int i =0; i<N; i++) {
			cout<< arr[i] <<" "; 			
		}
		cout<< "]"<< "\n"; 
	cout << "Il minimo è: "<< minimo<< "\n"; 
	cout << "Il massimo è: "<< massimo << "\n"; 
	cout << "La media è: "<< media<< "\n"; 
	cout << "La somma è: "<< somma<< "\n"; 
	cout << "La deviazione standard è "<< dev<< "\n"; 
	
	return 0; 
}	