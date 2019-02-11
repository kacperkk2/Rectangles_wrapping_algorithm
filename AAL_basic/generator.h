#ifndef GENERATOR_H
#define GENERATOR_H

#include "generator_support.h"

/*****************************************************
generate convex hull with n points, and with n/2 rectangles in convex hull
*****************************************************/
Rectangle* generator(int number)
{
    if(number%2 !=0) number++; // in case of not even number
    float random = ((float) rand()) / (float) RAND_MAX;
    float side_length = 100 + random * 100; // randomize from 100 to 200

    Point* convex_vertices = generate_convex_hull(number, side_length);

    // got unregular polygon, now make rectangles
    Rectangle* rec = new Rectangle[number/2];
    int rec_iter = 0;
    Point tmp;

    vector<int> between_zero_two; // number of rectangles which vertices are between 0 and "side_length y

    for(int i = 2; i < number+2; i+=2) // start from [2] point, not to reach number-1 in first iteration
    {
        float closer;
        Line l;
        l.set_line(convex_vertices[i%number], convex_vertices[(i+1)%number]);

        // closer is value of max height rectangle, to not cause overlapping
        closer = get_distance(l, convex_vertices[(i+2)%number]);

        if(rec_iter > 0)
        {
            if(get_distance(l, rec[rec_iter-1].get_vertices(3)) < closer)
                closer = get_distance(l, rec[rec_iter-1].get_vertices(3));

            if(rec_iter == (number/2)-1 ) // last iterarion of loop, must check 0 rectangle, because last recnatgle can colid with it
            {
                if(get_distance(l, rec[0].get_vertices(2)) < closer)
                    closer = get_distance(l, rec[0].get_vertices(2));
            }
        }

        if(get_distance(l, convex_vertices[i-1]) < closer)
            closer = get_distance(l, convex_vertices[i-1]);

        Point second_side_first, second_side_second;
        if(number > 14 && i >= number && between_zero_two.size() == 0)
        {
            second_side_first.x = second_side_second.x = 0;
            second_side_first.y = convex_vertices[number/2].x; // number/2 point will be moved max to i+1 point, i+1 point in regular polygon is x_right max
            second_side_second.y = -convex_vertices[number/2].x;
        }
        else if(number > 14 && i >= number) // do this only for point in covex > 14 and only in last iteration
        {
            Point f = convex_vertices[(between_zero_two[0]*2)-1]; // between_zero_two[0] = index of first rectangle which vertices are between 0 2 y, between_zero_two[0]*2 = number of point in convexhull, -1 is one on left
            Point s = convex_vertices[(between_zero_two[between_zero_two.size()-1]*2)+2];

            second_side_first.x = second_side_second.x = 0; // before transform, recntagle from 00 0y, will have x = 0
            second_side_first.y = f.x;
            second_side_second.y = -f.x;
            closer = f.x+1;
            if(s.x < f.x)
            {
                second_side_first.y = s.x;
                second_side_second.y = -s.x;
                closer = s.x+1;
            }
        }
        else
        {
            second_side_first.x = convex_vertices[(i+1)%number].x - convex_vertices[i%number].x;
            second_side_first.y = convex_vertices[(i+1)%number].y - convex_vertices[i%number].y;

            second_side_second.x = convex_vertices[i%number].x - convex_vertices[(i+1)%number].x;
            second_side_second.y = convex_vertices[i%number].y - convex_vertices[(i+1)%number].y;
        }

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

        // if points are to far (collision is possible) then move them into safe area
        float curr_side_length = sqrt((second_side_second.x-second_side_first.x)*(second_side_second.x-second_side_first.x)+
                                    (second_side_second.y-second_side_first.y)*(second_side_second.y-second_side_first.y));

        if(curr_side_length > closer)
        {
            float proportion = closer / curr_side_length;

            second_side_first.x = proportion*second_side_first.x + (1-proportion)*convex_vertices[i%number].x;
            second_side_first.y = proportion*second_side_first.y + (1-proportion)*convex_vertices[i%number].y;
            second_side_second.x = proportion*second_side_second.x + (1-proportion)*convex_vertices[(i+1)%number].x;
            second_side_second.y = proportion*second_side_second.y + (1-proportion)*convex_vertices[(i+1)%number].y;
        }

        rec[rec_iter].set_vertices(convex_vertices[(i+1)%number], convex_vertices[i%number], second_side_first, second_side_second);

        // adding index of rectangle which vertices are between 0side_length y
        if(( (convex_vertices[(i+1)%number].y <= side_length && convex_vertices[(i+1)%number].y >= 0) ||
        (convex_vertices[i%number].y <= side_length && convex_vertices[i%number].y >= 0) ||
        (second_side_first.y <= side_length && second_side_first.y >= 0) ||
        (second_side_second.y <= side_length && second_side_second.y >= 0) ))
            between_zero_two.push_back(rec_iter);

        rec_iter++;
    }

    monte_carlo(convex_vertices, rec, number);

    return rec;
}


#endif
