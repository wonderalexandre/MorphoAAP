#include <list>
#include <vector>
#include <stack>
#include <unordered_map>

#include "../include/AttributeProfile.hpp"
#include "../include/ComponentTree.hpp"
#include "../include/NodeCT.hpp"
#include "../include/AttributeComputedIncrementally.hpp"
#include "../include/AttributeFilters.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

AttributeProfile::AttributeProfile(py::array_t<int> &input, int numRows, int numCols){
		
	this->numRows = numRows;
	this->numCols = numCols;
	this->maxtree = new ComponentTree(input, numRows, numCols, true);
	this->mintree = new ComponentTree(input, numRows, numCols, false);

}

py::array_t<double> AttributeProfile::getAP(std::list<double> thresholds, int attributeType){
		

	int n = this->numRows * this->numCols;
	int ap_size = (2*thresholds.size()+1);
	auto ap_numpy = py::array(py::buffer_info(
			nullptr,            /* Pointer to data (nullptr -> ask NumPy to allocate!) */
			sizeof(int),     /* Size of one item */
			py::format_descriptor<int>::value, /* Buffer format */
			2,          /* How many dimensions? */
			{ n, ap_size  },  /* Number of elements for each dimension */
			{ sizeof(int), sizeof(int) * n  }  /* Strides for each dimension */
	));
	if(attributeType != 0){
		std::cout << "attribute type not implemented. At the moment only type (attributeType = 0) of area is available";
        return ap_numpy;
	}

	double *attributeMintree = new double[this->mintree->getNumNodes()];
	double *attributeMaxtree = new double[this->maxtree->getNumNodes()]; 
	
	AttributeComputedIncrementally::computerAttribute(this->mintree->getRoot(),
						[&attributeMintree](NodeCT* node) -> void {
							attributeMintree[node->getIndex()] = node->getCNPs().size();
						},
						[&attributeMintree](NodeCT* root, NodeCT* child) -> void {
							attributeMintree[root->getIndex()] += attributeMintree[child->getIndex()];
						},
						[](NodeCT* node) -> void { 							
						});

	AttributeComputedIncrementally::computerAttribute(this->maxtree->getRoot(),
						[&attributeMaxtree](NodeCT* node) -> void {
							attributeMaxtree[node->getIndex()] = node->getCNPs().size();
						},
						[&attributeMaxtree](NodeCT* root, NodeCT* child) -> void {
							attributeMaxtree[root->getIndex()] += attributeMaxtree[child->getIndex()];
						},
						[](NodeCT* node) -> void { 							
						});


	auto buf_ap = ap_numpy.request();
	int *ap = (int *) buf_ap.ptr;
	int *img = this->mintree->getInputImage();
	
	int mid_offset = int(ap_size/2) * n;
	for (int p=0; p < n; p++){	
		ap[p + mid_offset] = img[p];	
	}
	int offset = mid_offset - n;
	mid_offset += n;
	
	for(double threshold: thresholds){
		AttributeFilters::prunningMin(this->maxtree, attributeMaxtree, threshold, &ap[mid_offset]);
		AttributeFilters::prunningMin(this->mintree, attributeMintree, threshold, &ap[offset]);
		mid_offset += n;
		offset -= n;
	}
		
	return ap_numpy;

}
