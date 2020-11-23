#ifndef RSPROJECT_MESH_H
#define RSPROJECT_MESH_H

#include <fstream>

class mesh {
public:
    virtual void generate_obj(std::ofstream& out) = 0;
    virtual void write_vertices(std::ofstream& out) = 0;
    virtual void write_faces(std::ofstream& out) = 0;

};

class square: mesh
{
public:
    square(int rows, int columns);

    void generate_obj(std::ofstream& out);
    void write_vertices(std::ofstream& out);
    void write_faces(std::ofstream& out);

private:
    int rows, columns;
};

class triangle: mesh
{
public:
    triangle(int rows, int columns);

    void generate_obj(std::ofstream& out);
    void write_vertices(std::ofstream& out);
    void write_faces(std::ofstream& out);

private:
    int rows, columns;
};

class hexagon: mesh
{
public:
    hexagon(int rows, int columns);

    void generate_obj(std::ofstream& out);
    void write_vertices(std::ofstream& out);
    void write_faces(std::ofstream& out);

private:
    int rows, columns;
};


#endif //RSPROJECT_MESH_H
