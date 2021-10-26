#include "bits/stdc++.h"

using namespace std;
const int size = 800;
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
public:
    double x;
    double y;
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
    void plot()
    {
        set_pixel(x, y);
    }
    double distance(Point p)
    {
        return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2));
    }
    void drawCircle(double radius1, int c = 1)
    { // 1 = black, 2 = red
        double a1 = x;
        double b1 = y;
        int a = a1 * 800;
        int b = b1 * 800;
        int radius = radius1 * 800;
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
class ppd
{
public:
    Point p1;
    Point p2;
    double d;
    ppd(Point pone, Point ptwo)
    {
        p1 = pone;
        p2 = ptwo;
        d = p1.distance(p2);
    }
};
list<Point> ps;
void gen_points(int n)
{
    ofstream myfile;
    myfile.open("points.txt");
    myfile << setprecision(23);
    myfile << fixed;
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        Point p = Point(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
        //ps.push_back(p);

        myfile << p.x << "  " << p.y << "\n";
    }
}
void get_points()
{
    ps.clear();
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
            ps.push_back(p);
        }
    }
}
bool comp(Point a, Point b)
{
    if (a.x == b.x)
    {
        return a.y < b.y;
    }
    return a.x < b.x;
}
ppd merge_helper(Point array[], int const f, int const l)
{

    int mid = f + (l - f) / 2;
    if (l - f == 2)
    {
        //cout << f << " " << l << "\n";
        //cout << ppd(array[f], array[l]).d << "\n";
        return ppd(array[f], array[l]);
    }
    if (l - f == 3)
    {
        //cout << f << " " << l << "\n";
        ppd pp1 = ppd(array[f], array[l]);
        ppd pp2 = ppd(array[f + 1], array[l]);
        ppd pp3 = ppd(array[f], array[f + 1]);
        if (pp1.d < pp2.d and pp1.d < pp3.d)
        {
            //cout << pp1.d << "\n";
            return pp1;
        }
        else if (pp2.d < pp3.d)
        {
            //cout << pp2.d << "\n";
            return pp2;
        }
        //cout << pp3.d << "\n";
        return pp3;
    }

    Point m = array[mid];
    ppd min_points1 = merge_helper(array, f, mid);
    ppd min_points2 = min_points1;
    min_points2 = merge_helper(array, mid, l);

    double minx = m.x - min(min_points1.d, min_points2.d);
    double maxx = m.x + min(min_points1.d, min_points2.d);
    ppd mind = min_points2;

    if (min_points1.d < min_points2.d)
    {
        mind = min_points1;
    }

    int curr1 = mid;
    while (curr1 >= f and array[curr1].x < maxx)
    {
        int curr2 = mid;
        while (curr2 <= l and curr2 >= f and array[curr2].x > minx)
        {
            if (array[curr1].x != array[curr2].x and array[curr1].distance(array[curr2]) < mind.d)
            {
                mind = ppd(array[curr1], array[curr2]);
            }
            curr2 -= 1;
        }
        curr1 += 1;
    }
    //cout << mind.d << "\n";
    return mind;
}

ppd brute_force()
{
    double min = INT_MAX;
    Point a;
    Point b;
    for (list<Point>::iterator i = ps.begin(); i != ps.end(); ++i)
    {
        for (list<Point>::iterator j = next(i); j != ps.end(); j++)
        {
            double dist = (*i).distance(*j);
            if (dist < min)
            {
                min = dist;
                a = *i;
                b = *j;
            }
        }
    }
    a.drawCircle(2.0 / 800.0, 2);
    b.drawCircle(2.0 / 800.0, 2);
    a.drawCircle(3.0 / 800.0, 2);
    b.drawCircle(3.0 / 800.0, 2);
    ppd out = ppd(a, b);
    return out;
}
ppd merge_sort()
{

    ps.sort(comp);
    Point arr[ps.size()];
    int k = 0;
    for (Point const &i : ps)
    {
        arr[k++] = i;
    }
    ppd shortest = merge_helper(arr, 0, ps.size() - 1);
    Point a = shortest.p1;
    Point b = shortest.p2;
    a.drawCircle(2.0 / 800.0, 2);
    b.drawCircle(2.0 / 800.0, 2);
    a.drawCircle(3.0 / 800.0, 2);
    b.drawCircle(3.0 / 800.0, 2);
    return shortest;
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
void part1()
{
    gen_points(60);
    get_points();
    brute_force();
    ppd smalldist = brute_force();
    cout << smalldist.d << "\n";
    arrToFile();
}
void part2()
{
    //gen_points();
    get_points();
    ppd smalldist = merge_sort();
    arrToFile();
    cout << smalldist.d;
}

int main()
{
    part1();
    part2();
}