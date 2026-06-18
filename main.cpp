#include <fstream>
#include <iostream>
#include <string>
int main(int argc, const char *argv[]) {
	// controllo se il file è dato in input
	if (argc<2) {
		std:: cerr << " Argomenti insufficienti: specificare nome file" << std:: endl;
		return 1; 
	}
	
	
	std:: string filename = argv[1]; 
	std:: ifstream ifs(filename); 
	std::vector <componente> circuito;
	
	if ( ifs.is_open() ) { 
		std::string riga; 
		while( !ifs.eof()) { // leggo fino alla fine il file RIGA PER RIGA
			
			
			std::string tipo;  
			double quantità; 
			int n_from; 
			int n_to; 
			
			if(ifs >> tipo >> quantità >> n_from >> n_to){
				circuito.push_back(componente(tipo, valore, n_from, n_to)); 
			}
		}
	}
	else {
		// caso in cui non si riesca ad aprire il file 
		std:: cerr << "Errore apertura del file " << std:: endl;
		return 1; 
	}
	ifs.close(); 
	return 0;
}




