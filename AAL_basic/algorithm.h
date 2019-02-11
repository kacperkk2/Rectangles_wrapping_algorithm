#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <algorithm>

using namespace std;

/*****************************************************
sorting by angle, which is second value, used in graham algorithm
*****************************************************/
bool sort_by_sec(const pair<float, float> a, pair<float, float> b)
{
    return (a.second < b.second);
}

/*****************************************************
showing if third point (argument) is on the left, right
or collinear with two first
*****************************************************/
int orientation(Point p, Point q, Point r)
{
    float val = (float)((q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y));

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2;  // clock or counterclock wise
}

/*****************************************************
count field of convex hull given in argument
*****************************************************/
float shoelace(vector<Point> convex_hull)
{
    float first = 0.0, second = 0.0;

    convex_hull.push_back(convex_hull[0]); // pushing back first element to algoritm work

    for(unsigned i = 1; i < convex_hull.size(); i++)
    {
        first += (convex_hull[i-1].x * convex_hull[i].y);
        second += (convex_hull[i-1].y * convex_hull[i].x);
    }

    convex_hull.pop_back(); // undo what done before loop

    float convex_field = (abs(first - second))/2;

    return convex_field;
}

/*****************************************************
find point which is most on bottom and most on left
*****************************************************/
int find_most_bottom_left(vector<Point> all_vertices)
{
    Point most_bottom_l;
    most_bottom_l.x = all_vertices[0].x;
    most_bottom_l.y = all_vertices[0].y;
    int most_bottom_l_position = 0;
    for(unsigned i = 1; i < all_vertices.size(); i++)
    {
        if(all_vertices[i].y < most_bottom_l.y)
        {
            most_bottom_l.x = all_vertices[i].x;
            most_bottom_l.y = all_vertices[i].y;
            most_bottom_l_position = i;
        }
        else if(all_vertices[i].y == most_bottom_l.y)
        {
            if(all_vertices[i].x < most_bottom_l.x)
            {
                most_bottom_l.x = all_vertices[i].x;
                most_bottom_l.y = all_vertices[i].y;
                most_bottom_l_position = i;
            }
        }
    }

    return most_bottom_l_position;
}

/*****************************************************
find convex hull for list of points given in argument
*****************************************************/
vector<Point> find_convex_hull_graham(vector<Point> all_vertices)
{
    vector<Point> convex_hull;
    vector<pair<int, float>> tangens;
    vector<int> collinear_with_first;

    int most_bottom_l_position = find_most_bottom_left(all_vertices);
    Point most_bottom_l = all_vertices[most_bottom_l_position];

    float x = most_bottom_l.x, y = most_bottom_l.y;
    // count angles
    for(int i = 0; i < (int)all_vertices.size(); i++)
    {
        if(i == most_bottom_l_position)
        {
            continue;
        }
        if(all_vertices[i].y == y) // to avoid dividing by 0
        {
            collinear_with_first.push_back(i); // vector with collinear with first, after sorting set at the end of tangens vector
        }
        else
        {
            float a = all_vertices[i].x - x;
            float b = all_vertices[i].y - y;
            float w = a/b;
            tangens.push_back(make_pair(i,w)); // i = position of element in all_vertices, w = tangens
        }
    }

    sort(tangens.begin(), tangens.end(), sort_by_sec);

    // in case of many vertices have the same angle, leave only which is farest from "most_bottom_left"
    if(tangens.size() > 0)
    {
        for(unsigned i = 1; i < tangens.size(); i++)
        {
            if(tangens[i-1].second == tangens[i].second)
            {
                if(all_vertices[tangens[i-1].first].y <= all_vertices[tangens[i].first].y)
                {
                    tangens.erase(tangens.begin() + i-1);
                    i--; // deleted element, so size changed, must go back one
                }
                else
                {
                    tangens.erase(tangens.begin() + i);
                    i --; // deleted element, so size changed, must go back one
                }
            }
        }
    }

    // in case of ponts with angle 0 pick only farest from "most_bottom_left"
    if(collinear_with_first.size() != 0)
    {
        float farest_x = 0;
        for(int i=1; i < (int)collinear_with_first.size(); i++)
        {
            if(all_vertices[collinear_with_first[i]].x > all_vertices[collinear_with_first[farest_x]].x)
                farest_x = i;
        }
        tangens.push_back(make_pair(collinear_with_first[farest_x],0));
    }

    int tan_size = tangens.size();

    convex_hull.push_back(most_bottom_l); // push most bottom left
    convex_hull.push_back(all_vertices[tangens[tan_size-1].first]);
    convex_hull.push_back(all_vertices[tangens[tan_size-2].first]);

    // added 3 points higher, left n-3 points
    for (int i = tan_size-3; i >= 0; i--)
    {
        // keep removing top while have a non-left turn
        while (orientation(convex_hull[convex_hull.size()-2], convex_hull[convex_hull.size()-1], all_vertices[tangens[i].first]) != 2) // point is not on the right
            convex_hull.pop_back();

        convex_hull.push_back(all_vertices[tangens[i].first]);
    }

    return convex_hull;
}


#endif
