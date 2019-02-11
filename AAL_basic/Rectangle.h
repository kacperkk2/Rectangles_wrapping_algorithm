#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <vector>
#include <math.h>

class Point
{
public:
    float x;
    float y;
};

class Line
{
public:
    // reprezentation ax + by = c
    float a;
    float b;
    float c;

    // a = y2 - y1
    // b = x1 - x2
    // c = ax1 + by1
    void set_line(Point f, Point s)
    {
        a = s.y - f.y;
        b = f.x - s.x;
        c = (a*f.x) + (b*f.y);
    }
};

class Rectangle
{
protected:
    Point vertices[4];
    Point center;
    Line lines[4];
    float height;
    float width;
    float field;
    float fi;

public:
    void set_all(float x, float y,float w, float h, float f)
    {
        center.x = x;
        center.y = y;
        height = h;
        width = w;
        fi = -f;

        field = height*width;

        vertices[0].x = vertices[1].x = x - width/2;
        vertices[1].y = vertices[2].y = y + height/2;
        vertices[2].x = vertices[3].x = x + width/2;
        vertices[0].y = vertices[3].y = y - height/2;

        if(fi != 0.0) // rotate all vertices, except first
        {
            //setting point of rotation in center point, not in the middle of coordination system
            for(int i = 0; i <= 3; i++)
            {
                vertices[i].x -= x;
                vertices[i].y -= y;
            }

            //rotating vertices
            for(int i = 0; i <= 3; i++)
            {
                float last_x = vertices[i].x;
                float last_y = vertices[i].y;
                // x' = x*cosfi - y*sinfi
                vertices[i].x = ((last_x)*cos((M_PI*fi)/180)) - (last_y*sin((M_PI*fi)/180));
                // y' = x*sinfi + y*cosfi
                vertices[i].y = ((last_x)*sin((M_PI*fi)/180)) + (last_y*cos((M_PI*fi)/180));
            }

            // undo setting point of rotation in center point
            for(int i = 0; i <= 3; i++)
            {
                vertices[i].x += x;
                vertices[i].y += y;
            }
        }

        // set all 4 lines in rectangle
        for(int i = 0; i < 4; i++)
            lines[i].set_line(vertices[i], vertices[(i+1)%4]);
    }

    // settings vertices got from generator
    void set_vertices(Point a, Point b, Point c, Point d)
    {
        vertices[0].x = a.x;
        vertices[0].y = a.y;
        vertices[1].x = b.x;
        vertices[1].y = b.y;
        vertices[2].x = c.x;
        vertices[2].y = c.y;
        vertices[3].x = d.x;
        vertices[3].y = d.y;

        height = sqrt((vertices[0].x-vertices[1].x)*(vertices[0].x-vertices[1].x) +
                     (vertices[0].y-vertices[1].y)*(vertices[0].y-vertices[1].y));
        width = sqrt((vertices[0].x-vertices[3].x)*(vertices[0].x-vertices[3].x) +
                     (vertices[0].y-vertices[3].y)*(vertices[0].y-vertices[3].y));

        field = width*height;
    }

    float get_field()
    {
        return field;
    }

    Point get_vertices(int i)
    {
        return vertices[i];
    }
};

#endif // RECTANGLE_H
