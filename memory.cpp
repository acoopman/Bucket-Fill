/** ***************************************************************************
 * @file
 *
 * @brief contains the source code for functions that does any dynamic 
 memory allocation or freeing up the allocated memory
 *****************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function allocates memory for an array of any specified size.
 *
 * @param[in]      rows - rows that is requested to be allocated
 * @param[in]      cols - columns that is requested to be allocated
 *
 * @return returns the allocated array
 *****************************************************************************/
pixel** alloc2d(int rows, int cols)
{
    //create new array
    pixel** array2D = nullptr;

    //set new array
    array2D = new pixel * [rows];

    //check to make sure array is not null
    if (array2D == nullptr)
    {
        return nullptr;
    }

    //allocate memory for the array until rows is reached
    for (int i = 0; i < rows; i++)
    {
        array2D[i] = nullptr;
        array2D[i] = new pixel[cols];
        if (array2D[i] == nullptr)
            return nullptr;
    }

    //return the allocated array
    return array2D;
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function frees up the memory created by dynamicly allocated memory.
 * Starts at the ptr, deleting and traversing throught the array until it
 * reaches the end
 * 
 * @param[in]      ptr - ptr to the start of the array
 * @param[in]      rows - rows that is requested to be allocated
 *****************************************************************************/
void free2d(pixel**& ptr, int rows)
{
    //check if ptr == nullptr
    if (ptr == nullptr)
        return;

    //go throught the rows deleting as it goes until EOF
    for (int i = 0; i < rows; i++)
    {
        if (ptr[i] != nullptr)
            delete[] ptr[i];
    }

    //if the ptr is null ptr, delete the ptr
    if (ptr != nullptr)
        delete[] ptr;
}