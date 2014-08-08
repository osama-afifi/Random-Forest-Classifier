#ifndef __RandomTree_h__
#define __RandomTree_h__

#include <sstream>
#include <list>

#include "Node.h"
#include "Sampleset.h"
#include "BootstrapSet.h"
#include "SamplesetSplit.h"

class RandomTree
{

private:
	Node* root;
	BootstrapSet * dataSet;
	int nodeLimit;
	int keySize;

public:

	RandomTree(BootstrapSet * dataSet, int nodeLimit, int keySize)
	{
		this->dataSet = dataSet;
		root = NULL;
		this->nodeLimit = nodeLimit;
		this->keySize= keySize;
	}

	~RandomTree()
	{
		// clean heap
		burnTree();
	}

	void buildRandomTree()
	{

		burnTree();
		if ( dataSet->getSize() < nodeLimit )
			return;
		buildRandomTreeAux(*dataSet, root, "<root>");

	}

	int classify(const vector<double> & dataEntry)
	{
		if(dataEntry.size() != dataSet->getNumFeatures())
			return -1;

		Node * curNode = root;

		while(curNode != NULL)
		{
			if(curNode->is_leaf())
				return curNode->get_classification();

			SplitNode * split = reinterpret_cast<SplitNode*>(curNode);
			Node::NodeList children = curNode->get_children();
			if(children.size() == 2)
			{
				int splitCol = split->get_column();
				double threshold = split->get_threshold();
				int childIndex = dataEntry[splitCol] > threshold;
				curNode = children[childIndex];		
			}
			else
				return -1 ; //invalid
		}
		return -1;
	}

	void burnTree()
	{
		if ( root != NULL)
		{
			Node::NodeList children = root->get_children();
			for(int i = 0 ; i<children.size(); ++i)
				delete children[i];
			delete root;
			root = NULL;
		}
	}

	
	string drawVisualization() 
	{
		stringstream output;
		if (root != NULL)
		{
			output << "graph G{\n";
			list<Node*> open_list;
			open_list.push_back(root);

			unsigned int node_id      = 0;
			unsigned int last_node_id = 0;
			while ( !open_list.empty() )
			{
				Node * node = open_list.front();

				// Draw node.
				output << "N" << node_id << "[" << node->drawVisualization() << "];\n";

				// Has children?
				if ( !node->is_leaf() )
				{
					Node::NodeList children = node->get_children();
					for (
						Node::NodeList::const_iterator iter = children.begin();
						iter != children.end(); ++iter )
					{
						Node * child = *iter;

						++last_node_id;
						output << "N" << node_id << " -- N" << last_node_id << ";\n";
						open_list.push_back( child );
					}
				}

				// Pop element and increment ID.
				open_list.pop_front();
				++node_id;
			}
			output << "}";
		}
		return output.str();
	}


private:

	void buildRandomTreeAux(Sampleset sampleset, Node * parent, string action)
	{
		//if ( sampleset.getSize() < nodeLimit )
		//	return;

		int classLabel = 0; // will pass by ref to find the best label split
		Dataset::BranchProbabilty pm_class = sampleset.bestClassSplit(classLabel);
		double classEntropy = pm_class.entropy();

		int numFeatures = sampleset.getNumFeatures();
		vector<int> featuresCand = sampleset.featureCandidates(keySize);
		double bestIG = 0.0;
		double bestThreshold = 0.0;
		int splitFeature = -1;
		bool foundSplit = false;
		
		for(int i = 0 ; i<featuresCand.size() ; i++)
		{
			int feature = featuresCand[i];
			double threshold = 0;
			double curIG = sampleset.informationGain(feature, threshold);
			if(curIG > bestIG)
			{
				bestIG = curIG;
				bestThreshold = threshold;
				splitFeature = feature;
				foundSplit = true;
			}

		}

		if ( sampleset.getSize() < nodeLimit  || foundSplit == false )
		{
			int bestLabel = sampleset.getAverageLabel(); //pm_class.greaterProb >=0.5;
			Node * leaf = new LeafNode(action, bestLabel);
			if (parent == NULL)
				root = leaf;    
			else
				parent->add_child(leaf);
		}
		else
		{

			Node * split = new SplitNode(action, splitFeature, bestThreshold );
			SamplesetSplit split_pair = SamplesetSplit(sampleset, splitFeature, bestThreshold);

			stringstream action_le;
			action_le << " <= " << bestThreshold;
			buildRandomTreeAux(split_pair.sampleLessEq, split, action_le.str() );

			stringstream action_g;
			action_g << " <= " << bestThreshold;
			buildRandomTreeAux(split_pair.sampleGreater, split, action_g.str() );

			if ( parent == NULL )
				root = split;		
			else	
				parent->add_child(split);			
		}
	}



};

#endif