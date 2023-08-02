
#include <list>

#ifndef ADJACENCY_H
#define ADJACENCY_H

class Adjacency8 {
private:
    int id;
    
    int row;
    int col;    
    int numCols;
    int numRows;

    /*
    visitation order
    3 | 4 | 5
    2 | 1 | 6
    9 | 8 | 7    
    const int offsetRow[9] = {0, 0, -1, -1, -1, 0, 1, 1,  1};
    const int offsetCol[9] = {0,-1, -1,  0,  1, 1, 1, 0, -1};
   */   

    /*
    visitation order
    2 | 3 | 4
    1 | o | 5
    8 | 7 | 6    
    */
    const int offsetRow[8] = {0, -1, -1, -1, 0, 1, 1,  1};
    const int offsetCol[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
      

public:

    Adjacency8(int numCols, int numRows);
    int nextValid();
    Adjacency8& getAdjPixels(int row, int col);
    Adjacency8& getAdjPixels(int index);

    class IteratorAdjacency{ 
        private:
    	    int index;
            Adjacency8&  instance;
        public:
        	using iterator_category = std::input_iterator_tag;
            using value_type = int; 
            
            IteratorAdjacency(Adjacency8& obj, int id): instance(obj), index(id)  { }

            IteratorAdjacency& operator++() { 
                this->index = instance.nextValid(); return *this; 
            }
            bool operator==(IteratorAdjacency other) const { 
                return index == other.index; 
            }
            bool operator!=(IteratorAdjacency other) const { 
                return !(*this == other);
            }
            int operator*() const { 
                return (instance.row + instance.offsetRow[index]) * instance.numCols + (instance.col + instance.offsetCol[index]); 
            }    
    };
    IteratorAdjacency begin();
    IteratorAdjacency end();
    
};

#endif
