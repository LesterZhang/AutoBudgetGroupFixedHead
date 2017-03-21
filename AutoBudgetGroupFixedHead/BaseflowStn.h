#pragma once
#include <string>
using namespace std;

class CBaseflowStn
{
public:
	CBaseflowStn();
	CBaseflowStn(int i);
    ~CBaseflowStn();
	// Baseflow Station name

	string StnName;
	// Number of nodes representing the stations for baseflow measurement
	int iNoNode;

	// A pointer to point to the array storing the nodal number of the stations
	int *pNodes;
	// Initial the base flow station class
	int InitBFStn(string _stnName, int _iNoNodes);
	// Total positive baseflow for the station
	double dBaseFlowPos;
	// Total negative baseflow for the station
	double dBaseFlowNeg;
};

