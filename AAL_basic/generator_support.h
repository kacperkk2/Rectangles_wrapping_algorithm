#ifndef GENERATOR__SUPPORT_H
#define GENERATOR__SUPPORT_H

/*****************************************************
count how many points are in rectangles, and how many are in convex hull with MC
*****************************************************/
template <class Rec>
void monte_carlo(Point* convex_vertices, Rec* rec, int number)
{
    cout<<"Used space - MONTE CARLO: "<<endl;

    // make square on convex hull
    float x_left, x_right, y_down, y_up;
    x_left = x_right = y_down = y_up = 0;

    for(int i = 1; i < number; i++)
    {
        if(convex_vertices[i].x > x_right) x_right = convex_vertices[i].x;
        if(convex_vertices[i].y > y_up) y_up = convex_vertices[i].y;
        if(convex_vertices[i].y < y_down) y_down = convex_vertices[i].y;
    }

    float field_convex_hull = 0;
    float field_rectangles = 0;
    Point random;

    // rand points in square, check if they are in convex hull, check if they are in rectangle
    for(int i = 0; i < 70000; i++)
    {
        random.x = ((float) rand()) / (float) RAND_MAX; // 0-1
        random.x = x_left + random.x * (x_right - x_left); // number between x_left and x_right
        random.y = ((float) rand()) / (float) RAND_MAX; // 0-1
        random.y = y_down + random.y * (y_up - y_down); // number between y_down and y_up

        int rand_in = 0; // to count how many times points is in rectagnle or convex

        //check if point is in convex hull
        for(int j = 0; j < number; j++)
        {
            if(orientation(convex_vertices[j], convex_vertices[(j+1)%number], random) != 2)  // point is not on the right
                rand_in++;
        }

        if(rand_in == number) // point is in convex hull, check if it is in rectangle
        {
            field_convex_hull++;

            for(int j = 0; j < number/2; j++)
            {
                rand_in = 0;

                // check every side of rectangle
                for(int k = 0; k < 4; k++)
                {
                    if(orientation(rec[j].get_vertices(k), rec[j].get_vertices((k+1)%4), random) != 1) // point is not on the left
                        rand_in++;
                }

                if(rand_in == 4) // point is in rectangle
                {
                    field_rectangles++;
                    break;
                }
            }
        }
    }

    cout<<(float)(field_rectangles/field_convex_hull)*100<<"%"<<endl;
}

/*****************************************************
check if angle in given triangle is more or equal 90
*****************************************************/
bool more_than_ninety(Point P1, Point P2, Point P3)
{
    float P12 = sqrt((P1.x - P2.x)*(P1.x - P2.x) + (P1.y - P2.y)*(P1.y - P2.y));
    float P13 = sqrt((P1.x - P3.x)*(P1.x - P3.x) + (P1.y - P3.y)*(P1.y - P3.y));
    float P23 = sqrt((P2.x - P3.x)*(P2.x - P3.x) + (P2.y - P3.y)*(P2.y - P3.y));

    float angle = acos((P12*P12 + P13*P13 - P23*P23)/(2*P12*P13)) * 180/ M_PI;

    return (angle >= 90)? true: false;
}

/*****************************************************
return distance between line and point given
*****************************************************/
float get_distance(Line l, Point p)
{
    return ((abs(l.a*p.x + l.b*p.y - l.c))/(sqrt(l.a*l.a + l.b*l.b)));  // -c because of reprezentation line
}

/*****************************************************
generates convex hull, at first regular polygon, then
it moves vertces and retrun deformed polygon which is convex hull
*****************************************************/
Point* generate_convex_hull(int number, float side_length)
{
    Point* convex_vertices = new Point[number];
    convex_vertices[0].x = 0;
    convex_vertices[0].y = 0;

    float angle = -360/(float)number;

    // finding every point of regular polygon
    convex_vertices[1].x = convex_vertices[0].x;
    convex_vertices[1].y = convex_vertices[0].y + side_length;
    float curr_angle = angle;
    Point tmp;

    for(int i = 2; i < number; i++)
    {
        // setting point in center to transform
        tmp.x = 0;
        tmp.y = side_length;

        //transform the point
        // x' = x*cosfi - y*sinfi
        convex_vertices[i].x = ((tmp.x)*cos((M_PI*curr_angle)/180)) - (tmp.y*sin((M_PI*curr_angle)/180));
        // y' = x*sinfi + y*cosfi
        convex_vertices[i].y = ((tmp.x)*sin((M_PI*curr_angle)/180)) + (tmp.y*cos((M_PI*curr_angle)/180));

        // setting point in order to previos point
        convex_vertices[i].x += convex_vertices[i-1].x;
        convex_vertices[i].y += convex_vertices[i-1].y;

        curr_angle += angle;
    }

    // got regular polygon, now move points on egdes with interpolation: (1-a)P1 + aP2
    for(int i = 2; i < number; i++)
    {
        bool too_small_angle = true;

        while(too_small_angle)
        {
            float a = ((float) rand()) / (float) RAND_MAX; // number between 0 and 1
            a = -0.6 + a * 1.2; // number between -0.6 and 0.6

            if(a <= 0)
            {
                tmp.x = (1-abs(a))*convex_vertices[i].x + abs(a)*convex_vertices[i-1].x;
                tmp.y = (1-abs(a))*convex_vertices[i].y + abs(a)*convex_vertices[i-1].y;

                // now check if moving point not causes the angle <90 in our polygon
                if(more_than_ninety(tmp, convex_vertices[i-1], convex_vertices[(i+1)%number]) &&
                more_than_ninety(convex_vertices[(i+1)%number], tmp, convex_vertices[(i+2)%number]))
                    too_small_angle = false;
            }
            else
            {
                tmp.x = (1-a)*convex_vertices[i].x + a*convex_vertices[(i+1)%number].x;
                tmp.y = (1-a)*convex_vertices[i].y + a*convex_vertices[(i+1)%number].y;

                // now check if moving point not causes the angle <90 in our polygon
                if(more_than_ninety(tmp, convex_vertices[i-1], convex_vertices[(i+1)%number])
                && more_than_ninety(convex_vertices[(i-1)], tmp, convex_vertices[(i-2)]))
                    too_small_angle = false;
            }
        }
        // theres no angle <90 in our polygon, so bind x and y to i point
        convex_vertices[i].x = tmp.x;
        convex_vertices[i].y = tmp.y;
    }

    return convex_vertices;
}


#endif
