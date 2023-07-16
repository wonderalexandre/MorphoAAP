
#include "../include/AttributeFilters.hpp"


    AttributeFilters::AttributeFilters(ComponentTree* tree){
        this->tree = tree;
    }

    py::array_t<int> AttributeFilters::prunningMin(py::array_t<double> &attr, double threshold){

        auto bufAttribute = attr.request();
        
        double *attribute = (double *) bufAttribute.ptr;

        int n = this->tree->getNumRowsOfImage() * this->tree->getNumColsOfImage();
        auto imgNumpy = py::array(py::buffer_info(
                nullptr,            
                sizeof(int),     
                py::format_descriptor<int>::value, 
                1,         
                { ( n ) }, 
                { sizeof(int) }
            ));
        auto bufImgOutput = imgNumpy.request();
        int *imgOutput = (int *) bufImgOutput.ptr;

        AttributeFilters::prunningMin(this->tree, attribute, threshold, imgOutput);

        return imgNumpy;

    }


    py::array_t<int> AttributeFilters::prunningMinByAdaptativeThreshold(py::array_t<double> &attr, double threshold, int delta){
        auto bufAttribute = attr.request();
        
        double *attribute = (double *) bufAttribute.ptr;

        int n = this->tree->getNumRowsOfImage() * this->tree->getNumColsOfImage();
        auto imgNumpy = py::array(py::buffer_info(
                nullptr,            
                sizeof(int),     
                py::format_descriptor<int>::value, 
                1,         
                { ( n ) }, 
                { sizeof(int) }
            ));
        auto bufImgOutput = imgNumpy.request();
        int *imgOutput = (int *) bufImgOutput.ptr;

        AttributeFilters::prunningMinByAdaptativeThreshold(this->tree, attribute, threshold, delta, imgOutput);

        return imgNumpy;
    }