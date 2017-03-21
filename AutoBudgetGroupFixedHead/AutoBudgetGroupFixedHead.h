#pragma once
#include "FEFLOW_struct.h"
#include "BaseflowStn.h"

// Plugin implementation class
class CAutoBudgetGroupFixedHead
{
public:
  CAutoBudgetGroupFixedHead(IfmDocument pDoc);
  ~CAutoBudgetGroupFixedHead();
  static CAutoBudgetGroupFixedHead* FromHandle(IfmDocument pDoc);

#pragma region IFM_Definitions
  // Implementation
public:
  void OnActivate (IfmDocument pDoc, Widget wParent);
  void OnEditDocument (IfmDocument pDoc, Widget wParent);
  void PostSimulation (IfmDocument pDoc);
  void PostTimeStep (IfmDocument pDoc);
  void PostFlowSimulation (IfmDocument pDoc);
#pragma endregion

private:
  IfmDocument m_pDoc;

public:
	CFeflowModel currentModel;
	// Import the data
	int iInputData(string _strFileName);
	// baseflow station information
	CBaseflowStn * pBaseFlowStns = NULL;

	// Number of baseflow station
	int iNoStn;
	string strFileName; 	// Input file name
							// Calculate the baseflow at the stations
	void vCalBAseFlowAtStation(IfmDocument pDoc, CBaseflowStn * _baseStation);
	// calculate the baseflow using the IfmBudgetFlowCreate function.
	void vCalBAseFlowAtStation1(IfmDocument pDoc, CBaseflowStn * _baseStation);

	int iBudgetCal(IfmDocument _pDoc, ofstream& _foup, int iTimeClass);
	ifstream finp;
	ofstream foupDEF;
	ofstream foupOut;

	// Keep track if the input data is read properly
	int i_DataInput;
};
