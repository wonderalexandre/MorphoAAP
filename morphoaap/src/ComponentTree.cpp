#include <list>
#include <vector>
#include <stack>
#include <unordered_map>

#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"
#include "../include/Adjacency8.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>


int* ComponentTree::countingSort(int* img){
	int n = this->numRows * this->numCols;
	int maxvalue = img[0];
	for (int i = 1; i < n; i++)
		if(maxvalue < img[i]) maxvalue = img[i];
			
	std::vector<int> counter(maxvalue + 1, 0); 
	int *orderedPixels = new int[n];
		
	if(this->isMaxtree()){
		for (int i = 0; i < n; i++)
			counter[img[i]]++;

		for (int i = 1; i < maxvalue; i++) 
			counter[i] += counter[i - 1];
		counter[maxvalue] += counter[maxvalue-1];
		
		for (int i = n - 1; i >= 0; --i)
			orderedPixels[--counter[img[i]]] = i;	

	}else{
		for (int i = 0; i < n; i++)
			counter[maxvalue - img[i]]++;

		for (int i = 1; i < maxvalue; i++) 
			counter[i] += counter[i - 1];
		counter[maxvalue] += counter[maxvalue-1];

		for (int i = n - 1; i >= 0; --i)
			orderedPixels[--counter[maxvalue - img[i]]] = i;
	}
	
	return orderedPixels;
}

int ComponentTree::findRoot(int *zPar, int x) {
	if (zPar[x] == x)
		return x;
	else {
		zPar[x] = findRoot(zPar, zPar[x]);
		return zPar[x];
	}
}

int* ComponentTree::createTreeByUnionFind(int* orderedPixels, int* img) {
	const int n = this->numRows * this->numCols;
	Adjacency8 adj(this->numRows, this->numCols);
	int *zPar = new int[n];
	int *parent = new int[n];
		
	for (int p = 0; p < n; p++) {
		zPar[p] =  -1;
	}
		
	for(int i=n-1; i >= 0; i--){
		int p = orderedPixels[i];
		parent[p] = p;
		zPar[p] = p;
		for (int n : adj.getAdjPixels(p)) {
			if(zPar[n] != -1){
				int r = this->findRoot(zPar, n);
				if(p != r){
					parent[r] = p;
					zPar[r] = p;
				}
			}
		}
	}
			
	// canonizacao da arvore
	for (int i = 0; i < n; i++) {
		int p = orderedPixels[i];
		int q = parent[p];
				
		if(img[parent[q]] == img[q]){
			parent[p] = parent[q];
		}
	}
		
	delete[] zPar;
	return parent;		
}

void ComponentTree::reconstruction(NodeCT* node, int* imgOut){
	for (int p : node->getCNPs()){
		imgOut[p] = node->getLevel();
	}
	for(NodeCT* child: node->getChildren()){
		reconstruction(child, imgOut);
	}
}

ComponentTree::ComponentTree(int numRows, int numCols, bool isMaxtree){
	this->numRows = numRows;
	this->numCols = numCols;
	this->maxtreeTreeType = isMaxtree;
 }

 ComponentTree::~ComponentTree(){
	for (NodeCT *node: this->listNodes){
		delete node;
	}
 }

ComponentTree::ComponentTree(py::array_t<int> &input, int numRows, int numCols, bool isMaxtree){
	this->numRows = numRows;
	this->numCols = numCols;
	this->maxtreeTreeType = isMaxtree;
		
	auto buf_input = input.request();
		
	int* img = (int *) buf_input.ptr;

	int n = this->numRows * this->numCols;
	//this->parent = new int[ n ];
	int* orderedPixels = countingSort(img);
	int* parent = createTreeByUnionFind(orderedPixels, img);
		
	std::unordered_map<int, NodeCT*> nodes;
	this->numNodes = 0;
	for (int i = 0; i < n; i++) {
		int p = orderedPixels[i];
		if (p == parent[p]) { //representante do node raiz
			this->root = nodes[p] = new NodeCT(this->numNodes++, p, nullptr, img[p]);
			this->listNodes.push_back(nodes[p]);
			nodes[p]->addCNPs(p);
		}
		else if (img[p] != img[parent[p]]) { //representante de um node
			nodes[p] = new NodeCT(this->numNodes++, p, nodes[parent[p]], img[p]);
			this->listNodes.push_back(nodes[p]);
			nodes[p]->addCNPs(p);
			nodes[parent[p]]->addChild(nodes[p]);
		}
		else if (img[p] == img[parent[p]]) {
			nodes[parent[p]]->addCNPs(p);
		}
	}

	delete[] parent;
	delete[] orderedPixels;
}

/*
py::array_t<int> ComponentTree::getParent(){
		
	int n = this->numRows * this->numCols;
		
	auto parent_numpy = py::array(py::buffer_info(
		this->parent,            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(int),     // Size of one item 
		py::format_descriptor<int>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( n ) },  // Number of elements for each dimension 
		{ sizeof(int) }  // Strides for each dimension 
	));

	return parent_numpy;
}

int* ComponentTree::getInputImage(){
	return this->img;
}*/
	
NodeCT* ComponentTree::getRoot() {
	return this->root;
}

bool ComponentTree::isMaxtree(){
	return this->maxtreeTreeType;
}

std::list<NodeCT*> ComponentTree::getListNodes(){
	return this->listNodes;
}

int ComponentTree::getNumNodes(){
	return this->numNodes;
}

int ComponentTree::getNumRowsOfImage(){
	return this->numRows;
}

int ComponentTree::getNumColsOfImage(){
	return this->numCols;
}


py::array_t<int> ComponentTree::reconstructionImage(){
	int n = this->numRows * this->numCols;
	auto img_numpy = py::array(py::buffer_info(
			nullptr,            /* Pointer to data (nullptr -> ask NumPy to allocate!) */
			sizeof(int),     /* Size of one item */
			py::format_descriptor<int>::value, /* Buffer format */
			1,          /* How many dimensions? */
			{ ( n ) },  /* Number of elements for each dimension */
			{ sizeof(int) }  /* Strides for each dimension */
	));
	auto buf_img = img_numpy.request();
	int *imgOut = (int *) buf_img.ptr;
	this->reconstruction(this->root, imgOut);
	return img_numpy;

}

int* ComponentTree::getInputImage(){
	int n = this->numRows * this->numCols;
	int* img = new int[n];
	this->reconstruction(this->root, img);
	return img;
}
	
/*
int ComponentTree::coord2Index(int row, int col) {
	return (row * this->numCols) + col;
}


std::vector<int> ComponentTree::getAdjPixels(int index) {
	int row = index / this->numCols;
	int col = index % this->numCols;
	std::vector<int> listAdj(8);
	for (int l=row - 1; l <= row + 1; l++ )
		for(int c=col -1; c <= col + 1; c++)
			if (l >= 0 && c >= 0 && l < this->numRows && c < this->numCols) 
				listAdj.push_back( coord2Index(l, c) );
			
		
	return listAdj;
}*/	