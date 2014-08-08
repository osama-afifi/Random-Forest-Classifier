#ifndef __SamplesetSplit_h__
#define __SamplesetSplit_h__

#include "Sampleset.h"
//#include "Dataset.h"

using namespace std;

class SamplesetSplit
{
public:
	Sampleset sampleLessEq;
	Sampleset sampleGreater;

	SamplesetSplit(Sampleset & sampleset, int feature , double threshold)
	{
		split(sampleset, feature, threshold);
	}

private:

	__inline void split(Sampleset & sampleset, int feature , double threshold)
	{
		vector<int> LEQ;
		vector<int> G;

		// Partition data.
		for ( unsigned int row = 0; row < sampleset.getSize() ; ++row )
		{
			if ( sampleset.getEntry(row,feature) > threshold )
				G.push_back(row);
			else
				LEQ.push_back(row);
		}

		sampleLessEq = Sampleset(*sampleset.getDatasetRef(), LEQ);
		sampleGreater = Sampleset(*sampleset.getDatasetRef(), G);

	}
};

double Sampleset::informationGain(int feature, double &threshold)
{
	// Calculate classification entropy.
	int labelThreshold = 0;
	Dataset::BranchProbabilty class_split = bestClassSplit(labelThreshold);
	double classEntropy = class_split.entropy();

	// Iterate over all possible thresholds.
	vector<double> threshold_candidates = thresholdCandidates(feature);
	double bestSplitThreshold =  -1e199;
	double bestSplitIG = -1000;

	for(int i = 0 ; i<threshold_candidates.size() ; i++)
	{
		double splitIG = classEntropy;
		double curThreshold = threshold_candidates[i];

		// Enumerate at threshold.
		SamplesetSplit split_pair = SamplesetSplit(*this, feature, curThreshold);
		double leqProb = (double)split_pair.sampleLessEq.getSize() / (double)getSize();
		double gProb = (double)split_pair.sampleGreater.getSize() / (double)getSize();

		// Less or Equal entropy.
		Dataset::BranchProbabilty enumLEQ = split_pair.sampleLessEq.calculateFeatureProb(feature, curThreshold);
		double LEQ_Entropy = enumLEQ.entropy();
		splitIG -= leqProb * LEQ_Entropy;

		//  Greater entropy.
		Dataset::BranchProbabilty enumG = split_pair.sampleGreater.calculateFeatureProb(feature, curThreshold);
		double G_Entropy = enumG.entropy();
		splitIG -= gProb * G_Entropy;

		// Best split?
		if ( splitIG > bestSplitIG )
		{
			bestSplitIG = splitIG;
			bestSplitThreshold = curThreshold;
		}
	}

	threshold = bestSplitThreshold;

	return bestSplitIG;
}



#endif
