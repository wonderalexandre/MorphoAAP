
#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"
#include "../include/ComputerMSER.hpp"

#include <stack>
#include <vector>
#include <limits.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#ifndef ATTRIBUTE_FILTERS_H
#define ATTRIBUTE_FILTERS_H

#define UNDEF -999999999999

class AttributeFilters{
    private:
        ComponentTree *tree;

    public:

    AttributeFilters(ComponentTree *tree);

    py::array_t<int> prunningMin(py::array_t<double> &attr, double threshold);

    py::array_t<int> prunningMinByAdaptativeThreshold(py::array_t<double> &attr, double threshold, int delta);

    static void prunningMin(ComponentTree *tree, double *attribute, double threshold, int *imgOutput){
        std::stack<NodeCT*> s;
        s.push(tree->getRoot());
        while(!s.empty()){
            NodeCT *node = s.top(); s.pop();
            for (int p : node->getCNPs()){
                imgOutput[p] = node->getLevel();
            }
            for (NodeCT *child: node->getChildren()){
                if(attribute[child->getIndex()] >= threshold){
                    s.push(child);
                }else{
                    std::stack<NodeCT*> stackSubtree;
                    stackSubtree.push(child);
                    while(!stackSubtree.empty()){
                        NodeCT *childChild = stackSubtree.top(); stackSubtree.pop();
                        for (int p : childChild->getCNPs()){
                            imgOutput[p] = node->getLevel();
                        }
                        for (NodeCT *childChildChild: childChild->getChildren())
                            stackSubtree.push(childChildChild);
                    }
                }
                
            }
        }
    }


    
    static void prunningMinByAdaptativeThreshold(ComponentTree *tree, double *attribute, double threshold, int delta, int *imgOutput){
		/*
        std::vector<bool> selectedPruned(tree->getNumNodes(), false); //nodes pruned
		for(NodeCT *node: tree->getListNodes()){
			if(node->getParent() != nullptr && attribute[node->getIndex()] < threshold){
				if ( attribute[node->getParent()->getIndex()] != attribute[node->getIndex()] ) {
					selectedPruned[node->getIndex()] = true;
				}
			}
		}*/
		
        ComputerMSER mser(tree);
		mser.computerMSER(delta);

		std::vector<double> stability = mser.getStabilities();
		std::vector<bool> isPruned(tree->getNumNodes(), false);
		for(NodeCT *node: tree->getListNodes()){
            //if(selectedPruned[node->getIndex()] && stability[node->getIndex()] != UNDEF && attribute[node->getIndex()] <= threshold){ //node pruned
            if(stability[node->getIndex()] != UNDEF && attribute[node->getIndex()] <= threshold){ //node pruned
				double min = stability[node->getIndex()];
				
				int indexDescMinStabilityDesc = mser.descendantNodeWithMinStability(node);
				int indexAncMinStabilityAsc = mser.ascendantNodeWithMinStability(node);
				double minDesc = INT_MIN;
				double minAnc =  INT_MIN;
				if(indexDescMinStabilityDesc != -1) {
					minDesc = stability[indexDescMinStabilityDesc];
				}
				if(indexAncMinStabilityAsc != -1) {
					minAnc = stability[indexAncMinStabilityAsc];
				}
				
				if(min >= minDesc && min >= minAnc) {
					isPruned[node->getIndex()] = true;
				}else if (minDesc >= min && minDesc >= minAnc) {
					isPruned[indexDescMinStabilityDesc] = true;
				}else {
					isPruned[indexAncMinStabilityAsc] = true;
				}
			}
			
		}
		
		std::stack<NodeCT*> s;
        s.push(tree->getRoot());
        while(!s.empty()){
            NodeCT *node = s.top(); s.pop();
            for (int p : node->getCNPs()){
                imgOutput[p] = node->getLevel();
            }
            for (NodeCT *child: node->getChildren()){
                if(!isPruned[child->getIndex()]){
                    s.push(child);
                }else{
                    std::stack<NodeCT*> stackSubtree;
                    stackSubtree.push(child);
                    while(!stackSubtree.empty()){
                        NodeCT *childChild = stackSubtree.top(); stackSubtree.pop();
                        for (int p : childChild->getCNPs()){
                            imgOutput[p] = node->getLevel();
                        }
                        for (NodeCT *childChildChild: childChild->getChildren())
                            stackSubtree.push(childChildChild);
                    }
                }
                
            }
        }
		
	}

};


#endif