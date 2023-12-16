/*-------------------------------------------------
FILE: grid.h

AUTHOR: Viraj Saudagar
INSTITUTION: University of Illinois at Chicago
COURSE: CS 211 - Fall 2023

DESCRIPTION:
This class allows for the creation of 2D grids of
any type in the structure of heap allocated pointers
to a heap allocated array of the elements. Default
grids are 4x4 but any valid combination of row/column
size can be specified. These 2D grids will be used later 
for the creation of the mazes and labyrinths. 
-------------------------------------------------*/

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

// template allows for the array of columns within the class to contain elements of any type. 
template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows (0..NumRows-1)

  // Frees the allocated memory within the grid object
  void clear(){

      // traverse over the number of rows to access each column array. 
      // free the memory of each column array at each row.
      for(size_t r = 0; r < NumRows; r++){

          delete[] Rows[r].Cols;

      }

      // finally free the memory allocated for the row array itself. 
      delete[] Rows;

  }
    
public:
  // Default constructor -> will initialize the grid to be a 4x4 (4 rows x 4 cols)
  // sets each of the elements in the columns array to the default value of the type T of the object being created.
  Grid() {

    // Allocated an array of size 4 which the pointer to the start is stored in the Rows data member & NumRows is set to 4.
    Rows = new ROW[4];  
    NumRows = 4;

    // traverse through the 4 rows and allocate size 4 column array of elements of type T for each row. 
    // sets each element to their default value by calling the types "T" constructor.
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  
      }
    }
  }


  // Parameterized Constructor -> allows for custom creation of a grid with custom row & column values.
  // each element will still be initialized to default value of T. 
  Grid(size_t R, size_t C) {

    // Checks if Rows and Columns Provided are Valid.
    if(R <= 0 || C <= 0){
        throw invalid_argument("Grid Parameterized Constructor -> invalid row or column input");
    }
   
    // Allocates space for R rows within the Rows array & sets NumRows to R.
    Rows = new ROW[R];
    NumRows = R;

    // Traverses through the custom number of rows and for each row allocates a new 
    // array of column elements of type T which are initialized with default T value. 
    for(size_t r = 0; r < NumRows; r++){

        Rows[r].Cols = new T[C];
        Rows[r].NumCols = C;

        for(size_t c = 0; c < Rows[r].NumCols; c++){
            Rows[r].Cols[c] = T();
        }

    }

  }
    
  // Destructor -> will free all the heap allocated memory used in the class (object).
  virtual ~Grid() {
      // frees the allocated memory within the class
      clear();
  }



  // Copy Constructor -> creates a deep copy of the object, used when passing Grid objects
  // to other functions to ensure the local object does not point to same memory as original object.
  Grid(const Grid<T>& other) {
    
    // Frees the memory of what is current in the object
    clear();
      
    // Allocates memory for new grid Rows array based on copying object number of rows.
    Rows = new ROW[other.NumRows];
    NumRows = other.NumRows;

    for(size_t r = 0; r < NumRows; r++){

        // Allocates memory at each row for the array of elements (column) based on the object being copied. 
        Rows[r].Cols = new T[other.Rows[r].NumCols];
        Rows[r].NumCols = other.Rows[r].NumCols;

        // Traverses the column array and sets each element to corresponding element in the object being copied. 
        for(size_t c = 0; c < Rows[r].NumCols; c++){
            Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }

    }
  
  }
    

  // Asignment operator overload -> performs a deep copy of the object, not memberwise assignment. 
  Grid& operator=(const Grid& other) {

    // Ensures that the current object is simply not the same as the other object being copied. 
    if(this != &other){

        // first frees the heap allocated memory within the object as new memory will be used. 
        clear();

        // Allocate space for number of rows based on copied object and set NumRows to that value.  
        Rows = new ROW[other.NumRows];
        NumRows = other.NumRows;

        // Traverse the new number of rows and at each row allocate space for a new column while copying all elements from the column & updating NumCols. 
        for(size_t r = 0; r < NumRows; r++){

            Rows[r].Cols = new T[other.Rows[r].NumCols];
            Rows[r].NumCols = other.Rows[r].NumCols;

            for(size_t c = 0; c < Rows[r].NumCols; c++){
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }

        }

    }

    // Return the reference to this (implicity pointer to itself) obj which is the one where the new copy is stored. 
    return *this;

  }

  // Returns the number of rows in the grid. 
  size_t numrows() const {
      return NumRows;
  }
  

  // returns the number of columns in row R of the grid.
  size_t numcols(size_t r) const {
      return Rows[r].NumCols;
  }


  // Returns the total number elements in the grid object.
  size_t size() const {

      size_t sum = 0;

      // Traverses the number of rows in the grid and sums each rows number of columns
      for(size_t r = 0; r < NumRows; r++){
          sum += Rows[r].NumCols;
      }

    return sum;
  }


  // Parenthesis Operator overload -> allows passing row and column value as parameters 
  // and returns a reference to the element at that position if valid row & column
  // values are given. 
  T& operator()(size_t r, size_t c) {

    // Checks if row & column value provided are valid. 
    if(r < 0 || c < 0){
        throw invalid_argument("Grid () operator overload -> Invalid row or column argument provided");
    }
    else if((r > (NumRows-1)) || c > ((Rows[r].NumCols - 1))){
        throw invalid_argument("Grid () operator overload -> Invalid row or column argument provided");
    }
    else{
        // Row & Column are valid so return the reference to the element at that position. 
        return Rows[r].Cols[c];
    }


  }


};
