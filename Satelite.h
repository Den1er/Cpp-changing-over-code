//Sateite.h
//
// Created by 苏思睿 on 16/5/17.
//

#ifndef SATELITE_SATELITE_H
#define SATELITE_SATELITE_H



using namespace std;
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>
#include <math.h>

#define PI                      3.1415926
#define EARTH_RADIUS            6378.137        //地球近似半径
const int N=1; //每次刷新N条数据
const int U=2; //注册用户数

const int M=2; //用户收到的广播包数
const double R=2700.0;             //451.262
/*union Ip{
    struct
    {
        unsigned char seg0;
        unsigned char seg1;
        unsigned char seg2;
        unsigned char seg3;


    unsigned int address;
    bool Ip::operator==(const Ip& rhs)
    {
        return( seg0 == rhs.seg0) && (seg1 == rhs.seg1) && (seg2 == rhs.seg2) && (seg3 == rhs.seg3);

    }
};*/
struct Ip{
    int seg0;
    int seg1;
    int seg2;
    int seg3;
    bool operator==(const Ip& rhs)
    {
        return( seg0 == rhs.seg0) && (seg1 == rhs.seg1) && (seg2 == rhs.seg2) && (seg3 == rhs.seg3);

    }
};


struct pos{
    double lat;
    double lon;

};
struct sat_id{
    int orbit_num,num;
    bool operator==(const sat_id& rhs)
    {
        return( orbit_num == rhs.orbit_num) && (num == rhs.num);

    }
};


enum pkt_type{AdPkt,RegistPkt,DataPkt};

/*double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}*/

//已知经纬度求两点距离求距离


class UserAddress{
private:
    Ip Id;
public:
    //  UserAddress(int=0,int=0,int=0,int=0);  //构造函数
    void SetUserAddress();  //调用一下两个函数
    void SetAreaID(double longtitude,double latitude);       //查找一张全局表
    void SetUserID(vector<int> random,int order);       //生成唯一性的用户地址,需定义一个全局函数,
    void GetUserAddress();  //用四段法显示IP地址
    int GetAreaID();
    void GetUserID();
    int GetSeg2();
    int GetSeg1();
    int GetSeg0();
    Ip GetIp();
    void SetAreaidTest(int n);
};

class SatAddress{
private:
    Ip area_id;
    int satid;
public:
    void SetSatAddressToGround(int area[]);
    Ip GetSatAddressToGround();
    void SetSatID();
    int GetSatID();
    void SetAreaID(int area[]);                 //根据映射表得到区域号
    int GetAreaID();
    //void SetOrbitNum(sat_id sateid);
    //int GetOrbitNum();
    //void SetNum(sat_id sateid);
    //int GetNum();
    void SetSatAddressIp(Ip a);
    void SetSatAddressSatId(int b);
    void DisplayIp();
    void SetAreaID_test(int n);
    void SetSatid(int n);
};

struct RegistUser
{
    int regist_sat_area; //正确的分区对应卫星
//  char regist_sat_id;
    int user_area;
    SatAddress relay_sat;  //中继卫星
//  char relay_sat_id;
    Ip user_id;
    bool regist;  //表示是否中继
};

class Packet{
private:
    enum pkt_type type;
    Ip s_addr,d_addr;
    SatAddress next_hop,relay_sat_id;
    bool registornot;
    pos satpos;

public:
    void CreatePkt(int n,Ip s_addr,Ip d_address,SatAddress n_hop,SatAddress relaysatid,bool regist,pos sat_pos);
    void SetSAddr(Ip s_addr);
    void SetDAddr(Ip d_address);
    void SetNextHop(SatAddress n_hop);
    void SetRelaySatID(SatAddress relaysatid);
    void SetRegist(bool n);
//    void SendPkt();
    //   void DropPkt();
    pkt_type GetType();
    Ip GetS_ip();
    Ip GetD_ip();
    SatAddress GetNextHop();
    SatAddress GetRelaySatId();
    pos GetSatPos();
    bool GetRegist();
    vector<int> path;
};

class AreaInfo{
    int area;//分区号
    pos area_center;
};

class SatToArea{
    int area;//分区号
    SatAddress sat_id;
    void UpdateSatToArea(int area,SatAddress sat_id);
};
/*
class UserInfo{
private:
    SatAddress relay_sat_id;
    struct user_id{
        unsigned char seg0;
        unsigned char seg1;
        unsigned char seg2;
    };
    double timestamp;
    bool regist;

};
 */


struct AdInfo{

    pos satPos;
    SatAddress sat_addr;
    bool regist = 0; //用于判断注册卫星
};
struct RoutingTable{
    int des_Ip;
    int n_hop;
};


#endif //SATELITE_SATELITE_H