#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <iomanip>
#include "unidirected_graph1.hpp"
#include "unidirected_edge1.hpp"
#include "gradiente_coniugato.hpp"
#include "ciclifondamentali.hpp"
#include "DePina_Dijkstra.hpp"
#include "netlist.hpp"
#include "Sistema_lineare.hpp"

int main(int argc, const char *argv[]) {
	
	// controllo se il file è dato in input
	if (argc<2) {
		std:: cerr << " Argomenti insufficienti: specificare nome file" << std:: endl;
		return 1; 
	}

	
	std:: string filename = argv[1]; 
	std:: ifstream ifs(filename); 
	
	std::vector<componente> circuito;
	unidirected_graph<int> G ; 
	int contatore_R = 0;
	
	if ( ifs.is_open() ) { 
		std::string riga; 
		while( !ifs.eof()) { // leggo fino alla fine il file RIGA PER RIGA

			std::string tipo;  
			double valore; 
			int n_from; 
			int n_to; 
			
			//gestione righe vuote
			if(ifs >> tipo >> valore >> n_from >> n_to){
				int idx = -1;
				if (!tipo.empty() && tipo[0] == 'R') {
					idx = contatore_R++;
				}
				circuito.push_back(componente(tipo, valore, n_from, n_to, idx)); 
				G.add_edge(n_from, n_to); 
			}	
		}
		ifs.close(); 
	}

	else {
		// caso in cui non si riesca ad aprire il file 
		std:: cerr << "Errore apertura del file " << std:: endl;
		return 1; 
	}
	
	//controllo netlist vuota, presenza di almeno un resistore e di almeno un generatore
	if (circuito.empty()) {
		std::cerr << "Errore: la netlist e' vuota." << std::endl;
		return 1;
	}

	int n_resistori = 0;
	for (const auto& comp : circuito)
		if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') n_resistori++;
	
	if (n_resistori == 0) {
		std::cerr << "Errore: il circuito non contiene resistori." << std::endl;
		return 1;
	}
	
	int n_generatori = 0;
	for (const auto& comp : circuito)
		if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'V') n_generatori++;
	
	if (n_generatori == 0) {
		std::cerr << "Errore: il circuito non contiene generatori." << std::endl;
		return 1;
	}

	// parametri utilizzati per il gradiente coniugato
	unsigned int it_max = 1000;
    double res_tol = 1e-7;
    
	//0. Dati: v, R (R è la stessa per 1. 2. )
	Eigen::MatrixXd R = costruzione_R(G, circuito); 
	 
	//1. IMPLEMENTAZIONE DE PINA
	std::cout << "Opzione 1: ALGORITMO DI DE PINA" << std::endl;
    
	
	//Derefenziamo l'iteratore che punta al primo nodo (anzichè prendere il nodo 1 nel caso in cui non esista)
	int primo_nodo = *G.all_nodes().begin(); 
    std::vector<std::vector<int>> B1 = De_Pina(G, primo_nodo);
	Eigen::MatrixXd matrice_B1 = costruzione_B(B1); 
	
	
	
	//risolviamo il sistema (consideriamo la matrice B_R ristretta ai resistori)
	Eigen::MatrixXd matrice_B1_R = estrai_B_R(G, matrice_B1, circuito);
	
	Eigen::VectorXd vR1 = costruzione_v(G, matrice_B1, circuito);
	Eigen::VectorXd I_maglia_1= risoluzione_sistema(matrice_B1_R, R, vR1, it_max, res_tol); 
	// CONTROLLO : verifico il residuo del sistema dopo la soluzione B^T R B * i = v
	//il sistema è Ax=v, quindi il residuo sarà la norma di Ax-v
	Eigen::MatrixXd A1 = matrice_B1_R.transpose() * R * matrice_B1_R; 
	double residuo1 = (A1 * I_maglia_1 - vR1).norm();
	if (residuo1 > 1e-5) {
		std::cerr << "Attenzione: residuo alto dopo gradiente coniugato (De Pina): " << residuo1 << std::endl;
	}
	else { 
		std::cout << "Residuo del gradiente coniugato accettabile:  "<< residuo1 << std::endl;
	}

	//stampa matrice
	Eigen::IOFormat fmt(Eigen::StreamPrecision, 0, "\t", "\n");
	std::cout << std::fixed << std::setprecision(0);
    std::cout << "Matrice dei cicli di De Pina:\n" << matrice_B1_R << "\n\n";
	
	
	// Stampa risultati: per stampare le correnti usiamo l'indice del resistore
	// e non l'indice del grafo
	
	Eigen::VectorXd I_ramo_1 = matrice_B1_R * I_maglia_1;

	std::cout << "I risultati utilizzando l'algoritmo De Pina sono" << std::endl;
	for (const auto& comp : circuito) {
		if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') {
			
			int idx = comp.get_indice();
			double I = I_ramo_1(idx);
			double V = comp.get_valore() * I;
			std::cout << comp.get_tipo()
					  << ": V = " << V << " volts, I = " << I << " amps." << std::endl;
		}
	}

	
	//2. IMPLEMENTAZIONE CICLI NON NECESSARIAMOENTE MINIMI
	std::cout << "Opzione 2: CICLI FONDAMENTALI non minimi" << std::endl;
	std::vector<std::vector<int>> cicli_dfs = cicli_fondamentali(G);
	Eigen::MatrixXd matrice_B2 = matrice_incidenza_cicli(G, cicli_dfs);
	Eigen::MatrixXd matrice_B2_R = estrai_B_R(G, matrice_B2, circuito);
	
	
	Eigen::VectorXd vR2 = costruzione_v(G, matrice_B2, circuito);
	
	//risolviamo il sistema
	Eigen::VectorXd I_maglia_2 = risoluzione_sistema( matrice_B2_R, R, vR2, it_max, res_tol); 
	
	//Stampa matrice di incidenza B
	std::cout << std::fixed << std::setprecision(0);
	std::cout << "Matrice dei cicli non necessariamente minimi:\n" << matrice_B2_R << "\n\n";
	
	// Stampa risultati
	Eigen::VectorXd I_ramo_2 = matrice_B2_R * I_maglia_2;


	// CONTROLLO: verifico il residuo del sistema
	Eigen::MatrixXd A2 = matrice_B2_R.transpose() * R * matrice_B2_R;
	double residuo2 = (A2 * I_maglia_2 - vR2).norm();
	if (residuo2 > 1e-5) {
		std::cerr << "Attenzione: residuo alto dopo gradiente coniugato (DFS): " << residuo2 << std::endl;
	}
	else { 
		std::cout << "Residuo del gradiente coniugato accettabile:  "<< residuo2 << std::endl;
	}

	std::cout << "I risultati usando cicli fondamentali non minimi sono" << std::endl;
	for (const auto& comp : circuito) {
		if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') {
			int idx = comp.get_indice();
			double I = I_ramo_2(idx);
			double V = comp.get_valore() * I;
			std::cout << comp.get_tipo() 
					  << ": V = " << V << " volts, I = " << I << " amps." << std::endl;
		}
	}

	return 0;
}




