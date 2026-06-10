#pragma once
#include "unidirected_graph1.hpp"
#include "unidirected_edge1.hpp"
#include "ciclifondamentali.hpp"
#include "netlist.hpp"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>
#include <map>
#include <vector>
#include <string>
#include <iostream>

// FUNZIONE: costruzione_R
// Costruisce la matrice diagonale delle resistenze R

// Output:
//   Eigen::MatrixXd R di dimensione m×m, dove m = numero di archi di G.
//   R(i,i) = valore della resistenza associata all'arco i-esimo.
//   Gli archi corrispondenti a generatori hanno R(i,i) = 0
//   (i generatori non contribuiscono alla matrice R, solo al termine noto).

//   Per ogni resistore nella netlist, troviamo l'indice dell'arco corrispondente
//   in G tramite edge_number(), e inseriamo il valore sulla diagonale.

template <typename T> 
Eigen::MatrixXd costruzione_R(
    unidirected_graph<T>& G,
    const std::vector<componente>& circuito)
{
	//Contiamo il numero di resistori 
	int n_R = 0; 
	
	for (const auto& comp : circuito){
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R'){
            n_R++;
		}
	}
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(n_R, n_R);

    for (const auto& comp : circuito) {
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') {
            
            int idx = comp.get_indice();
			R(idx, idx) = comp.get_valore();
            
        }    
	}
    return R;
}

// FUNZIONE: costruzione_v

//Per ogni ciclo i, scorriamo sui suoi nodi 
//- se si attraversa un generatore da "-" a "+": contributo = +valore
//- se si attraversa un generatore da "+" a "-": contributo = -valore
//- i resistori non contribuiscono

template <typename T> 
Eigen::VectorXd costruzione_v(unidirected_graph<T>& G, const Eigen::MatrixXd& B, const std::vector<componente>& circuito)
{
    int m = G.all_edges().size();
	
	//1. COSTRUZIONE DELLA FEM CALCOLATA SULL'INTERO GRAFO
    // fem(i) = generatore sull'arco i-esimo
    // nel verso canonico dell'arco (from < to)
    Eigen::VectorXd fem = Eigen::VectorXd::Zero(m);

    for (const auto& comp : circuito) {
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'V') {

            unidirected_edge<T> e(comp.from(), comp.to());
            int idx = G.edge_number(e);

            if (idx != -1) {
                if (comp.get_segno_positivo()) {
                    fem(idx) = -comp.get_valore();   // "+" su nodo from: percorrere l'arco da from-->to significa entrare dal "+": contributo NEGATIVO
                } else {
                    fem(idx) = comp.get_valore();  // "+" su nodo to: percorrere l'arco da from→to significa entrare dal "-": contributo POSITIVO
                }
            }
        }
    }

    // v(i) = somma dei contributi dei generatori pesata dal verso del ciclo i
    // ovvero: v = B^T * fem
	// dim: (k×m) * (m×1) = k×1  
    Eigen::VectorXd v =  B.transpose() * fem;
    return v;
}


template <typename T>
Eigen::MatrixXd estrai_B_R(
    unidirected_graph<T>& G,
    const Eigen::MatrixXd& B,
    const std::vector<componente>& circuito)
{
    int n_R = 0;
    for (const auto& comp : circuito)
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R')
            n_R++;

    int k = B.cols(); // numero di cicli
    Eigen::MatrixXd B_R = Eigen::MatrixXd::Zero(n_R, k);

    for (const auto& comp : circuito) {
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') {
            unidirected_edge<T> e(comp.from(), comp.to());
            int idx_arco = G.edge_number(e);    // riga in B
            int idx_R = comp.get_indice();   // riga in B_R
            if (idx_arco != -1 && idx_R != -1) {
				//copio le riga di B corrispondenti ai resistori nella matrice B_R
                B_R.row(idx_R) = B.row(idx_arco);
            }
        }
    }
    return B_R;
}	

//RISOLUZIONE SISTEMA
Eigen::VectorXd risoluzione_sistema(const Eigen::MatrixXd& B_R, const Eigen::MatrixXd& R,const Eigen::VectorXd& v, unsigned int it_max  = 10000, double res_tol = 1e-10)   
{
    //costruiamo A = B_R^T * R * B_R
    Eigen::MatrixXd A = B_R.transpose() * R * B_R;
 
    int k = v.size();
 
    //inizializziamo la soluzione x_0 = 0
    Eigen::VectorXd x = Eigen::VectorXd::Zero(k);
    gradiente_coniugato(A, v, x, it_max, res_tol);
    return x;
}