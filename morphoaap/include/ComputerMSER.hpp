
#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"

#ifndef COMPUTER_MSER_H
#define COMPUTER_MSER_H



class ComputerMSER {
private:
	
	ComponentTree* tree;
	double maxVariation;
	int minArea;
	int maxArea;
	int num;
	int* ascendant;
	int* descendants;
	double* stability;
	double *attribute;

	NodeCT* getNodeAscendant(NodeCT* node, int h);

	void maxAreaDescendants(NodeCT* nodeAsc, NodeCT* nodeDes);
	
	double getStability(NodeCT* node);

public:
	ComputerMSER(ComponentTree* tree);

	bool* computerMSER(int delta);

    int getNumNodes();

	int descendantNodeWithMinStability(NodeCT* node);
	
    int ascendantNodeWithMinStability(NodeCT* node);

	double* getStabilities();
};

#endif