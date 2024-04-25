#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <unordered_set>
#include <string>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <queue>
#include <limits>

const int TAM = 7;
const int TAM_ACTION = 4; 
const int TAM_BRUJULA = 4;
const int TAM_BOOL = 1; 

int ComportamientoJugador::CalcDistanciaColab(const stateN3 &origin, const pair<int,int> &p){
	return (max(abs(origin.colaborador.f - p.first), abs(origin.colaborador.c - p.second)));
}
int ComportamientoJugador::Minimos(const stateN3 &st){
	int devolver;
	devolver = min(bateriaColab.at(2).at(st.colaborador.f).at(st.colaborador.c),bateriaColab.at(1).at(st.colaborador.f).at(st.colaborador.c));
	return(devolver);
}
int ComportamientoJugador::Mejorada(const stateN3 &st){
	int devolver;
	if(st.colaborador_bikini){
		devolver = costo_real.at(2).at(st.colaborador.f).at(st.colaborador.c);
	}else if(st.colaborador_zapatillas){
		devolver = costo_real.at(1).at(st.colaborador.f).at(st.colaborador.c);
	}else{
		devolver = costo_real.at(0).at(st.colaborador.f).at(st.colaborador.c);
	}

	return devolver; 
}
int ComportamientoJugador::CalcHeuristica(const stateN3& st){
	int coste; 
	int min_bikinis = numeric_limits<int>::max(); 
	int min_zapatillas = numeric_limits<int>::max();
	int contador;

	if(st.colaborador_bikini){

		for(auto it = zapatillas.begin(); it != zapatillas.end(); it++){
			contador = bateriaColab.at(1).at(it->first).at(it->second) + CalcDistanciaColab(st, *it);
			if(min_zapatillas > contador and contador > 0){
				min_zapatillas = contador;
			}
		}
		coste = min(min_zapatillas,bateriaColab.at(2).at(st.colaborador.f).at(st.colaborador.c));
		
	}else if(st.colaborador_zapatillas){

		for(auto it = bikinis.begin(); it != bikinis.end(); it++){
			contador = bateriaColab.at(2).at(it->first).at(it->second) + CalcDistanciaColab(st, *it);
			if(min_bikinis > contador and contador > 0){
				min_bikinis = contador;
			}
		}
		coste = min(min_bikinis, bateriaColab.at(1).at(st.colaborador.f).at(st.colaborador.c));
	}else{

		for(auto it = zapatillas.begin(); it != zapatillas.end(); it++){
			contador = bateriaColab.at(1).at(it->first).at(it->second) + CalcDistanciaColab(st, *it);
			if(min_zapatillas > contador and contador > 0){
				min_zapatillas = contador;
			}
		}

		for(auto it = bikinis.begin(); it != bikinis.end(); it++){
			contador = bateriaColab.at(2).at(it->first).at(it->second) + CalcDistanciaColab(st, *it);
			if(min_bikinis > contador and contador > 0){
				min_bikinis = contador;
			}
		}

		coste = min(min_bikinis,min_zapatillas);
		coste = min(coste, bateriaColab.at(0).at(st.colaborador.f).at(st.colaborador.c));


	}

	return coste;
}

unsigned int costeBateriaBasic(const Action &act,int f,int c,bool zapatillas,bool bikini, const vector<vector<unsigned char>> &mapa){
	unsigned int coste; 
	switch (mapa.at(f).at(c))
	{
	case 'A':
		switch (act)
		{
		case actWALK:
			if(bikini){
				coste = 10;
			}else{
				coste = 100;
			}
			break;
		case actRUN:
			if(bikini){
				coste = 15;
			}else{
				coste = 150;
			}
			break;
		case actTURN_L:
			if(bikini){
				coste = 5;
			}else{
				coste = 30;
			}
			break;
		case actTURN_SR:
			if(bikini){
				coste = 2;
			}else{
				coste = 10;
			}
			break;
		default:
			coste = 0;
			break;
		}
	break;
	case 'B':
		switch (act)
		{
		case actWALK:
			if(zapatillas){
				coste = 15;
			}else{
				coste = 50;
			}
			break;
		case actRUN:
			if(zapatillas){
				coste = 25;
			}else{
				coste = 75;
			}
			break;
		case actTURN_L:
			if(zapatillas){
				coste = 1;
			}else{
				coste = 7;
			}
			break;
		case actTURN_SR:
			if(zapatillas){
				coste = 1;
			}else{
				coste = 5;
			}
			break;
		default:
			coste = 0;
			break;
		}
		break;
	break;
	case 'T':
		switch (act)
		{
		case actWALK:
			coste = 2;
			break;
		case actRUN:
			coste = 3;
			break;
		case actTURN_L:
			coste = 2;
			break;
		case actTURN_SR:
			coste = 1;
			break;
		default:
			coste = 0;
			break;
		}
		break;
	break;
	
	default:
		coste = 1;
		break;
	}

	return coste;
}



unsigned int ComportamientoJugador::costeBateriaNX(const Action &act,const stateN2 &st) {
	unsigned int coste; 
	switch (mapaResultado.at(st.jugador.f).at(st.jugador.c))
	{
	case 'A':
		switch (act)
		{
		case actWALK:
			if(st.jugador_bikini){
				coste = 10;
			}else{
				coste = 100;
			}
			break;
		case actRUN:
			if(st.jugador_bikini){
				coste = 15;
			}else{
				coste = 150;
			}
			break;
		case actTURN_L:
			if(st.jugador_bikini){
				coste = 5;
			}else{
				coste = 30;
			}
			break;
		case actTURN_SR:
			if(st.jugador_bikini){
				coste = 2;
			}else{
				coste = 10;
			}
			break;
		default:
			coste = 0;
			break;
		}
	break;
	case 'B':
		switch (act)
		{
		case actWALK:
			if(st.jugador_zapatillas){
				coste = 15;
			}else{
				coste = 50;
			}
			break;
		case actRUN:
			if(st.jugador_zapatillas){
				coste = 25;
			}else{
				coste = 75;
			}
			break;
		case actTURN_L:
			if(st.jugador_zapatillas){
				coste = 1;
			}else{
				coste = 7;
			}
			break;
		case actTURN_SR:
			if(st.jugador_zapatillas){
				coste = 1;
			}else{
				coste = 5;
			}
			break;
		default:
			coste = 0;
			break;
		}
		break;
	break;
	case 'T':
		switch (act)
		{
		case actWALK:
			coste = 2;
			break;
		case actRUN:
			coste = 3;
			break;
		case actTURN_L:
			coste = 2;
			break;
		case actTURN_SR:
			coste = 1;
			break;
		default:
			coste = 0;
			break;
		}
		break;
	break;
	
	default:
		coste = 1;
		if(act == actIDLE){
			coste = 0;
		}
		break;
	}

	return coste;

}

unsigned int ComportamientoJugador::costeBateriaColab(const Action &act,const stateN3 &st){
	unsigned int coste; 
	switch (mapaResultado.at(st.colaborador.f).at(st.colaborador.c))
	{
	case 'A':
		switch (act)
		{
		case act_CLB_WALK:
			if(st.colaborador_bikini){
				coste = 10;
			}else{
				coste = 100;
			}
			break;
		case act_CLB_TURN_SR:
			if(st.colaborador_bikini){
				coste = 2;
			}else{
				coste = 10;
			}
			break;
		default:
			coste = 0;
			break;
		}
	break;
	case 'B':
		switch (act)
		{
		case act_CLB_WALK:
			if(st.colaborador_zapatillas){
				coste = 15;
			}else{
				coste = 50;
			}
			break;
		case act_CLB_TURN_SR:
			if(st.colaborador_zapatillas){
				coste = 1;
			}else{
				coste = 5;
			}
			break;
		default:
			coste = 0;
			break;
		}
	break;
	case 'T':
		switch (act)
		{
		case act_CLB_WALK:
			coste = 2;
			break;
		case act_CLB_TURN_SR:
			coste = 1;
			break;
		default:
			coste = 0;
			break;
		}
	break;

	default:
		
		if(act == act_CLB_STOP){
			coste = 0;
		}else{
			coste = 1;
		}
		break;
	}

	return coste;

}


int CosteNormal(const Action &act,const point &p, const vector<vector< unsigned char>> &mapa){
	return (costeBateriaBasic(act,p.i,p.j,false,false, mapa));
}

int CosteZapatillas(const Action &act,const point &p, const vector<vector< unsigned char>> &mapa){
	return (costeBateriaBasic(act,p.i,p.j,true,false, mapa));
}

int CosteBikini(const Action &act,const point &p, const vector<vector<unsigned char>> &mapa){
	return (costeBateriaBasic(act,p.i,p.j,false,true, mapa));
}


void ComportamientoJugador::PlanJugador(const nodeNX<state> &node){

	nodeNX<state> sig = node;
	while(sig.padre != nullptr){
		if(sig.st.jugador.f == sig.padre->st.jugador.f ){
			if(sig.st.jugador.c == sig.padre->st.jugador.c){
				if((sig.padre->st.jugador.brujula - sig.st.jugador.brujula +8)%8 == 7){
					plan.emplace_front(actTURN_SR);
				}else{
					plan.emplace_front(actTURN_L);
				}
			}else{
				if(abs(sig.st.jugador.c - sig.padre->st.jugador.c) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
			
		}else{
			if(abs(sig.st.jugador.f -sig.padre->st.jugador.f) == 1){
				plan.emplace_front(actWALK);
			}else{
				plan.emplace_front(actRUN);
			}
		}
		sig = *sig.padre; 
		
	}

}

void ComportamientoJugador::PlanJugador(const nodeNX<stateN2> &node){

	nodeNX<stateN2> sig = node;
	while(sig.padre != nullptr){
		if(sig.st.jugador.f == sig.padre->st.jugador.f ){
			if(sig.st.jugador.c == sig.padre->st.jugador.c){
				if((sig.padre->st.jugador.brujula - sig.st.jugador.brujula +8)%8 == 7){
					plan.emplace_front(actTURN_SR);
				}else{
					plan.emplace_front(actTURN_L);
				}
			}else{
				if(abs(sig.st.jugador.c - sig.padre->st.jugador.c) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
			
		}else{
			if(abs(sig.st.jugador.f -sig.padre->st.jugador.f) == 1){
				plan.emplace_front(actWALK);
			}else{
				plan.emplace_front(actRUN);
			}
		}
		sig = *sig.padre; 
		
	}

}

void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

struct Hash{

static unsigned long long hash;

static unsigned long long hashN0(const nodeNX<state> &rhs){
	return (hash0(rhs.st));
}
static unsigned long long hashN1(const nodeNX<stateN1> &rhs){
	return (hash1(rhs.st, hash0(rhs.st)));
}
static unsigned long long hashN2(const nodeNX<stateN2> &rhs){
	return (hash2(rhs.st, hash0(rhs.st)));
}
static unsigned long long hashN3(const nodeNX<stateN3> &rhs){
	return (hash3(rhs.st, hash2(rhs.st, hash1(rhs.st, hash0(rhs.st)))));
}


private:
static unsigned long long hash0(const state &rhs){
	
	hash = ((unsigned int)rhs.jugador.f << TAM) | rhs.jugador.c ;
	hash = (hash << TAM_BRUJULA) | rhs.jugador.brujula;

	return (hash);
}

static unsigned long long hash1(const stateN1 &rhs, const unsigned long long &c){
	hash = (c << TAM) | rhs.colaborador.f; 
	hash = (hash << TAM) | rhs.colaborador.c;
	hash = (hash << TAM_BRUJULA) | rhs.colaborador.brujula;
	hash = (hash << TAM_ACTION) | rhs.ultimaOrdenColaborador;

	return (hash);
		
}

static unsigned long long hash2(const stateN2 &rhs, const unsigned long long &c){
	hash = (c << TAM_BOOL) | rhs.jugador_zapatillas ; 
	hash = (hash << TAM_BOOL) | rhs.jugador_bikini ;

	return hash;
}

static unsigned long long hash3(const stateN3 &rhs, const unsigned long long &c){
	hash = (c << TAM_BOOL) | rhs.colaborador_zapatillas ; 
	hash = (hash << TAM_BOOL) | rhs.colaborador_bikini ;

	return hash;
}

};

unsigned long long Hash::hash = 0;

void ActualizarConOrientacion(const Orientacion& way,int &f, int &c, int k){

	switch (way)
		{
		case norte:
			f-=k;
			break;
		case noreste:
			f-=k;
			c+=k;
			break;	
		case este:
			c+=k;
			break;
		case sureste:
			f+=k;
			c+=k;
			break;
		case sur:
			f+=k;
			break;
		case suroeste:
			f+=k;
			c-=k;
			break;
		case oeste:
			c-=k;
			break;
		case noroeste:
			f-=k;
			c-=k;
			break;
		default:
			break;
		}

}

void ComportamientoJugador::Actualizar(ubicacion &pos, int k){

	switch (pos.brujula)
		{
		case norte:
			pos.f-=k;
			break;
		case noreste:
			pos.f-=k;
			pos.c+=k;
			break;	
		case este:
			pos.c+=k;
			break;
		case sureste:
			pos.f+=k;
			pos.c+=k;
			break;
		case sur:
			pos.f+=k;
			break;
		case suroeste:
			pos.f+=k;
			pos.c-=k;
			break;
		case oeste:
			pos.c-=k;
			break;
		case noroeste:
			pos.f-=k;
			pos.c-=k;
			break;
		default:
			break;
		}

}

bool ComportamientoJugador::PuedoVerColaborador(const stateN1 &st){
	bool encontrado = false; 
	int min_f, min_c, abs_f, abs_c;

	min_f = st.colaborador.f - st.jugador.f;
	min_c = st.colaborador.c - st.jugador.c;
	abs_f = abs(min_f);
	abs_c = abs(min_c);
	if( abs_f<=3 && abs_c <=3){
		switch (st.jugador.brujula)
		{
		case 0:
			if(min_f < 0 && abs_f >= abs_c){
				encontrado = true; 
			}
			break;
		case 1:
			if(min_f <= 0 and min_c >= 0){
				encontrado = true; 
			}
			break;
		case 2:
			if(min_c > 0 && abs_f <= abs_c){
				encontrado = true; 
			}
		break;
		case 3:
			if(min_f >= 0 and min_c >= 0){
				encontrado = true; 
			}
			break;
		case 4:
			if(min_f > 0 && abs_f >= abs_c){
				encontrado = true; 
			}
			break;
		case 5:
			if(min_f >= 0 and min_c <= 0){
				encontrado = true; 
			}
			break;
		case 6:
			if(min_c < 0 && abs_f <= abs_c){
				encontrado = true; 
			}
			break;
		case 7:
			if(min_f <= 0 and min_c <= 0){
				encontrado = true; 
			}
			break;
		default:
			break;
		}
	}
	
	return encontrado; 
}


bool ComportamientoJugador::CasillaTransitable(const ubicacion &x){
	return (mapaResultado[x.f][x.c] != 'P' and mapaResultado[x.f][x.c] != 'M' );
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	Actualizar(next,1);

	return next;
}

void ComportamientoJugador::priv_apply(const Action &a, state &st){
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st.jugador = sig_ubicacion;
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula+1)%8);
		break;
	}
}

state ComportamientoJugador::apply(const Action &a, const state &st){
	state st_result = st;
	priv_apply(a,st_result);
	return st_result;
}

stateN2 ComportamientoJugador::apply(const Action &a, const stateN2 &st){
	stateN2 st_result = st;
	priv_apply(a,st_result);
	return st_result;
}

bool ComportamientoJugador::ConsideraColaborador(const stateN1 &st, state &st_result, const ubicacion & sig_ubicacion){
	bool actualizado = false; 
	ubicacion sig_ubicacion_colab;
	switch (st.ultimaOrdenColaborador){
	case act_CLB_WALK:
		sig_ubicacion_colab = NextCasilla(st.colaborador);
		if (CasillaTransitable(sig_ubicacion_colab) and 
			!(sig_ubicacion.f == sig_ubicacion_colab.f and sig_ubicacion.c == sig_ubicacion_colab.c)){
				st_result.colaborador = sig_ubicacion_colab;
				actualizado = true; 
		}
		break;
	
	case act_CLB_TURN_SR:
		st_result.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula+1)%8);
	default:
		actualizado = true;
		break;
	}

	return actualizado;
}

void ComportamientoJugador::apply_priv_NI(const Action &a, stateN1 &st){
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no la va a ocupar el colaborador
		sig_ubicacion = NextCasilla(st.jugador);

		if (CasillaTransitable(sig_ubicacion) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
			if(ConsideraColaborador(st,st,sig_ubicacion)){
				st.jugador = sig_ubicacion;
			}
		}

		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
					if(ConsideraColaborador(st,st,sig_ubicacion2)){
						st.jugador = sig_ubicacion2;
					}
				}
			}
		break;

	case actTURN_L:
		if(ConsideraColaborador(st,st,st.jugador)){
			st.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula+6)%8);
		}
		
		
		break;

	case actTURN_SR:
		if(ConsideraColaborador(st,st,st.jugador)){
			st.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula+1)%8);
		}

		break;
	case act_CLB_WALK:
		st.ultimaOrdenColaborador = act_CLB_WALK;
		if(!ConsideraColaborador(st,st,st.jugador)){
			st.ultimaOrdenColaborador = st.ultimaOrdenColaborador;
		}
		break;
	case act_CLB_TURN_SR:
		st.ultimaOrdenColaborador = act_CLB_TURN_SR;
		ConsideraColaborador(st,st,st.jugador);
		break;
	case act_CLB_STOP:
		st.ultimaOrdenColaborador = act_CLB_STOP;
		break;
	case actIDLE:
		ConsideraColaborador(st,st,st.jugador);
		break;
	}
}

stateN1 ComportamientoJugador::apply(const Action &a, const stateN1 &st){
	stateN1 st_result = st;
	apply_priv_NI(a,st_result);
	return st_result;
}
stateN3 ComportamientoJugador::apply(const Action &a, const stateN3 &st){
	stateN3 st_result = st;
	apply_priv_NI(a,st_result);
	return st_result;
}



bool ComportamientoJugador::AnchuraSoloJugador(const state &inicio, const ubicacion &final)
{
	nodeNX<state> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;

	vector<Action> accion = {actWALK,actRUN,actTURN_L,actTURN_SR};
	list<nodeNX<state> > frontier;
	map<unsigned long long, nodeNX<state>> explored;
	frontier.push_back(current_node);
	explored.insert({Hash::hashN0(current_node),current_node});
	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);
	int i = 0;
	nodeNX<state> child;

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();

		// Generar hijo actWALK
		i = 0; 

		map<unsigned long long,nodeNX<state>>::iterator it = explored.find(Hash::hashN0(current_node));
		
		do{
			child.st = apply(accion.at(i),current_node.st);
			child.padre = &(it->second); 
			
			if (child.st.jugador.f == final.f and child.st.jugador.c == final.c){
				current_node = child;
				SolutionFound = true;
			} else if (explored.find(Hash::hashN0(child)) == explored.end()){
				
				frontier.push_back(child);
				explored.insert({Hash::hashN0(child),child}); 
			}
			i++;

		}while (i < 2 and !SolutionFound);

		while(i < 4 and !SolutionFound){
			child.st = apply(accion.at(i), current_node.st);
			child.padre = &(it->second);

			if (explored.find(Hash::hashN0(child)) == explored.end()){
				frontier.push_back(child);
				explored.insert({Hash::hashN0(child),child}); 
			}
			i++;
		} 

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
		}

	}

	PlanJugador(current_node);

	cout << explored.size() << endl;

	return SolutionFound;
}

bool ComportamientoJugador::AnchuraColaborador(const stateN1 &inicio, const ubicacion &final)
{
	nodeNX<stateN1> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;

	vector<Action> accion = {act_CLB_WALK,act_CLB_STOP,act_CLB_TURN_SR,actWALK,actRUN,actTURN_L,actTURN_SR};
	list<nodeNX<stateN1>> frontier;
	unordered_map<unsigned long long, nodeNX<stateN1>> explored;
	frontier.push_back(current_node);
	explored.insert({Hash::hashN1(current_node),current_node});

	bool SolutionFound = (current_node.st.colaborador.f== final.f and
						  current_node.st.colaborador.c == final.c);

	int i = 0;
	nodeNX<stateN1> child;

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();

		i = 0; 

		unordered_map<unsigned long long,nodeNX<stateN1>>::iterator it = explored.find(Hash::hashN1(current_node));
		if(PuedoVerColaborador(current_node.st)){

			child.st = apply(accion.at(i),current_node.st);
			child.padre = &(it->second); 
			
			if (child.st.colaborador.f == final.f and child.st.colaborador.c == final.c){
				current_node = child;
				SolutionFound = true;
			}

			else if (explored.find(Hash::hashN1(child)) == explored.end()){
				frontier.push_back(child);
				explored.insert({Hash::hashN1(child),child}); 
			}
			i++;

			while( i < 3 and !SolutionFound ){
				child.st = apply(accion.at(i),current_node.st);
				child.padre = &(it->second); 

				if (explored.find(Hash::hashN1(child)) == explored.end()){
					frontier.push_back(child);
					explored.insert({Hash::hashN1(child),child}); 
				}
				i++;
			}
			
		}else{
			i = 3;
		}
		
		
		do{
			child.st = apply(accion.at(i),current_node.st);
			child.padre = &(it->second); 

			if (child.st.colaborador.f == final.f and child.st.colaborador.c == final.c){
				current_node = child;
				SolutionFound = true;
			}
			
			if (explored.find(Hash::hashN1(child)) == explored.end()){
				frontier.push_back(child);
				explored.insert({Hash::hashN1(child),child}); 
			}
			i++;

		}while (i < 7 and !SolutionFound);

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
		}
	}

	while(current_node.padre != nullptr){

		if(current_node.st.jugador == current_node.padre->st.jugador){
			if(current_node.st.colaborador.f == current_node.padre->st.colaborador.f){
				if(current_node.st.colaborador.c == current_node.padre->st.colaborador.c){
					if((current_node.padre->st.colaborador.brujula - current_node.st.colaborador.brujula +8)%8 == 7){
						plan.emplace_front(act_CLB_TURN_SR);
					}else{
						plan.emplace_front(act_CLB_STOP);
					}
				}else{
					plan.emplace_front(act_CLB_WALK);
				}
			}else{
				plan.emplace_front(act_CLB_WALK);
			}
		}else{
			if(current_node.st.jugador.f == current_node.padre->st.jugador.f ){
			if(current_node.st.jugador.c == current_node.padre->st.jugador.c){
				if((current_node.padre->st.jugador.brujula - current_node.st.jugador.brujula +8)%8 == 7){
					plan.emplace_front(actTURN_SR);
				}else{
					plan.emplace_front(actTURN_L);
				}
			}else{
				if(abs(current_node.st.jugador.c - current_node.padre->st.jugador.c) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
			
			}else{
				if(abs(current_node.st.jugador.f -current_node.padre->st.jugador.f) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
		}
		
		current_node = *current_node.padre; 
		
	}

	cout << explored.size() << endl;



	return SolutionFound ;

}

struct Cmp
{
	bool operator()(const nodeNX<stateN2> &lhs, const nodeNX<stateN2> &rhs )const{
		return(lhs.st > rhs.st);
	}
	bool operator()(const nodeNX<stateN3> &lhs, const nodeNX<stateN3> &rhs)const{
		return(lhs.st.heuristica > rhs.st.heuristica);
	}
	bool operator()(const point &p1, const point &p2 ) const{
		return(p1.bateria > p2.bateria);
	}
};



bool ComportamientoJugador::CosteUniforme(const stateN2 &inicio, const ubicacion &final){
	nodeNX<stateN2> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;
	current_node.st.costo_bateria = 0;

	if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'D'){
		current_node.st.jugador_zapatillas = true;
		current_node.st.jugador_bikini = false;
	}else if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'K') {
		current_node.st.jugador_zapatillas = false;
		current_node.st.jugador_bikini = true;
	}

	unsigned int max = numeric_limits<unsigned int>::max();
	vector<Action> accion = {actWALK,actRUN,actTURN_L,actTURN_SR};

	priority_queue<nodeNX<stateN2>, vector<nodeNX<stateN2>>, Cmp> frontier;
	unordered_map<unsigned long long, nodeNX<stateN2>> explored;

	frontier.push(current_node);
	explored.insert({Hash::hashN2(current_node),current_node});
	nodeNX<stateN2> solution = nodeNX<stateN2>();
	int i = 0;
	nodeNX<stateN2> child;

	while(!frontier.empty()){
		frontier.pop();
		unordered_map<unsigned long long,nodeNX<stateN2>>::iterator it = explored.find(Hash::hashN2(current_node));
		cout << max << endl;
		if(current_node.st.costo_bateria < max and current_node.st.costo_bateria <= it->second.st.costo_bateria){
			i = 0; 
			unordered_map<unsigned long long,nodeNX<stateN2>>::iterator j;
			

			do{
				child.st = apply(accion.at(i),current_node.st);
				child.st.costo_bateria = it->second.st.costo_bateria + costeBateriaNX(accion.at(i),it->second.st);
				child.padre = &(it->second); 

				if(mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c) == 'D'){
					child.st.jugador_zapatillas = true;
					child.st.jugador_bikini = false;
				}else if(mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c) == 'K') {
					child.st.jugador_zapatillas = false;
					child.st.jugador_bikini = true;
				}
				
				if (child.st.jugador.f == final.f and child.st.jugador.c == final.c){
					if(child.st.costo_bateria < solution.st.costo_bateria){
						solution = child; 
						max = child.st.costo_bateria;
					}
				}else{
					j = explored.find(Hash::hashN2(child));
					if (j == explored.end()){
						frontier.push(child);
						explored.insert({Hash::hashN2(child),child}); 
					}else{
						if(j->second.st.costo_bateria > child.st.costo_bateria){
							frontier.push(child);
							j->second = child;
						}
					}
				} 
				i++;

			}while (i < 2);

			while(i < 4){
				child.st = apply(accion.at(i),current_node.st);
				child.st.costo_bateria = it->second.st.costo_bateria + costeBateriaNX(accion.at(i),it->second.st);
				child.padre = &(it->second);

				j = explored.find(Hash::hashN2(child));

				if (j == explored.end()){
				
					frontier.push(child);
					explored.insert({Hash::hashN2(child),child}); 

				}else{
					if(j->second.st.costo_bateria > child.st.costo_bateria){
						frontier.push(child);
						j->second = child;
					}
				}
				i++;
			} 

			if (!frontier.empty()){
				current_node = frontier.top();
			}

		}else{
			current_node = frontier.top();
		}

	}

	PlanJugador(solution);

	return (true);

}


bool ComportamientoJugador::a_star(const stateN3 &inicio, const ubicacion &final){
	nodeNX<stateN3> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;
	current_node.st.costo_bateria = 0;

	if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'D'){
		current_node.st.jugador_zapatillas = true;
		current_node.st.jugador_bikini = false;
	}else if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'K') {
		current_node.st.jugador_zapatillas = false;
		current_node.st.jugador_bikini = true;
	}

	if(mapaResultado.at(current_node.st.colaborador.f).at(current_node.st.colaborador.c) == 'D'){
		current_node.st.colaborador_zapatillas = true;
		current_node.st.colaborador_bikini = false;
	}else if(mapaResultado.at(current_node.st.colaborador.f).at(current_node.st.colaborador.c) == 'K') {
		current_node.st.colaborador_zapatillas = false;
		current_node.st.colaborador_bikini = true;
	}

	vector<Action> accion = {act_CLB_WALK,act_CLB_TURN_SR, act_CLB_STOP, actWALK,actRUN,actTURN_L,actTURN_SR,actIDLE};

	priority_queue<nodeNX<stateN3>, vector<nodeNX<stateN3>>, Cmp> frontier;
	unordered_map<unsigned long long, nodeNX<stateN3>> explored;

	bool SolutionFound = (current_node.st.colaborador.f== final.f and
						  current_node.st.colaborador.c == final.c);

	frontier.push(current_node);
	explored.insert({Hash::hashN3(current_node),current_node});
	nodeNX<stateN3> solution = nodeNX<stateN3>();
	int i = 0;
	nodeNX<stateN3> child;


	while (!frontier.empty() and !SolutionFound){
		frontier.pop();

		i = 0; 
		unordered_map<unsigned long long,nodeNX<stateN3>>::iterator it = explored.find(Hash::hashN3(current_node));
		//La heurística es monótona

		if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
				solution = current_node;
				SolutionFound = true;
		}else if(current_node.st.costo_bateria <= it->second.st.costo_bateria){

			unordered_map<unsigned long long,nodeNX<stateN3>>::iterator j;
			if(PuedoVerColaborador(current_node.st)){
				while( i < 3){
					child.st = apply(accion.at(i),current_node.st);
					child.padre = &(it->second); 
					child.st.costo_bateria = current_node.st.costo_bateria + costeBateriaColab(accion.at(i),current_node.st);
					child.st.heuristica = Minimos(child.st) + child.st.costo_bateria;
					

					if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'D'){
						child.st.colaborador_zapatillas = true;
						child.st.colaborador_bikini = false;
					}else if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'K') {
						child.st.colaborador_zapatillas = false;
						child.st.colaborador_bikini = true;
					}

					j = explored.find(Hash::hashN3(child));

					if(j == explored.end()){
						frontier.push(child);
						explored.insert({Hash::hashN3(child),child}); 
					}else{
						if(j->second.st.costo_bateria > child.st.costo_bateria){
							frontier.push(child);
							j->second = child;
						}
					}
					i++;
				}
			}else{
				i = 3;
			}

			while( i < 8){
				child.st = apply(accion.at(i),current_node.st);
				child.padre = &(it->second); 
				child.st.costo_bateria = current_node.st.costo_bateria + costeBateriaNX(accion.at(i),current_node.st) + 
										 costeBateriaColab(current_node.st.ultimaOrdenColaborador, current_node.st);
				child.st.heuristica =  Minimos(child.st) + child.st.costo_bateria;

				if(mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c) == 'D'){
					child.st.jugador_zapatillas = true;
					child.st.jugador_bikini = false;
				}else if(mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c) == 'K') {
					child.st.jugador_zapatillas = false;
					child.st.jugador_bikini = true;
				}

				if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'D'){
					child.st.colaborador_zapatillas = true;
					child.st.colaborador_bikini = false;
				}else if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'K') {
					child.st.colaborador_zapatillas = false;
					child.st.colaborador_bikini = true;
				}

				j = explored.find(Hash::hashN3(child));

				if (j == explored.end()){
					frontier.push(child);
					explored.insert({Hash::hashN3(child),child}); 
				}else{
					if(j->second.st.costo_bateria > child.st.costo_bateria){
						frontier.push(child);
						j->second = child;
					}
				}
				i++;
			}
			
			

			if (!SolutionFound and !frontier.empty()){
				current_node = frontier.top();
			}
		}else{
			if (!frontier.empty()){
				current_node = frontier.top();
			}
		}

	}

	while(solution.padre != nullptr){

		if(solution.st.jugador == solution.padre->st.jugador){
			if(solution.st.colaborador.f == solution.padre->st.colaborador.f){
				if(solution.st.colaborador.c == solution.padre->st.colaborador.c){
					if((solution.padre->st.colaborador.brujula - solution.st.colaborador.brujula +8)%8 == 7){
						if(solution.padre->st.ultimaOrdenColaborador != act_CLB_TURN_SR){
							plan.emplace_front(act_CLB_TURN_SR);
						}else{
							plan.emplace_front(actIDLE);
						}
					}else{
						plan.emplace_front(act_CLB_STOP);
					}
				}else if(solution.padre->st.ultimaOrdenColaborador != act_CLB_WALK){
					plan.emplace_front(act_CLB_WALK);
				}else{
					plan.emplace_front(actIDLE);
				}
			}else if(solution.padre->st.ultimaOrdenColaborador != act_CLB_WALK){
				plan.emplace_front(act_CLB_WALK);
			}else{
				plan.emplace_front(actIDLE);
			}
		}else{
			if(solution.st.jugador.f == solution.padre->st.jugador.f ){
			if(solution.st.jugador.c == solution.padre->st.jugador.c){
				if((solution.padre->st.jugador.brujula - solution.st.jugador.brujula +8)%8 == 7){
					plan.emplace_front(actTURN_SR);
				}else if((solution.padre->st.jugador.brujula - solution.st.jugador.brujula +8)%8 == 2){
					plan.emplace_front(actTURN_L);
				}else{
					plan.emplace_front(actIDLE);
				}
			}else{
				if(abs(solution.st.jugador.c - solution.padre->st.jugador.c) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
			
			}else{
				if(abs(solution.st.jugador.f -solution.padre->st.jugador.f) == 1){
					plan.emplace_front(actWALK);
				}else{
					plan.emplace_front(actRUN);
				}
			}
		}
		
		solution = *solution.padre; 
		
	}

		

		
		


	


	return SolutionFound;




}

void GenerarMapa(const point &origin,int caso,int (*punt_funct)(const Action&,const point&, const vector<vector< unsigned char>> &mapa), 
				 const vector<vector< unsigned char>> &mapa, vector<vector<vector<int>>> &bateriaColab){
	priority_queue<point, vector<point>, Cmp> q;

	point current_point = origin;
	current_point.bateria = 0;
	q.push(current_point); 
	point child;
	while (!q.empty()){
		q.pop();
		bateriaColab.at(caso).at(current_point.i).at(current_point.j) = current_point.bateria;
		for(int i = 0; i <= 8; i++){
			child = current_point;
			ActualizarConOrientacion(static_cast<Orientacion>(i),child.i,child.j,1);
			if(mapa.at(child.i).at(child.j) != 'M' and mapa.at(child.i).at(child.j) != 'P' and bateriaColab.at(caso).at(child.i).at(child.j) < 0){
					child.bateria = punt_funct(actWALK,child,mapa) + current_point.bateria;
					q.push(child);
			}
		}
		
		current_point = q.top();

		while (bateriaColab.at(caso).at(current_point.i).at(current_point.j) >= 0 and !q.empty()){
			q.pop();
			current_point = q.top();
		}

	}

}

void GenerarMapa2(int k, int (*punt_funct)(const Action&,const point&, const vector<vector< unsigned char>> &mapa),
const vector<vector< unsigned char>> &mapa, const vector<vector<vector<int>>> &bateriaColab, vector<vector<vector<int>>> &costo_real){

	priority_queue<point, vector<point>, Cmp> q;

	for(int i = 0; i< mapa.size(); i++){
		for(int j = 0; j < mapa.size(); j++){
			if(mapa.at(i).at(j) == 'D' and k != 1){
				if(bateriaColab.at(1).at(i).at(j) >= 0 ){
					q.push(point(i,j,bateriaColab.at(1).at(i).at(j)));
				}
			}else if(mapa.at(i).at(j) == 'K' and k != 2){
				if(bateriaColab.at(2).at(i).at(j) >= 0 ){
					q.push(point(i,j,bateriaColab.at(2).at(i).at(j)));
				}
			}
		}
    }

	point current_point;
	point child;

	if(!q.empty()){
		current_point = q.top();
	}

	while (!q.empty()){
		q.pop();
		costo_real.at(k).at(current_point.i).at(current_point.j) = current_point.bateria;
		for(int i = 0; i <= 8; i++){
			child = current_point;
			ActualizarConOrientacion(static_cast<Orientacion>(i),child.i,child.j,1);
			if(mapa.at(child.i).at(child.j) != 'M' and mapa.at(child.i).at(child.j) != 'P' and costo_real.at(k).at(child.i).at(child.j) < 0){
					child.bateria = min(punt_funct(actWALK,child,mapa) + current_point.bateria, bateriaColab.at(k).at(child.i).at(child.j));
					q.push(child);
			}
		}
		
		current_point = q.top();

		while (costo_real.at(k).at(current_point.i).at(current_point.j) >= 0 and !q.empty()){
			q.pop();
			current_point = q.top();
		}

	}


}








void PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK){
			cout << "W ";
		}
		else if (*it == actRUN){
			cout << "R ";
		}
		else if (*it == actTURN_SR){
			cout << "r ";
		}
		else if (*it == actTURN_L){
			cout << "L ";
		}
		else if (*it == act_CLB_WALK){
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP){
			cout << "cS ";
		}
		else if (*it == actIDLE){
			cout << "I ";
		}
		else{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

void ComportamientoJugador::VisualizaPlan(const state &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	state cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}


//Empezamos a meter el Nivel 4 

void ComportamientoJugador::PonerTerrenoMapa(const vector<unsigned char> &terreno, const stateN4 &st){
	pair<int,int> pivote;
	pair<int,int> pivote_izd;
	pair<int,int> pivote_der;
	pivote.first = st.jugador.f; 
	pivote.second = st.jugador.c;

	pivote_izd = pivote_der = pivote; 

	int i = 0;

	do{

		mapaResultado.at(pivote.first).at(pivote.second) = terreno.at(i*(i+1));
		pivote_izd = pivote_der = pivote; 

		for(int j = i + 1; j <= 3; j++){
			ActualizarConOrientacion(static_cast<Orientacion>((st.jugador.brujula + 7)%8), pivote_izd.first, pivote_izd.second,1);
			ActualizarConOrientacion(static_cast<Orientacion>((st.jugador.brujula + 1)%8), pivote_der.first, pivote_der.second,1);

			mapaResultado.at(pivote_izd.first).at(pivote_izd.second) = terreno.at(j*j + i);
			mapaResultado.at(pivote_der.first).at(pivote_der.second) = terreno.at(j*(j+2) - i);

		}
		ActualizarConOrientacion(st.jugador.brujula, pivote.first, pivote.second,1);
		i++;

	}while(i < 4);
	

}

void ComportamientoJugador::ActualizarEstado(const Sensores &sensores){

	switch (last_action)
	{
	case actWHEREIS:
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;
			estoy_ubicado = true; 
		break;
	
	default:
		if ((last_action != act_CLB_WALK) and (last_action != act_CLB_TURN_SR) and (last_action != act_CLB_STOP))
		{
			switch (c_state.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				c_state.colaborador = NextCasilla(c_state.colaborador);
				break;
			case act_CLB_TURN_SR:
				c_state.colaborador.brujula = (Orientacion)((c_state.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (last_action)
		{
		case actRUN:
			c_state.jugador = NextCasilla(c_state.jugador);
			c_state.jugador = NextCasilla(c_state.jugador);
			break;
		case actWALK:
			c_state.jugador = NextCasilla(c_state.jugador);
			break;
		case actTURN_SR:
			c_state.jugador.brujula = (Orientacion)((c_state.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			c_state.jugador.brujula = (Orientacion)((c_state.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			c_state.colaborador = NextCasilla(c_state.colaborador);
			c_state.ultimaOrdenColaborador = act_CLB_WALK;
			break;
		case act_CLB_TURN_SR:
			c_state.colaborador.brujula = (Orientacion)((c_state.colaborador.brujula + 1) % 8);
			c_state.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			c_state.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		case actIDLE: 
			break;
		}

		break;
	}
	
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	if(sensores.nivel != 4){

		if (!hayPlan){

			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			switch (sensores.nivel)
			{
			case 0:
				hayPlan = AnchuraSoloJugador(c_state,goal);
				break;
			case 1:
				hayPlan = AnchuraColaborador(c_state,goal);
				break;
			case 2:
				hayPlan = CosteUniforme(c_state,goal);
				break;
			case 3:
				GenerarN3(goal);
				N3Mejora(goal);
				hayPlan = a_star(c_state, goal);
				break;
			default:
				break;
			}
			
			if(plan.size() > 0){
				VisualizaPlan(c_state,plan);
				PintaPlan(plan);
			}
			
		}

		if (hayPlan and plan.size()>0){
			accion = plan.front();
			plan.pop_front();
		}

		if (plan.size()== 0){
			hayPlan = false;
		}

	}else{
		ActualizarEstado(sensores);

		if(estoy_ubicado == false){
			accion = actWHEREIS;
		}else{
			PonerTerrenoMapa(sensores.terreno, c_state);
			accion = actTURN_SR;
		}
		
		
	}
	
	last_action = accion;

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
