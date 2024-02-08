#ifndef DATA_H
#define DATA_H

#include "parameters.h"

//////////////////////////////////////////////////////////////////////////////////
// Módulo Data - Funciones para la carga/almacenamiento de matrices en ficheros //
//////////////////////////////////////////////////////////////////////////////////
// DEFINICION DE CABECERAS ///////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// Función para cargar una matriz desde un fichero de texto //
// IN:
//  fileIn -> nombre (+ruta) del fichero de texto con la matriz a cargar
//  format -> formato de la matriz (0->HPC, 1->HPB)
//  nrows -> numero de filas de la matriz
//  ncols -> numero de columnas de la matriz
// OUT:
//  matOut -> matriz de de OpenCV (Mat) con los datos cargados
Mat* loadMat(char* fileIn, int format, int nrows, int ncols);


/////////////////////////////////////////////////////////////////
// Función para imprimir por pantalla una matriz Mat de OpenCV //
// IN:
//  mat -> matriz de OpenCV (Mat)
// OUT:
//  void
void printMat(Mat *mat);


/////////////////////////////////////////////////////////////////
// Función para guardar una matriz CvMat a un fichero de texto //
// IN:
//  matIn -> matriz OpenCV (Mat) a guardar en el fichero de salida
//  fileOut -> nombre (+ruta) del fichero de texto en el que guardaremos la matriz
//  format -> formato de la matriz (0->HPC, 1->HPB)
//  app -> indica si el formato para abrir el fichero (0->trunc, 1->append)
// OUT:
//  void
void saveMat(Mat* matIn, char* fileOut, int format, int app=0);


//////////////////////////////////////////////////////////////
// Función para crear una matriz inicializada a 0 o random  //
// IN:
//  nrows -> numero de filas de la matriz
//  ncols -> numero de columnas de la matriz
//  rnd  -> 0 para inicializar la matriz a 0 
//           1 para inicializar la matriz random normalizada por filas
// OUT:
//  matOut -> matriz de de OpenCV (Mat)
Mat* createMat(int nrows, int ncols, int rnd=0);


//////////////////////////////////////////////////////////////////
// Función para crear una matriz N-dimensional inicializada a 0 //
// IN:
//  vec_dims -> vector con un elemento por dimension con el tamanyo de cada dimension
//  N -> numero de dimensiones de la matriz
// OUT:
//  matOut -> matriz de de OpenCV (Mat)
Mat* createMatND(int *vec_dims, int N);


//////////////////////////////////////////////////////////////////////////////
// Función para multiplicar una matriz por un escalar y convertir a entero  //
// IN:
//  mat -> matriz de OpenCV (Mat)
//  scl -> valor que se multiplicara por cada elemento de la matriz para quedarnos con la parte entera
// OUT:
//  void
void scale(Mat *mat, int scl);


//////////////////////////////////////////////////////////////////////////////
// Función para multiplicar una matriz por un escalar y convertir a entero  //
// IN:
//  mat -> matriz de OpenCV (Mat) a liberar
// OUT:
//  void
inline void freeMat(Mat *mat){ mat->release(); }  // en realidad esta funcion no hace falta ya que se invocara automaticamente al destructor de Mat 


//////////////////////////////////////////////////////////////
// Función para crear una submatriz a partir de otra dada   //
// IN:
//  matIn -> nombre (+ruta) del fichero de texto con la matriz a cargar
//  row_ini -> fila de inicio para la copia
//  row_fin -> fila de fin para la copia
// OUT:
//  matOut -> matriz de de OpenCV (Mat) con los datos capiados
//
// NOTA: el rango a copiar es [row_ini,row_fin)
Mat* subMatRows(Mat* matIn, int row_ini, int row_fin);

#endif