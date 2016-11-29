//
// Created by 苏思睿 on 16/6/20.
//

#ifndef SAT_MAC_AREA2SAT_H
#define SAT_MAC_AREA2SAT_H
#include <iostream>
#include <math.h>
#define PI                      3.1415926
#define EARTH_RADIUS            6378.137
#define DATA                    69168    //48*(24*60+1)
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <cstdlib>

using namespace std;


struct Coord {double x; double y;};
double radian1(double d);
int GetAddressID(double lon,double lat);
int Fin(int t,Coord mat[]);
double Dec(double lat);
void FixLon(int t,Coord sat[]);
void FindSat(int t,int s_a[],int a_s[]);

#endif //SAT_MAC_AREA2SAT_H
