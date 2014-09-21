#ifndef __Dataset_h__
#define __Dataset_h__

#include<map>
#include<set>
#include<vector>
#include<assert.h>

typedef vector< double > Row;
typedef vector< Row > Matrix;

class Dataset
{

public:

	class LabelVector : public std::vector<int>
	{

	public:

		int numOfLabels;


		LabelVector()
		{}
		LabelVector(const int rows)
		{
			resize(rows);
		}

		int averageLabel()
		{
			map<int,int> labelFreq;
			for(int i = 0 ; i<size() ; i++)
			{
				++labelFreq[at(i)];
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

		int getNumOfLabels()
		{
			return numOfLabels;
		}

	};

	class DataMatrix
	{
	private:


		Matrix featureMatrix;
	public:

		DataMatrix()
		{}
		DataMatrix(const int rows ,const int cols)
		{
			featureMatrix.clear();
			featureMatrix.resize(rows);
			for(int i = 0 ; i<rows ; i++)
				featureMatrix[i].resize(cols);
		}

		int getRows()
		{
			return featureMatrix.size();
		}
		int getCols()
		{
			if(featureMatrix.size())
				return featureMatrix[0].size();
			return 0;
		}

		__inline Row getRowData(int row)
		{
			return featureMatrix[row];
		}

		double & operator()(const int row , const int col)
		{
			assert(row >= 0 && row < getRows());
			assert(col >= 0 && col <  getCols());
			return featureMatrix[row][col];
		}
	};

	class BranchProbabilty
	{
	public:

		double lessEqProb;
		double greaterProb;

		BranchProbabilty()
		{
			lessEqProb= 0.0;
			greaterProb = 0.0;
		}

		double entropy()
		{
			double e = 0;
			// bits of information i.e base 2
			if(lessEqProb>0)
				e += -lessEqProb * (log10(lessEqProb) / log10(2.0));
			if(greaterProb>0)
				e += -greaterProb * (log10(greaterProb) / log10(2.0));
			return e;
		}
	};

private:

	DataMatrix dataMatrix;
	LabelVector labelVector;

public:

	Dataset(const int rows ,const int cols, const int numOfLabels) 
	{
		dataMatrix = DataMatrix(rows,cols);
		labelVector = LabelVector(rows);
		labelVector.numOfLabels = numOfLabels;
	}

	int getDataSize()
	{
		return dataMatrix.getRows();
	}

	int getNumFeatures()
	{
		return dataMatrix.getCols();
	}

	double & getEntry(const int row , const int col)
	{
		return dataMatrix(row,col);
	}

	Row getRowData(const int row)
	{
		return dataMatrix.getRowData(row);
	}

	int getLabel(const int row)
	{
		return labelVector[row];
	}

	void setLabel(const int row , int label)
	{
		labelVector[row] = label;
	}

	int getNumOfLabels()
	{
		return labelVector.getNumOfLabels();
	}

};

#endif
