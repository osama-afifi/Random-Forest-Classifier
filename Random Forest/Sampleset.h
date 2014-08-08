#ifndef __Sampleset_h__
#define __Sampleset_h__


#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>

#include "Dataset.h"
//#include "SamplesetSplit.h"


class SamplesetSplit;

class Sampleset
{

protected:

	vector<int> dataRef;
	Dataset *dataSet;

public:

	Sampleset()
	{
		dataRef.clear();
		dataSet = NULL;
	}

	Sampleset(Dataset & dataSet, const vector<int> & dataRef )
	{
		this->dataRef = dataRef;
		this->dataSet = &dataSet;
	}

	__inline double & getEntry(const int row , const int col)
	{
		return dataSet->getEntry(dataRef[row] ,col);
	}

	__inline double & operator()(const int row , const int col)
	{
		return getEntry(row ,col);
	}

	__inline int getSize()
	{
		return dataRef.size();
	}

	__inline int getNumFeatures()
	{
		return dataSet->getNumFeatures();
	}

	__inline int getLabel(const int row)
	{
		return dataSet->getLabel(dataRef[row]);
	}

	__inline int getAverageLabel()
	{
		map<int,int> labelFreq;
		for(int i = 0 ; i<getSize() ; i++)
		{
			++labelFreq[getLabel(i)];
		}
		int maxFreq = 0;
		int maxLabel = 0;
		for(map<int,int>::iterator it = labelFreq.begin() ; it!=labelFreq.end() ; it++)
			if(it->second>maxFreq)
			{
				maxFreq = it->second;
				maxLabel = it->first;
			}
			return maxLabel;
	}

	__inline int getNumOfLables()
	{
		return dataSet->getNumOfLabels();
	}

	vector<int> getKeys()
	{
		return dataRef;	
	}

	vector<double> thresholdCandidates(int feature)
	{
		set<double> thresholds;
		for(int i = 0 ; i<getSize() ; i++)
			thresholds.insert(getEntry(i,feature));
		vector<double> candidates;
		candidates.push_back(0.0);
		for(set<double>::iterator it = thresholds.begin(); it!=thresholds.end(); it++)
			candidates.push_back(*it);
		return candidates;
	}

		vector<int> featureCandidates(int keySize)
	{
		set<int> features;
		srand (time(NULL));
		if(keySize>getNumFeatures())
			keySize = getNumFeatures();
		while(features.size() == keySize)
			features.insert(rand()%getNumFeatures());
		vector<int> candidates;
		candidates.push_back(0.0);
		for(set<int>::iterator it = features.begin(); it!=features.end(); it++)
			candidates.push_back(*it);
		return candidates;
	}

	Dataset::BranchProbabilty calculateFeatureProb(int feature , double threshold)
	{
		Dataset::BranchProbabilty branch;
		for(int i = 0 ; i<getSize() ; i++)
		{
			if(getEntry(i,feature) <= threshold)
				branch.lessEqProb++;
			else
				branch.greaterProb++;
		}
		branch.lessEqProb /= (double)getSize();
		branch.greaterProb /= (double)getSize();

		return branch;
	}

	Dataset::BranchProbabilty bestClassSplit(int & labelThreshold)
	{
		//int numLables = getNumOfLables();
		//Dataset::BranchProbabilty bestSplitProb;
		//double bestEntropy = -1e9;
		//for(int label = 0 ; label<numLables ; label++)
		//{
		//	Dataset::BranchProbabilty curSplitProb  = calculateClassProb(label);
		//	if(curSplitProb.entropy() > bestEntropy)
		//	{
		//		bestSplitProb = curSplitProb;
		//		bestEntropy = curSplitProb.entropy();
		//		labelThreshold = label;
		//	}
		//}
		//return bestSplitProb;
		return calculateClassProb(0);
	}

	Dataset* getDatasetRef()
	{
		return dataSet;
	}

	double Sampleset::informationGain(int feature, double &threshold);

protected:

	Dataset::BranchProbabilty calculateClassProb(int label)
	{
		Dataset::BranchProbabilty branch;
		for(int i = 0 ; i<getSize() ; i++)
		{
			if(getLabel(i) <= label)
				branch.lessEqProb++;
			else
				branch.greaterProb++;
		}
		branch.lessEqProb /= (double)getSize();
		branch.greaterProb /= (double)getSize();

		return branch;
	}

};


#endif