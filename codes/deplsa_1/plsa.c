#include "plsa.h"

////////////////////////////////////////////////////////////////////////////////////
// Module pLSA - Functions to calculate the parameters of the pLSA model          //
// d -> z -> w estimating p(w/z) and p(z/w,d) by EM of the corpus likelihood.     //
////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION //////////////////////////////////////////////////////////////////


/////////////////////////////////////
// VARIABLES GLOBALES DEL PROGRAMA //

// Matrices p(w|z), p(z|d), p(z|w,d) y p(d)
Mat *pwz, *pzd, *pzwd, *pd;

// ATENCION: FILAS/COLUMNAS SEGUN ESTA REPRESENTACION //
// pwz -> z filas y w columnas
// pzd -> d filas y z columnas
// pzwd -> w dimension 1, d dimension 2 y z dimension 3
// pd -> d dimension 1

// Condiciones de parada algoritmo EM
#define MAXITERS 1000
#define THRESHOLD 1e-6
#define SCALAR 1e6 // escalar para convertir HPC->HPB

// Mostrar traza (0->NO, 1->SI)
//#define VERBOSE 0
#define VERBOSE 1

/////////////////////////////////////
/////////////////////////////////////


// Crea e inicializa las matrices globales p(w|z), p(z|d), p(z|w,d) y p(d) //
// p(w|z) -> aleatoria normalizada por filas (z)
// p(z|d) -> aleatoria normalizada por filas (d)
// p(z|w,d) -> inicializada a 0
// p(d) -> contiene las priors de los documentos de la coleccion

void iniStructures(Mat *pwd, int Z){
	
	// Numero de documentos y palabras
	int D=pwd->rows, W=pwd->cols;

	// Iicializacion aleatoria de los parametros p(w|z) y p(z|d)
	pwz = createMat(Z, W, 1);
	pzd = createMat(D, Z, 1);
	
	// Inicializacion del resto de matrices
	int dims_pzwd[]={W,D,Z}, N_pzwd=3;
	pzwd = createMatND(dims_pzwd, N_pzwd);
	int dims_pd[]={D}, N_pd=1;
	pd = createMatND(dims_pd, N_pd);
	// Calculamos las priors de los documentos p(d)
	estimatePd(pwd);
}



// Estima p(d) la probabilidad a priori de los documentos de la coleccion //
// p(d) = (SUMw n(w,d)) / (SUMd SUMw n(w,d)) <-- los documentos con mas palabras son mas probables

void estimatePd(Mat *pwd){
	
	// Numero de documentos y palabras
	int D=pwd->rows, W=pwd->cols;

	double norm=0.0;
  	for(int d=0;d<D;d++){
		double wd =0.0; // numero de palabras del documento d
		for(int w=0;w<W;w++)
			wd += pwd->at<float>(d,w);
		pd->at<float>(d) = wd;
		norm += wd;
	}
	for(int d=0;d<D;d++) // normalizacion respecto al numero total de palabras en la coleccion
		pd->at<float>(d) /= norm;
}



// Estima el valor de la matriz p(z|w,d) a partir de p(w|z) y p(z|d) //
// p(z|w,d) = (p(w|z)*p(z|d)) / (SUMz(p(w|z)*p(z|d))) 

void Estep(){
  
	// Numero de palabras, documentos y topicos
	int W=pzwd->size[0], D=pzwd->size[1], Z=pzwd->size[2];
  
	// Actualizamos la matriz p(z|w,d)
	#pragma omp parallel for
	for(int w=0;w<W;w++){
		for(int d=0;d<D;d++){
			double norm=0.0;
			for(int z=0;z<Z;z++){
				pzwd->at<float>(w,d,z) = pwz->at<float>(z,w) * pzd->at<float>(d,z);
				norm += pzwd->at<float>(w,d,z);
			}
			if(norm!=0.0)
				for(int z=0;z<Z;z++)
					pzwd->at<float>(w,d,z) /= norm;
		}
	}
}



// Estima los valores de las matrices p(w|z) y p(z|d) a partir de p(z|w,d) y p(w|d) //
// p(w|z) = (SUMd n(w,d)*p(z|w,d)) / (SUMw SUMd n(w,d)*p(z|w,d))
// p(z|d) = (SUMw n(w,d)*p(z|w,d)) / (SUMz SUMw n(w,d)*p(z|w,d))

void Mstep(Mat *pwd, float lambda){
	
	// Numero de palabras, documentos y topicos
	int W=pzwd->size[0], D=pzwd->size[1], Z=pzwd->size[2];
  
	// Actualizamos la matriz p(w|z)
	#pragma omp parallel for
  	for(int z=0;z<Z;z++){
		double norm=0.0;
		for(int w=0;w<W;w++){
			pwz->at<float>(z,w) = 0.0;
			for(int d=0;d<D;d++)
				pwz->at<float>(z,w) += pwd->at<float>(d,w) * pzwd->at<float>(w,d,z);
			norm += pwz->at<float>(z,w);
		}
		if(norm!=0.0) 
			for(int w=0;w<W;w++)
				pwz->at<float>(z,w) /= norm;
	}
	
	// Actualizamos la matriz p(z|d)
	#pragma omp parallel for
  	for(int d=0;d<D;d++){
		double norm=0.0;
		for(int z=0;z<Z;z++){
			pzd->at<float>(d,z) = 0.0;
			for(int w=0;w<W;w++)
				pzd->at<float>(d,z) += pwd->at<float>(d,w) * pzwd->at<float>(w,d,z);
			if(lambda>0){ // sparse regularisation //
				pzd->at<float>(d,z) -= lambda/Z;
				if(pzd->at<float>(d,z)<0) pzd->at<float>(d,z);
			} ///////////////////////////////////////
			norm += pzd->at<float>(d,z);
		}
		if(norm!=0.0)
			for(int z=0;z<Z;z++)
				pzd->at<float>(d,z) /= norm;
	}
}
 


// Devuelve el valor de la log-likelihood (L) a partir de p(w|d), p(w|z), p(z|d), p(z|w,d) y p(d) //
// L = SUMd SUMw n(w,d) log [ p(d) SUMz p(w|z)p(z|d) ]
 
double logL(Mat *pwd){

	// Numero de palabras, documentos y topicos
	int W=pzwd->size[0], D=pzwd->size[1], Z=pzwd->size[2];

	// LogLikelihood: L
	double L=0.0;
	
	#pragma omp parallel for reduction(+:L)
	for(int d=0;d<D;d++){
		for(int w=0;w<W;w++){
			double sumz=0.0;
			for(int z=0;z<Z;z++)
				sumz += pwz->at<float>(z,w) * pzd->at<float>(d,z);
			sumz = pd->at<float>(d) * sumz;
			if(sumz>0.0) // para evitar calcular el log de un valor negativo
				L += pwd->at<float>(d,w) * log(sumz);
		}
	}

	return L;
}



// Ejecuta el algoritmo EM para estimar los parametros p(w|z) y p(z|d) //
 
void plsaEM(Mat *pwd, int Z, float lambda, int fix_iters){
	
	double anteriorL, actualL=0.0;
	int NUM_ITERS;
	
	// Inicializamols las estructuras
	if(VERBOSE) cout<<"Inicializando pwz, pzd, pzwd y pd..."<<endl;
	iniStructures(pwd, Z);
	
	if(fix_iters==0) // regular mode (1000 EM iters & 1e-6 threshold)
		NUM_ITERS=MAXITERS;
	else // efficiency mode (fix EM iters)
		NUM_ITERS=fix_iters;
	
	// Iteramos algoritmo EM
	if(VERBOSE) cout<<"Algoritmo EM (MAXITERS="<<NUM_ITERS<<",THRESHOLD="<<THRESHOLD<<")"<<endl;
	for(int it=0; it<NUM_ITERS; it++){
		if(VERBOSE) cout<<"ITER "<<it<<"/"<<NUM_ITERS<<endl;
		if(VERBOSE) cout<<"  Estep..."<<endl;
		Estep();
		if(VERBOSE) cout<<"  Mstep..."<<endl;
		Mstep(pwd,lambda);
		if(fix_iters==0){ // regular mode (1000 EM iters & 1e-6 threshold)
			// Evaluamos la evolucion de la log-likelihood
			anteriorL = actualL;
			actualL = logL(pwd);
			double diff=actualL-anteriorL;
			if(VERBOSE){
				if(it==0) cout<<"  LogL="<<actualL<<endl;
				else cout<<"  LogL="<<actualL<<" (+"<<diff<<")"<<endl;
			}
			if(abs(diff)<THRESHOLD) return;
		}
	}
}



// Guardamos las matrices p(w|z) y p(z|d) en formato HPC y HPB //

void saveParameters(char *outName){

	char fout[MAXTAM];
	int format;
	
	// Guardamos p(w|z) y p(z|d) en formato HPC (clasico)
	format=0;
	sprintf(fout,"%s.pwz_hpc.txt",outName);
	saveMat(pwz, fout, format);
	sprintf(fout,"%s.pzd_hpc.txt",outName);
	saveMat(pzd, fout, format);	
	
	// Para poder guardar las matrices en formato HPB tenemos que "reescalar" las probabilidades a valores enteros
	scale(pwz,SCALAR);
	scale(pzd,SCALAR);
	
	// Guardamos p(w|z) y p(z|d) en formato HPB (disperso)
	format=1;
	sprintf(fout,"%s.pwz_hpb.txt",outName);
	saveMat(pwz, fout, format);
	sprintf(fout,"%s.pzd_hpb.txt",outName);
	saveMat(pzd, fout, format);
}



// Liberar las matrices globles p(w|z), p(z|d), p(z|w,d) y p(d)

void freeStructures(void){
	freeMat(pwz);
	freeMat(pzd);
	freeMat(pzwd);
	freeMat(pd);
}

















