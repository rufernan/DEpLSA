#include "plsa.h"
#include "parameters.h"

//////////////////////////////////////////////////////////////////////////////////
// Programa Principal -                                                         //
//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
	
	// Uso del programa
	if(argc!=12){
		cout<<endl<<"usage: ./pLSA file1_pwd ndocs nwords format1 file2_pwz ntopics format2 lambda fix_iters dir_out num_parts_file1"<<endl<<endl;
		exit(0);
	}
	
	// Parametros del programa
	char *file1_pwd=argv[1];
	int ndocs=atoi(argv[2]);
	int nwords=atoi(argv[3]);
	int format1=atoi(argv[4]);
	char *file2_pwz=argv[5];
	int ntopics=atoi(argv[6]);
	int format2=atoi(argv[7]);
	float lambda=atof(argv[8]); // sparse_regularisation over pzd (lambda=0 regular plsa)
	int fix_iters=atof(argv[9]); // efficiency mode fixing the number of EM iter7 (fix_iters=0 regular mode)
	char *dir_out=argv[10];
	int num_parts_file1=atoi(argv[11]); // number of partitions to process file1 (num_parts_file1=1 process the whole file at once)

	cout<<"## PLSA_PZD Algorithm (fixing PWZ) ##"<<endl;

	
	// Matriz p(w|d) y p(w|z)
	Mat *pwd, *pwz;
	pwd = loadMat(file1_pwd, format1, ndocs, nwords);
	pwz = loadMat(file2_pwz, format2, ntopics, nwords);

	int append=(num_parts_file1>1);

	if(append){ // Ejecutamos pLSA por partes
		
		int nrows_part = ndocs/num_parts_file1; // numero filas por matriz auxiliar
		int first_row, last_row;
		
		for(int p=0;p<num_parts_file1;p++){
			
			cout<<"# Partition pwd "<<p+1<<"/"<<num_parts_file1<<" #"<<endl;
			
			first_row = nrows_part*p;
			last_row = nrows_part*p + nrows_part - 1;
			if(p==(num_parts_file1-1)) last_row = ndocs - 1; // para absorver las muestras de pico

			Mat *pwd_part = subMatRows(pwd, first_row, last_row);
			
			
			// cout<<first_row<<endl;
			// cout<<pwd->at<float>(first_row,0)<<","<<pwd->at<float>(first_row,1)<<"..."<<pwd->at<float>(first_row,nwords-2)<<","<<pwd->at<float>(first_row,nwords-1)<<endl;
			// cout<<pwd_part->at<float>(0,0)<<","<<pwd_part->at<float>(0,1)<<"..."<<pwd_part->at<float>(0,nwords-2)<<","<<pwd_part->at<float>(0,nwords-1)<<endl;
			
			plsaEM(pwd_part, pwz, lambda, fix_iters);
			
			if(p==0) saveParameters(dir_out); // truncate file, just in case it is already created
			else saveParameters(dir_out,append); // append mode
			
			freeMat(pwd_part);
		}
	}
	else{ // Ejecutamos pLSA sobre todo el fichero
		plsaEM(pwd, pwz, lambda, fix_iters);
		saveParameters(dir_out);
	}
	
	freeMat(pwd);
	freeMat(pwz);
	
	cout<<"## FIN ##"<<endl;

}