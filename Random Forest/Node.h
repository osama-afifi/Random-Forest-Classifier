#ifndef __Node_h__
#define __Node_h__

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Node
{
  private:

    const string action; 

  public:

    typedef std::vector<Node*> NodeList;
    Node( const string action ) : action(action)
    {  }
    virtual ~Node() {}
    const string get_action() const
    {
      return action;
    }

    virtual bool get_classification() const = 0;
    virtual bool is_leaf() const = 0;
    virtual void add_child( Node * const node ) = 0;
    virtual NodeList get_children() = 0;
    virtual string drawVisualization() const = 0;

};


class LeafNode : public Node // for decision
{
  private:

    const bool classification;

  public:

    LeafNode( const string action, const bool classification ) : Node(action), classification(classification)
    {}

    virtual bool get_classification()
    {
      return classification;
    }

    virtual bool is_leaf() const
    {
      return true;
    }

    virtual void add_child( Node * const node ) //useless
    {}

    virtual NodeList get_children()
    {
      NodeList set; // return empty list
      return set;
    }

    virtual string drawVisualization() const
    {
      string draw_string =  "shape=ellipse,label=\"(" + get_action() + ")\\n" + (classification ? "1" : "0") +"\"";
      return draw_string;
    }
};

class SplitNode : public Node //ordinary nodes of rest of random tree
{	
  private:

    NodeList children;          
    const unsigned int column;   
    const double threshold;   

  public:

    SplitNode(const string action, const unsigned int splitColumn, const double splitThreshold) :
        Node(action), column(column), threshold(threshold)
    {}

    virtual ~SplitNode() // deletes all children
    {
      if ( children.size() > 0 )
        for (NodeList::iterator iter = children.begin(); iter != children.end(); ++iter )
          delete *iter;      	  
    }

    virtual bool get_classification() const //useless as get children for leaves
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
      std::string draw_string = "shape=box,label=\"(" + get_action() + ")\\n" + ss.str() +"\"";
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
  
};

#endif