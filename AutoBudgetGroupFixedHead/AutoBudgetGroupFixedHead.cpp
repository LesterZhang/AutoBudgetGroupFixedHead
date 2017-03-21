#include "stdifm.h"
#include "AutoBudgetGroupFixedHead.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <direct.h> 
#include <math.h>
#include <ifm/document.h>

IfmModule g_pMod;  /* Global handle related to this plugin */

#pragma region IFM_Definitions
/* --- IFMREG_BEGIN --- */
/*  -- Do not edit! --  */

static IfmResult OnBeginDocument (IfmDocument);
static void OnEndDocument (IfmDocument);
static void OnActivate (IfmDocument, Widget);
static void OnEditDocument (IfmDocument, Widget);
static void PostSimulation (IfmDocument);
static void PostTimeStep (IfmDocument);
static void PostFlowSimulation (IfmDocument);

/*
 * Enter a short description between the quotation marks in the following lines:
 */
static const char szDesc[] = 
  "Please, insert a plug-in description here!";

#ifdef __cplusplus
extern "C"
#endif /* __cplusplus */

IfmResult RegisterModule(IfmModule pMod)
{
  if (IfmGetFeflowVersion (pMod) < IFM_REQUIRED_VERSION)
    return False;
  g_pMod = pMod;
  IfmRegisterModule (pMod, "SIMULATION", "AUTOBUDGETGROUPFIXEDHEAD", "AutoBudgetGroupFixedHead", 0x1000);
  IfmSetDescriptionString (pMod, szDesc);
  IfmSetCopyrightPath (pMod, "AutoBudgetGroupFixedHead.txt");
  IfmSetHtmlPage (pMod, "AutoBudgetGroupFixedHead.htm");
  IfmSetPrimarySource (pMod, "AutoBudgetGroupFixedHead.cpp");
  IfmRegisterProc (pMod, "OnBeginDocument", 1, (IfmProc)OnBeginDocument);
  IfmRegisterProc (pMod, "OnEndDocument", 1, (IfmProc)OnEndDocument);
  IfmRegisterProc (pMod, "OnActivate", 1, (IfmProc)OnActivate);
  IfmRegisterProc (pMod, "OnEditDocument", 1, (IfmProc)OnEditDocument);
  IfmRegisterProc (pMod, "PostSimulation", 1, (IfmProc)PostSimulation);
  IfmRegisterProc (pMod, "PostTimeStep", 1, (IfmProc)PostTimeStep);
  IfmRegisterProc (pMod, "PostFlowSimulation", 1, (IfmProc)PostFlowSimulation);
  return True;
}

static void OnActivate (IfmDocument pDoc, Widget button)
{
  CAutoBudgetGroupFixedHead::FromHandle(pDoc)->OnActivate (pDoc, button);
}
static void OnEditDocument (IfmDocument pDoc, Widget wParent)
{
  CAutoBudgetGroupFixedHead::FromHandle(pDoc)->OnEditDocument (pDoc, wParent);
}
static void PostSimulation (IfmDocument pDoc)
{
  CAutoBudgetGroupFixedHead::FromHandle(pDoc)->PostSimulation (pDoc);
}
static void PostTimeStep (IfmDocument pDoc)
{
  CAutoBudgetGroupFixedHead::FromHandle(pDoc)->PostTimeStep (pDoc);
}
static void PostFlowSimulation (IfmDocument pDoc)
{
  CAutoBudgetGroupFixedHead::FromHandle(pDoc)->PostFlowSimulation (pDoc);
}

/* --- IFMREG_END --- */
#pragma endregion


static IfmResult OnBeginDocument (IfmDocument pDoc)
{
  if (IfmDocumentVersion (pDoc) < IFM_CURRENT_DOCUMENT_VERSION)
    return false;

  try {
    IfmDocumentSetUserData(pDoc, new CAutoBudgetGroupFixedHead(pDoc));
  }
  catch (...) {
    return false;
  }

  return true;
}

static void OnEndDocument (IfmDocument pDoc)
{
  delete CAutoBudgetGroupFixedHead::FromHandle(pDoc);
}

///////////////////////////////////////////////////////////////////////////
// Implementation of CAutoBudgetGroupFixedHead

// Constructor
CAutoBudgetGroupFixedHead::CAutoBudgetGroupFixedHead(IfmDocument pDoc)
	: m_pDoc(pDoc)
	, pBaseFlowStns(NULL)
	, iNoStn(0)
	, i_DataInput(1)
{
	/*
	 * TODO: Add your own code here ...
	 */
	currentModel.QueryFemInfo(pDoc);
	currentModel.SetCurrentPath(currentModel.femPath);
	strFileName = "AutoBudgetGroupFixedHead"; //input file
											  //Write the heading of the output file

	i_DataInput = iInputData(strFileName);
	if (i_DataInput != 0)
	{
		IfmInfo(pDoc, "The input data is not read properly, please check to make sure the input data is in the right format and located in the same directory as the model file!");

	}
	else
	{

		foupOut.open((strFileName + ".out").c_str(), ios_base::out);
		foupOut << setiosflags(ios::fixed | ios::left);

		foupOut << setw(20) << "STN ID" << "\t" << setw(15) << "IN" << "\t" << setw(15) << "OUT" << "\t" << setw(15) << "IN-OUT" << endl;
		foupOut.close();
	}

}

// Destructor
CAutoBudgetGroupFixedHead::~CAutoBudgetGroupFixedHead ()
{
  /*
   * TODO: Add your own code here ...
   */
	if (pBaseFlowStns != NULL)
		delete[] pBaseFlowStns;
}

// Obtaining class instance from document handle
CAutoBudgetGroupFixedHead* CAutoBudgetGroupFixedHead::FromHandle (IfmDocument pDoc)
{
  return reinterpret_cast<CAutoBudgetGroupFixedHead*>(IfmDocumentGetUserData(pDoc));
}

// Callbacks
void CAutoBudgetGroupFixedHead::OnActivate (IfmDocument pDoc, Widget button)
{

	int i;
	foupOut.open((strFileName + ".out").c_str(), ios_base::app);

	if (i_DataInput == 0)
	{
		i = iBudgetCal(pDoc, foupOut, currentModel.iTimeClass);
		if (i == 1)
		{
			IfmInfo(pDoc, "errors occure when calculate the budget for the group fixed-heads nodes, please check the input file");

		}
	}
	else
	{

		IfmInfo(pDoc, "The input data is not read properly while loading the plug-in, now we will try again!");
		i = iInputData(strFileName);
		if (i != 0)
		{
			IfmInfo(pDoc, "The input data is not read properly, please check to make sure the input data is in the right format and located in the same directory as the model file!");
			return;
		}
		else 
		{
			i_DataInput = 0;
			i = iBudgetCal(pDoc, foupOut, currentModel.iTimeClass);
			if (i == 1)
			{
				IfmInfo(pDoc, "errors occure when calculate the budget for the group fixed-heads nodes, please check the input file");

			}

		}

	}
	foupOut.close();

}

void CAutoBudgetGroupFixedHead::OnEditDocument (IfmDocument pDoc, Widget wParent)
{
  /* 
   * TODO: Add your own code here ...
   */
}

void CAutoBudgetGroupFixedHead::PostSimulation (IfmDocument pDoc)
{
  /* 
   * TODO: Add your own code here ...
   */
   

}

void CAutoBudgetGroupFixedHead::PostTimeStep (IfmDocument pDoc)
{
  
}

void CAutoBudgetGroupFixedHead::PostFlowSimulation (IfmDocument pDoc)
{
	int i;
	foupOut.open((strFileName + ".out").c_str(), ios_base::app);

	if (i_DataInput == 0)
	{
		i = iBudgetCal(pDoc, foupOut, currentModel.iTimeClass);
		if (i == 1)
		{
			IfmInfo(pDoc, "errors occure when calculate the budget for the group fixed-heads nodes, please check the input file");

		}
	}
	else
	{

		IfmInfo(pDoc, "The input data is not read properly while loading the plug-in, now we will try again!");
		i = iInputData(strFileName);
		if (i != 0)
		{
			IfmInfo(pDoc, "The input data is not read properly, please check to make sure the input data is in the right format and located in the same directory as the model file!");
			return;
		}
		else
		{
			i_DataInput = 0;
			i = iBudgetCal(pDoc, foupOut, currentModel.iTimeClass);
			if (i == 1)
			{
				IfmInfo(pDoc, "errors occure when calculate the budget for the group fixed-heads nodes, please check the input file");

			}

		}

	}
	foupOut.close();
		
}

int CAutoBudgetGroupFixedHead::iInputData(string _strFileName)
{	// Import the data
	//ifstream finp;
	//ofstream foup;
	int i, j;
	//int iNoStn; // number of baseflow station for assessment
	string stnName; // station number
	int iNoNode; // number of nodes for this stations


	IfmInfo(m_pDoc, "reading group nodes information from %s.\n", (_strFileName + ".inp").c_str());

	finp.open((_strFileName + ".inp").c_str(), ios_base::in);  //input file

	foupDEF.open((_strFileName + ".def").c_str(), ios_base::out);  //write out the nodel definiation of each station
	if (finp.is_open())
	{
		finp >> iNoStn;
		foupDEF << iNoStn << endl;

		pBaseFlowStns = new CBaseflowStn[iNoStn];

		//read station defintation data
		for (i = 0; i < iNoStn; i++)
		{
			finp >> stnName >> iNoNode;
			//getline(finp, strName);
			foupDEF << stnName << "\t" << iNoNode << endl;

			pBaseFlowStns[i].InitBFStn(stnName, iNoNode);
			//read the nodes information of the stations
			for (j = 0; j < iNoNode; j++)
			{
				finp >> pBaseFlowStns[i].pNodes[j];
				//change the node number to be zero based. 
				pBaseFlowStns[i].pNodes[j] -= 1;
				foupDEF << pBaseFlowStns[i].pNodes[j] << "\t";

			}
			foupDEF << endl;
		}
		finp.close();
		IfmInfo(m_pDoc, "Input data is read successfully!");

	}
	else
	{
		IfmInfo(m_pDoc, "Can not open the file %s.\n", (_strFileName + ".inp").c_str());
		return 1;
	}
	return 0;
}


// Calculate the baseflow at the stations
void CAutoBudgetGroupFixedHead::vCalBAseFlowAtStation(IfmDocument pDoc, CBaseflowStn * _baseStation)
{

	IfmBudgetComponentsEx pCumulativeBudget;

	IfmBudgetCompute(pDoc, currentModel.iClassKind, _baseStation->pNodes, _baseStation->iNoNode, NULL, &pCumulativeBudget);
	_baseStation->dBaseFlowPos = pCumulativeBudget.dirichlet_in;
	_baseStation->dBaseFlowNeg = pCumulativeBudget.dirichlet_out;

}


// calculate the baseflow using the IfmBudgetFlowCreate function.
void CAutoBudgetGroupFixedHead::vCalBAseFlowAtStation1(IfmDocument pDoc, CBaseflowStn * _baseStation)
{
	//I think MatrixSolution's plug-IN autoExportGroupFlowsAndRatios using this method to calculate the baseflow, and the totoal flux is calcaulted to be same with the calcualtion for the 
	//vCalBAseFlowAtStation for the constant head. But vCalBAseFlowAtStation is much fast if it is only selected for a few nodes. 

	IfmBudgetComponents BudgetComp;
	IfmBudget *b = IfmBudgetFlowCreate(pDoc);
	int i;
	for (i = 0; i < _baseStation->iNoNode; i++)
	{
		if (IfmBudgetComponentsQueryFlowAtNode(pDoc, b, _baseStation->pNodes[i], &BudgetComp))
		{
			_baseStation->dBaseFlowNeg += BudgetComp.total_flux;
			_baseStation->dBaseFlowPos += BudgetComp.boundary_flux;
		}

	}

}

int CAutoBudgetGroupFixedHead::iBudgetCal(IfmDocument _pDoc, ofstream& _foup, int iTimeClass)
{
	int j; 
	double d_absTime;
	d_absTime = 0.0;


	if (iTimeClass == 0 || iTimeClass == 2)
	{

		_foup << setiosflags(ios_base::fixed | ios_base::left);
		IfmInfo(_pDoc, "There are %i stations to calculate...\n", iNoStn);
		for (j = 0; j < iNoStn; j++)
		{
			IfmInfo(_pDoc, "Calculating the # %i station...\n", j);
			vCalBAseFlowAtStation(_pDoc, &pBaseFlowStns[j]);
			//vCalBAseFlowAtStation(_pDoc, &pBaseFlowStns[j]);
			//IfmInfo(_pDoc, "Station %s  positive flow of Dirichlet nodes is %lf,\n.", pBaseFlowStns[j].StnName.c_str(), pBaseFlowStns[j].dBaseFlowPos);
			//IfmInfo(_pDoc, "Station %s  negative flow of Dirichlet nodes is %lf,\n", pBaseFlowStns[j].StnName.c_str(), pBaseFlowStns[j].dBaseFlowNeg);
			_foup << setw(20) << pBaseFlowStns[j].StnName << "\t" << setw(15) << setprecision(2) << pBaseFlowStns[j].dBaseFlowPos << "\t" << setw(15)
				<< setprecision(2) << pBaseFlowStns[j].dBaseFlowNeg << "\t" << setw(15) << setprecision(2) <<
				pBaseFlowStns[j].dBaseFlowPos - pBaseFlowStns[j].dBaseFlowNeg << endl;
		}
		IfmInfo(_pDoc, "Calculations are completed, please check the output file 'AutoBudgetGroupFixedHead.out' for the result.\n");

	}
	else if (currentModel.iTimeClass == 1)
	{
		d_absTime = IfmGetAbsoluteSimulationTime(_pDoc);


		_foup << setiosflags(ios_base::scientific | ios_base::left);
		IfmInfo(_pDoc, "Time= %E days", d_absTime);
		_foup << "Time = " << setw(20) << std::scientific <<std::left<<setprecision(4)<<d_absTime << " days."<<endl;
		_foup.unsetf(ios_base::scientific | ios_base::left);

		IfmInfo(_pDoc, "There are %i stations to calculate...\n", iNoStn);
		for (j = 0; j < iNoStn; j++)
		{
			IfmInfo(_pDoc, "Calculating the # %i station...\n", j);
			vCalBAseFlowAtStation(_pDoc, &pBaseFlowStns[j]);
			//vCalBAseFlowAtStation(_pDoc, &pBaseFlowStns[j]);
			//IfmInfo(_pDoc, "Station %s  positive flow of Dirichlet nodes is %lf,\n.", pBaseFlowStns[j].StnName.c_str(), pBaseFlowStns[j].dBaseFlowPos);
			//IfmInfo(_pDoc, "Station %s  negative flow of Dirichlet nodes is %lf,\n", pBaseFlowStns[j].StnName.c_str(), pBaseFlowStns[j].dBaseFlowNeg);
			_foup << setw(20)<<std::fixed<<std::left << pBaseFlowStns[j].StnName << "\t" << setw(15) << setprecision(2) << pBaseFlowStns[j].dBaseFlowPos << "\t" << setw(15)
				<< setprecision(2) << pBaseFlowStns[j].dBaseFlowNeg << "\t" << setw(15) << setprecision(2) <<
				pBaseFlowStns[j].dBaseFlowPos - pBaseFlowStns[j].dBaseFlowNeg << endl;
		}
		IfmInfo(_pDoc, "Calculations are completed, please check the output file 'AutoBudgetGroupFixedHead.out' for the result.\n");
	}
	return 0;
}
