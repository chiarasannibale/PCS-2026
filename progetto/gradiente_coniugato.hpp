#pragma once
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/SVD>

//nel gradiente coniugato introduciamo la direzione coniugata p

double condA(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}

//passando i valori per riferimento la funzione lavora direttamente sulla memoria del main, 
//quindi non vengono generate copie nè restituite
void gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, Eigen::VectorXd& x, unsigned int it_max, double res_tol) 
{

	//inizializziamo  il residuo inizlae r_0 = b - A*x_0
	Eigen::VectorXd res = b - A * x;
	double res_norm_0 = res.norm();
	
	//inizializziamo la direzione coniugata p_0 = r_0
	Eigen::VectorXd p = res;
	unsigned int it = 0; 

	

	std::cout.precision(2);
	std::cout<< std::scientific<< "Matrix Cond: "<< condA(A)<< std::endl;
	
	//ciclo while del gradiente coiugato (non conosciamo a priori il numero di iterazioni)
	while (it < it_max && res.norm() > res_tol * res_norm_0){
		
		Eigen::VectorXd Ap = A*p; 
		
		//calcolo alpha_k
		const double alpha_k = ((p.transpose() * res) / (p.transpose() * Ap)).value();
		// const double alpha_k_dot = p.dot(res) / (p.dot(A * p));
		
		//aggiornamento soluzione e residuo
		x = x + alpha_k * p;
		res = res - alpha_k * Ap;
		
		//beta_k 
		double beta_k = p.dot(A*res) / p.dot(Ap); 
		
		//aggiornamento della direzione di ricerca 
		p = res- beta_k*p; 
		
		it++;
	}
	return ; // non scrivo return 0 perche è void la funzione
}

//passsiamo il numero finale di iterazioni al main

