#include <iostream>
#include <cmath>
#include "gradiente_coniugato.hpp"
#include <vector>
#include <Eigen/Dense>

int main(int argc, char **argv){
	const double  tol = 1.0e-15; 
	unsigned int n = 3; 
	
	
	Eigen::MatrixXd B = Eigen::MatrixXd::Random(n, n);
	
	//controllo che sia invertibile
	if (std::abs(B.determinant()) < tol){
		std::cout << "Matrice B non invertibile"<< std::endl; 
		return -1;
	}
	
	//rendo tale matrice simm def pos
	Eigen::MatrixXd A = B.transpose() * B;
	Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
	
	//inizializziamo il vettore x
	Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
	
	//costruzione del termine noto
	Eigen::VectorXd b = A * x_ex;
	
	//inizializzo variabili per l'algoritmo
	const unsigned int it_max = 10000;
	unsigned int it = 0;
	const double res_tol = 1.0e-12;
	double res_norm_0 = 0.0; 
	
	//chiamata della funzione
	gradiente_coniugato(A, b, x, it_max, res_tol, it, res_norm_0);

	//inizializziamo il residuo finale 
	Eigen::VectorXd res = b-A*x; 
	
	const auto err_rel = (x_ex.norm() == 0.0) ? (x - x_ex).norm() :(x - x_ex).norm() / x_ex.norm();

	std::cout.precision(4); //formattazione output con 4 cifre decimali
	std::cout<< std::scientific<< "it"<< "/"<< "it_max"<< std::endl; //iterazioni effettutate rispetto al massimo
	std::cout<< std::scientific<< it<< "/"<< it_max<< std::endl;
	std::cout<< std::scientific<< "res"<< "/"<< "res_tol"<< std::endl;
	std::cout<< std::scientific<< res.norm() / res_norm_0<< "/"<< res_tol<< std::endl;
	std::cout<< std::scientific<< "err_rel"<< std::endl;
	std::cout<< std::scientific<< err_rel<< std::endl;
	std::cout<< std::scientific<< "soluzione"<< std::endl;
	std::cout<< std::scientific<< x << std::endl; //stampo il vettore finale a cui convergo

	return 0; 

}