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
	int m= G.all_edges().size(); 
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);
 
    for (const auto& comp : circuito) {
        if (!comp.get_tipo().empty() && comp.get_tipo()[0] == 'R') {
			unidirected_edge<T> e(comp.from(), comp.to()); 
            int idx = G.edge_number(e);
            if (idx != -1) {
                R(idx, idx) = comp.get_valore();
            }
        }
        // I generatori (tipo 'V') non compaiono in R: R(i,i) rimane 0
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
    Eigen::VectorXd v = B.transpose() * fem;

    return v;
}


//RISOLUZIONE SISTEMA
Eigen::VectorXd risoluzione_sistema(const Eigen::MatrixXd& B, const Eigen::MatrixXd& R,const Eigen::VectorXd& v, unsigned int it_max  = 10000, double res_tol = 1e-10)   
{
    //costruiamo A = B^T * R * B
    Eigen::MatrixXd A = B.transpose() * R * B;
 
    int k = v.size();
 
    //inizializziamo la soluzione x_0 = 0
    Eigen::VectorXd x = Eigen::VectorXd::Zero(k);
    gradiente_coniugato(A, v, x, it_max, res_tol);
    return x;
}