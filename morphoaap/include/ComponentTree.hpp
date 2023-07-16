#include <list>
#include <vector>

#include "../include/NodeCT.hpp"

#include <pybind11/numpy.h>

#ifndef COMPONENT_TREE_H
#define COMPONENT_TREE_H


namespace py = pybind11;


class ComponentTree {

private:
    int* img;
	int numCols;
	int numRows;
	bool maxtreeTreeType;
	NodeCT* root;
	int* parent;
	int *orderedPixels;
	int numNodes;
	std::list<NodeCT*> listNodes;

	int* countingSort();
	int findRoot(int *zPar, int x);
	int coord2Index(int linha, int coluna);
	std::vector<int> getAdjPixels(int indice);
	void createTreeByUnionFind();
	void reconstruction(NodeCT* node, int* img);

public:
   
    ComponentTree(py::array_t<int> &input, int numRows, int numCols, bool isMaxtree);

    py::array_t<int> getParent();

	int* getInputImage();
	
	NodeCT* getRoot();

	bool isMaxtree();

	std::list<NodeCT*> getListNodes();

	int getNumNodes();

	int getNumRowsOfImage();

	int getNumColsOfImage();

	py::array_t<int> reconstructionImage();


};

#endif