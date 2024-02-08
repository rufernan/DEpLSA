#!/bin/bash

# Mostramos uso y definimos parametros por defecto
if [ $# -lt 5 ]; then
	echo "Programa BASH para ejecutar pLSA"
	echo " ./run_pLSA file1_pwd format1 file_voc file2_pwz_hpc dir_out [fix_iters] [lambda] [num_part_file1]"
	exit
elif [ $# -eq 5 ]; then
	FIX_ITERS=0
	LAMBDA=0
	NUM_PART=1
elif [ $# -eq 6 ]; then
	FIX_ITERS=$6
	LAMBDA=0
	NUM_PART=1
elif [ $# -eq 7 ]; then
	FIX_ITERS=$6
	LAMBDA=$7
	NUM_PART=1
else
	FIX_ITERS=$6
	LAMBDA=$7
	NUM_PART=$8
fi

# ///////////////////////
# Programas de ejecucion
PLSA="./pLSA" # 08_plsa_pzd_float_cvmat_tbb
# ./pLSA file1_pwd ndocs nwords format1 file2_pwz ntopics format2 lambda fix_iters dir_out num_part_file1
# ///////////////////////

# ///////////////////
# Fichero de entrada con la matrix pwd
PWD=$1
# Numero de documentos
ND=`wc -l $1 | cut -d" " -f1`
# Formato (0->HPC, 1->HPB)
FORMAT_PWD=$2 
# Numero de palabras
NW=`wc -l $3 | cut -d" " -f1`
# Fichero de entrada con la matrix pwz
PWZ=$4
# Numero de topicos
NT=`wc -l $PWZ | cut -d" " -f1`
# Formato (0->HPC, 1->HPB)
FORMAT_PWZ=0 
# Directorio de salida 
DIROUT=$5
# ///////////////////

# ////////////////
# Ejecutamos pLSA
$PLSA $PWD $ND $NW $FORMAT_PWD $PWZ $NT $FORMAT_PWZ $LAMBDA $FIX_ITERS $DIROUT $NUM_PART
# ////////////////
