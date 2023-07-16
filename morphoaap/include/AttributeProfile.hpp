#include <list>


#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "../include/ComponentTree.hpp"
#include "../include/AttributeFilters.hpp"


#ifndef ATTRIBUTE_PROFILE_H
#define ATTRIBUTE_PROFILE_H

namespace py = pybind11;


class AttributeProfile{
private:
	int numRows;
	int numCols;
	ComponentTree *maxtree;
	ComponentTree *mintree;
	

public:
	AttributeProfile(py::array_t<int> &input, int numRows, int numCols);

	py::array_t<double> getAP(std::list<double> thresholds, int attributeType);

};

#endif
