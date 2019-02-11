#ifndef RECTANGLE_ADD_H
#define RECTANGLE_ADD_H

#include <vector>
#include <math.h>
#include "../AAL_basic/Rectangle.h"

class Rectangle_add : public Rectangle
{
public:

    void set_lines()
    {
        for(int i = 0; i < 4; i++)
            lines[i].set_line(vertices[i], vertices[(i+1)%4]);
    }

    Line get_lines(int i)
    {
        return lines[i];
    }
};


#endif // RECTANGLE_H
