#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <fstream>
#include <random>
#include <deque>
#include <algorithm>

//Definicion de rectangulo.
class rectangulo{
  public:
    int alto;
    int ancho;
    bool giro; //True si se gira y False si no se gira.
    int id;
  rectangulo(int al, int an, int gi,int ide){
    this->alto=al;
    this->ancho=an;
    this->giro=gi;
    this->id=ide;
  }
};
class posicion{
  public:
    int x1;
    int y1;
    int x2;
    int y2;
  posicion(int al, int an,int al2, int an2){
    this->x1=al;
    this->y1=an;
    this->x2=al2;
    this->y2=an2;
  }
};

class dos{
  public:
    int uno;
    int d;
  dos(int u, int uu){
    this->uno=u;
    this->d=uu;
  }
};

class punto{
  public:
  int x;
  int y;
  int altura;
  punto(int ek, int i, int al){
    this->x=ek;
    this->y=i;
    this->altura=al;
  }
};


void print(std::vector<rectangulo> l){
  int count=1;
  for(rectangulo r : l){
    std::cout<<count <<" "<<r.id<<" "<<r.giro<<"\n";
    count+=1;
  }
}

unsigned int evaluacion(std::vector<rectangulo> rec, int w){

  std::vector<punto> pu = std::vector<punto>();
  rectangulo r=rec[0];
  int altura,ancho,mini,ymax,save;
  bool cambio=false,puntox;

  punto pun= punto(1,2,3),pun2=punto(1,2,3);
  if (!r.giro){
    altura=r.alto;
    ancho=r.ancho;
  }
  else{
    altura=r.ancho;
    ancho=r.alto;
  }

  pu.push_back(punto(ancho,0,altura));
  ymax=altura;

  for(unsigned int i=1; i<rec.size();i++){
    cambio=false;
    r=rec.at(i);
    if (!r.giro){
      altura=r.alto;
      ancho=r.ancho;
    }
    else{
      altura=r.ancho;
      ancho=r.alto;
    }
    mini=std::numeric_limits<int>::max();
    puntox=false;
    for(unsigned int k=0;k<pu.size();k++){

      if(pu[k].x+ancho<=w && pu[k].altura>=altura){
        //Se puede agregar el rectangulo
        if(mini>pu[k].y){
          puntox=false;
          mini=pu[k].y;
          cambio=true;
          save=k;
          pun= punto(pu[k].x+ancho,pu[k].y,altura);
          if(pu[k].altura!=altura){
          pun2=punto(pu[k].x,pu[k].y+altura,pu[k].altura-altura);
          puntox=true;
          }
        }
      }
    }
    if(cambio==false){
      //Si no se puede agregar en los otros puntos, se agrega en el siguiente nivel mas a la izq.
      pu.push_back(punto(ancho,ymax,altura));
      ymax=ymax+altura;
    }
    else{
      pu[save]=pun;
      if(puntox){
      pu.push_back(pun2);
      }
    }
  }
  if(ymax<20){
  std::cout<<"ALGO\n";
    for(punto p : pu){
      std::cout<<"PUNTO:"<<p.x<<" "<<p.y<<" "<<p.altura<<"\n";}
    }
  return ymax;
}



std::vector<rectangulo> girar(std::vector<rectangulo> a,int w,std::deque<int> &tabu,unsigned int TAMA){
  std::vector<rectangulo> fe;
  std::vector<rectangulo> mejor;
  unsigned int minimo = std::numeric_limits<int>::max(),valor,k;
	for(unsigned int i=0;i<a.size();i++){
  if(std::find(tabu.begin(), tabu.end(), a[i].id) == tabu.end()){
    fe=a;
    fe[i].giro=!(fe[i].giro);   //Se calcula el giro para cada rectangulo.
    if((fe[i].ancho<=w && fe[i].giro==false) || (fe[i].alto<=w && fe[i].giro==true)){
    valor=evaluacion(fe,w);
    if (valor<minimo){          //Se escoge girar el rectangulo con mejor funcion de evaluacion.
      minimo=valor;
      mejor=std::vector<rectangulo>{fe};
      k=i;
    }
  }
    }
  }
  if(tabu.size()<TAMA){
    tabu.push_front(a[k].id);
  }
  else{
    tabu.pop_back();
    tabu.push_front(a[k].id);
  }
  return mejor;
}

bool estabu(int a, int b, std::deque<dos> &tabu){
  if(a==b){
    return true;
  }
  for(dos obj : tabu){
    if((obj.uno==a || obj.d==a) && (obj.d==b || obj.uno==b)){
      return true;
    }
  }
  return false;
}

int index(std::vector<rectangulo> l, int n){
  for(unsigned int i=0;i<l.size();i++){
    if(l[i].id==n){
      return i;
    }
  }
  return 0;
}



std::vector<rectangulo> dosOpt(std::vector<rectangulo> a,std::deque<dos> &tabu,int w,unsigned int TAMA2){

  int R1,R2,mini=std::numeric_limits<int>::max(),ev,index1,index2;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, a.size());
  std::vector<rectangulo> finale= std::vector<rectangulo>();
  std::vector<rectangulo> d= std::vector<rectangulo>();
  for(unsigned int k=0;k<TAMA2;k++){
    d=a;
    R1=dis(gen);
    R2=dis(gen);
    // std::reverse(a.begin()+index1,a.begin()+index2);
    if(estabu(R1,R2,tabu)){
    }
    else{

      index1=index(d,R1);
      index2=index(d,R2);
      if(index1<index2){

        std::reverse(d.begin()+index1,d.begin()+index2);
        ev=evaluacion(d,w);

      }
      else{

        std::reverse(std::begin(d)+index2,std::begin(d)+index1);
        ev=evaluacion(d,w);

      }
      if(tabu.size()<TAMA2){
        tabu.push_front(dos(R1,R2));
      }
      else{
        tabu.pop_back();
        tabu.push_front(dos(R1,R2));
      }
    }
    if(ev<mini){
      mini=ev;
      finale=d;
    }
  }
  return finale;
}




//Tabu search para 2DSPP
int main(int args, char **argv){
  rectangulo rec= rectangulo(1,2,0,0);
  std::vector<rectangulo> arr = std::vector<rectangulo>();
  std::vector<rectangulo> finale =std::vector<rectangulo>();
  std::vector<rectangulo> mejor =std::vector<rectangulo>();
  std::deque<int> tabulistr=std::deque<int>();
  std::deque<dos> tabulist2opt=std::deque<dos>();
  int id,anc,alt,cantidad,w,rep,valor,ache=std::numeric_limits<int>::max(),areatotal=0;
  unsigned int TAMA,TAMA2;
  std::ifstream arch (argv[1], std::ifstream::in);
  arch >> id;
  cantidad = id;
  arch >> id;
  w = id;
  while(arch >> id >> anc >> alt){
      rec=rectangulo(alt,anc,false,id);
      arr.push_back(rec);
      areatotal+=alt*anc;
  }
  rep=atoi(argv[2]);
  TAMA=cantidad/4; //Tabulist de giro corresponde a la mitad de rectangulos diponibles.
  TAMA2=cantidad; //Tabulist de 2opt corresponde a la cantidad de rectangulos disponibles pues pueden ser las combinaciones.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1.0);
  float ran;
  for(int i=0;i<rep;i++){
    ran=dis(gen);
    if(ran<0.4){
      //Se girará un rectangulo.
      mejor=girar(arr,w,tabulistr,TAMA);
      valor=evaluacion(mejor,w);

      arr=std::vector<rectangulo>{mejor};
    }
    else{
      //Se realizará 2opt.
      mejor=dosOpt(arr,tabulist2opt,w,TAMA2);
      valor=evaluacion(mejor,w);

      arr=std::vector<rectangulo>{mejor};

    }
    if(ache>valor){
      ache=valor;
      finale=mejor;
    }
  }
  std::cout<<ache << '\n';
  std::cout<< ache*w-areatotal <<"\n";
  print(finale);
}
