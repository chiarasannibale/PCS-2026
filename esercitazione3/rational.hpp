#pragma once
#include <concepts>
#include <cmath>

template<typename I> requires std::integral<I>
class rational
{
	I num_;
	I den_;	

private: 
	I mcd(I x, I y) {
		//usiamo l'algoritmo di euclide su val positivi
		x = std::abs(x);  
		y = std::abs(y); 
	
		
		while (y!=0) {
			I r = x % y;   // trovo il resto della divisione di x per y
			x = y; 
			y = r;         // il resto diventa il nuovo divisore 
		}
		return x;            // se il resto è zero il massimmo comune divisore è x
	}
	
	void semplificazione() {
		if (den_ == 0) return; //escludo Inf e Nan
		
		I maxcomdiv = mcd(num_, den_); 
		num_ /= maxcomdiv; 
		den_ /= maxcomdiv; 
	}
		
public: 
	//costruttore di default
	rational()
			:num_(I{0}), den_(I{1}) 
	{
	semplificazione();
	}
//costruttore user defined
// per Nan e Inf creo un parametro dentro la classe che non dipende dagli input dell'utente 
// per l'inf deve gestire anceh le soluuzioni nell'operazioni
	rational(const I& pnum, const I& pden)
		: num_(pnum), den_(pden)
	{
		if (den_==0 && num_!=0 ){
		// l'infinito forma standard = 1\0
			num_=I{1}; 
			den_= I{0};
		}
		// l'not a number forma standard = 0\0
		else if (den_==0 && num_==0){
			num_=I{0}; 
			den_= I{0};
		}
		semplificazione(); 
	}

	//metodi per leggere i valori num e den ---> sono dei metodi pubblici per leggere num e den 
	I num() const {return num_; }
	I den() const {return den_; }
	
	//implementazione canonica dell'incremento
	// OPERATOR += MODIFICA L'OGGETTO STESSO SU CUI VIENE CHIAMATO
	// IL CODICE DEFINISCE ESATTAMENTE LA SOMMMA
	rational& operator+= (const rational& other) {
		// la & creo un indirizzo che andremo a -deferenziare usando *
		// 1. gestione Inf	
		if ((den_==0 && num_==1)||(other.den_==0 && other.num_==1)){ 
			num_=I{1}; 
			den_= I{0};
			return *this; 
		}
		// 2. gestione Nan
		if ((den_==0 && num_==0)||(other.den_==0 && other.num_==0)){
			//ritorno un numero nan
			num_=I{0}; 
			den_= I{0};
			return *this; 
		}
		else{
		// calcolo SOMMA tra frazioni: a/b * c/d = a*d + c*b)/ b*d
			num_ = (num_ * other.den_) + (other.num_ * den_); 
			den_ = den_ * other.den_; 
		semplificazione(); 
		return *this; 
		}
	}
		
	//implementazione canonica della somma 
	// OPERATOR + ESEGUE L'ADDIZIONE IN SE, L'OPERATORE NON MODIFICA I DUE ADDENDI MA CREA UN RISULTATO NUOVO 
	// SENZA MODIFICARE RATIONAL, CREA UNA COPIA DI SE SETSSSO E USO LA FORMULA PRIMA DEFINITA IN QUESTA COPIA 
	rational operator+(const rational& other) const {
		rational ret =*this;  //faccio una copia di rational la chiamo ret 
		ret += other;         //uso il += che ho definito prima 
		return ret;           // ritorno il valore modificato
	}
	
	//OSS: Poichè stiamo creando la variabile temporanea ret dentro la funzione.
	//La variabile ret viene eliminata dalla memoria.
    //Per questo motivo nell'operator+ non usiamo la & nel tipo di ritorno 
	//in quanto stai restituendo l'indirizzo di una memoria 
	//che è stata distrutta
	
	//implementazione canonica del decremento
	rational& operator-= (const rational& other) {
		// 1. gestione Inf
		if ((den_==0 && num_==1)||(other.den_==0 && other.num_==1)){ //se non ho un numero inf e nan
			num_=I{1}; 
			den_= I{0};
			return *this; //fermo l'esecuzione dopo ogni caso "speciale"
		}
		// 2. gestione Nan
		if ((den_==0 && num_==0)||(other.den_==0 && other.num_==0)){
			//ritorno un numero nan
			num_=I{0}; 
			den_= I{0};
			return *this; 
		}
		else {
			num_ = (num_ * other.den_) - (other.num_ * den_); 
			den_ = den_ * other.den_;
		semplificazione(); 
		return *this; 
		}
	}
		
	//implementazione canonica della sottrazione 
	rational operator-(const rational& other) const {
		rational ret =*this; 
		ret -= other; 
		return ret; 
	}
	
	//implementazione canonica del prodotto
	rational& operator*= (const rational& other) {
		// 1. gestione Inf
		if ((den_==0 && num_==1)||(other.den_==0 && other.num_==1)){ //se non ho un numero inf e nan
			num_=I{1}; 
			den_= I{0};
		return *this; 
		}
		// 2. gestione Nan
		if ((den_==0 && num_==0)||(other.den_==0 && other.num_==0)){
			//ritorno un numero nan
			num_=I{0}; 
			den_= I{0};
		return *this; 
		}
		
		else {
			num_ = num_ * other.num_; 
			den_ = den_ * other.den_; 
			semplificazione(); 
		return *this;
		}
	}
	
	rational operator *(const rational& other) const{
		rational ret = *this; 
		ret *=  other; 
		return ret; 
	}
	
	//implementazione canonica della divisione
	rational& operator/= (const rational& other) {
		// 1. gestione Inf
		if ((den_==0 && num_==1)||(other.den_==0 && other.num_==1)){ 
			num_=I{1}; 
			den_= I{0};
			return *this; 
		}
		// 2. gestione Nan
		if ((den_==0 && num_==0)||(other.den_==0 && other.num_==0)){
			//ritorno un numero nan
			num_=I{0}; 
			den_= I{0};
			return *this; 
		}
		
		else {
			num_ = num_ * other.den_; 
			den_ = den_ * other.num_; 
			semplificazione(); 
		return *this;
		}
	}

	
	rational operator/ (const rational& other) const{
		rational ret = *this; 
		ret /=  other; 
		return ret; 
		}
	
	
	
}; 
//implemtentazione operatore<< (operatore di stampa)
template<typename I>
std::ostream&
	operator<<(std::ostream& os, const rational<I>& r){
		if (r.num() ==0 && r.den()==0){
			return os <<"Nan"; 
		}
		if (r.num() ==1 && r.den()==0){
			return os <<"Inf"; 
		}
		else{
			os << r.num()<<"/"<<r.den(); 
		}
		return os; 
		
	}