/** ***************************************************************************
 * @file
 *
 * @brief contains the source code for functions that does file IO
 * and opening, closing, and parsing files
 *****************************************************************************/
#include "netPBM.h"


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function parses an input ppm file
 *
 * Starts out by defining and opening an ifstream, then inputs the magic
 * number and checks the magic number. From there it checks for a comment
 * line and grabs it, then gets the rows, columns and max pixel value.
 * Then it allocates and checks for allocation and calls the input_data
 * function and closes the ifstream
 *
 * @param[in]      filename - the filenamed inputed thru command line arg
 * @param[in]      info - the main image struct
 * 
 * @return   returns 0 if no error occured, returns 1 if an error occured
 *****************************************************************************/
int parse_ppm(string filename, image& info)
{
    ifstream in;
    char temp;
    info.comment_line = "\0"; //set the comment line to null

    //open input file
    in.open(filename, ios::in | ios::binary);
 
    //check to make sure file opened
    if (!in.is_open())
    {
        std::cout << "Error while opening file '" << filename << "'\n";
        return 1;
    }
   
    //input magic number and check it
    in >> info.input_type;
    if (info.input_type != "P3" && info.input_type != "P6")
    {
        std::cout << "Invalid magic number " << info.input_type << endl;
        return 1;
    }

    //check if a comment line is present
    while (temp = in.peek(), temp == '\n')
        in.get();
    if (temp == '#')
    {
        //grab the comment line
        getline(in, info.comment_line);
    }

    //get the rows and cols for dynamic allocation
    in >> info.cols;
    in >> info.rows;

    //get the max pixel value
    in >> info.max_pixel_value;

    //allocate memory for red, green and red
    info.red = alloc2d(info.rows, info.cols);
    info.green = alloc2d(info.rows, info.cols);
    info.blue = alloc2d(info.rows, info.cols);

    //check to make sure allocation was sucessfull
    if (info.red == nullptr
        || info.green == nullptr
        || info.blue == nullptr)
    {
        cout << "Memory allocation failed. Exiting" << endl;
        return 1;
    }

    //input the data into the 2D arrays
    input_data(info, in);

    //close input file and open it as output file
    in.close();

    return 0; //no error occured
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function writes to a ppm file.
 *
 * Starts out by defining and opening an ofstream, then it opens the inputted
 * file and checks to make sure it opened. Next it checks the magic number
 * is P3 or P6 and then it calls the output_data function and frees the
 * allocated memory. Lastly it closes the ofstream.
 *
 * @param[in]      filename - the filenamed inputed thru command line arg
 * @param[in]      info - the main image struct
 * @return         returns 0 if no error occured, returns 1 if an error occured

 *****************************************************************************/
int write_ppm(string filename, image& info)
{
    ofstream out;
    out.open(filename, ios::out | ios::binary);

    // check to make sure file opened
    if (!out.is_open())
    {
        std::cout << "Error while opening file '" << filename << "'\n";
        return 1;
    }

    // return 1 if not open
    if (info.input_type != "P3" && info.input_type != "P6")
    {
        std::cout << "Invalid magic number " << info.input_type << endl;
        return 1;
    }

    //output the data
    output_data(info, out);

    //free the memory and close files
    free2d(info.red, info.rows);
    free2d(info.green, info.rows);
    free2d(info.blue, info.rows);

    //close output file
    out.close();

    return 0;
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function does all the inputing from the inputed file to the struct.
 *
 * First it determines what the inputed magic type was, if it is P3 it
 * inputs it right into the rgb arrays. If it is P6 it inputs it reads the 
 * data and inputs it into the rgb arrays.
 *
 * @param[in,out]      info - the main image struct
 * @param[in]          in - ifsteam input file
 *****************************************************************************/
void input_data(image& info, ifstream& in)
{
    int temp;

    if (info.input_type == "P3") //ASCII
    {
        //initialize the arrays
        for (int i = 0; i < info.rows; i++) // rows
        {
            for (int j = 0; j < info.cols; j++) // collumns
            {
                in >> temp;
                info.red[i][j] = temp;
                in >> temp;
                info.green[i][j] = temp;
                in >> temp;
                info.blue[i][j] = temp;
            }
        }
    }

    if (info.input_type == "P6") //BINARY
    {
        in.get();

        //initialize the arrays
        for (int i = 0; i < info.rows; i++) // rows
        {
            for (int j = 0; j < info.cols; j++) // collumns
            {
                in.read((char*)&info.red[i][j], sizeof(pixel));
                in.read((char*)&info.green[i][j], sizeof(pixel));
                in.read((char*)&info.blue[i][j], sizeof(pixel));
            }
        }
    }
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function grabs the comment if there is one and moves over it.
 *
 * @param[in]      in - ifsteam input file
 *****************************************************************************/
void get_comment(ifstream& in)
{
    char linebuffer[1024]; //set line buffer
    char tmp;
    while (tmp = in.peek(), tmp == '\n')
        in.get();
    if (tmp == '#')
        in.getline(linebuffer, 1023);
}


/** ***************************************************************************
 * @author: Aidan Coopman
 *
 * @par Description:
 * This function does all the outputting of the final image.
 *
 * First it ouputs the magic number, then the comment line, then the columns
 * and rows. Then based on what the output magic number is it will output
 * it in either ASCII format or BINARY format.
 *
 *
 *
 * @param[in]      info - the main image struct
 * @param[out]      out - ifsteam input file
 *****************************************************************************/
void output_data(image& info, ofstream& out)
{
    //print out type
    out << info.input_type << endl;

    //check to see if there is a comment
    //print out comment line (if there is one)
    if (info.comment_line != "\0")
        out << info.comment_line << endl;

    //print out cols x rows
    out << info.cols << " " << info.rows << endl;

    //print out the max pixel value
    out << info.max_pixel_value;

    if (info.input_type == "P3") //P3 ASCII
    {
        //if P3 add an endline
        out << endl; // add endline 

        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out << (int)info.red[i][j] << endl;
                out << (int)info.green[i][j] << endl;
                out << (int)info.blue[i][j] << endl;
            }
        }
    }
    if (info.input_type == "P6") //P6 BINARY
    {
        //if P6 add a space
        out << " ";

        for (int i = 0; i < info.rows; i++)
        {
            for (int j = 0; j < info.cols; j++)
            {
                out.write(reinterpret_cast<char*> 
                    (&info.red[i][j]), sizeof(pixel));
                out.write(reinterpret_cast<char*> 
                    (&info.green[i][j]), sizeof(pixel));
                out.write(reinterpret_cast<char*> 
                    (&info.blue[i][j]), sizeof(pixel));
            }
        }
    }
}//output_data