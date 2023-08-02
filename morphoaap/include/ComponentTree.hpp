#include <list>
#include <vector>

#include "../include/NodeCT.hpp"

#include <pybind11/numpy.h>

#ifndef COMPONENT_TREE_H
#define COMPONENT_TREE_H


namespace py = pybind11;


class ComponentTree {

private:
    //int* img;
	int numCols;
	int numRows;
	bool maxtreeTreeType;
	NodeCT* root;
	//int* parent;
	//int *orderedPixels;
	int numNodes;
	std::list<NodeCT*> listNodes;

	int* countingSort(int* img);
	int* createTreeByUnionFind(int* orderedPixels, int* img);
	int findRoot(int *zPar, int x);
	void reconstruction(NodeCT* node, int* imgOut);

public:
   
    ComponentTree(int numRows, int numCols, bool isMaxtree);

    ComponentTree(py::array_t<int> &input, int numRows, int numCols, bool isMaxtree);

	~ComponentTree();

    static py::array_t<int> computerParent(py::array_t<int> &input, int numRows, int numCols, bool isMaxtree){
		
		auto buf_input = input.request();
		int* img = (int *) buf_input.ptr;
		ComponentTree *tree = new ComponentTree(numRows, numCols, isMaxtree);
	
		int* orderedPixels = tree->countingSort(img);
		int* parent = tree->createTreeByUnionFind(orderedPixels, img);
		delete[] orderedPixels;
		delete tree;

		int n = numRows * numCols;
		auto parent_numpy = py::array(py::buffer_info(
			parent,            // Pointer to data (nullptr -> ask NumPy to allocate!) 
			sizeof(int),     // Size of one item 
			py::format_descriptor<int>::value, // Buffer format 
			1,          // How many dimensions? 
			{ ( n ) },  // Number of elements for each dimension 
			{ sizeof(int) }  // Strides for each dimension 
		));
		
		return parent_numpy;
	}

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