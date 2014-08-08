#ifndef __Node_h__
#define __Node_h__

#include <vector>
#include <string>
#include <sstream>

#include <iostream>

using namespace std;

class Node
{
public:

	typedef vector<Node*> NodeList;

public:

	Node( const string action ) :
		action(action)
	{}

	virtual ~Node()
	{}

	const string get_action() const
	{
		return action;
	}

	virtual bool get_classification() const = 0;

	virtual bool is_leaf() const = 0;

	virtual void add_child( Node * const node ) = 0;

	virtual NodeList get_children() = 0;

	virtual string drawVisualization() const = 0;

private:
	const string action; ///< The action that led to the node.
};

class LeafNode : public Node
{
public:
	LeafNode(
		const string action,
		const bool classification ) :
	Node(action),
		classification(classification)
	{}

	virtual bool get_classification() const
	{
		return classification;
	}

	virtual bool is_leaf() const
	{
		return true;
	}

	virtual void add_child( Node * const node )
	{}

	virtual NodeList get_children()
	{
		NodeList set;
		return set;
	}

	virtual string drawVisualization() const
	{
		string draw_string =
			"shape=ellipse,label=\"(" + get_action() +
			")\\n" + (classification ? "1" : "0") +"\"";
		return draw_string;
	}

private:
	const int classification;  ///< Node classification.
};

class SplitNode : public Node
{
public:

	SplitNode(
		const string action,
		const unsigned int column,
		const double threshold ) :
	Node(action),
		column(column),
		threshold(threshold)
	{}

	virtual ~SplitNode()
	{
		//if ( this && children.size() > 0 )
		//{
		//	for (NodeList::iterator iter = children.begin();iter != children.end(); ++iter )
		//	{
		//		delete *iter;
		//		*iter = NULL;
		//	}
		//	
		//}
	}

	virtual bool get_classification() const // useless
	{
		return false;
	}

	virtual bool is_leaf() const
	{
		return false;
	}


	virtual void add_child( Node * const node )
	{
		children.push_back( node );
	}

	virtual NodeList get_children()
	{
		return children;
	}

	virtual string drawVisualization() const
	{
		stringstream ss;
		ss << column;
		string draw_string =
			"shape=box,label=\"(" + get_action() +
			")\\n" + ss.str() +"\"";
		return draw_string;
	}

	unsigned int get_column() const
	{
		return column;
	}

	double get_threshold() const
	{
		return threshold;
	}

private:
	NodeList children;             ///< The child nodes.
	const unsigned int column;    ///< The split column.
	const double threshold;       ///< The split threshold.
};

#endif