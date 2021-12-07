#include "bits/stdc++.h"

using namespace std;

const int size = 400;
int (*result)[size] = new int[size][size];
int (*ppm)[size] = new int[size * 3][size];
void set_pixel(int x, int y, int c = 1)
{
    if (x > size or x < 0 or y > size or y < 0)
    {
        return;
    }
    result[x][y] = c;
}

class Point
{
private:
    double x;
    double y;

public:
    Point(double x1, double y1)
    {
        x = x1;
        y = y1;
    };
    Point()
    {
        x = 0;
        y = 0;
    };
    double getx()
    {
        return x;
    }
    double gety()
    {
        return y;
    }
    void plot()
    {
        set_pixel(x, y);
    }
    void drawCircle(double radius1, int c = 1)
    { // 1 = black, 2 = red
        double a1 = x;
        double b1 = y;
        int a = a1 * size;
        int b = b1 * size;
        int radius = radius1 * size;
        int xmax = (int)(radius * 0.70710678);
        int y = radius;
        int y2 = y * y;
        int ty = (2 * y) - 1;
        int y2_new = y2;
        for (int x = 0; x <= xmax + 1; x++)
        {
            if ((y2 - y2_new) >= ty)
            {
                y2 -= ty;
                y -= 1;
                ty -= 2;
            }
            set_pixel(x + a, y + b, c);
            set_pixel(x + a, -y + b, c);
            set_pixel(-x + a, y + b, c);
            set_pixel(-x + a, -y + b, c);
            set_pixel(y + a, x + b, c);
            set_pixel(y + a, -x + b, c);
            set_pixel(-y + a, x + b, c);
            set_pixel(-y + a, -x + b, c);
            y2_new -= (2 * x) - 3;
        }
    };
};
class Line
{
public:
    Point p1;
    Point p2;
    Line(Point x1, Point y1)
    {
        p1 = x1;
        p2 = y1;
    };
    Line()
    {
        p1 = Point();
        p2 = Point();
    };
    void drawLine()
    {
        double x1d = p1.getx();
        double y1d = p1.gety();
        double x2d = p2.getx();
        double y2d = p2.gety();
        int x1 = x1d * size;
        int y1 = y1d * size;
        int x2 = x2d * size;
        int y2 = y2d * size;
        int dx = x2 - x1;
        int dy = y2 - y1;

        if ((dx <= 0 and dy <= 0) or (dx <= 0 and dy > 0))
        {
            dx = -1 * (dx); // 150
            dy = -1 * (dy); // -300
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        int j = y1;
        int c = dy - dx;
        if (abs(dx) > abs(dy))
        {
            if (dy < 0)
            {
                for (int i = x1; i < x2; i++)
                {
                    set_pixel(i, j);
                    if (c > 0)
                    {
                        j -= 1;
                        c -= dx;
                    }
                    c -= dy;
                }
            }
            else
            {
                for (int i = x1; i < x2; i++)
                {
                    set_pixel(i, j);
                    if (c > 0)
                    {
                        j += 1;
                        c -= dx;
                    }
                    c += dy;
                }
            }
        }
        else
        {
            j = x1;
            int c = dx - dy;
            if (dy < 0)
            {
                for (int i = y1; i > y2; i--)
                {
                    set_pixel(j, i);
                    if (c > 0)
                    {
                        j += 1;
                        c += dy;
                    }
                    c += dx;
                }
            }
            else
            {
                for (int i = y1; i < y2; i++)
                {
                    set_pixel(j, i);
                    if (c > 0)
                    {
                        j += 1;
                        c -= dy;
                    }
                    c += dx;
                }
            }
        }
    }
    void drawLineExtended()
    {
        if (p1.getx() - p2.getx() == 0)
        {
            drawLine();
        }
        else
        {
            double slope = (p2.gety() - p1.gety()) / (p2.getx() - p1.getx());
            double point1y = slope * 0 - (slope * p1.getx()) + p1.gety();
            double point2y = slope * 0.99 - (slope * p1.getx()) + p1.gety();
            Line(Point(0, point1y), Point(0.99, point2y)).drawLine();
        }
    }
    vector<Point> findPoint(double d, Point p)
    { // finds a point d distance away from a point on a line
        vector<Point> o;
        double dy = p2.gety() - p1.gety();
        double dx = p2.getx() - p1.getx();
        double magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
        dy = dy / magnitude;
        dx = dx / magnitude;
        o.push_back(Point(p.getx() + (dx * d), p.gety() + (dy * d)));
        o.push_back(Point(p.getx() - (dx * d), p.gety() - (dy * d)));
        return o;
    }
    Line findPerp(Point p)
    { // finds the perpendicular to a line given a point p
        double dy = -1 * (p2.getx() - p1.getx());
        double dx = p2.gety() - p1.gety();
        double magnitude = sqrt(pow(dx, 2) + pow(dy, 2));
        dy = 20 * dy / magnitude;
        dx = 20 * dx / magnitude;
        Point o = Point(p.getx() + (dx), p.gety() + (dy));
        return Line(p, o);
    }
    vector<double> getEq()
    {
        vector<double> v;
        double dx = p2.getx() - p1.getx();
        double dy = p2.gety() - p1.gety();
        if (dx == 0)
        {
            v.push_back(1);
            v.push_back(0);
            v.push_back(p1.getx());
            return v;
        }
        double m = dy / dx;
        v.push_back(-1 * m);
        v.push_back(1);
        v.push_back((-1 * m * p1.getx()) + p1.gety());

        return v;
    }
    Point intersection(Line l)
    {
        vector<double> v1 = getEq();
        vector<double> v2 = l.getEq();
        double a1 = v1[0];
        double b1 = v1[1];
        double c1 = v1[2];
        double a2 = v2[0];
        double b2 = v2[1];
        double c2 = v2[2];
        double x = (c1 * b2 - c2 * b1) / (a1 * b2 - b1 * a2); // Cramers Rule
        double y = (c1 * a2 - c2 * a1) / (b1 * a2 - b2 * a1); // Cramers Rule
        return Point(x, y);
    }
    double distance()
    {
        return sqrt(pow((p1.getx() - p2.getx()), 2) + pow((p1.gety() - p2.gety()), 2));
    }
};
vector<Point> vs;
void gen_points(int n)
{
    ofstream myfile;
    myfile.open("points.txt");
    myfile << setprecision(23);
    myfile << fixed;
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        double x = rand() / double(RAND_MAX);
        double y = rand() / double(RAND_MAX);
        // Point p = Point(x, y);
        // ps.push_back(p);
        myfile << x << "  " << y << "\n";
    }
}
void get_points()
{
    vs.clear();
    string line;
    ifstream po("points.txt");
    if (po.is_open())
    {
        while (getline(po, line, '\n'))
        {
            int pos = line.find(" ");
            double x = stod(line.substr(0, pos));
            double y = stod(line.substr(pos));
            //cout << x << " " << y;
            Point p = Point(x, y);
            p.drawCircle(3.0 / 800.0);
            vs.push_back(p);
        }
    }
}
int get_side(Point p1, Point p2, Point p)
{
    double val = (p.gety() - p1.gety()) * (p2.getx() - p1.getx()) - (p2.gety() - p1.gety()) * (p.getx() - p1.getx());
    if (val > 0.0)
        return 1;
    if (val < 0.0)
        return -1;
    return 0;
}
double l_dist(Point p1, Point p2, Point p)
{
    return abs((p.gety() - p1.gety()) * (p2.getx() - p1.getx()) - (p2.gety() - p1.gety()) * (p.getx() - p1.getx()));
}
void arrToFile()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (result[i][j] == 2)
            {
                ppm[i * 3][j] = 1;
                ppm[i * 3 + 1][j] = 0;
                ppm[i * 3 + 1][j] = 0;
            }
            else if (result[i][j] == 1)
            {
                ppm[i * 3][j] = 0;
                ppm[i * 3 + 1][j] = 0;
                ppm[i * 3 + 2][j] = 0;
            }
            else
            {
                ppm[i * 3][j] = 1;
                ppm[i * 3 + 1][j] = 1;
                ppm[i * 3 + 2][j] = 1;
            }
        }
    }
    ofstream myfile;
    myfile.open("points.ppm");
    myfile << "P3 " << size << " " << size << " 1\n";
    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size; i++)
        {
            myfile << ppm[i * 3][j] << " ";
            myfile << ppm[i * 3 + 1][j] << " ";
            myfile << ppm[i * 3 + 2][j] << " ";
        }
        myfile << "\n";
    }
    myfile.close();
}

void quickhull(Point p1, Point p2, int side)
{
    int ind = -1;
    double max_distance = 0;

    for (int i = 0; i < vs.size(); i++)
    {
        double temp = l_dist(p1, p2, vs[i]);
        if (get_side(p1, p2, vs[i]) == side && temp > max_distance)
        {
            ind = i;
            max_distance = temp;
        }
    }

    if (ind == -1)
    {
        Line(p1, p2).drawLine();
        return;
    }

    quickhull(vs[ind], p1, -get_side(vs[ind], p1, p2));
    quickhull(vs[ind], p2, -get_side(vs[ind], p2, p1));
}
void part1()
{
    gen_points(60);
    get_points();
    //recursive method to find convex hull using QuickHull method
    //find the leftmost point
    int leftmost = 0;
    for (int i = 1; i < vs.size(); i++)
    {
        if (vs[i].getx() < vs[leftmost].getx())
        {
            leftmost = i;
        }
    }
    //find the rightmost point
    int rightmost = 0;
    for (int i = 1; i < vs.size(); i++)
    {
        if (vs[i].getx() > vs[rightmost].getx())
        {
            rightmost = i;
        }
    }
    quickhull(vs[leftmost], vs[rightmost], 1);
    quickhull(vs[leftmost], vs[rightmost], -1);
    arrToFile();
}
int orientation(Point p1, Point p2, Point p)
{
    double val = (p2.gety() - p1.gety()) * (p.getx() - p2.getx()) - (p2.getx() - p1.getx()) * (p.gety() - p2.gety());
    if (val == 0)
        return 0;             // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}
void part2()
{
    //recursive function to find a convex hull gien a set of points
    get_points();
    vector<Point> points = vs;
    vector<Point> hull;
    int start = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if (points[i].gety() < points[start].gety())
        {
            start = i;
        }
    }
    int p = start;
    int q;
    do
    {
        q = (p + 1) % points.size();
        for (int i = 0; i < points.size(); i++)
        {
            if (orientation(points[p], points[i], points[q]) == 2)
            {
                q = i;
            }
        }
        hull.push_back(points[q]);
        p = q;
    } while (p != start);
    for (int i = 0; i < hull.size(); i++)
    {
        hull[i].drawCircle(3.0 / 800.0, 2);
    }
    arrToFile();
}
int main()
{
    part1();
    part2();
}