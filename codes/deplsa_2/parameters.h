#ifndef PARAMETERS_H
#define PARAMETERS_H

/////////////////////////////////////////////////
// Includes para todos los m�dulos del programa /
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
// Par�metros necesarios para la ejecuci�n global del programa /
////////////////////////////////////////////////////////////////

// Tama�o m�ximo de los nombres completos de las muestras (nombre+ruta)
#define MAXTAM 1000 

// Desplazamiento saber en qu� n�mero empiezar a contar las palabras
//  (1�palabra=0 -> OFFSET=0, 1�palabra=1 -> OFFSET=1)
#define OFFSET 0

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#endif