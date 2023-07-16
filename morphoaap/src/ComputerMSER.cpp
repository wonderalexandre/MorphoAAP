
#include <list>
#include <vector>
#include <stack>
#include <limits.h>

#include "../include/NodeCT.hpp"
#include "../include/ComponentTree.hpp"
#include "../include/ComputerMSER.hpp"
#include "../include/AttributeComputedIncrementally.hpp"



    NodeCT* ComputerMSER::getNodeAscendant(NodeCT* node, int h){
		NodeCT* n = node;
		for(int i=0; i <= h; i++){
			if(this->tree->isMaxtree()){
				if(node->getLevel() >= n->getLevel() + h)
					return n;
			}else{
				if(node->getLevel() <= n->getLevel() - h)
					return n;
			}
			if(n->getParent() != this->tree->getRoot())
				n = n->getParent();
			else 
				return n;
		}
		return n;
	}

	void ComputerMSER::maxAreaDescendants(NodeCT* nodeAsc, NodeCT* nodeDes){
		if(this->descendants[nodeAsc->getIndex()] == -1)
			this->descendants[nodeAsc->getIndex()] = nodeDes->getIndex();
		
		if( this->attribute[ this->descendants[nodeAsc->getIndex()] ] < this->attribute[ nodeDes->getIndex() ])
			this->descendants[nodeAsc->getIndex()] = nodeDes->getIndex();
		
	}
	
	double ComputerMSER::getStability(NodeCT* node){
		if(this->ascendant[node->getIndex()] != -1 && this->descendants[node->getIndex()] != -1){
			return ( this->attribute[ascendant[node->getIndex()]] - this->attribute[this->descendants[node->getIndex()]]) / double(this->attribute[node->getIndex()]);
		}
		else{
			return INT_MAX;
		}
	}


	ComputerMSER::ComputerMSER(ComponentTree* tree){
		this->tree = tree;
		this->maxVariation = 0.5;
		this->minArea = 0;

		double *_attribute = new double[this->tree->getNumNodes()]; 
		AttributeComputedIncrementally::computerAttribute(this->tree->getRoot(),
						[&_attribute](NodeCT* node) -> void {
							_attribute[node->getIndex()] = node->getCNPs().size();
						},
						[&_attribute](NodeCT* root, NodeCT* child) -> void {
							_attribute[root->getIndex()] += _attribute[child->getIndex()];
						},
						[](NodeCT* node) -> void { 							
						});

		this->attribute = _attribute;
		this->maxArea = attribute[tree->getRoot()->getIndex()];
	}

	bool* ComputerMSER::computerMSER(int delta){
		this->ascendant = new int[this->tree->getNumNodes()];
		this->descendants = new int[this->tree->getNumNodes()];
		bool *mser = new bool[this->tree->getNumNodes()];
		this->num = 0;
		for(int i=0; i < this->tree->getNumNodes(); i++){
			this->ascendant[i] = -1;
			this->descendants[i] = -1;
			mser[i] = false;
		}

		for(NodeCT *node: tree->getListNodes()){
			NodeCT *nodeAsc = this->getNodeAscendant(node, delta);
			this->maxAreaDescendants(nodeAsc, node);
			this->ascendant[node->getIndex()] = nodeAsc->getIndex();
		}

		this->stability = new double[this->tree->getNumNodes()];
		for(NodeCT *node: tree->getListNodes()){
			if(this->ascendant[node->getIndex()] != -1 && this->descendants[node->getIndex()] != -1){
				this->stability[node->getIndex()] = this->getStability(node);
			}else{
				this->stability[node->getIndex()] = INT_MAX;
			}
			
		}


		for(NodeCT *node: tree->getListNodes()){
			if(this->stability[node->getIndex()] != -1 && this->stability[this->ascendant[node->getIndex()] ] != -1 && this->stability[this->descendants[node->getIndex()]] != -1){
				double minStabilityDesc = this->stability[this->descendants[node->getIndex()]];
				double minStabilityAsc = this->stability[this->ascendant[node->getIndex()]];
				if(this->stability[node->getIndex()] < minStabilityDesc && this->stability[node->getIndex()] < minStabilityAsc){
					if(stability[node->getIndex()] < maxVariation && this->attribute[node->getIndex()] >= minArea && this->attribute[node->getIndex()] <= maxArea){
						mser[node->getIndex()] = true;
						this->num++;
					}
				}
			}
		}

		return mser;
	}


	int ComputerMSER::descendantNodeWithMinStability(NodeCT* node) {
		if( this->stability[this->descendants[node->getIndex()] ] != -1)
			return this->descendants[node->getIndex()];
		else
			return -1;
	}
	
	double* ComputerMSER::getStabilities(){
		return this->stability;
	}

    int ComputerMSER::ascendantNodeWithMinStability(NodeCT* node) {
		if( this->stability[this->ascendant[node->getIndex()] ] != -1)
			return this->ascendant[node->getIndex()];
		else
			return -1;
	}

	int ComputerMSER::getNumNodes() {
		return  num;
	}

