#!/bin/bash

# Mostramos uso y definimos parametros por defecto
if [ $# -lt 5 ]; then
	echo "Programa BASH para ejecutar pLSA"
	echo " ./run_pLSA file_pwd format file_voc ntopics dir_out [fix_iters] [lambda]"
	exit
elif [ $# -eq 5 ]; then
	FIX_ITERS=0
	LAMBDA=0
elif [ $# -eq 6 ]; then
	FIX_ITERS=$6
	LAMBDA=0
else
	FIX_ITERS=$6
	LAMBDA=$7
fi

# ///////////////////////
# Programas de ejecucion
PLSA="./pLSA" # 03_plsa_float_cvmat_omp/pLSA
# ./pLSA file_pwd ndocs nwords format ntopics lambda fix_iters dir_out
# ///////////////////////

# ///////////////////
# Fichero de entrada con la matrix pwd
PWD=$1
# Numero de documentos
ND=`wc -l $1 | cut -d " " -f 1`
# Formato (0->HPC, 1->HPB)
FORMAT=$2 
# Numero de palabras
NW=`wc -l $3 | cut -d " " -f 1`
# Numero de topicos
NT=$4
# Directorio de salida 
DIROUT=$5
# ///////////////////

# ////////////////
# Ejecutamos pLSA
$PLSA $PWD $ND $NW $FORMAT $NT $LAMBDA $FIX_ITERS $DIROUT
# ////////////////
