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

    static py::array_t<double> computerArea(ComponentTree *tree){
	    double attribute[tree->getNumNodes()];
		
	    AttributeComputedIncrementally::computerAttribute(tree->getRoot(),
						[&attribute](NodeCT* node) -> void {
							attribute[node->getIndex()] = node->getCNPs().size();
						},
						[&attribute](NodeCT* root, NodeCT* child) -> void {
							attribute[root->getIndex()] += attribute[child->getIndex()];
						},
						[](NodeCT* node) -> void {
									
					});
	
	    return py::array(py::buffer_info(
			attribute,            
			sizeof(double),     
			py::format_descriptor<double>::value, 
			1,         
			{ ( tree->getNumNodes() ) }, 
			{ sizeof(double) }
	    ));
		
    }



};

#endif 