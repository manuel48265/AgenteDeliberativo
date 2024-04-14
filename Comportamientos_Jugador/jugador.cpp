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


map<Action,int> conversion = {{act_CLB_STOP,0}, {act_CLB_WALK,1}, {act_CLB_TURN_SR,2}};

void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	for (int i = 0; i < matriz.size(); i++)
		for (int j = 0; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

struct Hash{
	size_t operator()(const nodeN0 &rhs) const {
	string s; 
	s = to_string(rhs.st.jugador.f) + '/' + to_string(rhs.st.jugador.c) + '/'+ to_string(rhs.st.jugador.brujula) + '/';
	s += to_string(rhs.st.colaborador.f) + '/' +to_string(rhs.st.colaborador.c) + '/'+ to_string(rhs.st.colaborador.brujula) + '/';
	s += to_string(rhs.st.ultimaOrdenColaborador);
	

	return (hash<std::string>{}(s)); 
	}

};


bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M' );
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos){
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}


stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa){
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				st_result.jugador = sig_ubicacion;
			}
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (CasillaTransitable(sig_ubicacion, mapa) and 
			!(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				sig_ubicacion2 = NextCasilla(sig_ubicacion);
				if (CasillaTransitable(sig_ubicacion2, mapa) and 
					!(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						st_result.jugador = sig_ubicacion2;
				}
			}
		break;

	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}
	return st_result;
}

bool Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while (it != lista.end() and !(it->st == item))
		it++;

	return (!(it == lista.end()));
}


bool ComportamientoJugador::AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,
						const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	current_node.st = inicio;
	current_node.padre = nullptr; 

	vector<Action> accion = {actWALK,actRUN,actTURN_L,actTURN_SR};
	list<nodeN0> frontier;
	vector<nodeN0> v;
	map<int, nodeN0> explored;
	frontier.push_back(current_node);
	explored.insert({Hash{}(current_node),current_node});
	bool SolutionFound = (current_node.st.jugador.f == final.f and
						  current_node.st.jugador.c == final.c);
	int i = 0;
	nodeN0 child;

	while (!frontier.empty() and !SolutionFound){
		frontier.pop_front();

		// Generar hijo actWALK
		i = 0; 

		mapaConPlan[current_node.st.jugador.f][current_node.st.jugador.c] = 0;

		map<int,nodeN0>::iterator it = explored.find(Hash{}(current_node));
		
		do{
			child.st = apply(accion.at(i),current_node.st,mapa);
			child.padre = &(it->second); 
			
			if (child.st.jugador.f == final.f and child.st.jugador.c == final.c){
				current_node = child;
				SolutionFound = true;
			}

			else if (explored.find(Hash{}(child)) == explored.end()){
				frontier.push_back(child);
				explored.insert({Hash{}(child),child}); 
			}
			i++;

		}while (i < 2 and !SolutionFound);

		while(i < 4 and !SolutionFound){
			child.st = apply(accion.at(i), current_node.st,mapa);
			child.padre = &(it->second);

			if (explored.find(Hash{}(child)) == explored.end()){
				frontier.push_back(child);
				explored.insert({Hash{}(child),child}); 
			}
			i++;
		} 

		if (!SolutionFound and !frontier.empty()){
			current_node = frontier.front();
		}

	}
	
	while(current_node.padre != nullptr){
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
		current_node = *current_node.padre; 
		
	}

	cout << explored.size() << endl;

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

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

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
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 2;
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

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	if (!hayPlan){
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.colaborador.f = sensores.CLBposF;
		c_state.colaborador.c = sensores.CLBposC;
		c_state.colaborador.brujula = sensores.CLBsentido;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;
		hayPlan = AnchuraSoloJugador(c_state,goal,mapaResultado);
		PintaPlan(plan);
		
		VisualizaPlan(c_state,plan);
	}

	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}

	if (plan.size()== 0){
		hayPlan = false;
	}

	

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
