#include <queue>
#include <stack> 
#pragma once
template <typename T>
// FIFO ---> 
class fifo{
private:
	std::queue<T> q; //inizializzo la struttura coda
public: 
	//costruttore di default vuoto: inizializzato a coda vuota
	fifo (){}
	
	//PUT: inserisco un elemento in fondo alla lista
	void put(const T& val) {
		q.push(val); 
		}
	
	//GET: prende l'elemento in coda e lo elimino dsalla lista
	T get() {
		T v = q.front(); //accede all'elemento in coda
		q.pop();           //rimuove
		return v; 
	}
	
	//EMPTY: verfica se il contenitore è vuoto
	bool empty(){
		return q.empty(); 
	}
}; 

template <typename T>
class lifo{
private:
	std::stack<T> s; //inizializzo la struttura stack
public: 
	//costruttore di default vuoto
	lifo (){}
	
	// PUT: inserisce in cima 
	void put(const T& val) {
		s.push(val); 
	}
	
	//GET: prende l'elemento in cima 
	T get() {
		T v = s.top(); //legge elemento in cima
		s.pop();         //lo rimuove
		return v; 
	}
	
	
	//EMPTY: verfica se il contenitore è vuoto
	bool empty(){
		return s.empty(); 
	}
}; 