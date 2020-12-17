#ifndef RSPROJECT_VERTEX_H
#define RSPROJECT_VERTEX_H

class Vertex
{
public:
    Vertex(): x(0.0), y(0.0), index(0) {};
    Vertex(float x, float y, int index = 0) : x(x), y(y), index(index) {};
    Vertex(const Vertex &vertex);

    float x_coord() const;
    float y_coord() const;

    int getIndex() const;
    void setIndex(int new_index);

    static int dimension();

    bool operator ==(const Vertex& other) const;

private:
    float x, y;
    int index;

};

#endif //RSPROJECT_VERTEX_H
