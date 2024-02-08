#include "data.h"

//////////////////////////////////////////////////////////////////////////////////
// Módulo Data - Funciones para la carga/almacenamiento de matrices en ficheros //
//////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTACION ////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////
// Función para cargar una matriz desde un fichero de texto //
// IN:
//  fileIn -> nombre (+ruta) del fichero de texto con la matriz a cargar
//  format -> formato de la matriz (0->HPC, 1->HPB)
//  nrows -> numero de filas de la matriz
//  ncols -> numero de columnas de la matriz
// OUT:
//  matOut -> matriz de de OpenCV (Mat) con los datos cargados

Mat* loadMat(char* fileIn, int format, int nrows, int ncols){
	
	// Matriz de salida en formato OpenCV 
	Mat *matOut = new Mat(nrows, ncols, CV_32FC1, Scalar::all(0));
	
	// Abrimos el fichero de entrada
	ifstream fin(fileIn);
	
	// Formato HPC - histograma clasico
	if(format==0){
		double data_double;
		int r, c;
		for(r=0;r<nrows;r++){
			for(c=0;c<ncols;c++){	
				fin>>data_double;					
				matOut->at<float>(r,c) = data_double;
			}
		}
	}

	// Formato HPB - histograma disperso (format==1)
	else{
		double count_double;
		int r, c, lenght, word;
		char ch;
		for(r=0;r<nrows;r++){
			fin>>lenght;
			for(c=0;c<lenght;c++){
				fin>>word;
				fin>>ch;
				word = word - OFFSET; // OFFSET=0 (Si las palabras empiezan en 1 OFFSET=1)
				fin>>count_double;
				matOut->at<float>(r,word) = count_double;
			}
		}
	}
	
	// Cerramos el fichero de entrada
	fin.close();
	
	// Devolvemos la matriz de salida
	return matOut;
}



/////////////////////////////////////////////////////////////////
// Función para imprimir por pantalla una matriz Mat de OpenCV //
// IN:
//  mat -> matriz de OpenCV (Mat)
// OUT:
//  void

void printMat(Mat *mat){
	for(int r=0;r<mat->rows;r++){
		for(int c=0;c<mat->cols;c++){
			cout<< mat->at<float>(r,c) <<" ";
		}
		cout<<endl;
	}
}



/////////////////////////////////////////////////////////////////
// Función para guardar una matriz CvMat a un fichero de texto //
// IN:
//  matIn -> matriz OpenCV (Mat) a guardar en el fichero de salida
//  fileOut -> nombre (+ruta) del fichero de texto en el que guardaremos la matriz
//  format -> formato de la matriz (0->HPC, 1->HPB)
//  app -> indica si el formato para abrir el fichero (0->trunc, 1->append) <- POR DEFECTO 0
// OUT:
//  void

void saveMat(Mat* matIn, char* fileOut, int format, int app){
	
	// Abrimos el fichero de salida
	ofstream fout;
	if(app==0) fout.open(fileOut, ios::trunc); // modo trunc (si existe borra el contenido)
	else fout.open(fileOut, ios::app); // modo anyadir
	
	// Formato HPC - histograma clasico
	if(format==0){
		
		// Iteramos para cada fila de la matriz Mat
		for(int i=0;i<matIn->rows;i++){
			for(int j=0;j<matIn->cols;j++){
				if(j!=0) fout<<" ";
				float data = matIn->at<float>(i,j);
				fout<<data;
			}
			fout<<endl;
		}
	}	
	
	// Formato HPB - histograma disperso (format==1)
	else{
		
		for(int i=0;i<matIn->rows;i++){		
			
			// Contamos el número de palabras no nulas de la fila
			int nonulls=0;
			for(int j=0;j<matIn->cols;j++){
				float data = matIn->at<float>(i,j);
				if (data!=0) nonulls++;
			}
			
			if (nonulls>0){
			
				// Escribimos el numero de palabras no nulas de la fila
				fout<<nonulls;
				
				// Escribimos los elementos no nullos en formato disperso "word:count"
				for(int j=0;j<matIn->cols;j++){
					float data = matIn->at<float>(i,j);
					int word = j + OFFSET; // OFFSET para las palabras
					if (data!=0){ 
						float data = matIn->at<float>(i,j);
						fout<<" "<<word<<":"<<data; 
					}
				}
				fout<<endl;
			}	
		}
	}
	// Cerramos el fichero de salida
	fout.close();
}



//////////////////////////////////////////////////////////////
// Función para crear una matriz inicializada a 0 o random  //
// IN:
//  nrows -> numero de filas de la matriz
//  ncols -> numero de columnas de la matriz
//  rnd  -> 0 para inicializar la matriz a 0 (POR DEFECTO)
//           1 para inicializar la matriz random normalizada por filas
// OUT:
//  matOut -> matriz de de OpenCV (Mat)

Mat* createMat(int nrows, int ncols, int rnd){
	
	// Matriz de salida en formato OpenCV 
	Mat *matOut = new Mat(nrows, ncols, CV_32FC1, Scalar::all(0));
	
	if(rnd==1){ // Inicializacion aleatoria normalizada por filas
		srand(time(NULL));
		// Rellenamos la matriz con numeros aleatorios
		for(int i=0; i<nrows; i++){
			double vr, sum=0.0;
			for(int j=0; j<ncols; j++){
				vr = rand()%10001; // random [0,10000]
				matOut->at<float>(i,j) = vr;
				sum += vr;
			}
			// Normalizamos la fila i para que sume 1
			for(int j=0; j<ncols; j++)
				matOut->at<float>(i,j) /= sum; 
		}
	}
	
	// Devolvemos la matriz de salida
	return matOut;
}



//////////////////////////////////////////////////////////////////
// Función para crear una matriz N-dimensional inicializada a 0 //
// IN:
//  vec_dims -> vector con un elemento por dimension con el tamanyo de cada dimension
//  N -> numero de dimensiones de la matriz
// OUT:
//  matOut -> matriz de de OpenCV (Mat)

Mat* createMatND(int *vec_dims, int N){
	// Matriz de salida en formato OpenCV 
	Mat *matOut = new Mat(N, vec_dims, CV_32FC1, Scalar::all(0));
	// Devolvemos la matriz de salida
	return matOut;
}



//////////////////////////////////////////////////////////////////////////////
// Función para multiplicar una matriz por un escalar y convertir a entero  //
// IN:
//  mat -> matriz de OpenCV (Mat)
//  scl -> valor que se multiplicara por cada elemento de la matriz para quedarnos con la parte entera
// OUT:
//  void

void scale(Mat *mat, int scl){
	
	int nrows=mat->rows, ncols=mat->cols;
	
	// Multiplicamos cada elemento de la matriz por el escalar	
	for(int i=0; i<nrows; i++)
		for(int j=0; j<ncols; j++)
			mat->at<float>(i,j) = (int) (mat->at<float>(i,j) * scl); // convertimos a entero
}
