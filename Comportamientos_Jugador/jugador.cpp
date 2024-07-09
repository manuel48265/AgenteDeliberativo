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

bool Accesible(const stateN4& st, const ubicacion& goal){
	return(st.jugador.f == goal.f and st.jugador.c == goal.c);
}


bool bateria(const stateN4& st, const ubicacion& goal){
	return(st.finish);
}

int ComportamientoJugador::CalcDistanciaColab(const stateN3 &origin,  const ubicacion& goal){
	return (max(abs(origin.colaborador.f - goal.f), abs(origin.colaborador.c - goal.c)));
}
int ComportamientoJugador::CalcDistancia(const stateN4 &origin, const ubicacion &goal){
	return (max(abs(origin.jugador.f - goal.f), abs(origin.jugador.c - goal.c)));
}

int ComportamientoJugador::Minimos(const stateN3 &st){
	int devolver;
	devolver = min(bateriaColab.at(2).at(st.colaborador.f).at(st.colaborador.c),bateriaColab.at(1).at(st.colaborador.f).at(st.colaborador.c));
	return(devolver);
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

	case '?':
		if(st.jugador_bikini ){
			coste = 7;
			
		}if(st.jugador_zapatillas){
			coste = 13;
		}
		else{
			coste = 20;
		}
		
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

unsigned int ComportamientoJugador::costeBateriaPRIV(const Action &act,const stateN4 &st, unsigned char c) {
	unsigned int coste; 
	switch (c)
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

	case '?':
		if(st.jugador_bikini ){
			coste = 75;
			
		}if(st.jugador_zapatillas){
			coste = 100;
		}
		else{
			coste = 100;
		}
		
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

unsigned int ComportamientoJugador::costeBateriaN4(const Action &act,const stateN4 &st) {
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

	case '?':
		
		coste = 6*costeBateriaPRIV(act,st,st.ultima)/(st.pasos) + costeBateriaPRIV(act,st,'?');
		coste = coste/7 + 1;
		
		/*
		coste = costeBateriaPRIV(act,st,st.ultima)/(st.pasos);
		coste = coste + 1;
		*/
		
		
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



int ComportamientoJugador::costeBateriaColab(const Action &act,const stateN3 &st){
	int coste = 0; 
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

void ComportamientoJugador::PlanJugador(const nodeNX<stateN4> &node){

	nodeNX<stateN4> sig = node;
	pair<int,int> intermedio; 
	while(sig.padre != nullptr){
		if(mapaResultado.at(sig.st.jugador.f).at(sig.st.jugador.c) == '?'){
			plan.clear();
		}else{
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
						intermedio.first = (sig.st.jugador.f + sig.padre->st.jugador.f)/2;
						intermedio.second = (sig.st.jugador.c + sig.padre->st.jugador.c)/2;

						if(mapaResultado.at(intermedio.first).at(intermedio.second) != '?'){
							plan.emplace_front(actRUN);
						}else{
							plan.clear();
						}
						
					}
				}
				
			}else{
				if(abs(sig.st.jugador.f -sig.padre->st.jugador.f) == 1){
					plan.emplace_front(actWALK);
				}else{
					intermedio.first = (sig.st.jugador.f + sig.padre->st.jugador.f)/2;
					intermedio.second = (sig.st.jugador.c + sig.padre->st.jugador.c)/2;

					if(mapaResultado.at(intermedio.first).at(intermedio.second) != '?'){
						plan.emplace_front(actRUN);
					}else{
						plan.clear();
					}
					
				}
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

bool PuedoVerColaborador(const stateN1 &st){
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

bool PuedoVerCasilla(const stateN4 &st, const ubicacion &goal){
	bool encontrado = false; 
	int min_f, min_c, abs_f, abs_c;

	min_f = goal.f - st.jugador.f;
	min_c = goal.c - st.jugador.c;
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

bool llega_colab(const stateN4& st, const ubicacion& goal){
	return(PuedoVerColaborador(st));
}


bool ComportamientoJugador::CasillaTransitable(const ubicacion &x){
	return (mapaResultado[x.f][x.c] != 'P' and mapaResultado[x.f][x.c] != 'M' );
}

bool ComportamientoJugador::CasillaTransitable2(const ubicacion &x){
	return (mapaResultado[x.f][x.c] != 'P' and mapaResultado[x.f][x.c] != 'M' and mapaResultado[x.f][x.c] != '?' );
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

stateN4 ComportamientoJugador::applyJ(const Action &a, const stateN4 &st){
	stateN4 st_result = st;
	priv_apply(a,st_result);

	if(mapaResultado.at(st_result.jugador.f).at(st_result.jugador.c) != '?' ){
		st_result.ultima = mapaResultado.at(st_result.jugador.f).at(st_result.jugador.c);
		st_result.pasos = 1;
	}else{
		st_result.pasos++;
	}

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


bool ComportamientoJugador::ConsideraColaborador2(const stateN4 &st, state &st_result, const ubicacion & sig_ubicacion){
	bool actualizado = false; 
	ubicacion sig_ubicacion_colab;
	switch (st.ultimaOrdenColaborador){
	case act_CLB_WALK:
		sig_ubicacion_colab = NextCasilla(st.colaborador);
		if (CasillaTransitable2(sig_ubicacion_colab) and 
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
	Action save; 
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
		save = st.ultimaOrdenColaborador;
		st.ultimaOrdenColaborador = act_CLB_WALK;
		if(!ConsideraColaborador(st,st,st.jugador)){
			st.ultimaOrdenColaborador = save;
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

stateN4 ComportamientoJugador::applyJC(const Action &a, const stateN4 &st){
	stateN4 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	Action save; 
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no la va a ocupar el colaborador
		sig_ubicacion = NextCasilla(st_result.jugador);

		if (CasillaTransitable2(sig_ubicacion) and 
			!(sig_ubicacion.f == st_result.colaborador.f and sig_ubicacion.c == st_result.colaborador.c)){
			if(ConsideraColaborador2(st_result,st_result,sig_ubicacion)){
				st_result.jugador = sig_ubicacion;
			}
		}

		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st_result.jugador);
		if (CasillaTransitable2(sig_ubicacion) and 
			!(sig_ubicacion.f == st_result.colaborador.f and sig_ubicacion.c == st_result.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable2(sig_ubicacion2) and 
					!(sig_ubicacion2.f == st_result.colaborador.f and sig_ubicacion2.c == st_result.colaborador.c)){
					if(ConsideraColaborador2(st_result,st_result,sig_ubicacion2)){
						st_result.jugador = sig_ubicacion2;
					}
				}
			}
		break;

	case actTURN_L:
		if(ConsideraColaborador2(st_result,st_result,st_result.jugador)){
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		}
		
		
		break;

	case actTURN_SR:
		if(ConsideraColaborador2(st_result,st_result,st_result.jugador)){
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		}

		break;
	case act_CLB_WALK:
		save = st_result.ultimaOrdenColaborador;
		st_result.ultimaOrdenColaborador = act_CLB_WALK;
		if(!ConsideraColaborador2(st_result,st_result,st_result.jugador)){
			st_result.ultimaOrdenColaborador = save;
		}
		break;
	case act_CLB_TURN_SR:
		st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
		ConsideraColaborador2(st_result,st_result,st_result.jugador);
		break;
	case act_CLB_STOP:
		st_result.ultimaOrdenColaborador = act_CLB_STOP;
		break;
	case actIDLE:
		ConsideraColaborador2(st_result,st_result,st_result.jugador);
		break;
	}
	
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

struct CmpN4{
	bool operator()(const nodeNX<stateN4> &lhs, const nodeNX<stateN4> &rhs )const{
		return(lhs.st.heuristica > rhs.st.heuristica);
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

	int max = numeric_limits<int>::max();
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


bool ComportamientoJugador::a_starJugadorN4(const stateN4 &inicio, const ubicacion &final,bool (*verificador) (const stateN4&, const ubicacion&)){
	
	int COSTE_ACCION;
	if(hay_baterias){
		COSTE_ACCION = 10;
	}else{
		COSTE_ACCION = 0; 
	}
	int aux; 
	nodeNX<stateN4> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;
	current_node.st.costo_bateria = 0;
	current_node.st.finish = false;
	bool SolutionFound = false; 

	if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'D'){
		current_node.st.jugador_zapatillas = true;
		current_node.st.jugador_bikini = false;
	}else if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'K') {
		current_node.st.jugador_zapatillas = false;
		current_node.st.jugador_bikini = true;
	}

	vector<Action> accion = {actWALK,actRUN,actTURN_L,actTURN_SR};

	priority_queue<nodeNX<stateN4>, vector<nodeNX<stateN4>>, CmpN4> frontier;
	unordered_map<unsigned long long, nodeNX<stateN4>> explored;

	frontier.push(current_node);
	explored.insert({Hash::hashN2(current_node),current_node});
	nodeNX<stateN4> solution = nodeNX<stateN4>();
	int i = 0;
	nodeNX<stateN4> child;

	while(!frontier.empty()){
		frontier.pop();
		
		i = 0; 
		unordered_map<unsigned long long,nodeNX<stateN4>>::iterator it = explored.find(Hash::hashN2(current_node));
		unordered_map<unsigned long long,nodeNX<stateN4>>::iterator j;

		if(!verificador(it->second.st, final)){
			do{

				child.st = applyJ(accion.at(i),current_node.st);
				aux = costeBateriaN4(accion.at(i),it->second.st);
				child.st.costo_bateria = it->second.st.costo_bateria + COSTE_ACCION + aux;

				if(CalcDistancia(child.st,final) < 5){
					child.st.heuristica = child.st.costo_bateria + CalcDistancia(child.st,final) +  aux;
				}else{
					child.st.heuristica = child.st.costo_bateria + CalcDistancia(child.st,final);
				}

				child.padre = &(it->second); 


				switch (mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c))
				{
					case 'D':
						child.st.jugador_zapatillas = true;
						child.st.jugador_bikini = false;
						break;
					case 'K':
						child.st.jugador_zapatillas = false;
						child.st.jugador_bikini = true;
						break;
					default:
						break;
				}
				
				
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

			}while (i < 2);

			while(i < 4){
				child.st = applyJ(accion.at(i),current_node.st);
				aux = costeBateriaN4(accion.at(i),it->second.st);
				child.st.costo_bateria = it->second.st.costo_bateria + COSTE_ACCION + aux;
				
				if(CalcDistancia(child.st,final) < 5){
					child.st.heuristica = child.st.costo_bateria + CalcDistancia(child.st,final) +  aux;
				}else{
					child.st.heuristica = child.st.costo_bateria + CalcDistancia(child.st,final);
				}
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
			solution = current_node;
		}

	}

	

	if(!verificador(solution.st, final) and busco_colab and colab){
		colab = false; 
		SolutionFound = false;
	}else if(!verificador(solution.st, final)){
		SolutionFound = false;
	}else{
		SolutionFound = true;
		PlanJugador(solution);
	}
		

	return (SolutionFound);

}


bool ComportamientoJugador::a_starBateriaN4(const stateN4 &inicio, const ubicacion &final,bool (*verificador) (const stateN4&, const ubicacion&)){
	
	const int COSTE_ACCION = 10; 
	
	nodeNX<stateN4> current_node;
	current_node.st = inicio;
	current_node.padre = nullptr;
	current_node.st.costo_bateria = 0;
	current_node.st.finish = false;
	bool SolutionFound = false; 

	if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'D'){
		current_node.st.jugador_zapatillas = true;
		current_node.st.jugador_bikini = false;
	}else if(mapaResultado.at(current_node.st.jugador.f).at(current_node.st.jugador.c) == 'K') {
		current_node.st.jugador_zapatillas = false;
		current_node.st.jugador_bikini = true;
	}

	vector<Action> accion = {actWALK,actRUN,actTURN_L,actTURN_SR};

	priority_queue<nodeNX<stateN4>, vector<nodeNX<stateN4>>, CmpN4> frontier;
	unordered_map<unsigned long long, nodeNX<stateN4>> explored;

	frontier.push(current_node);
	explored.insert({Hash::hashN2(current_node),current_node});
	nodeNX<stateN4> solution = nodeNX<stateN4>();
	int i = 0;
	int min;
	int val_aux;
	nodeNX<stateN4> child;

	while(!frontier.empty()){
		frontier.pop();
		
		i = 0; 
		unordered_map<unsigned long long,nodeNX<stateN4>>::iterator it = explored.find(Hash::hashN2(current_node));
		unordered_map<unsigned long long,nodeNX<stateN4>>::iterator j;
		auto k = baterias.begin();

		while(k != baterias.end() and !SolutionFound){
			if(verificador(current_node.st,*k)){
				SolutionFound = true;
			}
			k++;
		}

		if(!SolutionFound){
			do{
				min = numeric_limits<int>::max();
				child.st = applyJ(accion.at(i),current_node.st);
				child.st.costo_bateria = it->second.st.costo_bateria + COSTE_ACCION + costeBateriaN4(accion.at(i),it->second.st);
				for(auto k = baterias.begin(); k != baterias.end(); k++){
					val_aux = CalcDistancia(child.st,(*k));
					if(val_aux < min){
						min = val_aux;
					}
				}
				child.st.heuristica = child.st.costo_bateria + COSTE_ACCION + min;
				child.padre = &(it->second); 


				switch (mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c))
				{
					case 'D':
						child.st.jugador_zapatillas = true;
						child.st.jugador_bikini = false;
						break;
					case 'K':
						child.st.jugador_zapatillas = false;
						child.st.jugador_bikini = true;
						break;
					default:
						break;
				}
				
				
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

			}while (i < 2);

			while(i < 4){
				min = numeric_limits<int>::max();
				child.st = applyJ(accion.at(i),current_node.st);
				child.st.costo_bateria = it->second.st.costo_bateria + COSTE_ACCION + costeBateriaN4(accion.at(i),it->second.st);
				for(auto k = baterias.begin(); k != baterias.end(); k++){
					val_aux = CalcDistancia(child.st,(*k));
					if(val_aux < min){
						min = val_aux;
					}
				}
				child.st.heuristica = child.st.costo_bateria + COSTE_ACCION + min;
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
			solution = current_node;
		}

	}
		

	PlanJugador(solution);

	return (SolutionFound);

}

void ComportamientoJugador::Heuristica(const stateN4 &inicio, const ubicacion &final){
	camino.clear();

	int COSTE_ACCION;
	if(hay_baterias){
		COSTE_ACCION = 10;
	}else{
		COSTE_ACCION = 0; 
	}

	nodeNX<stateN4> current_node;
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

	priority_queue<nodeNX<stateN4>, vector<nodeNX<stateN4>>, CmpN4> frontier;
	unordered_map<unsigned long long, nodeNX<stateN4>> explored;

	bool SolutionFound = (current_node.st.colaborador.f== final.f and
						  current_node.st.colaborador.c == final.c);

	frontier.push(current_node);
	explored.insert({Hash::hashN3(current_node),current_node});
	unordered_map<unsigned long long,nodeNX<stateN4>>::iterator j, it;

	nodeNX<stateN4> solution = nodeNX<stateN4>();
	nodeNX<stateN4> child;


	while (!frontier.empty() and !SolutionFound){
		frontier.pop();

		it = explored.find(Hash::hashN3(current_node));

		//La heurística es monótona
		if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
				solution = current_node;
				SolutionFound = true;
		}else if(current_node.st.costo_bateria <= it->second.st.costo_bateria){

			child.st = applyJC(act_CLB_WALK,current_node.st);
			child.padre = &(it->second); 
			child.st.costo_bateria = current_node.st.costo_bateria + COSTE_ACCION + costeBateriaColab(act_CLB_WALK,current_node.st);
			child.st.heuristica =  CalcDistanciaColab(child.st, goal) + child.st.costo_bateria;
			

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

			child.st = applyJC(act_CLB_TURN_SR,current_node.st);
			child.padre = &(it->second); 
			child.st.costo_bateria = current_node.st.costo_bateria + COSTE_ACCION + costeBateriaColab(act_CLB_TURN_SR,current_node.st);
			child.st.heuristica =  CalcDistanciaColab(child.st, goal) + child.st.costo_bateria;

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

		}

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.top();
		}
	}

	int costo = solution.st.costo_bateria;
	stateN4 aux;

	cout << "pasa" << endl; 

	while(solution.padre != nullptr){

		aux = solution.st;

		aux.costo_bateria = costo - solution.st.costo_bateria;
		cout << aux.costo_bateria << endl; 
		camino.insert({Hash::hashNS(aux), aux});
		solution = *solution.padre;
	}


}



bool ComportamientoJugador::a_starColabN4(const stateN4 &inicio, const ubicacion &final){
	int COSTE_ACCION;
	if(hay_baterias){
		COSTE_ACCION = 10;
	}else{
		COSTE_ACCION = 0; 
	}
	int rebaja = 0; 
	const int INFINITO = 5000;
	nodeNX<stateN4> current_node;
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

	priority_queue<nodeNX<stateN4>, vector<nodeNX<stateN4>>, CmpN4> frontier;
	unordered_map<unsigned long long, nodeNX<stateN4>> explored;

	bool SolutionFound = (current_node.st.colaborador.f== final.f and
						  current_node.st.colaborador.c == final.c);

	frontier.push(current_node);
	explored.insert({Hash::hashN3(current_node),current_node});
	unordered_map<unsigned long long,nodeNX<stateN4>>::iterator it, j;
	unordered_map<unsigned long long, stateN4>::iterator k;
	nodeNX<stateN4> solution = nodeNX<stateN4>();
	int i = 0;
	nodeNX<stateN4> child;
	bool veo_colab;


	while (!frontier.empty() and !SolutionFound and current_node.st.costo_bateria <= LIMITE){
		frontier.pop();

		i = 0; 
		cout << COSTE_ACCION << endl;
		if(current_node.st.costo_bateria < 0){
			cout << current_node.st.jugador.f << "," << current_node.st.jugador.c << endl; 
			cout << current_node.st.colaborador.f << "," << current_node.st.colaborador.c << endl;
			
		}

		it = explored.find(Hash::hashN3(current_node));
		//La heurística es monótona
		veo_colab = PuedoVerColaborador(current_node.st);

		if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c){
			if(veo_colab){
				solution = current_node;
				SolutionFound = true;
			}
		}else if(current_node.st.costo_bateria <= it->second.st.costo_bateria){

			if(veo_colab){
				while( i < 3){
					child.st = applyJC(accion.at(i),current_node.st);
					child.padre = &(it->second); 

					if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'D'){
						child.st.colaborador_zapatillas = true;
						child.st.colaborador_bikini = false;
					}else if(mapaResultado.at(child.st.colaborador.f).at(child.st.colaborador.c) == 'K') {
						child.st.colaborador_zapatillas = false;
						child.st.colaborador_bikini = true;
					}

					k = camino.find(Hash::hashNS(child.st));
					child.st.costo_bateria = current_node.st.costo_bateria + COSTE_ACCION + costeBateriaColab(accion.at(i),current_node.st);

					if(k == camino.end()){
						child.st.heuristica =  INFINITO + child.st.costo_bateria;

					}else{
						
						child.st.heuristica =  k->second.costo_bateria + child.st.costo_bateria;
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
				child.st = applyJC(accion.at(i),current_node.st);
				child.padre = &(it->second); 

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

				k = camino.find(Hash::hashNS(child.st));
				if(accion.at(i) == actIDLE and mapaResultado.at(child.st.jugador.f).at(child.st.jugador.c) == 'X'){
					rebaja = -10;
				}
				child.st.costo_bateria = current_node.st.costo_bateria + COSTE_ACCION + costeBateriaN4(accion.at(i),current_node.st) + 
										 costeBateriaColab(current_node.st.ultimaOrdenColaborador, current_node.st) + rebaja;
				

				if(k == camino.end()){
					child.st.heuristica =  INFINITO + child.st.costo_bateria;

				}else{
					
					child.st.heuristica = k->second.costo_bateria + child.st.costo_bateria;
				}

				rebaja = 0;

				

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
			
		}

		if (!frontier.empty()){
			current_node = frontier.top();
		}
		

	}
	
	if(solution.st.costo_bateria > LIMITE){
		SolutionFound = false; 
	}
	while(solution.padre != nullptr and SolutionFound){

		if(solution.st.jugador == solution.padre->st.jugador){
			if(solution.st.colaborador.f == solution.padre->st.colaborador.f){
				if(solution.st.colaborador.c == solution.padre->st.colaborador.c){
					if((solution.padre->st.colaborador.brujula - solution.st.colaborador.brujula +8)%8 == 7){
						if(solution.padre->st.ultimaOrdenColaborador == act_CLB_TURN_SR){
							plan.emplace_front(actIDLE);
						}else{
							plan.emplace_front(act_CLB_TURN_SR);
						}
					}else{
						plan.emplace_front(act_CLB_STOP);
					}
				}else if(solution.padre->st.ultimaOrdenColaborador == act_CLB_WALK){
					plan.emplace_front(actIDLE);
				}else{
					plan.emplace_front(act_CLB_WALK);
					
				}
			}else if(solution.padre->st.ultimaOrdenColaborador == act_CLB_WALK){
				plan.emplace_front(actIDLE);
			}else{
				plan.emplace_front(act_CLB_WALK);
				
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

	if(SolutionFound){
		plan.push_back(act_CLB_STOP);
	}

	

	return SolutionFound;

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
	pair<int,int> estable; 
	pivote.first = st.jugador.f; 
	pivote.second = st.jugador.c;

	pivote_izd = pivote_der = pivote; 

	int i = 0;

	do{

		mapaResultado.at(pivote.first).at(pivote.second) = terreno.at(i*(i+1));
		if(terreno.at(i*(i+1)) != 'P' and terreno.at(i*(i+1)) != 'M'){
			refresh.push(pivote);
		}
		pivote_izd = pivote_der = pivote; 

		for(int j = i + 1; j <= 3; j++){
			ActualizarConOrientacion(static_cast<Orientacion>((st.jugador.brujula + 7)%8), pivote_izd.first, pivote_izd.second,1);
			ActualizarConOrientacion(static_cast<Orientacion>((st.jugador.brujula + 1)%8), pivote_der.first, pivote_der.second,1);
			
			mapaResultado.at(pivote_izd.first).at(pivote_izd.second) = terreno.at(j*j + i);
			mapaResultado.at(pivote_der.first).at(pivote_der.second) = terreno.at(j*(j+2) - i);

			if(terreno.at(j*j + i) != 'P' and terreno.at(j*j + i) != 'M' ){
				refresh.push(pivote_izd);
			}
				
			if(terreno.at(j*(j+2) - i) != 'P' and terreno.at(j*(j+2) - i) != 'M'){
				refresh.push(pivote_der);
			}

		}
		ActualizarConOrientacion(st.jugador.brujula, pivote.first, pivote.second,1);
		i++;

	}while(i < 4);
	

}

void ComportamientoJugador::ActualizaCola(){
	pair<int,int> q;
	q = refresh.front();
	while(!refresh.empty()){
		refresh.pop();
		pair<int,int> pivote;
		if(!accesible.at(q.first).at(q.second)){
			for(int i = 0; i< 8; i++){
				pivote = q;
				ActualizarConOrientacion(static_cast<Orientacion>(i), pivote.first, pivote.second,1);
				if(accesible.at(pivote.first).at(pivote.second)){
					accesible.at(q.first).at(q.second) = true; 
				}
			}
		}

		unsigned char c;

		for(int i = 0; i< 8; i++){
			pivote = q;
			ActualizarConOrientacion(static_cast<Orientacion>(i), pivote.first, pivote.second,1);
			c = mapaResultado.at(pivote.first).at(pivote.second);
			if(accesible.at(q.first).at(q.second) and !accesible.at(pivote.first).at(pivote.second) and c != 'M' and c != 'P' and c != '?'){
				accesible.at(pivote.first).at(pivote.second) = true; 
				refresh.push(pivote);

			}

			if(mapaResultado.at(pivote.first).at(pivote.second) == 'X' and accesible.at(pivote.first).at(pivote.second)){
				hay_baterias = true; 
				ubicacion u;
				u.f = pivote.first;
				u.c = pivote.second;
				u.brujula = norte;
				baterias.push_back(u);
			}
		}

		if(mapaResultado.at(q.first).at(q.second) == 'X' and accesible.at(q.first).at(q.second)){
				hay_baterias = true; 
				ubicacion u;
				u.f = q.first;
				u.c = q.second;
				u.brujula = norte;
				baterias.push_back(u);
			}
		

		

		if(!refresh.empty()){
			q = refresh.front();
		}

	}
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
			accesible.at(c_state.jugador.f).at(c_state.jugador.c) = true;
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

		busco_colab = false; 

		if(hay_baterias){
			LIMITE = 1400;
		}else{
			LIMITE = 1020;
		}

		if(estoy_ubicado == false){
			accion = actWHEREIS;
		}else{
			
			if(recargando and mapaResultado.at(c_state.jugador.f).at(c_state.jugador.c) == 'X'){
				last_action = actIDLE;
				double limite = 1;
				if(sensores.vida > 500){
					if(sensores.bateria > 2980 ){
						recargando = false; 
						plan.clear();
					}
				}
				if(sensores.vida < 500){
					if(sensores.bateria  > 2000){
						recargando = false; 
						plan.clear();
					}
				}
				if(sensores.vida < 100){
					recargando = false; 
					plan.clear();
				}
				
				return actIDLE;

			}else if(sensores.bateria < 1200 and sensores.vida > 100 and hay_baterias and !hayPlan ){
				plan.clear();
				bool (*selector) (const stateN4&, const ubicacion&);
				selector = Accesible;
				hayPlan = a_starBateriaN4(c_state,goal,selector);

				if(plan.size() > 0){
					VisualizaPlan(c_state,plan);
					PintaPlan(plan);
					recargando = true;
				}

			}else if((c_state.jugador.f ==  sensores.destinoF and c_state.jugador.c == sensores.destinoC) or (c_state.colaborador.f ==  sensores.destinoF and c_state.colaborador.c == sensores.destinoC)){
				hayPlan = false; 
				plan.clear();
			}else if(mapaResultado.at(c_state.colaborador.f).at(c_state.colaborador.c) == '?' and colab and CalcDistancia(c_state, c_state.colaborador) < 100){
				cout << "entra" << endl; 
				goal.f = c_state.colaborador.f;
				goal.c = c_state.colaborador.c;
				busco_colab = true; 
			}else{
				goal.f = sensores.destinoF;
				goal.c = sensores.destinoC;
			}

			if(sensores.vida < 100){
				if(hayPlan and recargando){
					hayPlan = false; 
					plan.clear();
				}
				recargando = false; 
			}



			
			PonerTerrenoMapa(sensores.terreno, c_state);
			ActualizaCola();

			if(sensores.terreno.at(0) == 'K'){
				c_state.jugador_bikini = true;
				c_state.jugador_zapatillas = false; 
			}else if(sensores.terreno.at(0) == 'D'){
				c_state.jugador_bikini = false;
				c_state.jugador_zapatillas = true; 
			}
			switch(  mapaResultado.at(c_state.colaborador.f).at(c_state.colaborador.c) ){
				case 'K':
				c_state.colaborador_bikini = true; 
				c_state.colaborador_zapatillas = false;

				case 'D':
				c_state.colaborador_bikini = false;
				c_state.colaborador_zapatillas = true; 
				break;
				default: 
				break;
			}
			

			if(recargando){
				if(plan.size() > 0){
					VisualizaPlan(c_state,plan);
					PintaPlan(plan);
				}
				
				if (hayPlan and plan.size()>0){
					accion = plan.front();
					plan.pop_front();
				}

				if (plan.size()== 0){
					hayPlan = false;
				}
			}else{
				
				if(!hayPlan){
					if(mapaResultado.at(goal.f).at(goal.c) != '?' and !busco_colab){
						Refreshaceptable();
						unsigned char c = mapaResultado.at(goal.f).at(goal.c);
						if( c != 'M' and c != 'P'){
							aceptable.at(goal.f).at(goal.c) = 0;
							acepCol.push(pair<int,int>(goal.f,goal.c));
							genAceptable();
						}
						
						
						if(aceptable.at(c_state.colaborador.f).at(c_state.colaborador.c) != -1){
							
							Heuristica(c_state,goal);
							hayPlan = a_starColabN4(c_state,goal);
						}else{
							bool (*selector) (const stateN4&, const ubicacion&);
							selector = Accesible;
							hayPlan = a_starJugadorN4(c_state,goal,selector);

						}
						
					}else if(busco_colab){
						bool (*selector) (const stateN4&, const ubicacion&);
						selector = llega_colab;
						hayPlan = a_starJugadorN4(c_state,goal,selector);
					}
					
					if(!hayPlan){
						bool (*selector) (const stateN4&, const ubicacion&);
						selector = Accesible;
						hayPlan = a_starJugadorN4(c_state,goal,selector);
					}

					if(!hayPlan){
						cout << "entra2" << endl; 
						int max = 0;
						int suma = 0;
						ubicacion ubi;
						for(int i = 0; i< mapaResultado.size(); i++){
							for(int j = 0; j< mapaResultado.size(); j++){

								if(accesible.at(i).at(j) and i != c_state.colaborador.f and j != c_state.colaborador.c ){
									suma = 0;
									for(int k = i-3; k <=3+i; k++){
										for(int l = j-3 ; l <= j+3 ; l++){
											if(mapaResultado.at(k).at(l) == '?'){
												suma++;
											}
										}
									}

									if(suma > max){
										max = suma;
										goal.f = i;
										goal.c = j;
									}

								}
							}
						}

						bool (*selector) (const stateN4&, const ubicacion&);
						selector = Accesible;
						hayPlan = a_starJugadorN4(c_state,goal,selector);

						if(!hayPlan){
							accion = actTURN_SR;
						}


					}
					
					if(plan.size() > 0){
						VisualizaPlan(c_state,plan);
						//PintaPlan(plan);
					}
				}
				
				if (hayPlan and plan.size()>0){
					accion = plan.front();
					plan.pop_front();
				}

				if (plan.size()== 0){
					hayPlan = false;
				}

				

			}
			
		}

		last_action = accion;
		
	}


	

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
