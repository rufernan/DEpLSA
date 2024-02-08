#ifndef PLSA_H
#define PLSA_H

#include "data.h"
#include "parameters.h"

////////////////////////////////////////////////////////////////////////////////////
// Module pLSA - Functions to calculate the parameters of the pLSA model          //
// d -> z -> w estimating p(w/z) and p(z/w,d) by EM of the corpus likelihood.     //
////////////////////////////////////////////////////////////////////////////////////
// DECLARATIONS ////////////////////////////////////////////////////////////////////



// Crea e inicializa las matrices globales p(w|z), p(z|d), p(z|w,d) y p(d) //
void iniStructures(Mat *pwd, int Z);

// Estima p(d) la probabilidad a priori de los documentos de la coleccion //
void estimatePd(Mat *pwd);

// Estima el valor de la matriz p(z|w,d) a partir de p(w|z) y p(z|d) //
void Estep();
    
// Estima los valores de las matrices p(w|z) y p(z|d) a partir de p(z|w,d) y p(w|d) //
void Mstep(Mat *pwd, float lambda);

// Devuelve el valor de la log-likelihood (L) a partir de p(w|d), p(w|z), p(z|d), p(z|w,d) y p(d) //
double logL(Mat *pwd);

// Ejecuta el algoritmo EM para estimar los parametros p(w|z) y p(z|d) //
void plsaEM(Mat *pwd, int Z, float lambda, int fix_iters);

// Guardamos las matrices p(w|z) y p(z|d) en formato HPC y HPB //
void saveParameters(char *outName);

// Liberar las matrices globles p(w|z), p(z|d), p(z|w,d) y p(d)
void freeStructures(void);
  

#endif
