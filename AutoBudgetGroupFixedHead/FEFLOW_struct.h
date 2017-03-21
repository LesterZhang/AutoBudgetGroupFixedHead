#include "stdifm.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <direct.h> 
#include <string.h>
#include <cmath>
#include "IfmMLWInfo1.h"
using namespace std;

class CFeflowModel
{

public: 
		CFeflowModel();
		~CFeflowModel(void);

private:
		IfmDocument pDoc;
public:
		double originalX; //the  X coordinate at the origin of the FEFLOW model
		double originalY;  //the Y coordinate at the origin of the FEFLOW model
		double originalZ;  //the Z coordinate at the origin of the FEFLOW model
		int nNode; //Total number of nodes of the feflow model;
		int nLayer; //Total number of layer of the feflow model
		int nNodePerSlice; //number of nodes per model slice of the feflow model
		int nElePerLayer; //number of elements per model layer
		int nNMLW; //number of multilayer well in the FEFLOW model
		IfmMLWInfo1 *MLWInfo; //A pointer to store the MultiLayer well information
		string femPath;	//a string to store the path of the FEM file, which includes the name of the fem file
		string femNam;  //a string to store the file name of the FEM model
		//const char*femPath;	//a string to store the path of the FEM file, which includes the name of the fem file
		//const char* femNam=;  //a string to store the file name of the FEM model
		//sring test;
		IfmProblemClass iClassKind; // the Class of the problem
		int iClassType;		// Problem type flag 0 for saturated, 1 for unsaturated
		int iTimeClass; //integer for the time class 0 - SS flow and SS transport, 1 - TS flow and TS transport, 2 - SS flow and TS transport



		void QueryFemInfo(IfmDocument pDoc);
		// Set the current directory to the FEMDATA directory
		void SetCurrentPath(string CurrentFemPath);

};


