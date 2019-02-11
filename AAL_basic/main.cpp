#include <iostream>
#include <chrono>
#include "Rectangle.h"
#include "algorithm.h"
#include "generator.h"
#include "builder.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc == 2 && string(argv[1]) == "--help")
    {
        print_usage(argv[0]);
        exit(0);
    }
    else if(argc < 3)
    {
        cout<<"You must specify option and data."<<endl;
        cout<<"Try "<<argv[0]<<" --help."<<endl;
        exit(0);
    }
    else if(argc == 3 && string(argv[1]) == "-g")
    {
        srand((unsigned)time(NULL));

        int n = atoi(argv[2]);
        if(n%2 != 0) n++;
        float rectangles_field = 0;
        Rectangle* r = generator(n);

        cout<<"Used space - ALGORITHM: "<<endl;
        // start counting time
        auto start = std::chrono::system_clock::now();

        vector<Point> all_vertices = build_vertices_generator(r, n, rectangles_field);
        vector<Point> convex_hull = find_convex_hull_graham(all_vertices);
        float convex_hull_field = shoelace(convex_hull);

        // stop counting time
        auto endt = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endt - start);

        cout<<(float)(rectangles_field/convex_hull_field)*100<<"%"<<endl<<endl;
        cout<<"Elapsed time - ALGORITHM: "<<elapsed.count()<<"us"<<endl;

        delete []r;
    }
    else if(argc > 3 && string(argv[1]) == "-a")
    {
        int n = atoi(argv[2]);
        Rectangle* r = new Rectangle[n];
        float rectangles_field = 0;

        cout<<"Used space - ALGORITHM: "<<endl;
        // start counting time
        auto start = std::chrono::system_clock::now();

        vector<Point> all_vertices = build_vertices_input(argv, r, n, rectangles_field);
        vector<Point> convex_hull = find_convex_hull_graham(all_vertices);
        float convex_hull_field = shoelace(convex_hull);

        // stop counting time
        auto endt = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endt - start);

        cout<<(float)(rectangles_field/convex_hull_field)*100<<"%"<<endl<<endl;
        cout<<"Elapsed time - ALGORITHM: "<<elapsed.count()<<"us"<<endl;

        delete []r;
    }
    else
    {
        cout<<"Invalid options."<<endl;
        cout<<"Try "<<argv[0]<<" --help."<<endl;
        exit(0);
    }

    return 0;
}
