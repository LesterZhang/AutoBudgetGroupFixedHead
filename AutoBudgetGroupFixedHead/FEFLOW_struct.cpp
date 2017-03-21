#include "FEFLOW_struct.h"

CFeflowModel::CFeflowModel()
	: iClassType(0)
{
		
	
	femPath="";
	femNam="";
	originalX=0.0; 
	originalY=0.0; 
	originalZ=0.0; 
	nNode=0; 
	nLayer=0;
	nNodePerSlice=0; 
	nElePerLayer=0; 
	nNMLW=0; 
}

CFeflowModel::~CFeflowModel(void)
{
	//class destructurer
	if (nNMLW>0)
	{
//		delete [] MLWInfo;
	}
}

void CFeflowModel::QueryFemInfo(IfmDocument pDoc)
{
	using namespace std;
	int classkind;
	pDoc=pDoc;
	femPath=IfmGetProblemPath(pDoc);
	femNam=IfmGetProblemTitle(pDoc);
	femPath.resize(femPath.length() - femNam.length(), '+');
	nNode=IfmGetNumberOfNodes(pDoc);
	nLayer=IfmGetNumberOfLayers(pDoc);
	nNodePerSlice = IfmGetNumberOfNodesPerSlice(pDoc);;
	nElePerLayer = IfmGetNumberOfElementsPerLayer(pDoc);
	originalX=IfmGetOriginX(pDoc);
	originalY=IfmGetOriginX(pDoc);
	originalZ=IfmGetOriginX(pDoc);
	
	//Get the problem class
	classkind = IfmGetProblemClass(pDoc);
	switch (classkind)
	{
	case -1:
		iClassKind = IfmPCLS_NO_CLASS;
		break;
	case 0:
		iClassKind = IfmPCLS_FLOW;
		break;
	case 1:
		iClassKind = IfmPCLS_MASS_TRANSPORT;
		break;
	case 2:
		iClassKind = IfmPCLS_HEAT_TRANSPORT;
		break;
	case 3:
		iClassKind = IfmPCLS_THERMOHALINE;
		break;
	}

	iClassType = IfmGetProblemType(pDoc);
	iTimeClass = IfmGetTimeClass(pDoc);

}
// Set the current directory to the FEMDATA directory
void CFeflowModel::SetCurrentPath(string CurrentFemPath)
{
		_chdir(CurrentFemPath.c_str());

}
