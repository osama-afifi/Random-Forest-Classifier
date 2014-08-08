#ifndef __RandomForest_h__
#define __RandomForest_h__

#include<map>
#include<iostream>
#include<vector>

#include "RandomTree.h"

#define EQUAL(a,b) 

using namespace std;

class RandomForest
{
private:

	typedef vector <RandomTree*> Forest;

	Forest forest;
	Dataset * dataset;
	int numOfTrees;
	int bootstrapSize;
	int keySize;
	int nodeLimit;
	bool printProgress;

public:

	RandomForest(	Dataset * dataset, 	int numOfTrees, int bootstrapSize, int nodeLimit, int keySize, bool printProgress=true)
		: dataset(dataset), numOfTrees(numOfTrees), bootstrapSize(bootstrapSize), keySize(keySize), nodeLimit(nodeLimit), printProgress(printProgress)
	{
		forest.clear();
	}
	~RandomForest()
	{
		burnForest();
	}

	void buildForest()
	{	
		if(printProgress == true)
			cout << "Building Forest ..." << endl;

		for(int i = 0 ; i<numOfTrees; i++)
		{
			if(printProgress == true)
				showProgress(i);
			BootstrapSet * bootstrap = new BootstrapSet(*dataset, bootstrapSize);
			RandomTree* tree = new RandomTree(bootstrap, nodeLimit, keySize);
			tree->buildRandomTree();
			//cout << tree->drawVisualization();
			forest.push_back(tree);
		}

		if(printProgress == true)
		{
			cout << "\nForest Built..." << endl;
			showProgress(-1);
		}
	}

	int classify(const vector<double> & dataEntry)
	{
		map<int,int> labelFreq;
		for(int i = 0 ; i<numOfTrees ; i++)
		{
			RandomTree* tree = forest[i];
			++labelFreq[tree->classify(dataEntry)];
		}
		int bestLabel = -1;
		int maxFreq = 0;

		for(map<int,int>::iterator it=labelFreq.begin() ; it != labelFreq.end() ; it++)
		{
			if( it->second > maxFreq)
				bestLabel = it->first;
		}
		return bestLabel;
	}

	//for binary labels
	void trainingSetReport() 
	{
		cout << "Calculating Predictions ...\n";
		unsigned int truePositive = 0;
		unsigned int trueNegative = 0;
		unsigned int falsePositive = 0;
		unsigned int falseNegative = 0;

		for ( unsigned int row = 0; row < dataset->getDataSize() ;  ++row )
		{
			int predictedLabel = classify(dataset->getRowData(row));
			int trueLabel = (dataset->getLabel(row) == 1);
			if ( predictedLabel == 1 && trueLabel == 1 ) ++truePositive;
			else if ( predictedLabel == 1 && trueLabel == 0 ) ++falsePositive;
			else if ( predictedLabel == 0 && trueLabel == 1) ++falseNegative;
			else if ( predictedLabel == 0 && trueLabel == 0) ++ trueNegative;
			else cout << " Invalid Classification (make sure you are using binary labels)" << endl; //invalid
		}

		double accuracy = (truePositive + trueNegative) * 100.0 / (truePositive + falsePositive + trueNegative + falseNegative);
		double precision = truePositive * 100.0 / (truePositive + falsePositive);
		double true_negative_rate = trueNegative * 100.0 / (trueNegative + falsePositive);
		double recall = truePositive * 100.0 / (truePositive + falseNegative);

		cout
			<< "-----------------------------------------------------------------------\n"
			<< "Training Set Classification:\n"
			<< "-----------------------------------------------------------------------\n"
			<< "tp,fp,tn,fn       : " << truePositive << ", " << falsePositive << ", " << trueNegative << ", " << falseNegative << "\n"
			<< "Accuracy          : " << accuracy << "%\n"
			<< "Precision         : " << precision << "%\n"
			<< "True negative rate: " << true_negative_rate << "%\n"
			<< "Recall            : " << recall << "%\n"
			<< "-----------------------------------------------------------------------\n";

	}

private:

	__inline void burnForest()
	{
		for(int i = 0 ; i< forest.size() ; i++)
		{
			forest[i]->burnTree();
		}
		forest.clear();
	}

	__inline void showProgress(int curTree)
	{
		if(curTree==-1)
		{
			cout << "\n-----------------------------------------------------------------------\n";
			return;
		}
		++curTree;
		//printf("\33[2K\r");
		printf("\33\r");
		printf("Tree (%d out of %d) trees |  %.3f perc", curTree , numOfTrees, ((double)curTree/(double)numOfTrees)*100.0);
	}

};

#endif