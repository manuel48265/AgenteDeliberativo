#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <map>

struct stateN0{
  ubicacion jugador;
  ubicacion colaborador; 
  Action ultimaOrdenColaborador;
  
  bool operator==(const stateN0 &x)const{
    return(jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c);
  }


};

struct nodeN0{
	stateN0 st;
	nodeN0* padre;

  nodeN0(){
    this->st = stateN0();
    this->padre = nullptr;
  }

  nodeN0(const nodeN0 &N){
    if(this != &N){
      this->st = N.st;
      this->padre = N.padre;
    }
  }

	bool operator==(const nodeN0 &n) const {
		return (st == n.st);
	}

	bool operator<(const nodeN0 &b)  const {
		return  (st.jugador.f < b.st.jugador.f) || (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c) || (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula);
	}



};


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      tam = size;
      init();
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      tam = mapaR.size();
      init();
    }

    void init(){
      hayPlan = false; 
      c_state = stateN0();
      goal = ubicacion();
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,const vector<vector<unsigned char>> &mapa);
    ~ComportamientoJugador(){}

    
    
   

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    // Declarar Variables de Estado
    bool  hayPlan;
    list<Action> plan; 
    stateN0 c_state;
    ubicacion goal; 
    int tam; 





};

#endif
