
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
		if(this->descendants[nodeAsc->getIndex()] == nullptr)
			this->descendants[nodeAsc->getIndex()] = nodeDes;
		
		if( this->attribute[ this->descendants[nodeAsc->getIndex()]->getIndex() ] < this->attribute[ nodeDes->getIndex() ])
			this->descendants[nodeAsc->getIndex()] = nodeDes;
		
	}
	
	double ComputerMSER::getStability(NodeCT* node){
		return (this->attribute[this->getAscendant(node)->getIndex()] - this->attribute[this->getDescendant(node)->getIndex()]) / this->attribute[node->getIndex()]  ;
	}

	ComputerMSER::~ComputerMSER(){
		delete [] this->attribute;
	}

	ComputerMSER::ComputerMSER(ComponentTree* tree){
		this->tree = tree;
		this->maxVariation = 10.0;
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
		
	}

	std::vector<bool> ComputerMSER::computerMSER(int delta){

		std::vector<NodeCT*> tmp_asc (this->tree->getNumNodes(), nullptr);
		this->ascendants = tmp_asc;

		std::vector<NodeCT*> tmp_des (this->tree->getNumNodes(), nullptr);
		this->descendants = tmp_des;

		std::vector<double> tmp_stab (this->tree->getNumNodes(), UNDEF);
		this->stability = tmp_stab;
		
		for(NodeCT *node: tree->getListNodes()){
			NodeCT *nodeAsc = this->getNodeAscendant(node, delta);
			this->maxAreaDescendants(nodeAsc, node);
			this->ascendants[node->getIndex()] = nodeAsc;
		}
		
		for(NodeCT *node: tree->getListNodes()){
			if(this->ascendants[node->getIndex()] != nullptr && this->descendants[node->getIndex()] != nullptr){
				this->stability[node->getIndex()] = this->getStability(node);
			}
		}
		
		this->num = 0;
		double maxStabilityDesc, maxStabilityAsc;
		std::vector<bool> mser(this->tree->getNumNodes(), false);
		for(NodeCT *node: tree->getListNodes()){
			if(this->stability[node->getIndex()] != UNDEF && this->stability[this->getAscendant(node)->getIndex()] != UNDEF && this->stability[this->getDescendant(node)->getIndex()] != UNDEF){
				maxStabilityDesc = this->stability[this->getDescendant(node)->getIndex()];
				maxStabilityAsc = this->stability[this->getAscendant(node)->getIndex()];
				if(this->stability[node->getIndex()] < maxStabilityDesc && this->stability[node->getIndex()] < maxStabilityAsc){
					if(stability[node->getIndex()] < this->maxVariation && this->attribute[node->getIndex()] >= minArea){//} && this->attribute[node->getIndex()] <= maxArea){
						mser[node->getIndex()] = true;
						this->num++;
					}
				}
			}
		}
		return mser;
	}

	NodeCT* ComputerMSER::getNodeInPathWithMaxStability(NodeCT* node, std::vector<bool> isMSER){
		NodeCT* nodeAsc = this->ascendantWithMaxStability(node);
		NodeCT* nodeDes = this->descendantWithMaxStability(node);
		NodeCT* nodeMax = node;


		double max = stability[node->getIndex()];
        double maxDesc = stability[nodeDes->getIndex()];
        double maxAnc = stability[nodeAsc->getIndex()];
                    
                    if(max <= maxDesc && max <= maxAnc) {
                        return node;
                    }else if (maxDesc <= maxAnc) {
                        return nodeDes;
                    }else {
                       return nodeAsc;
                    }
		
	}


	NodeCT* ComputerMSER::descendantWithMaxStability(NodeCT* node) {
		return this->descendants[node->getIndex()];
	}
	
	std::vector<double> ComputerMSER::getStabilities(){
		return this->stability;
	}

    NodeCT* ComputerMSER::ascendantWithMaxStability(NodeCT* node) {
		return this->ascendants[node->getIndex()];
	}

	int ComputerMSER::getNumNodes() {
		return  num;
	}

	std::vector<NodeCT*> ComputerMSER::getAscendants(){
		return this->ascendants;
	}

	NodeCT* ComputerMSER::getAscendant(NodeCT* node){
		return this->ascendants[node->getIndex()];
	}
	
	NodeCT* ComputerMSER::getDescendant(NodeCT* node){
		return this->descendants[node->getIndex()];
	}

	std::vector<NodeCT*> ComputerMSER::getDescendants(){
		return this->descendants;
	}