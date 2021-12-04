#include <stdio.h>
#include <algorithm>
#include <list>
#include <iterator>
#include <chrono>
#include <climits>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unordered_map>
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
class LongPoint
{
private:
    long long x;
    long long y;

public:
    LongPoint(long long x1, long long y1)
    {
        x = x1;
        y = y1;
    };
    LongPoint()
    {
        x = 0;
        y = 0;
    };
    double getx() const
    {
        return x;
    }
    double gety() const
    {
        return y;
    }
    void plot()
    {
        set_pixel(x, y);
    }
    double distance(LongPoint p)
    {
        return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2));
    }
    bool operator==(const LongPoint &p) const
    {
        return p.getx() == x && p.gety() == y;
    };
};
class MyHashFunction
{
public:
    double operator()(const LongPoint &p) const
    {
        return p.getx() + p.gety();
    }
};
class ppd
{
private:
    Point p1;
    Point p2;
    double d;

public:
    ppd(Point pone, Point ptwo)
    {
        p1 = pone;
        p2 = ptwo;
        d = p1.distance(p2);
    }
    double getd()
    {
        return d;
    }
    Point getp1()
    {
        return p1;
    }
    Point getp2()
    {
        return p2;
    }
};

list<Point> ps;
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
    ps.clear();
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
            //p.drawCircle(3.0 / 800.0);
            ps.push_back(p);
            vs.push_back(p);
        }
    }
}
bool comp(Point a, Point b)
{
    if (a.getx() == b.getx())
    {
        return a.gety() < b.gety();
    }
    return a.getx() < b.getx();
}
bool compy(Point a, Point b)
{
    if (a.gety() == b.gety())
    {
        return a.getx() < b.getx();
    }
    return a.gety() < b.gety();
}
void toFile(ppd smalldist, chrono::steady_clock::time_point begin, chrono::steady_clock::time_point end, int method)
{
    ofstream r;
    r << setprecision(23) << fixed;
    cout << setprecision(23) << fixed;
    if (method == 0)
    {
        r.open("results.txt");
        r << "Brute Force";
        cout << "Brute Force";
    }
    else if (method == 1)
    {
        r.open("results.txt");
        r << "Intermediary Recursive";
        cout << "Intermediary Recursive";
    }
    else if (method == 2)
    {
        r.open("results.txt", std::ios_base::app);
        r << "Complete Recursive";
        cout << "Complete Recursive";
    }
    else if (method == 3)
    {
        r.open("results.txt", std::ios_base::app);
        r << "Linear Method";
        cout << "Linear Method";
    }
    r << " (" << ps.size() - 1 << " points): \n";
    r << "Points: (" << smalldist.getp1().getx() << "," << smalldist.getp1().gety() << ") & "
      << "(" << smalldist.getp2().getx() << "," << smalldist.getp2().gety() << ")"
      << "\n";
    r << "Distance: " << smalldist.getd() << "\n";
    r << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds\n\n";
    cout << " (" << ps.size() << " points): \n";
    cout << "Points: (" << smalldist.getp1().getx() << "," << smalldist.getp1().gety() << ") & "
         << "(" << smalldist.getp2().getx() << "," << smalldist.getp2().gety() << ")"
         << "\n";
    cout << "Distance: " << smalldist.getd() << "\n";
    cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds\n\n";
}
ppd merge_helper(vector<Point> &array, int const f, int const l)
{
    int mid = f + (l - f) / 2;
    if (l - f == 1)
    {
        return ppd(array[f], array[l]);
    }
    if (l - f == 2)
    {
        ppd pp1 = ppd(array[f], array[f + 2]);
        ppd pp2 = ppd(array[f + 1], array[f + 2]);
        ppd pp3 = ppd(array[f], array[f + 1]);
        if (pp1.getd() < pp2.getd() and pp1.getd() < pp3.getd())
        {
            //cout << pp1.d << "\n";
            return pp1;
        }
        else if (pp2.getd() < pp3.getd())
        {
            //cout << pp2.d << "\n";
            return pp2;
        }
        //cout << pp3.d << "\n";
        return pp3;
    }

    Point m = array[mid];
    ppd min_points1 = merge_helper(array, f, mid);
    ppd min_points2 = merge_helper(array, mid + 1, l);
    vector<Point> left_points = vector<Point>();
    vector<Point> right_points = vector<Point>();
    double minx = m.getx() - min(min_points1.getd(), min_points2.getd());
    double maxx = m.getx() + min(min_points1.getd(), min_points2.getd());
    ppd mind = min_points2;

    if (min_points1.getd() < min_points2.getd())
    {
        mind = min_points1;
    }
    int upperbound = mid + 1;
    int lowerbound = mid;
    while (upperbound >= f and upperbound <= l and array[upperbound].getx() < maxx)
    {
        right_points.push_back(array[upperbound]);
        upperbound += 1;
    }
    while (lowerbound <= l and lowerbound >= f and array[lowerbound].getx() > minx)
    {
        left_points.push_back(array[lowerbound]);
        lowerbound -= 1;
    }
    for (int i = 0; i < left_points.size(); i++)
    {
        for (int j = 0; j < right_points.size(); j++)
        {
            ppd pointpair = ppd(left_points[i], right_points[j]);
            if (pointpair.getd() < mind.getd())
            {
                mind = pointpair;
            }
        }
    }
    // int curr1 = mid;
    // while (curr1 >= f and curr1 <= l and array[curr1].getx() < maxx)
    // {
    //     int curr2 = mid;
    //     while (curr2 <= l and curr2 >= f and array[curr2].getx() > minx)
    //     {
    //         if (array[curr1].getx() != array[curr2].getx() and array[curr1].distance(array[curr2]) < mind.getd())
    //         {
    //             mind = ppd(array[curr1], array[curr2]);
    //         }
    //         curr2 -= 1;
    //     }
    //     curr1 += 1;
    // }
    //cout << mind.d << "\n";
    return mind;
}
ppd merge_helper_v2(vector<Point> &array, int const f, int const l)
{
    int mid = f + (l - f) / 2;
    //cout << f << " " << l << "\n";
    if (l - f == 1)
    {
        //cout << f << " " << l << "\n";
        //cout << ppd(array[f], array[l]).d << "\n";
        return ppd(array[f], array[f + 1]);
    }
    if (l - f == 2)
    {
        //cout << f << " " << l << "\n";
        ppd pp1 = ppd(array[f], array[f + 2]);
        ppd pp2 = ppd(array[f + 1], array[f + 2]);
        ppd pp3 = ppd(array[f], array[f + 1]);
        if (pp1.getd() < pp2.getd() and pp1.getd() < pp3.getd())
        {
            //cout << pp1.d << "\n";
            return pp1;
        }
        else if (pp2.getd() < pp3.getd())
        {
            //cout << pp2.d << "\n";
            return pp2;
        }
        //cout << pp3.d << "\n";
        return pp3;
    }

    Point m = array[mid];
    ppd min_points1 = merge_helper_v2(array, f, mid);
    ppd min_points2 = merge_helper_v2(array, mid + 1, l);
    ppd mind = min_points2;

    if (min_points1.getd() < min_points2.getd())
    {
        mind = min_points1;
    }

    double minx = m.getx() - mind.getd();
    double maxx = m.getx() + mind.getd();

    int upperbound = mid;
    int lowerbound = mid;
    while (upperbound > f and upperbound < l and array[upperbound].getx() < maxx)
    {
        upperbound += 1;
    }
    while (lowerbound < l and lowerbound > f and array[lowerbound].getx() > minx)
    {
        lowerbound -= 1;
    }
    vector<Point> subvector = vector<Point>();
    for (int i = lowerbound; i <= upperbound; i++)
    {
        subvector.push_back(array[i]);
    }
    sort(subvector.begin(), subvector.end(), compy);
    for (int i = 0; i < subvector.size(); i++)
    {
        int j = i + 1;
        for (int k = j; k <= j + 16 and k < subvector.size(); k++)
        {
            if (subvector[k].gety() - subvector[i].gety() > mind.getd())
            {
                break;
            }
            if (subvector[k].distance(subvector[i]) < mind.getd())
            {
                mind = ppd(subvector[k], subvector[i]);
            }
        }
        //i = j - 1;
    }
    //cout << mind.d << "\n";
    return mind;
}
ppd brute_force()
{
    double min = INT_MAX;
    Point a;
    Point b;
    for (list<Point>::iterator i = ps.begin(); i != ps.end(); i++)
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

    //Point arr[ps.size()];
    vector<Point> v;
    //int k = 0;
    for (list<Point>::iterator i = ps.begin(); i != ps.end(); i++)
    {
        v.push_back(*i);
    }
    ppd shortest = merge_helper(v, 0, ps.size() - 1);
    // Point a = shortest.getp1();
    // Point b = shortest.getp2();
    // a.drawCircle(2.0 / 800.0, 2);
    // b.drawCircle(2.0 / 800.0, 2);
    // a.drawCircle(3.0 / 800.0, 2);
    // b.drawCircle(3.0 / 800.0, 2);
    return shortest;
}
ppd merge_sort_v2()
{

    //Point arr[ps.size()];
    vector<Point> v;
    //int k = 0;
    for (list<Point>::iterator i = ps.begin(); i != ps.end(); i++)
    {
        v.push_back(*i);
    }
    ppd shortest = merge_helper_v2(v, 0, ps.size() - 1);
    // Point a = shortest.getp1();
    // Point b = shortest.getp2();
    //a.drawCircle(2.0 / 800.0, 2);
    //b.drawCircle(2.0 / 800.0, 2);
    //a.drawCircle(3.0 / 800.0, 2);
    //b.drawCircle(3.0 / 800.0, 2);
    return shortest;
}
vector<Point> randomize(vector<Point> v)
{
    for (int k = 0; k < v.size() - k; k++)
    {
        long long j = k + rand() % (v.size() - k);
        Point temp = v[k];
        v[k] = v[j];
        v[j] = temp;
    }
    for (int k = 0; k < v.size() - k; k++)
    {
        long long j = k + rand() % (v.size() - k);
        Point temp = v[k];
        v[k] = v[j];
        v[j] = temp;
    }
    return v;
}
ppd small4(vector<Point> &v)
{
    int remap = 0;
    //vector<Point> v = randomize(ps);
    unordered_map<LongPoint, Point, MyHashFunction> mapper;
    Point &a = v[0];
    Point &b = v[1];
    Point smallest1 = a;
    Point smallest2 = b;
    double delta = a.distance(b);
    long long xcoord = (long long)a.getx() / (delta / 2);
    long long ycoord = (long long)a.gety() / (delta / 2);
    mapper[LongPoint(xcoord, ycoord)] = a;
    xcoord = b.getx() / (delta / 2);
    ycoord = b.gety() / (delta / 2);
    mapper[LongPoint(xcoord, ycoord)] = b;
    for (int i = 2; i < v.size(); i++)
    {
        double lowdist = delta;
        bool enteredLoop = false;
        Point &p = v[i];
        xcoord = (long long)(p.getx() / (delta / 2));
        ycoord = (long long)(p.gety() / (delta / 2));
        for (int k = -2; k <= 2; k++)
        {
            for (int j = -2; j <= 2; j++)
            {
                if (mapper.find(LongPoint(xcoord + k, ycoord + j)) != mapper.end())
                {
                    Point temp = mapper[LongPoint(xcoord + k, ycoord + j)];
                    double disttemp = temp.distance(p);
                    if (disttemp < lowdist)
                    {
                        enteredLoop = true;
                        lowdist = disttemp;
                        smallest1 = temp;
                        smallest2 = p;
                    }
                }
            }
        }
        if (enteredLoop)
        {
            remap++;
            delta = lowdist;
            mapper.clear();
            for (int z = 0; z <= i; z++)
            {
                mapper[LongPoint(v[z].getx() / (delta / 2), v[z].gety() / (delta / 2))] = v[z];
            }
        }
        else
        {
            mapper[LongPoint(xcoord, ycoord)] = p;
        }
    }
    cout << remap << "\n";
    return ppd(smallest1, smallest2);
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
ppd part1()
{
    gen_points(60);
    get_points();
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ppd smalldist = brute_force();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    toFile(smalldist, begin, end, 0);
    //arrToFile();
    return smalldist;
}
ppd part2()
{
    gen_points(50000);

    get_points();

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ps.sort(comp);
    ppd smalldist = merge_sort();

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    toFile(smalldist, begin, end, 1);

    //arrToFile();
    return smalldist;
}
ppd part3()
{
    gen_points(100000);
    get_points();
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ps.sort(comp);
    ppd smalldist = merge_sort_v2();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    toFile(smalldist, begin, end, 2);
    //arrToFile();
    return smalldist;
}
ppd part4()
{
    get_points();
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<Point> v = randomize(vs);
    ppd smalldist = small4(v);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    toFile(smalldist, begin, end, 3);
    //arrToFile();
    return smalldist;
}

int main()
{
    //part1();
    //part2();
    part3();
    part4();
}