#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#ifndef ATTRIBUTE_COMPUTED_INCREMENTALLY_H
#define ATTRIBUTE_COMPUTED_INCREMENTALLY_H

class AttributeComputedIncrementally{

public:
    virtual void preProcessing(NodeCT *v);

    virtual void mergeChildren(NodeCT *parent, NodeCT *child);

    virtual void postProcessing(NodeCT *parent);

    void computerAttribute(NodeCT *root);

	static void computerAttribute(NodeCT* root, 
										std::function<void(NodeCT*)> preProcessing,
										std::function<void(NodeCT*, NodeCT*)> mergeChildren,
										std::function<void(NodeCT*)> postProcessing ){
		
		preProcessing(root);
			
		for(NodeCT *child: root->getChildren()){
			AttributeComputedIncrementally::computerAttribute(child, preProcessing, mergeChildren, postProcessing);
			mergeChildren(root, child);
		}

		postProcessing(root);
	}

	/*
	type=0 => area
	type=1 => diagonal
	type=2 => width
	type=3 => height
	type=4 => volume
	*/
	 static py::array_t<double> computerAttributes(ComponentTree *tree, int type){
		const int n = tree->getNumNodes();
		double *attrs = new double[n];
		if(type == 0){
			AttributeComputedIncrementally::computerArea(tree, attrs);
		}else if(type >= 1 && type <= 3){
			AttributeComputedIncrementally::computerBoundingBoxAttribute(tree, attrs, type);
		}else if(type == 4){
			AttributeComputedIncrementally::computerVolume(tree, attrs);
		}
		
	    py::array_t<double> numpy = py::array(py::buffer_info(
			attrs,            
			sizeof(double),     
			py::format_descriptor<double>::value, 
			1,         
			{  n }, 
			{ sizeof(double) }
	    ));
		return numpy;

	 }

    static void computerArea(ComponentTree *tree, double *attribute){
	    AttributeComputedIncrementally::computerAttribute(tree->getRoot(),
						[&attribute](NodeCT* node) -> void {
							attribute[node->getIndex()] = node->getCNPs().size();
						},
						[&attribute](NodeCT* root, NodeCT* child) -> void {
							attribute[root->getIndex()] += attribute[child->getIndex()];
						},
						[](NodeCT* node) -> void {
									
					});
    }

    static void computerVolume(ComponentTree *tree, double *attribute){
	    AttributeComputedIncrementally::computerAttribute(tree->getRoot(),
						[&attribute](NodeCT* node) -> void {
							attribute[node->getIndex()] = node->getCNPs().size() * node->getLevel();
						},
						[&attribute](NodeCT* root, NodeCT* child) -> void {
							attribute[root->getIndex()] += attribute[child->getIndex()];
						},
						[](NodeCT* node) -> void {
									
					});
    }


	/*
	type=1 => diagonal
	type=2 => width
	type=3 => height
	*/
	static void computerBoundingBoxAttribute(ComponentTree *tree, double *attribute, int type){
		int xmax[tree->getNumNodes()]; //min value
		int ymax[tree->getNumNodes()]; //min value
		int xmin[tree->getNumNodes()]; //max value
		int ymin[tree->getNumNodes()]; //max value
		int numCols = tree->getNumColsOfImage();
		int numRows = tree->getNumRowsOfImage();
	    AttributeComputedIncrementally::computerAttribute(tree->getRoot(),
						[&xmax, &ymax, &xmin, &ymin, numCols, numRows](NodeCT* node) -> void {
							xmax[node->getIndex()] = 0;
							ymax[node->getIndex()] = 0;
							xmin[node->getIndex()] = numCols;
							ymin[node->getIndex()] = numRows;
							for(int p: node->getCNPs()) {
								int x = p % numCols;
								int y = p / numCols;
								xmin[node->getIndex()] = std::min(xmin[node->getIndex()], x);
								ymin[node->getIndex()] = std::min(ymin[node->getIndex()], y);
								xmax[node->getIndex()] = std::max(xmax[node->getIndex()], x);
								ymax[node->getIndex()] = std::max(ymax[node->getIndex()], y);
							}
						},
						[&xmax, &ymax, &xmin, &ymin](NodeCT* parent, NodeCT* child) -> void {
							ymax[parent->getIndex()] = std::max(ymax[parent->getIndex()], ymax[child->getIndex()]);
							xmax[parent->getIndex()] = std::max(xmax[parent->getIndex()], xmax[child->getIndex()]);
							ymin[parent->getIndex()] = std::min(ymin[parent->getIndex()], ymin[child->getIndex()]);
							xmin[parent->getIndex()] = std::min(xmin[parent->getIndex()], xmin[child->getIndex()]);
						},
						[&attribute, &xmax, &ymax, &xmin, &ymin, type](NodeCT* node) -> void {
							//P1 = (xmin, ymin), P2 = (xmin, ymax), P3=(xmax, ymin), P4=(xmax, ymax)
							if(type == 1)
								attribute[node->getIndex()] = std::sqrt( std::pow(xmax[node->getIndex()]-xmin[node->getIndex()], 2) + std::pow(ymax[node->getIndex()]-ymin[node->getIndex()], 2) );
							else if(type == 2)
								attribute[node->getIndex()] = xmax[node->getIndex()] - xmin[node->getIndex()] + 1;	
							else if(type == 3)
								attribute[node->getIndex()] = ymax[node->getIndex()] - ymin[node->getIndex()] + 1;		
						});
   		}

};

#endif 