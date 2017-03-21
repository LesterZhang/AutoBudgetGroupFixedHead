#include "BaseflowStn.h"


CBaseflowStn::CBaseflowStn()
	: StnName("")
	, iNoNode(0)
	, pNodes(NULL)
	, dBaseFlowPos(0)
	, dBaseFlowNeg(0)
{
	//pNodes = new int[iNoNode]; //alllcate memory for pNodes pointer,  

}

CBaseflowStn::CBaseflowStn(int i)
{
	StnName = "";
	iNoNode = i;
	pNodes = new int[i]; //alllcate memory for pNodes pointer,  
	
}

CBaseflowStn::~CBaseflowStn()
{
	//deallocate the memory
	if (pNodes!=NULL)
	{ delete [] pNodes; }
	

}

// Initial the base flow station class
int CBaseflowStn::InitBFStn(string _stnName, int _iNoNodes)
{
	StnName = _stnName;
	iNoNode = _iNoNodes;
	pNodes = new int[_iNoNodes];
	dBaseFlowNeg = 0.0;
	dBaseFlowNeg = 0.0;

	return 0;
}
