#include "unidirected_graph1.hpp"
#include "unidirected_edge1.hpp"
#include "dfs.hpp"
#include <vector>
#pragma once
#include <Eigen/Dense>
#include <iostream>
#include <iterator>
#include <set>
#include <map>



// Funzione che genera il grafo esteso a partire da G e da C
unidirected_graph<int> GrafoEsteso(const unidirected_graph<int>& G, const unidirected_graph<int>& C) {
    int N = G.all_nodes().size(); // Numero di nodi di G
    
    unidirected_graph<int> G1;
	
	

    // Iteriamo su tutti gli archi del grafo G
    for (const auto& e : G.all_edges()) {
        int u = e.from();
        int v = e.to();

        // Verifichiamo se l'arco appartiene a C 
        bool in_C = false;
        for (const auto& edge_C : C.all_edges()) {
            if (edge_C == e) { 
			in_C = true; 
			break; 
			}
        }

//consideramo positivi i nodi da 1 a N e negativi da N+1 a 2N

        if (in_C) {
            // Se l'arco è in C, applichiamo la logica incrociata:
            // (u+, v-) e (u-, v+)
            G1.add_edge(u, v + N);     // u+ -> v-
            G1.add_edge(u + N, v);     // u- -> v+
        } else {
            // Se l'arco NON è in C:
            // (u+, v+) e (u-, v-)
            G1.add_edge(u, v);         // u+ -> v+
            G1.add_edge(u + N, v + N); // u- -> v-
        }
    }
    return G1;
}

// Restituisce un vettore di cammini
Eigen::VectorXd Bellman_Ford(const unidirected_graph<int>& G, int sorgente,const std::map<unidirected_edge<int>,double>& pesi_archi){
	
	
	std::map <int,int> pred; // key=figlio, //value=padre
	std::map <int,double> dist; //key = nodo, value= distanza del nodo dalla sorgente
	
	//poichè nessun cammino può essere più lungo della somma di tutti gli archi, utilizzo come limite la somma totale 
	//di tutti i pesi presenti nel grafo e aggiungo 1.
	double tot_pesi=0;
	for (const auto& w : pesi_archi){
		tot_pesi+=w.second;
	}
	tot_pesi+=1;
	
	//inizializzazione a infinito (limite)
	for (const auto& nodo : G.all_nodes()) {
        dist[nodo] = tot_pesi;
    }
	
	//inizializzo nodo sorgente
    dist[sorgente] = 0;
	
	
	int n = G.all_nodes().size(); 
	for(int i= 0; i < n; i++ ){
		
		for(const auto& u :  G.all_nodes()){

			for (const auto& v: G.neighbours(u)){
				unidirected_edge<int> e(u,v);
				double w = pesi_archi.at(e);
				//ho costruito edge_weights come una mappa che associa ad ogni arco il peso associato
				//--> W={(u,v),peso) : u,v in G}
				if(dist[u] != tot_pesi && dist[v] > dist[u]+w){
					dist[v]=dist[u]+w;
					pred[v]=u;				
				}
			}
		}
	}	
	
	
	
    // Controllo cicli negativi
    for (const auto& u : G.all_nodes()) {
        for (const auto& v : G.neighbours(u)) {
            if (dist[u] != tot_pesi && dist[v] > dist[u] + pesi_archi.at(unidirected_edge<int>(u,v))) {
                // Rilevato ciclo negativo
            }
        }
    }
	
	//int  k = G.all_edges().size()-G.all_nodes().size()+1; 
	int m =  G.all_edges().size(); 
	int N= G.all_nodes().size()/2; //noi diamo alla funzione in grafo sdoppiato G1 che avrà 2N nodi
	// Costruiamo il vettore di incidenza C
	
	Eigen::VectorXd cammino= Eigen::VectorXd::Zero(m); 
    for (auto const& tupla : pred) {
		//tupla=(key,value)
		int figlio=tupla.first;
		if (figlio!=sorgente){
			int padre= tupla.second;
			
			//adesso ci riportiamo al caso del grafo senza segni:
			if (padre>N){
				padre=padre-N;
			}
			if (figlio>N){
				figlio=figlio-N;
			}
			unidirected_edge<int> e(padre, figlio);
			int i = G.edge_number(e);
			//il verso canonimo è sempre min-->max : se andiamo dal figli al padre, allora -1; altrimenti +1 (verso canonico)
			if (padre<figlio){
				cammino(i)=1;
			}
			else{
				cammino(i)=-1;
			}
			
			if (figlio==sorgente+N){
			return cammino;
			}
		}
    }
	
	//A questo punto nodo sorgente+N non è mai stato "toccato"
	// ovvero non è raggiungibile dalla sorgente da nessun cammino
	return Eigen::VectorXd::Zero(m);

}


Eigen::MatrixXd DePina(unidirected_graph<int> G, std::map<unidirected_edge<int>,double> pesi_archi){
	
	int  k = G.all_edges().size()-G.all_nodes().size()+1; 
	
	lifo struttura_lifo;
	unidirected_graph<int> T = dfs(G, 1, struttura_lifo);
	unidirected_graph<int> C = G-T; 
	int m =  G.all_edges().size(); 
	
	//inizalizzo k vettori di dimensione m a 0 (default)
	
	Eigen::MatrixXd S(m,k); 
	
	int z =0; 
	// Generiamo S = [S_1;  S_2; .. S_k]
	for (const auto& e : C.all_edges()){
		int i  = C.edge_number(e); 
		S(i,z) = 1;
		z++; 
	}
	unidirected_graph<int> G1 = GrafoEsteso(G,C);
	
	// In DePina(), dopo aver costruito G1:
	std::map<unidirected_edge<int>, double> pesi_archi_estesi;
	int N = G.all_nodes().size();

	for (const auto& e : G1.all_edges()) {
		int u = e.from();
		int v = e.to();
		
		// Riconduco ai nodi originali senza usare l'operatore inline '?'
		int u_orig = u;
		if (u > N) {
			u_orig = u - N;
		}

		int v_orig = v;
		if (v > N) {
			v_orig = v - N;
		}
		
		unidirected_edge<int> e_orig(u_orig, v_orig);
		if (pesi_archi.count(e_orig)) {
			pesi_archi_estesi[e] = pesi_archi.at(e_orig);
		}
	}

	//generiamo i cammini minimi [C_1, .., C_k]
	Eigen::MatrixXd cammini(m, k);
	int t = 0; 
	
	for (const auto& e : C.all_edges()) {
		int u = e.from();
		
		// output Bellman_Ford= VectorXi
		cammini.col(t) = Bellman_Ford(G1, u, pesi_archi_estesi);
        t++;
		
		//pseudocodice DePina
		for (int i = 0; i < k; i++ ) {
			// C_i è il ciclo trovato da Bellman-Ford
			Eigen::VectorXd Ci = cammini.col(i);
			S.col(i) = Ci;
			
			for (int j = i + 1; j < k; j++) { //
				Eigen::VectorXd Si = S.col(i);
				// Calcolo prodotto scalare su GF(2):
				
				if (Si.dot(Ci)==1) {
					// Operazione XOR non è definito per gli oggetti vettoriali di Eigen e per vettori di tipo bool
					S.col(j) = (S.col(j).array().cast<bool>() ^ S.col(i).array().cast<bool>()).cast<double>();
				}
			}
		}
	}
	return cammini; 	
}