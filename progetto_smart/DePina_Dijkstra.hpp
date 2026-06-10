#pragma once
#include "unidirected_graph2.hpp"
#include "unidirected_edge1.hpp"
#include "dfs.hpp"
#include "dijkstra.hpp"
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <limits>
#include <iostream>
#include <Eigen/Dense>

template<typename T>
struct nodo_con_segno{
    T  nodo_G;
    bool sign; // true = "+", false = "-"
 
    
    bool operator<(const nodo_con_segno& other) const {
        if (nodo_G != other.nodo_G)
            return nodo_G < other.nodo_G;
        return sign < other.sign;
    }
 
    bool operator==(const nodo_con_segno& other) const {
        return nodo_G == other.nodo_G && sign == other.sign;
    }
};


//costruiamo G primo 
template<typename T>
unidirected_graph<nodo_con_segno<T>> grafo_esteso(
    const unidirected_graph<T>& G,
    const std::vector<int>& Si)   // vettore di incidenza dim = m
{
    auto archi = G.all_edges();
    size_t m = archi.size(); //num archi
 
    unidirected_graph<nodo_con_segno<T>> G1;
 
    for (size_t i = 0; i < m; i++) {
        T u = archi[i].from();
        T v = archi[i].to();
 
        nodo_con_segno<T> u_pos = {u, true};
        nodo_con_segno<T> u_neg = {u, false};
        nodo_con_segno<T> v_pos = {v, true};
        nodo_con_segno<T> v_neg = {v, false};
 
        if (Si[i] == 1) {
            // Arco attivo: connessioni "incrociate"
            G1.add_edge(u_pos, v_neg);
            G1.add_edge(u_neg, v_pos);
        } else {
            // Arco inattivo: connessioni "parallele"
            G1.add_edge(u_pos, v_pos);
            G1.add_edge(u_neg, v_neg);
        }
    }
    return G1;
}


 
template<typename T>
std::vector<std::vector<nodo_con_segno<T>>> tutti_cammini_minimi(const unidirected_graph<T>& G,const unidirected_graph<nodo_con_segno<T>>& G1)
{
    auto nodi = G.all_nodes();
	
    std::vector<std::vector<nodo_con_segno<T>>> cammini;
 
    for (auto& v : nodi) {
		nodo_con_segno<T> v_pos = {v, true};
        nodo_con_segno<T> v_neg = {v, false};        
		//RisultatoRicerca = mappa 
        std::map<nodo_con_segno<T>, nodo_con_segno<T>> predecessori= dijkstra(G1, v_neg);
 
        std::vector<nodo_con_segno<T>> cammino_minimo;
 
        // Se v+ non è raggiungibile da v-, aggiungo cammino vuoto
        if (predecessori.find(v_pos) == predecessori.end()) {
            cammini.push_back(cammino_minimo);
            continue;
        }
		
		nodo_con_segno<T> corrente = v_pos;
        while (!(corrente == v_neg)) {
            cammino_minimo.push_back(corrente);
            corrente = predecessori[corrente];
        }
		//chiuo il  ciclo 
        cammino_minimo.push_back(v_neg);
 
        // Il cammino è stato costruito al contrario (da v+ a v-), lo invertiamo
        std::reverse(cammino_minimo.begin(), cammino_minimo.end());
        cammini.push_back(cammino_minimo);
    }
    return cammini;
}

//Preparazione output Cicli
template<typename T>
struct RisultatoCiclo {
    std::vector<int> ciclo; // 0/1     per algebra De Pina 
    std::vector<int> ciclo_sgn; // -1/0/+1 per il sistema 
};

// Data una base di cicli (Si  vettori di incidenza) quersta funzione trova 
//il ciclo con il minor numero di 1 ovvero il ciclo di lunghezza minima 
template<typename T>
RisultatoCiclo<T> trova_ciclo_minimo(const unidirected_graph<T>& G,const std::vector<int>&     Si)  // vettore S_i 
{
    size_t m = G.all_edges().size();
 
    // Costruiamo il grafo esteso con il lifting basato su Si
    auto G1 = grafo_esteso(G, Si);
 
    // Troviamo tutti i cammini minimi nel grafo esteso
    auto cammini = tutti_cammini_minimi(G, G1);
 
	
	//tiene traccia del ciclo con MINOR numero di 1 
    int min_ones = std::numeric_limits<int>::max();
	
	std::vector<int> ciclo_migliore;
	std::vector<int> ciclo_migliore_sgn;
	
    for (auto& cammino : cammini) {
        if (cammino.empty()) continue; //vado al prossimo camminno
 
        // Costruiamo il vettore di incidenza C in G1 dal cammino
		//Inizializziamo il vettore C, di dimensione m a zero
        std::vector<int> C(m);
		std::vector<int> C_sgn(m);
        for (size_t j = 0; j < cammino.size() -1 ; j++) {
			T u = cammino[j].nodo_G;
            T v = cammino[j+1].nodo_G;
			unidirected_edge<T> e_corrente(u, v); // L'arco normalizzato secondo G
            int idx = G.edge_number(unidirected_edge<T>(u, v));
		
			//Gestiamo il caso dei VICOLI CIECHI: 1-2-1-3-4 ---> 1-3-4
			// - se incontro un arco per la prima volta: 0 ---> 1 
			// - se incontro un arco per la seconda volta: 1 ---> 0 lo annullo
			
			if (idx != -1) {
				C[idx] = (C[idx] + 1) % 2;
			
				
				// Confrontiamo se il verso (u->v) coincide con (e.from->e.to)
				// Assumiamo che e_corrente sia stato normalizzato da G.edge_number o dal costruttore
				if (u == e_corrente.from() && v == e_corrente.to()){
					C_sgn[idx] = 1;
				} else {
					C_sgn[idx] = -1;
				}
			
			
				if (C[idx] == 0){
					C_sgn[idx] = 0;
				}
			}
		}
		
        // Contiamo gli 1 nel vettore (= lunghezza del ciclo in numero di archi)
        int uni = 0;
        for (size_t i = 0; i < m; i++){
			
            if (C[i] == 1) {
				uni++;
			}
		}
 
        if (uni > 0 && uni < min_ones) {
            min_ones = uni;
            ciclo_migliore = C;
			ciclo_migliore_sgn = C_sgn;
        }
	}	
    return {ciclo_migliore, ciclo_migliore_sgn};
}


//Implementiamo funzioni prodotto sclare e XOR per implementare l'algoritmo di De Pina: 
//PRODOTTO SCALARE TRA BOOLEANI: 

int prodotto_scalare(
    const std::vector<int>& a,
    const std::vector<int>& b)
{
    int c = 0;
    for (size_t i = 0; i < a.size(); i++)
        c = (c + a[i] * b[i]) % 2;
    return c;
}

//XOR stardard è definito su c++ ma solo all'interno della libreria eigen
// funzionamento : dati due bit, restituisce 1 solo se i due bit sono diversi: 
std::vector<int> xor_vettori(
    const std::vector<int>& a,
    const std::vector<int>& b)
{
    std::vector<int> c(a.size());
    for (size_t i = 0; i < a.size(); i++)
        c[i] = (a[i] + b[i]) % 2;
    return c;
}
 
// Output De Pina: vettore di k = E -V +1 vettori booleani (interi)
template<typename T>
std::vector<std::vector<int>> De_Pina(unidirected_graph<T>& G, T sorgente)
{
    auto nodi = G.all_nodes();
    if (nodi.empty()) return {};
 
    // DFS, coalbero, inizializzazione S
	
    lifo s;
    unidirected_graph<T> T_dfs = dfs(G, sorgente, s);
    unidirected_graph<T> C = G - T_dfs; // coalbero: archi non nell'albero
 
    size_t m = G.all_edges().size();
    size_t k = C.all_edges().size(); // numero di cicli fondamentali= num archi in C
 
    //Inizializziamo vettore di vettori di incidenza S
    std::vector<std::vector<int>> S;
 
    // Per ogni arco del coalbero, inizializziamo S_i come vettore con un solo 1
    // nella posizione corrispondente all'arco del coalbero in G
    for (auto& arco : C.all_edges()) {
        std::vector<int> Si(m);
        int idx = G.edge_number(arco);
        if (idx != -1)
            Si[idx] = 1;
        S.push_back(Si);
    }
 
    std::vector<std::vector<int>> B; //--> contiene k cicli minimi trovati
 
    for (size_t i = 0; i < k; i++) {
        // Troviamo il ciclo minimo rispetto al vettore S[i] corrente
        RisultatoCiclo Ci = trova_ciclo_minimo(G, S[i]);
        B.push_back(Ci.ciclo_sgn);
 
        // Aggiornamento dei vettori S 
        for (size_t j = i + 1; j < k; j++) {
            if (prodotto_scalare(Ci.ciclo, S[j]) == 1 ){
                S[j] = xor_vettori(S[j], S[i]);
            }
        }
    }
    return B;
}
// B = vettori di vettori organizzato come [ciclo][arco] (ogni riga è un ciclo)
//Costruisco la matrice con i cicli su ogni colonna 
Eigen::MatrixXd costruzione_B(const std::vector<std::vector<int>>& B) {
	
    if (B.empty()) 
		return Eigen::MatrixXd(0, 0);

    size_t k = B.size();
    size_t m = B[0].size();

    // Inizializza la matrice Eigen di dimensioni (k x m)
    Eigen::MatrixXd matrice_B(m, k);

    for (size_t j = 0; j < k; ++j) { //j scorre i cicli
        for (size_t i = 0; i < m; ++i) { //i scorre gli archi
            
            matrice_B(i, j) = B[j][i]; // = ciclo j  arco i
        }
    }

    return matrice_B;
}