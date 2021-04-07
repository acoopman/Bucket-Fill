/** ***************************************************************************
 * @file
 *
 * @brief contains the main function and the functions for finding the
 * starting color and filling color
 *****************************************************************************/
#include "netPBM.h"

using namespace std;


/** ***************************************************************************
  * @mainpage Program 3 - Flood Fill
  *
  * @section course_section Course Information
  *
  * @authors Aidan Coopman
  *
  * @date  November 22, 2020
  *
  * @par Instructor
  *         Professor Roger Schrader
  *
  * @par Course
  *         CSC 215 - Section ? - 9:00 am
  *
  * @par Location
  *          - Room
  *
  * @section program_section Program Information
  *
  * @details
  * 
  * This program takes in a user inputed starting row and column, along with
  * red, green and blue values 0-255. It will then go to the user stated
  * starting position, record the color, and change that color and every
  * other color of the same type that are touching. This is the same thing
  * as bucket fill on many editing programs such as gimp.
  *
  * This program starts by finding the magic number, then copying and
  * skiping over the comment, then taking in the number of rows and columns
  * in the image, then finally taking in the maximum pixel value which
  * leaves it pointing now to the actual RGB pixel values.
  *
  * Now the rgb data will be inputed in two different ways based on the
  * inputted image's magic number. If the inputted image is in ASCII format
  * it will be read into the struct line by line. If the format is in
  * BINARY it will read the pixel values into the struct as well.
  *
  * After the image is read in it will be closed and reopened as 
  * a file out so it can be written to. It will then run the get_color
  * function to get the starting color that will be replaced, then it
  * will call the fill function and it will fill up the specified area
  * 
  * The program will return 0.

  *
  * @section compile_section Compiling and Usage
  *
  * @par Compiling Instructions:
  *      Set the stack reserve and commit size to 4 billion
  *
  * @par Usage
    @verbatim
    c:\> prog2.exe inputFile.exe row column red green blue
        row    - starting row
        column - starting column
        red    - red value that will be filled (0-255)
        green  - green value that will be filled (0-255)
        blue   - blue value that will be filled (0-255)

    @endverbatim
  *
  * @par Modifications and Development Timeline:
    @verbatim

  * @par Modifications and Development Timeline:
  *     <a href="https://gitlab.mcs.sdsmt.edu/101067892/csc215f20programs/-/tree/master/prog3" target=_blank>
  *     Gitlab Commit Log</a>
  *
  ****************************************************************************/
int main(int argc, char** argv)
{
    //struct that has image information 
    //parsed from ppm file and pointers to rgb 
    image info;
 
    //command line arguments
    string input_name;
    int    starting_row;
    int    starting_col;
    pixel  red_value;
    pixel  green_value;
    pixel  blue_value;

    //check to make sure there is enough arguments
    if (argc != 7)
    {
        cout << "Usage: prog3.exe input_image.ppm [row] [column] ";
        cout << "[red value] [green value] [blue value]" << endl;
        return 1;
    }

    //copy the data from command line into variables
    input_name   = argv[1];
    starting_row = stoi(argv[2]);
    starting_col = stoi(argv[3]);
    red_value    = stoi(argv[4]);
    green_value  = stoi(argv[5]);
    blue_value   = stoi(argv[6]);

    // parse the input file
    // returns 0 if no error occurred otherwise returns 1
    // memory for rgb is allocated in this function
    if(parse_ppm(input_name, info))
    {
        cout << "An error occurred while parsing ppm file\n";
        return 1;
    }

    // Fill the region the user requested
    // starting row and col are within boundaries and are not checked
    fill_region(info, starting_row, starting_col, 
        red_value, green_value, blue_value);

    // Replace the input ppm file with the new image with filled region
    // returns 0 if no error occurred otherwise returns 1
    // memory for rgb is deallocated in this function
    if (write_ppm(input_name, info))
    {
        cout << "An error occurred while writing ppm file\n";
        return 1;
    }
    
    return 0;
}//main



/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function gets the color at the starting location inputted by the user.
 * It does this by finding each pixel value at the starting point. Then
 * it compares it to the inputted color and if they are the same it exits
 * the progam, otherwise if the colors are different it will call the 
 * fill function
 *
 *
 *
 * @param[in,out]      info - the main image struct
 * @param[in]      row - the starting row
 * @param[in]      col - the starting column
 * @param[in]      red - the red value that the user inputted
 * @param[in]      green - the green value that the user inputted
 * @param[in]      blue - the blue value that the user inputted
 *****************************************************************************/
void fill_region(image& info, int row, int col, 
    pixel red, pixel green, pixel blue)
{
    //set the current colors
    pixel red_current = info.red[row][col];
    pixel green_current = info.green[row][col];
    pixel blue_current = info.blue[row][col];

    //check to make sure starting position color 
    //is not the same as the inputted color
    //if it is the same color, do nothing
    if (info.red[row][col] != red
        || info.green[row][col] != green
        || info.blue[row][col] != blue)
    {
        fill(info, row, col, red_current, green_current, 
            blue_current, red, green, blue);
    }
    else
    {
        cout << "Starting position color is the same color ";
        cout << "as inputed color so nothing will be done\n";
    }

}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function fills an area of the same color. It does this by recusivly
 * calling itslf and chnaging the pixels until it hits a edge, or it finds 
 * a pixel that is of a different color.
 * 
 *
 *
 *
 * @param[in,out]      info - the main image struct
 * @param[in]      row - the starting row
 * @param[in]      col - the starting column
 * @param[in]      red_current - the red value that is being replaced
 * @param[in]      green_current - the green value that is being replaced
 * @param[in]      blue_current - the blue value that is being replaced
 * @param[in]      new_red - the red value that the user inputted
 * @param[in]      new_green - the green value that the user inputted
 * @param[in]      new_blue - the blue value that the user inputted
 *****************************************************************************/
void fill(image& info, int row, int col, pixel red_current, 
    pixel green_current, pixel blue_current, pixel new_red, 
    pixel new_green, pixel new_blue)
{
    //make sure it stays in boundries, if it hits an edge then return
    if (row < 0 || row >= info.rows || col < 0 || col >= info.cols)
        return;

    //if it hits a different color then return
    if (info.red[row][col] != red_current 
        || info.green[row][col] != green_current 
        || info.blue[row][col] != blue_current)
        return;

    //replace color at row, col
    info.red[row][col] = new_red;
    info.green[row][col] = new_green;
    info.blue[row][col] = new_blue;

    //recursivly call in all directions, row+1, row-1, col+1, col-1
    fill(info, row + 1, col, red_current, green_current, 
        blue_current, new_red, new_green, new_blue);
    fill(info, row - 1, col, red_current, green_current, 
        blue_current, new_red, new_green, new_blue);
    fill(info, row, col + 1, red_current, green_current, 
        blue_current, new_red, new_green, new_blue);
    fill(info, row, col - 1, red_current, green_current, 
        blue_current, new_red, new_green, new_blue);
}
