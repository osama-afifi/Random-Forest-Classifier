#include <iostream>
#include <map>
#include <fstream>
#include <stdlib.h>


//#include "RandomForest.h"
#include "CsvParser.h"
#include "Sampleset.h"
#include "Dataset.h"
#include "RandomForest.h"

using namespace std;

typedef vector<double> row;
typedef vector<row> RealMatrix;

void SelectFeatures(CsvParser &myParser, vector<int>selColIndices )
{
	myParser.selectCol(selColIndices);
	//Sex
	myParser.replace(1,"female","0");
	myParser.replace(1,"male","1");
	//Age
	int countAge = 0;
	double sumAge = 0;
	for(int i = 0 ; i<myParser.getRows() ; i++)
		if (myParser.matrix[i][2] != "")
		{
			++countAge;
			sumAge += myParser.stringToNum(myParser.matrix[i][2]);
		}
		double mean = sumAge/(double)countAge;
		for(int i = 0 ; i<myParser.getRows() ; i++)
			if (myParser.matrix[i][2] == "")
				myParser.matrix[i][2] = myParser.numToString(mean);
		//Embarked
		myParser.replace(5,"Q","0");
		myParser.replace(5,"S","1");
		myParser.replace(5,"C","2");
}


int main()
{
	
	//Training Parsing Phase
	CsvParser myParser;
	myParser.readMatrix("D:\\Osama\\Programming\\Projects\\Titanic - Machine Learning from Disaster\\Data\\train.csv");
	const int trainSelColIndices[] = {2,4,5,6,7,11,1};
	const int trainColSize = 7;
	SelectFeatures(myParser, vector<int>(trainSelColIndices , trainSelColIndices+trainColSize));
	//PassengerId - Survived - Pclass - Name - Sex - Age - SibSp - Parch - Ticket - Fare - Cabin - Embarked
	//Pclass - Sex - Age - SibSp - Parch - Embarked - Survived
	// 0		1	  2		 3		 4		 5		     6			

	// Create dataset.
	int dataCols = myParser.getCols();

	Dataset dataset(myParser.getRows(), dataCols - 1, 2); // Ignore Class Label Column
	// Convert data.
	for (int row = 0; row < dataset.getDataSize() ; ++row )
		for ( unsigned int col = 0; col < dataset.getNumFeatures() ; ++col )
			dataset.getEntry(row,col) = myParser.stringToNum(myParser.matrix[row][col]);
	// Label Vector
	for (int row = 0; row < dataset.getDataSize() ; ++row )
		dataset.setLabel(row, (int)myParser.stringToNum(myParser.matrix[row][dataCols-1]));
	
	RandomForest forest(&dataset, 100, 500,  4, 5, true); 
	forest.buildForest();

	forest.trainingSetReport();


	return 0;

}