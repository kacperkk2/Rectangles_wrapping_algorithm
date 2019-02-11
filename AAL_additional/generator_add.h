#ifndef GENERATOR_ADD_H
#define GENERATOR_ADD_H

#include "../AAL_basic/generator_support.h"

/*****************************************************
generate convex hull with n points, and with n/2 rectangles in convex hull
*****************************************************/
Rectangle_add* generator(int number)
{
    if(number%2 !=0) number++; // in case of not even number
    float random = ((float) rand()) / (float) RAND_MAX;
    float side_length = 100 + random * 100; // randomize from 100 to 200

    Point* convex_vertices = generate_convex_hull(number, side_length);

    // got unregular polygon, now make rectangles
    Rectangle_add* rec = new Rectangle_add[number/2];
    int rec_iter = 0;
    int middle_vertice_index = number/2;

    int rand_rectangle1 = rand() % (number/2); // first rectangle to be longer
    int rand_rectangle2 = rand() % (number/2); // second rectangle to be longer
    Point tmp;

    for(int i = 2; i < number+2; i+=2)
    {
        float closer, x_y_square;
        Line l;
        l.set_line(convex_vertices[i%number], convex_vertices[(i+1)%number]);

        // find how far recntagle can grow and put it in closer
        closer = get_distance(l, convex_vertices[(i+middle_vertice_index-1)%number]);

        if(get_distance(l, convex_vertices[(i+middle_vertice_index)%number]) < closer)
            closer = get_distance(l, convex_vertices[(i+middle_vertice_index)%number]);

        if(get_distance(l, convex_vertices[(i+middle_vertice_index+1)%number]) < closer)
            closer = get_distance(l, convex_vertices[(i+middle_vertice_index+1)%number]);

        // for randomized rectangles make them longer
        if(rec_iter == rand_rectangle2 || rec_iter == rand_rectangle1)
            closer=(float)closer*9/10;
        else
            closer=(float)closer/100;

        Point second_side_first, second_side_second;

        second_side_first.x = convex_vertices[(i+1)%number].x - convex_vertices[i%number].x;
        second_side_first.y = convex_vertices[(i+1)%number].y - convex_vertices[i%number].y;
        x_y_square = second_side_first.x*second_side_first.x + second_side_first.y*second_side_first.y;
        // scale point x,y to value "closer"
        second_side_first.x = second_side_first.x * sqrt((closer*closer)/(x_y_square));
        second_side_first.y = second_side_first.y * sqrt((closer*closer)/(x_y_square));

        second_side_second.x = convex_vertices[i%number].x - convex_vertices[(i+1)%number].x;
        second_side_second.y = convex_vertices[i%number].y - convex_vertices[(i+1)%number].y;
        x_y_square = second_side_second.x*second_side_second.x + second_side_second.y*second_side_second.y;
        // scale point x,y to value "closer"
        second_side_second.x = second_side_second.x * sqrt((closer*closer)/(x_y_square));
        second_side_second.y = second_side_second.y * sqrt((closer*closer)/(x_y_square));

        // transform
        tmp.x = second_side_first.x;
        tmp.y = second_side_first.y;
        second_side_first.x = ((tmp.x)*cos((M_PI*(-90))/180)) - (tmp.y*sin((M_PI*(-90))/180));
        second_side_first.y = ((tmp.x)*sin((M_PI*(-90))/180)) + (tmp.y*cos((M_PI*(-90))/180));
        tmp.x = second_side_second.x;
        tmp.y = second_side_second.y;
        second_side_second.x = ((tmp.x)*cos((M_PI*(90))/180)) - (tmp.y*sin((M_PI*(90))/180));
        second_side_second.y = ((tmp.x)*sin((M_PI*(90))/180)) + (tmp.y*cos((M_PI*(90))/180));
        // undo
        second_side_first.x += convex_vertices[i%number].x;
        second_side_first.y += convex_vertices[i%number].y;

        second_side_second.x += convex_vertices[(i+1)%number].x;
        second_side_second.y += convex_vertices[(i+1)%number].y;

        // got vertices in proper distance, set all important variables
        rec[rec_iter].set_vertices(convex_vertices[(i+1)%number], convex_vertices[i%number], second_side_first, second_side_second);
        rec[rec_iter].set_lines();

        rec_iter++;
    }

    monte_carlo(convex_vertices, rec, number);

    return rec;
}


#endif
