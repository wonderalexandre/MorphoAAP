
#include "include/NodeCT.hpp"
#include "include/ComponentTree.hpp"
#include "include/AttributeProfile.hpp"
#include "include/AttributeComputedIncrementally.hpp"
#include "include/AttributeFilters.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


void init_NodeCT(py::module &m){
    py::class_<NodeCT>(m, "NodeCT")
		.def(py::init<>())
		.def_property_readonly("rep", &NodeCT::getRep )
		.def_property_readonly("cnps", &NodeCT::getCNPs )
		.def_property_readonly("level", &NodeCT::getLevel )
		.def_property_readonly("children", &NodeCT::getChildren )
		.def_property_readonly("parent", &NodeCT::getParent );
}
void init_ComponentTree(py::module &m){
      py::class_<ComponentTree>(m, "ComponentTree")
        .def(py::init<py::array_t<int> &, int,int, bool>())
        .def("reconstructionImage", &ComponentTree::reconstructionImage )
		.def_property_readonly("root", &ComponentTree::getRoot )
		.def_property_readonly("parent", &ComponentTree::getParent );

        //.def("prunningMin", py::overload_cast<py::array_t<double> &, double>(&ComponentTree::prunningMin))
        //.def("prunningMin", &ComponentTree::prunningMin)
        //.def("computerArea", &ComponentTree::computerArea)
        //.def("prunningMin", py::overload_cast<py::array_t<double> &, double>(&ComponentTree::prunningMin))
}

void init_AttributeProfile(py::module &m){
    	py::class_<AttributeProfile>(m, "AttributeProfile")
        .def(py::init<py::array_t<int> &, int,int>())
        .def("getAP", &AttributeProfile::getAP );
}

void init_AttributeComputedIncrementally(py::module &m){
    	py::class_<AttributeComputedIncrementally>(m, "Attribute")
        //.def_static("computerAttribute", &AttributeComputedIncrementally::computerAttribute)
        .def_static("computerArea", &AttributeComputedIncrementally::computerArea);
}

void init_AttributeFilters(py::module &m){
    py::class_<AttributeFilters>(m, "AttributeFilters")
    .def(py::init<ComponentTree *>())
    .def("prunningMin", py::overload_cast<py::array_t<double> &, double>(&AttributeFilters::prunningMin));
}


PYBIND11_MODULE(morphoaap, m) {
    // Optional docstring
    m.doc() = "Adative attribute profiles";
    
    init_NodeCT(m);
    init_ComponentTree(m);
    init_AttributeProfile(m);
    init_AttributeComputedIncrementally(m);
    init_AttributeFilters(m);
}
