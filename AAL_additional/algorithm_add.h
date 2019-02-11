#ifndef ALGORITHM_ADD_H
#define ALGORITHM_ADD_H

#include <algorithm>
#include "../AAL_basic/algorithm.h"

using namespace std;

/*****************************************************
check every pair of rectangles and substract overlapped field
from all rectangles field
*****************************************************/
void overlapping(float& rectangles_field, Rectangle_add* r, int n)
{
    vector<Point> points;
    // checking every rectangle with every next rectangle
    for(int i = 0; i < n-1; i++)
    {
        for(int j = i+1; j < n; j++)
        {
            points.clear();

            for(int k = 0; k < 4; k++)
            {
                // adding every point that is on intersection with every line in 2 rectangles
                Line l1 = r[i].get_lines(k);
                for(int l = 0; l < 4; l++)
                {
                    Line l2 = r[j].get_lines(l);

                    float determinant = (l1.a * l2.b) - (l2.a * l1.b);
                    if (determinant == 0)
                    {
                        // Lines are parallel
                    }
                    else
                    {
                        Point p;
                        p.x = (l1.c*l2.b - l2.c*l1.b)/determinant;
                        p.y = (l1.a*l2.c - l2.a*l1.c)/determinant;

                        // checking if the point is on cross of 2 sections
                        Point l1[2];
                        Point l2[2];
                        if(r[j].get_vertices(l).x <= r[j].get_vertices((l+1)%4).x)
                        {
                            l2[0] = r[j].get_vertices(l);
                            l2[1] = r[j].get_vertices((l+1)%4);
                        }
                        else
                        {
                            l2[0] = r[j].get_vertices((l+1)%4);
                            l2[1] = r[j].get_vertices(l);
                        }
                        if(r[i].get_vertices(k).x <= r[i].get_vertices((k+1)%4).x)
                        {
                            l1[0] = r[i].get_vertices(k);
                            l1[1] = r[i].get_vertices((k+1)%4);
                        }
                        else
                        {
                            l1[0] = r[i].get_vertices((k+1)%4);
                            l1[1] = r[i].get_vertices(k);
                        }
                        if(p.x >= l1[0].x && p.x >= l2[0].x && p.x <= l1[1].x && p.x <= l2[1].x)
                        {
                            if(r[j].get_vertices(l).y <= r[j].get_vertices((l+1)%4).y)
                            {
                                l2[0] = r[j].get_vertices(l);
                                l2[1] = r[j].get_vertices((l+1)%4);
                            }
                            else
                            {
                                l2[0] = r[j].get_vertices((l+1)%4);
                                l2[1] = r[j].get_vertices(l);
                            }
                            if(r[i].get_vertices(k).y <= r[i].get_vertices((k+1)%4).y)
                            {
                                l1[0] = r[i].get_vertices(k);
                                l1[1] = r[i].get_vertices((k+1)%4);
                            }
                            else
                            {
                                l1[0] = r[i].get_vertices((k+1)%4);
                                l1[1] = r[i].get_vertices(k);
                            }
                            if(p.y >= l1[0].y && p.y >= l2[0].y && p.y <= l1[1].y && p.y <= l2[1].y)
                            {
                                points.push_back(p);
                            }
                        }
                    }
                }
            }

            // adding every vertice of first that are in second rectangle
            for(int k = 0; k < 4; k++)
            {
                bool inside = true;
                for(int l = 0; l < 4; l++)
                {
                    if(orientation(r[j].get_vertices(l), r[j].get_vertices((l+1)%4), r[i].get_vertices(k)) ==1 )
                    {
                        inside = false;
                        break;
                    }
                }
                if(inside)
                    points.push_back(r[i].get_vertices(k));
            }
            // adding every vertice of second that are in first rectangle
            for(int k = 0; k < 4; k++)
            {
                bool inside = true;
                for(int l = 0; l < 4; l++)
                {
                    if(orientation(r[i].get_vertices(l), r[i].get_vertices((l+1)%4), r[j].get_vertices(k)) ==1)
                    {
                        inside = false;
                        break;
                    }
                }
                if(inside)
                    points.push_back(r[j].get_vertices(k));
            }

            // if there is overlapping, subsctract from rectangle field
            if(points.size() > 2)
            {
                int most_bottom_l = find_most_bottom_left(points);
                Point most_bottom = points[most_bottom_l];
                float x = most_bottom.x, y = most_bottom.y;

                vector<pair<int, float>> tangens;
                for(int i = 0; i < (int)points.size(); i++)
                {
                    if(i == most_bottom_l)
                    {
                        continue;
                    }
                    if(points[i].y == y) // to avoid dividing by 0
                    {
                        tangens.push_back(make_pair(i,0));
                    }
                    else
                    {
                        float a = points[i].x - x;
                        float b = points[i].y - y;
                        float w = a/b;
                        tangens.push_back(make_pair(i,w)); // position of element in all_vertices, w = tangens
                    }
                }
                sort(tangens.begin(), tangens.end(), sort_by_sec);

                vector<Point>final_points;
                final_points.push_back(points[most_bottom_l]);
                for(int i = 0; i < (int)tangens.size(); i++)
                    final_points.push_back( points[tangens[i].first] );

                rectangles_field -= shoelace(final_points);
            }
        }
    }
}

#endif
