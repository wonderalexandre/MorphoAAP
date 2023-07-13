
#include "../cpp/include/NodeCT.hpp"
#include "../cpp/include/ComponentTree.hpp"
#include <stack>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#ifndef ATTRIBUTE_FILTERS_H
#define ATTRIBUTE_FILTERS_H


class AttributeFilters{
    private:
        ComponentTree *tree;

    public:

    AttributeFilters(ComponentTree *tree);

    static void prunningMin(ComponentTree *tree, double *attribute, double threshold, int *imgOutput){
        std::stack<NodeCT*> s;
        s.push(tree->getRoot());
        while(!s.empty()){
            NodeCT *node = s.top(); s.pop();
            for (int p : node->getCNPs()){
                imgOutput[p] = node->getLevel();
            }
            for (NodeCT *child: node->getChildren()){
                if(attribute[child->getIndex()] > threshold){
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


    py::array_t<int> prunningMin(py::array_t<double> &attr, double threshold);

};


#endif