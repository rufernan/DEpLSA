#include "plsa.h"
#include "parameters.h"

//////////////////////////////////////////////////////////////////////////////////
// Programa Principal -                                                         //
//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
	
	// Uso del programa
	if(argc!=9){
		cout<<endl<<"usage: ./pLSA file_pwd ndocs nwords format ntopics lambda fix_iters dir_out"<<endl<<endl;
		exit(0);
	}
	
	// Parametros del programa
	char *file_pwd=argv[1];
	int ndocs=atoi(argv[2]);
	int nwords=atoi(argv[3]);
	int format=atoi(argv[4]);
	int ntopics=atoi(argv[5]);
	float lambda=atof(argv[6]); // sparse_regularisation over pzd (lambda=0 regular plsa)
	int fix_iters=atof(argv[7]); // efficiency mode fixing the number of EM iter7 (fix_iters=0 regular mode)
	char *dir_out=argv[8];

	cout<<"## PLSA Algorithm ##"<<endl;
	
	// Matriz p(w|d)
	Mat  *pwd; pwd = loadMat(file_pwd, format, ndocs, nwords);
	
	// Ejecutamos pLSA
	plsaEM(pwd, ntopics, lambda, fix_iters);
	
	// Guardamos los parametros (pwz, pzd) en formato HPC y HPB en el directorio de salida
	saveParameters(dir_out);
	
	cout<<"## FIN ##"<<endl;
	
}