
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
			if(n->getParent() != nullptr)
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
		if(this->ascendants[node->getIndex()] != -1 && this->descendants[node->getIndex()] != -1){
			return (this->attribute[ascendants[node->getIndex()]] - this->attribute[this->descendants[node->getIndex()]]  ) / ( (double) this->attribute[node->getIndex()]);
		}
		else{
			return INT_MAX;
		}
	}

	ComputerMSER::~ComputerMSER(){
		delete [] this->attribute;
	}

	ComputerMSER::ComputerMSER(ComponentTree* tree){
		this->tree = tree;
		this->maxVariation = 0.5;
		this->minArea = 0;
		this->maxArea = INT_MAX;

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
		
	}

	std::vector<bool> ComputerMSER::computerMSER(int delta){

		std::vector<int> tmp_asc (this->tree->getNumNodes(), -1);
		this->ascendants = tmp_asc;

		std::vector<int> tmp_des (this->tree->getNumNodes(), -1);
		this->descendants = tmp_des;

		std::vector<double> tmp_stab (this->tree->getNumNodes(), -1);
		this->stability = tmp_stab;
		
		for(NodeCT *node: tree->getListNodes()){
			NodeCT *nodeAsc = this->getNodeAscendant(node, delta);
			this->maxAreaDescendants(nodeAsc, node);
			this->ascendants[node->getIndex()] = nodeAsc->getIndex();
		}
		
		for(NodeCT *node: tree->getListNodes()){
			if(this->ascendants[node->getIndex()] != -1 && this->descendants[node->getIndex()] != -1){
				this->stability[node->getIndex()] = this->getStability(node);
			}else{
				this->stability[node->getIndex()] = INT_MAX;
			}
			
		}
		
		this->num = 0;
		double minStabilityDesc, minStabilityAsc;
		std::vector<bool> mser(this->tree->getNumNodes(), false);
		for(NodeCT *node: tree->getListNodes()){
			if(this->stability[node->getIndex()] != INT_MAX && this->stability[this->ascendants[node->getIndex()] ] != INT_MAX && this->stability[this->descendants[node->getIndex()]] != INT_MAX){
				minStabilityDesc = this->stability[this->descendants[node->getIndex()]];
				minStabilityAsc = this->stability[this->ascendants[node->getIndex()]];
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
		return this->descendants[node->getIndex()];
	}
	
	std::vector<double> ComputerMSER::getStabilities(){
		return this->stability;
	}

    int ComputerMSER::ascendantNodeWithMinStability(NodeCT* node) {
		return this->ascendants[node->getIndex()];
	}

	int ComputerMSER::getNumNodes() {
		return  num;
	}

	std::vector<int> ComputerMSER::getAscendants(){
		return this->ascendants;
	}

	std::vector<int> ComputerMSER::getDescendants(){
		return this->descendants;
	}