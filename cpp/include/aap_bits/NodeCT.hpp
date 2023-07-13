#include <list>

#ifndef NODECT_H
#define NODECT_H

class NodeCT {
private:
	int index; 
    int rep;
    int level;
	NodeCT* parent;
	std::list<int> cnps;
    std::list<NodeCT*> children;
    
public:
	
    NodeCT();
    NodeCT(int index, int rep, NodeCT* parent, int level);
    void addCNPs(int p);
    void addChild(NodeCT* child);
	int getRep();
	int getIndex();
	int getLevel();
	NodeCT* getParent();
	std::list<int> getCNPs();
	std::list<NodeCT*> getChildren();
};

#endif