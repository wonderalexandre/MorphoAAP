#include "../include/Adjacency8.hpp"


Adjacency8::Adjacency8(int numRows, int numCols){
    this->numRows = numRows;
    this->numCols = numCols;
}

int Adjacency8::nextValid(){
    this->id += 1;
    while (this->id < 8){
        if (0 <= this->row + this->offsetRow[this->id] && this->row + this->offsetRow[this->id] < this->numRows && 0 <= this->col + this->offsetCol[this->id] && this->col + this->offsetCol[this->id] < this->numCols)
            break;
        this->id += 1;
    }
    return this->id;
} 

Adjacency8::IteratorAdjacency Adjacency8::begin() { 
    return IteratorAdjacency(*this, nextValid()); 
}

Adjacency8::IteratorAdjacency Adjacency8::end() { 
    return IteratorAdjacency(*this, 8); 
}

Adjacency8& Adjacency8::getAdjPixels(int row, int col){
    this->row = row;
    this->col = col;
    this->id = -1;
    return *this;
}

Adjacency8& Adjacency8::getAdjPixels(int indexVector){
    return getAdjPixels(indexVector / this->numCols, indexVector % this->numCols);
}
