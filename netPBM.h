/** ***************************************************************************
 * @file
 *
 * @brief header file for the entire project
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std; 
#ifndef  __NETPBM__H__ 
#define  __NETPBM__H__ 

/** ***************************************************************************
* typedef named pixel, unsigned char
******************************************************************************/
typedef unsigned char pixel; 

/** ***************************************************************************
* struct named image. Main struct used in the program.
* Contains image rows and columns, 2D arrays for RGB, comment line,
* max pixel value, and the magic number
******************************************************************************/
struct image 
{ 
    // information read from ppm file
    int rows;     
    int cols;     
    pixel **red; 
    pixel **green;     
    pixel **blue; 

    // file information for ppm format
    string comment_line;
    int    max_pixel_value;
    string input_type;
}; 



//Image Operations*************************************************************

/** ***************************************************************************
* goes to the starting point and gets the color, then calls the fill function
******************************************************************************/
void fill_region(image& info, int row, int col, 
    pixel red, pixel green, pixel blue);

/** ***************************************************************************
* fills the selected area with the inputed color
******************************************************************************/
void fill(image& info, int row, int col, pixel red_current, pixel green_current,
    pixel blue_current, pixel new_red, pixel new_green, pixel new_blue);


//Memory***********************************************************************

/** ***************************************************************************
* allocates the memory for the 2D arrays
******************************************************************************/
pixel** alloc2d(int rows, int cols); 

/** ***************************************************************************
* frees up the memory that was allocated
******************************************************************************/
void free2d(pixel**& ptr, int rows); 


//Image File IO****************************************************************

/** ***************************************************************************
* parses the inputted ppm file
* returns 0 if no error occurred, returns 1 if an error occured
* Allocates memory for rgb in struct info
******************************************************************************/
int parse_ppm(string filename, image& info);


/** ***************************************************************************
* Writes a ppm file. Returns 0 if no error occurred, returns 1 if 
* an error occured.
* Deallocates memory for rgb in struct info
******************************************************************************/
int write_ppm(string filename, image& info);

/** ***************************************************************************
* inputes the data to the 2D arrays for both P6 and P3
******************************************************************************/
void input_data(image& info, ifstream& in);

/** ***************************************************************************
* outputs the data in the specified format
******************************************************************************/
void output_data(image& info, ofstream& out); 


//other************************************************************************

/** ***************************************************************************
* grabs the comment line
******************************************************************************/
void get_comment(ifstream& f); 

#endif