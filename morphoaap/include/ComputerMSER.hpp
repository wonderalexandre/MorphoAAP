
#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"
#include <vector>

#ifndef COMPUTER_MSER_H
#define COMPUTER_MSER_H



class ComputerMSER {
private:
	
	ComponentTree* tree;
	double maxVariation;
	int minArea;
	int maxArea;
	int num;
	std::vector<int> ascendants;
	std::vector<int> descendants;
	std::vector<double> stability;
	double *attribute;

	NodeCT* getNodeAscendant(NodeCT* node, int h);

	void maxAreaDescendants(NodeCT* nodeAsc, NodeCT* nodeDes);
	
public:
	ComputerMSER(ComponentTree* tree);

	~ComputerMSER();

	std::vector<bool> computerMSER(int delta);

    int getNumNodes();

	int descendantNodeWithMinStability(NodeCT* node);
	
    int ascendantNodeWithMinStability(NodeCT* node);

	std::vector<double> getStabilities();

	double getStability(NodeCT* node);

	std::vector<int> getAscendants();

	std::vector<int> getDescendants();

};

#endif