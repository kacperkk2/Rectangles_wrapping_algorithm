#ifndef BUILDER_H
#define BUILDER_H

template <class Rec>
vector<Point> build_vertices_generator(Rec* r, int n, float& rectangles_field)
{
    vector<Point> all_vertices;

    for(int i = 0; i < n/2; i++)
    {
        rectangles_field += r[i].get_field();

        for(int j = 0; j < 4; j++)
            all_vertices.push_back(r[i].get_vertices(j));
    }

    return all_vertices;
}

template <class Rec>
vector<Point> build_vertices_input(char** argv, Rec* r, int n, float& rectangles_field)
{
    float x, y, height, width, fi;
    int iter = 3;
    vector<Point> all_vertices;

    for(int i = 0; i < n; i++)
    {
        x = atof(argv[iter++]);
        y = atof(argv[iter++]);
        height = atof(argv[iter++]);
        width = atof(argv[iter++]);
        fi = atof(argv[iter++]);

        r[i].set_all(x, y, height, width, fi);

        rectangles_field += r[i].get_field();

        for(int j = 0; j < 4; j++)
            all_vertices.push_back(r[i].get_vertices(j));
    }

    return all_vertices;
}

void print_usage(string name)
{
    cout<<"Usage: "<<name<<" [OPTION] [DATA]"<<endl;
    cout<<"Application counts % of used space, made by wrapping all rectangles."<<endl<<endl;
    cout<<"Options:"<<endl;
    cout<<"   -g [NUMBER]\t generates rectangles and convex hull with NUMBER points,"<<endl;
    cout<<"              \t and runs algorithm for generated data."<<endl;
    cout<<"   -a [DATA]\t runs algorithm for data specified after -a,"<<endl;
    cout<<"            \t convention: first number specifies number of rectangles,"<<endl;
    cout<<"            \t every next five numbers are: x coord of rectangle center,"<<endl;
    cout<<"            \t y coord of rectangle center, length, width, angle."<<endl;
    cout<<"            \t Do not press enter after any single data, only at the end."<<endl<<endl;
    cout<<"OUTPUT:"<<endl;
    cout<<"% of used space counting MONTE CARLO method (only in -g option)"<<endl;
    cout<<"% of used space counting ALGORITHM method"<<endl;
    cout<<"time needed to count with algoithm method"<<endl;
}


#endif
