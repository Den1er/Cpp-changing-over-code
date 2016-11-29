//
// Created by 苏思睿 on 16/6/20.
//
#include "Area2Sat.h"
double radian1(double d)
{
    return d * PI / 180.0;

}
int GetAddressID(double lon,double lat){//用户读取区域号
    int mat[12][4]={
            0,1,2,3,
            4,5,6,7,
            8,9,10,11,
            12,13,14,15,
            16,17,18,19,
            20,21,22,23,
            24,25,26,27,
            28,29,30,31,
            32,33,34,35,
            36,37,38,39,
            40,41,42,43,
            44,45,46,47
    };
    int r,c;
    if(lon==180) r = 5;
    else if(lon==-180) r = 11;
    else if(lon < 0) r = (-lon/30)+6;
    else r = lon/30;
    if(lat == 90) c = 1;
    else if(lat == -90) c = 3;
    else if(lat < 0) c = (-lat/45)+2;
    else c = lat/45;
    return mat[r][c];
}

int Fin(int t,Coord mat[]){//读入2进制位置信息
    int start_time=0;
    ifstream infile;
    if(!infile)
    {
        cerr<<"open error!"<<endl;
        //exit(1);
    }
    infile.open("/Users/Den1er/Desktop/a_2h.bin",ios::in/*|ios::binary*/);
    for(int i=0; i<48; i++){
        start_time = i*7201+t;
        infile.seekg(sizeof(Coord) * start_time,ios_base::beg);
        infile.read((char*)(mat+i), sizeof(Coord));
    }
    infile.close();
    return 0;
}

double Dec(double lat){//得出修正的经度差
    double lat_fix = lat;
    if(lat>86) lat_fix =86;
    if(lat<-86) lat_fix =-86;
    double a = radian1(4);
    double b = radian1(lat_fix);
    double c = tan(a)*tan(b);
    double dec = asin(c);
    return double((dec/PI)*180);
}

void FixLon(int t,Coord sat[]){//修正经度
    Coord later[48];
    Fin(t+1,later);
    double out;
    for(int i=0; i<48; i++){
        if(sat[i].x>later[i].x){
            out = sat[i].y+Dec(sat[i].x);
            if(out>180){
                out = -180+(sat[i].y+Dec(sat[i].x)-180);
            }
            else if(out<-180){
                out = 180+(sat[i].y+Dec(sat[i].x)+180);
            }
            sat[i].y = out;
        }
        else {
            out = sat[i].y-Dec(sat[i].x);
            if(out<-180){
                out = 180+(out+180);
            }
            else if(out>180){
                out = -180+(out-180);
            }
            sat[i].y = out;
        }
    }
}

void FindSat(int t,int s_a[],int a_s[]){//得到映射表
    int sheet_s_a[48] =  {99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99};
    int sheet_a_s[48] =  {99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99,
                          99,99,99,99,99,99,99,99};
    Coord sats[48];
    Fin(t,sats);
    FixLon(t,sats);
    for(int i=0; i<48; i++){
        sheet_s_a[i] = GetAddressID(sats[i].y,sats[i].x);
        //cout<<i<<"\t"<<sizeof(Coord)<<"\t"<<sats[i].y<<endl;
    }
    for(int i=0; i<48; i++){
        int k = sheet_s_a[i];
        if(k%2==0){
            sheet_a_s[k] = i;
        }
    }
    for(int i=0; i<12; i++){
        int a = sheet_a_s[4*i];
        int b = sheet_a_s[4*i+2];
        if(a%8==0&&b%8==7){
            sheet_a_s[4*i+1] = a+1;
            sheet_a_s[4*i+3] = b-1;
        }
        else if(a%8==7&&b%8==0){
            sheet_a_s[4*i+1] = a-1;
            sheet_a_s[4*i+3] = b+1;
        }
        else if(a%8==0&&b%8==1){
            sheet_a_s[4*i+1] = a+7;
            sheet_a_s[4*i+3] = b+1;
        }
        else if(a%8==1&&b%8==0){
            sheet_a_s[4*i+1] = a+1;
            sheet_a_s[4*i+3] = b+7;
        }
        else if(a%8==6&&b%8==7){
            sheet_a_s[4*i+1] = a-1;
            sheet_a_s[4*i+3] = b-7;
        }
        else if(a%8==7&&b%8==6){
            sheet_a_s[4*i+1] = a-7;
            sheet_a_s[4*i+3] = b-1;
        }
        else if(a<b){
            sheet_a_s[4*i+1] = a-1;
            sheet_a_s[4*i+3] = b+1;
        }
        else if(a>b){
            sheet_a_s[4*i+1] = a+1;
            sheet_a_s[4*i+3] = b-1;
        }
    }
    for(int i=0; i<48; i++){
        s_a[i] = sheet_s_a[i];
    }
    for(int i=0; i<48; i++){
        int n = sheet_a_s[i];
        s_a[n] = i;
        a_s[i] = sheet_a_s[i];
    }
}