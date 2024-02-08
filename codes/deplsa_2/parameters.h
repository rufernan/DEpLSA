#ifndef PARAMETERS_H
#define PARAMETERS_H

/////////////////////////////////////////////////
// Includes para todos los módulos del programa /
/////////////////////////////////////////////////

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>

#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/flann/flann.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/ml/ml.hpp>

#include "tbb/tbb.h"

using namespace std;
using namespace cv;
using namespace tbb;


/////////////////////////////////////////////////
/////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// Parámetros necesarios para la ejecución global del programa /
////////////////////////////////////////////////////////////////

// Tamaño máximo de los nombres completos de las muestras (nombre+ruta)
#define MAXTAM 1000 

// Desplazamiento saber en qué número empiezar a contar las palabras
//  (1ªpalabra=0 -> OFFSET=0, 1ªpalabra=1 -> OFFSET=1)
#define OFFSET 0

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif