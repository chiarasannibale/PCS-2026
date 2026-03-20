#include <iostream>

int main()
{
	double ad[4] = {0.0, 1.1, 2.2, 3.3}; 
	float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7}; 
	int ai[3] = {0, 1, 2}; 
	
	int x = 1; 
	float y = 1.1; 
	
	//UNDEFINED BEHAVIOR
	//scrive 0 subito dopo y in memoria ma non aggiunge una variabilein memoria
	//allora perchè lozero viene associato ad x?
	//vado all'indirizzo di y, mi sposto di un posto avanti nella memoria e ci associo =0, 
	//aggiungendo questa riga allora stiamo sovrascrivendo il valore della x se essa sta subito dopo la y in memoria 
	(&y) [1] = 0; // perchè se levo questa riga stampa 1? che problema porta questa riga 
	
	std::cout << x<< "\n"; 
	std::cout <<&x<< "\n"; //indirizzo di x 
	std::cout<<&y<< "\n";  //indirizzo di y
	std::cout<<&y+1<< "\n";//indirizzo di y+4 byte dopo che coincide con quello della x pertanto 
	std::cout <<&ad[2]<< "\n"; 
	

	
	return 0; 
	
}

//0x7ffe13950a50= 140.729.226.955.344 = 0111 1111 1111 1110 0001 0011 1001 0101 0000 1010 0101 0000
//0x7ffe13950a90= 140.729.226.955.408 = 0111 1111 1111 1110 0001 0011 1001 0101 0000 1010 1001 0000
