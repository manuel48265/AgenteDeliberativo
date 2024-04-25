#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <map>
#include <limits>
#include <iostream>

struct point{
	int i;
	int j;
	int bateria; 

	point(){
		i = 0;
		j = 0;
		bateria = 0;
	
	}
  point(int i, int j,int bateria){
    this->i = i;
    this->j = j;
    this->bateria = bateria; 
  }
};

int CosteNormal(const Action &act,const point &p, const vector<vector< unsigned char>> &mapa);
int CosteZapatillas(const Action &act,const point &p, const vector<vector< unsigned char>> &mapa);
int CosteBikini(const Action &act,const point &p, const vector<vector<unsigned char>> &mapa);
void GenerarMapa(const point &origin,int caso,int (*punt_funct)(const Action&,const point&, const vector<vector< unsigned char>> &mapa), 
				 const vector<vector< unsigned char>> &mapa, vector<vector<vector<int>>> &bateriaColab);
void GenerarMapa2(int k, int (*punt_funct)(const Action&,const point&, const vector<vector< unsigned char>> &mapa),
const vector<vector< unsigned char>> &mapa, const vector<vector<vector<int>>> &bateriaColab, vector<vector<vector<int>>> &costo_real);

template<typename T> struct nodeNX{
  T st;
  nodeNX<T>* padre; 

  nodeNX(){
    st = T();
    padre = nullptr;
  }

  void copy(const nodeNX &rhs){
    st = rhs.st;
    padre = rhs.padre;
  }

  nodeNX(const T &rhs){
    copy(rhs);
  }

  nodeNX(const nodeNX &rhs){
    if(this != &rhs){
      copy(rhs);
    }
  }

  nodeNX& operator=(const nodeNX &rhs){
    if(this != &rhs){
      copy(rhs);
    }
    return *this;
  }

};

struct state{
  ubicacion jugador;
  ubicacion colaborador; 
  Action ultimaOrdenColaborador;

  state(){
    jugador = ubicacion();
    colaborador = ubicacion();
    ultimaOrdenColaborador = act_CLB_STOP;
  }

  void copy(const state &rhs){
    
    jugador = rhs.jugador;
    colaborador = rhs.colaborador;
    ultimaOrdenColaborador = rhs.ultimaOrdenColaborador;
  
  }

  state(const state &rhs){
    if(this != &rhs){
      copy(rhs);
    }
  }

  state& operator=(const state &rhs){
    if(this != &rhs){
      copy(rhs);
    }
    return *this;
  }

  
  bool operator==(const state &x)const{
    return(jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c );
  }

};

struct stateN1 : state{
  stateN1() : state(){}

  bool operator==(const stateN1 &x)const{
    bool devolver_1 = (jugador == x.jugador and colaborador == x.colaborador and ultimaOrdenColaborador == x.ultimaOrdenColaborador);

    return (devolver_1);
  }

};


struct stateN2 : stateN1
{
  unsigned int costo_bateria; 
  bool jugador_zapatillas;
  bool jugador_bikini;

  stateN2() : stateN1(){
    costo_bateria = numeric_limits<unsigned int>::max();
    jugador_zapatillas = false;
    jugador_bikini = false; 
  }

  void copy(const stateN2 &rhs){
    stateN1::copy(rhs);
    costo_bateria = rhs.costo_bateria;
    jugador_zapatillas = rhs.jugador_zapatillas;
    jugador_bikini = rhs.jugador_bikini;
  }

  stateN2(const stateN2 &rhs){
    if(this != &rhs){
      copy(rhs);
    }
  }

  stateN2& operator=(const stateN2 &rhs){
    if(this != &rhs){
      copy(rhs);
    }
    return *this;
  }

  bool operator==(const stateN2 &x)const{
    bool devolver_1 = (jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c );
    bool devolver_2 = (costo_bateria == x.costo_bateria and jugador_bikini == x.jugador_bikini and jugador_zapatillas == x.jugador_zapatillas);

    return (devolver_1 and devolver_2);
  }

  bool operator>(const stateN2 &x)const{
    return (this->costo_bateria > x.costo_bateria);
  }

};

struct stateN3 : stateN2
{
  bool colaborador_zapatillas; 
  bool colaborador_bikini;
  unsigned int heuristica;

  stateN3() : stateN2(){
    colaborador_bikini = false;
    colaborador_zapatillas = false; 
    heuristica = 0;
  }

  void copy(const stateN3 &rhs){
    stateN2::copy(rhs);
    colaborador_bikini = rhs.colaborador_bikini;
    colaborador_zapatillas = rhs.colaborador_zapatillas;
    this->heuristica = rhs.heuristica;
  }

  stateN3(const stateN3 &rhs){
    if(this != &rhs){
      copy(rhs);
    }
  }

  stateN3& operator=(const stateN3 &rhs){
    if(this != &rhs){
      copy(rhs);
    }
    return *this;
  }

  bool operator>(const stateN3 &x)const{
    return (this->heuristica > x.heuristica);
  }

  bool operator==(const stateN3 &x)const{
    bool devolver_1 = (jugador == x.jugador and colaborador == x.colaborador and ultimaOrdenColaborador == x.ultimaOrdenColaborador);
    bool devolver_2 = (jugador_bikini == x.jugador_bikini and jugador_zapatillas == x.jugador_zapatillas and costo_bateria == x.costo_bateria);
    bool devolver_3 = (colaborador_zapatillas == x.colaborador_zapatillas and colaborador_bikini == x.colaborador_bikini);

    return (devolver_1 and devolver_2 and devolver_3);
  }

};


struct stateN4 : stateN3
{

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
      c_state = stateN4();
      goal = ubicacion();
      estoy_ubicado = false; 
      last_action = actIDLE;
      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    void VisualizaPlan(const state &st, const list<Action> &plan);
    bool CasillaTransitable(const ubicacion &x);
    bool AnchuraSoloJugador(const state &inicio, const ubicacion &final);
    bool AnchuraColaborador(const stateN1 &inicio, const ubicacion &final);
    bool PuedoVerColaborador(const stateN1 &st);
    bool CosteUniforme(const stateN2 &inicio, const ubicacion &final);
    bool a_star(const stateN3 &inicio, const ubicacion &final);
    void Actualizar(ubicacion &pos, int k);
    ubicacion NextCasilla(const ubicacion &pos);
    void priv_apply(const Action &a, state &st);
    void apply_priv_NI(const Action &a, stateN1 &st);
    state apply(const Action &a, const state &st);
    stateN1 apply(const Action &a, const stateN1 &st);
    stateN2 apply(const Action &a, const stateN2 &st);
    stateN3 apply(const Action &a, const stateN3 &st);
    unsigned int costeBateriaNX(const Action &act,const stateN2 &st);
    unsigned int costeBateriaColab(const Action &act,const stateN3 &st);
    void PlanJugador(const nodeNX<state> &node);
    void PlanJugador(const nodeNX<stateN2> &node);
    bool ConsideraColaborador(const stateN1 &st, state &st_result, const ubicacion & sig_ubicacion);
    int CalcDistanciaColab(const stateN3 &origin, const pair<int,int> &p);
    int CalcHeuristica(const stateN3& st);
    int Mejorada(const stateN3 &st);
    void PonerTerrenoMapa(const vector<unsigned char> &terreno, const stateN4 &st);
    void ActualizarEstado(const Sensores &sensores);
    int Minimos(const stateN3 &st);

    void GenerarN3(const ubicacion &goal){
      vector<int> v; 
      vector<vector<int>> u;
      for(int i = 0; i< mapaResultado.size(); i++){
        v.push_back(-1);
      }

      for(int i = 0; i< mapaResultado.size(); i++){
        u.push_back(v);
      }

      for(int i = 0; i<3; i++){
        costo_real.push_back(u);
      }
      
      for(int i = 0; i<3; i++){
        bateriaColab.push_back(u);
      }


      point p; 
      p.i = goal.f;
      p.j = goal.c;

      int (*ptr_funct)(const Action&,const point&, const vector<vector<unsigned char>> &mapa) = CosteNormal;

      GenerarMapa(p,0,ptr_funct, mapaResultado,bateriaColab);
      ptr_funct = CosteZapatillas;
      GenerarMapa(p,1,ptr_funct, mapaResultado,bateriaColab);
      ptr_funct = CosteBikini;
      GenerarMapa(p,2,ptr_funct, mapaResultado,bateriaColab);

      for(int i = 0; i< mapaResultado.size(); i++){
        for(int j = 0; j < mapaResultado.size(); j++){
          if(mapaResultado.at(i).at(j) == 'D'){
            zapatillas.push_back(make_pair(i,j));
          }else if(mapaResultado.at(i).at(j) == 'K'){
            bikinis.push_back(make_pair(i,j));
          }
        }
      }
    }

    void N3Mejora(const ubicacion &goal){
      int (*ptr_funct)(const Action&,const point&, const vector<vector<unsigned char>> &mapa) = CosteNormal;

      GenerarMapa2(0,ptr_funct, mapaResultado,bateriaColab, costo_real);
      ptr_funct = CosteZapatillas;
      GenerarMapa2(1,ptr_funct, mapaResultado,bateriaColab, costo_real);
      ptr_funct = CosteBikini;
      GenerarMapa2(2,ptr_funct, mapaResultado,bateriaColab, costo_real);
      

    } 

    ~ComportamientoJugador(){}

    
    
   

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    // Declarar Variables de Estado
    const int VISION = 3;
    bool  hayPlan;
    list<Action> plan; 
    stateN4 c_state;
    ubicacion goal; 
    int tam; 
    bool estoy_ubicado;
    Action last_action;

    vector<vector<vector<int>>> bateriaColab;
    vector<pair<int, int>> zapatillas;
    vector<pair<int, int>> bikinis;
    vector<vector<vector<int>>> costo_real;


};

#endif
