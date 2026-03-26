#include <iostream>
#include "rational.hpp" //importiamo il file con all'interno la definzioni della classe e le implementazione degli operatori


int main(void)
{
	rational<int> a(2, 3);
	rational<int> b(4, 5); 
	//ricordiamo che operator += restituisce un riferimento all'oggetto su cui è chiamato
	//mentro         operator + restituisce l'oggetto stesso come modifica della copia di rational 
	rational<int> c = a.operator+(b); //sommo ad a il valore b come l'ho definito nel mododo "operator+"
	rational<int> d = a.operator-(b);
	rational<int> e = a.operator*(b);
	rational<int> f = a.operator/(b);
	
	std::cout << "La somma:" <<a<< "+" << b<< "=" << c << "\n";
	std::cout << "La sottrazione:" <<a<< "-" << b<< "=" << d<< "\n";
	std::cout << "Il prodotto:" <<a<< "*" << b<< "=" << e << "\n";
	std::cout << "La divisione:" <<a<< "/" << b<< "=" << f << "\n";
return 0;
}

