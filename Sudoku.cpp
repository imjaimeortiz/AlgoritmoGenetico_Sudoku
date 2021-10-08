/* ------------------------- PROBLEMA DEL SUDOKU ----------------------- */
#include <ga/GASimpleGA.h> //  Algoritmo Genetico simple
#include <ga/GA1DArrayGenome.h> // Genoma
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

struct plantilla{
       int tam; // Tamaño del sudoku
       int *fijo;
};

void leerSudoku(struct plantilla *S,char *nombreF){
   ifstream f(nombreF);

   f>>S->tam;

   S->fijo = new int[S->tam*S->tam];

   for(int i=0;i<S->tam*S->tam;i++)
           f>>S->fijo[i];

   f.close();
}

void InicioSudoku(GAGenome& g){

    // 2 )
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;

     struct plantilla * plantilla1;
     plantilla1 = (struct plantilla *) genome.userData();

     int aux[plantilla1->tam];

     for(int f=0;f<plantilla1->tam;f++){

        // Inicializamos a 0 el array de tamaño tam
          for(int j=0;j<plantilla1->tam;j++) aux[j]=0;

          for(int j=1;j<=plantilla1->tam;j++){
            // Cogemos un número aleatorio entre 0-(tam-1)
            int v=GARandomInt(0,plantilla1->tam-1);
            while (aux[v]!=0) v=(v+1)%plantilla1->tam;
          // Ponemos donde haya un 0 en el array el iterador por el que vayamos
            aux[v]=j;
          }

          int i=0;

          while(i<plantilla1->tam){

              while((plantilla1->fijo[(f*plantilla1->tam)+i]==0) && (i<plantilla1->tam)) i++;

              if (i<plantilla1->tam){

                     bool encontrado=false;
                     for(int j=0;(j<plantilla1->tam) && (!encontrado);j++)
                             if (aux[j]==plantilla1->fijo[(f*plantilla1->tam)+i]) {
                                encontrado=true;
                                aux[j]=aux[i];
                             }

                     aux[i]=plantilla1->fijo[(f*plantilla1->tam)+i];
              }
              i++;

          }

          for(int c=0;c<plantilla1->tam;c++)
                  genome.gene((f*plantilla1->tam)+c,aux[c]);
     }
}

int CruceSudoku(const GAGenome& p1,const GAGenome & p2,GAGenome* c1,GAGenome* c2){

     // 6 )
     GA1DArrayAlleleGenome<int> & m = (GA1DArrayAlleleGenome<int> &)p1;
     GA1DArrayAlleleGenome<int> & p = (GA1DArrayAlleleGenome<int> &)p2;

    struct plantilla * plantilla1 = (struct plantilla *) m.userData();
    int n=0;

    int punto1=GARandomInt(0,m.length());
    while ((punto1%plantilla1->tam)!=0) punto1++;
    int punto2=m.length()-punto1;

    if (c1){
            GA1DArrayGenome<int> & h1 = (GA1DArrayGenome<int> &)* c1;
            h1.copy(m,0,0,punto1); // el metodo copy esta definido en la clase GA1DArrayGenome
            h1.copy(p,punto1,punto1,punto2);
            n++;
    }

    if (c2){
            GA1DArrayGenome<int> & h2 = (GA1DArrayGenome<int> &)* c2;
            h2.copy(p,0,0,punto1);
            h2.copy(m,punto1,punto1,punto2);
            n++;
    }

    return n;

}

bool checkColumna(int col[], int * check, int tam){
     bool repe=false;

     for(int i=0;i<tam;i++) check[i]=0;

     for(int i=0;i<tam;i++)
             check[col[i]-1]++;
     for(int i=0;i<tam;i++) if (check[i]>1) repe=true;

     return repe;
}


int MutacionSudoku(GAGenome& g,float pmut){

    // 8 )
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;


    struct plantilla * plantilla1;
    plantilla1 = (struct plantilla *) genome.userData();
    int nmut=0;
    int aux;
    int fil;
    bool fila;

    int caux[plantilla1->tam];
    int *checkC=new int[plantilla1->tam];

    if (pmut<=0.0) return 0;

    for(int f=0; f<plantilla1->tam; f++)
       for(int c=0; c<plantilla1->tam; c++)
          if (plantilla1->fijo[(f*plantilla1->tam)+c]==0){
           // Solo entra cuando GAFlipCoin(pmut) devuelve 1, es decir, pmut % de las veces
           if (GAFlipCoin(pmut) ){
                if (GAFlipCoin(0.5)) fila = true;
                else fila = false;

                if (!fila){
                        // Guardamos en caux la columna c
                      for(int j=0;j<plantilla1->tam;j++) caux[j]=genome.gene((j*plantilla1->tam)+c);
                      // Si hay repetidos entra
                      if (checkColumna(caux,checkC,plantilla1->tam)){
                            // Genera número random entre 0-8
                         int v1 = GARandomInt(0,plantilla1->tam-1);
                         // Buscamos en el array hasta que encontremos un repetido
                         while (checkC[v1]<=1) v1=(v1+1)%plantilla1->tam;
                         // v1 es el número (1-9) que está repetido
                         v1++;
                         int v2 = GARandomInt(0,plantilla1->tam-1);
                         // Buscamos un número que no exista para meterlo
                         while (checkC[v2]!=0) v2=(v2+1)%plantilla1->tam;
                         // Obtenemos el número
                         v2++;

                         bool encontrado = false;
                         for(int j=0;j<plantilla1->tam && !encontrado;j++)
                                // Si lo que hay en la posicion j de la columna c es 0 (original, si fuera un numero seria fijo y no podemos cambiarlo) y un número en el genome
                                 if ((plantilla1->fijo[j*(plantilla1->tam)+c]==0)&&(genome.gene(j*(plantilla1->tam)+c)==v1)){
                                    encontrado = true;
                                    // Ponemos en la posicion el valor v2
                                    genome.gene((j*plantilla1->tam)+c,v2);
                                    fil = j;
                                 }

                         int col=(c+1)%plantilla1->tam;
                         // Recorremos las columnas de fil buscando si en alguna está v2
                         while(genome.gene((fil*plantilla1->tam)+col)!=v2) col=(col+1)%plantilla1->tam;
                         // fila con desplazamiento col
                         if (plantilla1->fijo[(fil*plantilla1->tam)+col]==0) {
                                // si está vacía aumentamos las mutaciones y ponemos v1 en esa posicion
                                nmut++;
                                genome.gene((fil*plantilla1->tam)+col,v1);
                         }
                         else {
                                // Si no, deshacemos el cambio
                              genome.gene((fil*plantilla1->tam)+c,v1);
                         }

                      }

                }
                // Si la fila es true
                else{
                    // Guarda en v1 la columna siguiente a la que estamos examinando
                   int v1 = (c + 1) %plantilla1->tam;
                   //  Buscamos por filas en la columna v1 mientras sea distinto a 0
                   while ((plantilla1->fijo[(f*plantilla1->tam)+v1]!=0)) v1=(v1+1)%plantilla1->tam;
                   // Te guardas en aux donde estás
                   aux = genome.gene((f*plantilla1->tam)+c);
                   // El numero por el que nos hemos quedado antes lo guardamos en la posicion actual
                   genome.gene((f*plantilla1->tam)+c,genome.gene((f*plantilla1->tam)+v1));
                   // Terminamos el intercambio
                   genome.gene((f*plantilla1->tam)+v1,aux);
                   nmut++;
                }
           }
          }

    return nmut;
}


// Con las funciones inicio, cruce y mmutación nos estamos asegurando que las filas se insertan correctamente adecuandose al sudoku que queremos resolver,
// esto quiere decir que sólo tenemos que examinar las columnas y el cuadrado de cada casilla
float Objective(GAGenome& g) {
    GA1DArrayAlleleGenome<int> & genome = (GA1DArrayAlleleGenome<int> &)g;
    struct plantilla * p;
    p = (struct plantilla *) genome.userData();
    float repetidos=0;
    int cuad[p->tam];

    // COLUMNAS
    // La lógica de esta función es ir recorriendo todas las filas de cada columna buscando repetidos hasta el final:
    // Desde la columna 0 hasta la 8 o desde la 0 hasta la 3 vamos a ir comparando el elemento de esa posición con todos los de su columna, que estarán en las pociciones +=p->tam
    // Si al ir iterando se repiten los valores, estarán repetidos
    for (int c = 0; c < p->tam; c++)
    {
        for (int j = c; j < p->tam*p->tam-p->tam ; j+=p->tam)
        {
            for (int k = j+p->tam; k < p->tam*p->tam; k+=p->tam)
            {
                 if (genome.gene(j)==genome.gene(k))
                 {
                     repetidos++;
                     break;
                                     }
            }
        }
    }

    // CUADRÍCULAS
    for (int iF = 0; iF < p->tam; iF += sqrt(p->tam))   // Para cada fila (vamos a tener p->tam filas y como comprobamos cuadriculas, vamos saltando de +=sqrt)
    {
        for (int iC = 0; iC < p->tam; iC += sqrt(p->tam))   // Para cada columna (vamos a tener p->tam columnas y como comprobamos cuadriculas, vamos saltando de +=sqrt)
        {
            for (int x = 0; x < p->tam; x++) cuad[x] = 0;   // Inicializamos una matriz de tamaño p->tam que debe tener los p->tam números sin repetir
            for (int i = iF; i < iF+sqrt(p->tam); i++)      // Iteración dentro de cada cuadrícula del sudoku. Antes al hacer +=sqrt nos situábamos en el primer valor de cada cuadricula
            {
                for (int j = iC; j < iC+sqrt(p->tam); j++)  // Iteración para cada columna dentro de la cuadricula
                {
                    int x = genome.gene(i*p->tam+j)-1;      // genome.gene(i*p->tam+j)-1 nos devolverá el valor dentro de la posición i,j del sudoku
                    if (cuad[x] == 1) repetidos++;          // cada vez que encontramos un valor para una cuadrícula, ponemos a 1 su posición en el array mencionado anteriormente, si al obtener el valor ya hay un 1 quiere decir que está repetido
                    else cuad[x] = 1;
                }
            }
        }
    }
    return repetidos;
}

// Funcion de terminacion : cuando encontramos un valor fitness de 0 o cuando no hay más generaciones
GABoolean Termina(GAGeneticAlgorithm & ga){
    if ( (ga.statistics().minEver()==0) ||
        (ga.statistics().generation()==ga.nGenerations()) ) return gaTrue;
    else return gaFalse;
}

int main (int argc, char **argv) {

    // Declaramos variables para los parametros del GA y las inicializamos
    char * f = argv[1];
    int popsize = atoi(argv[2]);
    int ngen = 12000;
    float pcross = atof(argv[3]);
    float pmut = atof(argv[4]);
    int selec = atoi(argv[5]);

    cout << "Parametros:    - Tamano poblacion : " << popsize << endl;
    cout << "               - Numero de generaciones : " << ngen << endl;
    cout << "               - Probabilidad cruce : " << pcross << endl;
    cout << "               - Probabilidad mutacion: " << pmut << endl;
    string selector;
    if (selec == 0)
        selector = "Roulette";
    else
        selector = "Tournament";
    cout << "               - Selector : " << selector << endl << endl;

    struct plantilla p;
    leerSudoku(&p, f);
    int nSudoku = p.tam;

// Conjunto enumerado de alelos --> valores posibles de cada gen del genoma

    GAAlleleSet<int> alelos;
    for(int i=0;i<nSudoku;i++) alelos.add(i+1);

// Creamos el genoma y definimos operadores de inicio, cruce y mutación

    GA1DArrayAlleleGenome<int> genome(nSudoku*nSudoku,alelos,Objective,&p); // p.tam el tamaño y &p el sudoku sobre el que vamos a trabajar
    genome.initializer(InicioSudoku);
    genome.crossover(CruceSudoku);
    genome.mutator(MutacionSudoku);

// Creamos el algoritmo genetico

    GASimpleGA ga(genome);

// Inicializamos - minimizar funcion objetivo, tamaño poblacion, nº generaciones,
// pr. cruce y pr. mutacion, selección y le indicamos que evolucione.

    ga.minimaxi(-1);
    ga.populationSize(popsize);
    ga.nGenerations(ngen);
    ga.pCrossover(pcross);
    ga.pMutation(pmut);
    GARouletteWheelSelector selectorRoulette;
    GATournamentSelector selectorTournament;
    if (selec == 0) ga.selector(selectorRoulette);
    else ga.selector(selectorTournament);
    ga.terminator(Termina);
    ga.evolve(1);

// Imprimimos el mejor individuo que encuentra el GA y su valor fitness

    GA1DArrayAlleleGenome<int> & ge = (GA1DArrayAlleleGenome<int> &) ga.statistics().bestIndividual();
    int i = 0;
    cout << "Mejor valor fitness : " << ga.statistics().minEver() << endl;
    while (i < (nSudoku*nSudoku)) {
        cout << ge.gene(i) << " ";
        i++;
        if (i % nSudoku == 0) cout << endl;
    }
    cout << endl;

}

