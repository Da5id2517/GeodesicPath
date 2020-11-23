#include "mesh.h"

//----------------------------------------------------------------------------------------------------------------------
// Constructors

square::square(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
}

triangle::triangle(int rows, int columns)
{
    this->rows =rows;
    this->columns = columns;
}

hexagon::hexagon(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
}

//----------------------------------------------------------------------------------------------------------------------
// Generate object functions

void square::generate_obj(std::ofstream &out)
{
    square::write_vertices(out);
    square::write_faces(out);
}

void triangle::generate_obj(std::ofstream &out)
{
    triangle::write_vertices(out);
    triangle::write_faces(out);
}

void hexagon::generate_obj(std::ofstream &out)
{
    hexagon::write_vertices(out);
    hexagon::write_faces(out);
}

//----------------------------------------------------------------------------------------------------------------------
// Write vertices

void square::write_vertices(std::ofstream &out)
{
    float px, py;
    for(int y=0; y<rows; y++)
    {
        for(int x=0; x<columns; x++)
        {
            px = x;
            py = y;
            out << "v " << px << " " << py << " 0.0" << std::endl;
        }
    }
}

void triangle::write_vertices(std::ofstream &out)
{
    float px, py;
    for(int y=0; y<rows; y++)
    {
        for(int x=0; x<columns; x++)
        {
            //TODO: add lambdas that transform these expressions.
            px = 0.5f*y + x;
            py = y * sqrtf(3.0f)/2.0f;
            out << "v " << px << " " << py << " 0.0" << std::endl;
        }
    }
}

void hexagon::write_vertices(std::ofstream &out)
{
    float px, py;

    for( int y=0; y<rows; y++)
    {
        for( int x=0; x<columns; x++)
        {
            px = floorf((x+(y%2))/2.0f)*3.0f + ((x+y)%2) - (y%2)*1.5f;
            py = y * sqrtf(3.0f)/2.0f;

            out << "v " << px << " " << py << " 0.0" << std::endl;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Write faces

void square::write_faces(std::ofstream &out)
{
    for(int y=0; y<this->rows-1; y++)
    {
        for(int x=0; x<this->columns-1; x++)
        {
            out << "f " << ((x+0)+(y+0)*columns)+1 << " "
                        << ((x+1)+(y+0)*columns)+1 << " "
                        << ((x+1)+(y+1)*columns)+1 << " "
                        << ((x+0)+(y+1)*columns)+1 << std::endl;
        }
    }
}

void triangle::write_faces(std::ofstream &out)
{
    for(int y=0; y<rows-1; y++)
    {
        for(int x=0; x<columns-1; x++)
        {
            out << "f " << ((x+0)+(y+0)*columns)+1 << " "
                << ((x+1)+(y+0)*columns)+1 << " "
                << ((x+1)+(y+1)*columns)+1 << std::endl;

            out << "f " << ((x+1)+(y+0)*columns)+1 << " "
                << ((x+1)+(y+1)*columns)+1 << " "
                << ((x+0)+(y+1)*columns)+1 << std::endl;
        }
    }
}

void hexagon::write_faces(std::ofstream &out)
{
    for( int y=0; y<rows-2; y++ )
    {
        for( int x=0; x<columns-1; x++ )
        {
            if(( y%2 == 0 && x%2 == 0 ) ||
               ( y%2 == 1 && x%2 == 1 ))
            {
                out << "f " << ((x+0)+(y+0)*columns)+1 << " "
                    << ((x+1)+(y+0)*columns)+1 << " "
                    << ((x+1)+(y+1)*columns)+1 << " "
                    << ((x+1)+(y+2)*columns)+1 << " "
                    << ((x+0)+(y+2)*columns)+1 << " "
                    << ((x+0)+(y+1)*columns)+1 << std::endl;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
// TODO: operators

// TODO: semii i=1..8