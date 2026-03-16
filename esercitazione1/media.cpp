#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int main(int argc, const char *argv[]) {
	// controllo se il file è dato in input
	if (argc<2) {
		std:: cerr << " Argomenti insufficienti: specificare nome file" << std:: endl;
		return 1; 
	}
	
	
	std:: string filename = argv[1]; 
	std:: ifstream ifs(filename); 
	
	if ( ifs.is_open() ) { 
		while( !ifs.eof() ) { // leggo fino alla fine il file
			string location;
			double temp1, temp2, temp3, temp4; 
			ifs >> location >> temp1 >> temp2 >> temp3>> temp4; 
			double media = (temp1 +temp2 +temp3 +temp4) / 4.0; 
			cout << location << "  " << media << "\n";
			}
	}
	else {
		// caso in cui non si riesca ad aprire il file 
		std:: cerr << "Errore apertura del file " << std:: endl;
		return 1; 
	}
		
	return 0;
}