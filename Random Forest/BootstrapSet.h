#ifndef __BootstrapSet_h__
#define __BootstrapSet_h__

#include "Dataset.h"
#include "Sampleset.h"

class BootstrapSet : public Sampleset
{

public:

	BootstrapSet(Dataset & dataSet , int bootSize)
	{
		this->dataSet = &dataSet;
		srand (time(NULL));
		for(int i= 0 ; i<bootSize ; i++)
			dataRef.push_back(rand()%(this->dataSet->getDataSize()));
	}

};

#endif
