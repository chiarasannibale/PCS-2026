#include "dfs.hpp"
#include "unidirected_graph1.hpp"
#include "unidirected_edge1.hpp"
#include <Eigen/Dense>
#pragma once

template <typename T> 
bool findpath(unidirected_graph<T> T_albero, T u, T v,std::map<T, bool>& visitato, std::vector<T>& cammino){
	
	visitato[u] =  true;
	cammino.push_back(u); 
	
	//trovo v e esco
	if (u==v) { 
		return true; 
	}

	//continuo ricorsivamente cercando nei nodi vicini il nodo corrente n 
	//fintanto che trovo v. A questo punto ho creato il cammino che va da u a v 	
	for (const auto& n : T_albero.neighbours(u)){
		if (!visitato[n]){
			if (findpath(T_albero, n, v, visitato, cammino)){
				return true; //1: ho trovato il cammino 
			}
		}
		
	}
	
	//se un certo nodo non porta a v allora "torno indietro"
	cammino.pop_back();   // backtrack
    return false;
}

template <typename T> 
std::vector<std::vector<T>> cicli_fondamentali(unidirected_graph<T>& G){
	
	//1. calcoliamo T grafo DFS 
	lifo struttura_lifo;
	T primo_nodo= *G.all_nodes().begin();
	unidirected_graph<T> T_albero= dfs(G, primo_nodo, struttura_lifo); 
	
	 
	//2. Coalbero C, lo inizializzo e ne calcolo al differenza 
	unidirected_graph<T> C; 
	
	C = G-T_albero; 
	
	//inizializzo la lista di cicli 
	std::vector<std::vector<T>> cicli;
	
	//. Per ogni arco di C trovo il ciclo fondamentale 
	for (const auto& arco : C.all_edges()) {
        T u = arco.from();
        T v = arco.to();
		
		std::map<T, bool> visitato;
        for (const auto& nodo : T_albero.all_nodes()){
            visitato[nodo] = false;
		}
		
		std::vector<T> cammino;
		
		//il percorso tra u e v necessariamente esiste perchè G è un grafo non orientato
		//ed è chiuso pertanto per ogni (u, v) in C esiste un cammino tra u e v in T
		
		findpath(T_albero, u, v, visitato, cammino); 
		
		//cammino = [u, ..., v]
		//chiudiamo il ciclo tornando in u 
		cammino.push_back(u); 
		cicli.push_back(cammino); 
	}
	return cicli; 
}

template <typename T> 
//Funzione che  partire dai cicli fondamentali ci restituisce la matrice di incidenza 
Eigen::MatrixXd matrice_incidenza_cicli(unidirected_graph<T>& G, const std::vector<std::vector<T>>& cicli) {
    int k = cicli.size();             // Numero di cicli fondamentali
    int m = G.all_edges().size();      // Numero totale di archi nel grafo G

    // Inizializziamo la matrice B (k righe, m colonne) a tutti zeri
    Eigen::MatrixXd B = Eigen::MatrixXd::Zero(m,k);

    // Iteriamo su ciascun ciclo fondamentale
    for (int i = 0; i < k; i++) {
        const auto& cammino = cicli[i];

        // Un ciclo è rappresentato come sequenza di nodi [n0, n1, ..., n_p] 
        // ha archi formati dalle coppie consecutive (cammino[j], cammino[j+1])
        for (size_t j = 0; j < cammino.size() - 1; j++) {
            T u = cammino[j];
            T v = cammino[j + 1];

            // Creiamo l'arco corrispondente(il costruttore della classe lo ordina n_from < n_to)
            unidirected_edge<T> e(u, v);

            // Troviamo l'indice dell'arco rispetto all'ordine di inserimento in G
            int indice_arco = G.edge_number(e);

            // Se l'arco appartiene al grafo, marchiamo la presenza nel ciclo: teniamo conto del verso 
			//in cui la corrente percorre la maglia, ovvero positiva se dal nodo più piccolo 
			//a quello più grande, negativa altrimenti
            if (indice_arco != -1) {
				// Confronta il verso di percorrenza del ciclo (u -> v) 
				// con il verso canonico dell'arco in G (e.from() -> e.to())
				
				if (u == e.from() && v == e.to()){
					// Il verso del ciclo coincide con quello dell'arco in G
					B(indice_arco, i) = 1;
				} else {
					// Il verso del ciclo è opposto a quello dell'arco in G
					B(indice_arco, i) = -1;
				}
            }
        }
    }

    return B;
}