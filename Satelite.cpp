//
// Created by 苏思睿 on 16/6/15.
//
#include "Satelite.h"

/*
UserAddress::UserAddress(int s0, int s1, int s2, int s3) {
    Id.seg3 = s0;
    Id.seg2 = s1;
    Id.seg1 = s2;
    Id.seg0 = s3;
}*/
/*int GetAddressID(double lon,double lat) {//通过经纬度判断用户处于哪个分区
    int mat[8][6] = {1, 2, 3, 4, 5, 6,
                     7, 8, 9, 10, 11, 12,          //21 20 19 22 23 24
                     13, 14, 15, 16, 17, 18,       //15 14 13 16 17 18
                     19, 20, 21, 22, 23, 24,       //
                     25, 26, 27, 28, 29, 30,       //3  2  1  4  5  6
                     31, 32, 33, 34, 35, 36,       //
                     37, 38, 39, 40, 41, 42,       //45 44 43 46 47 48
                     43, 44, 45, 46, 47, 48};

    int a;
    int c;
    if (lon < 0) a = (-lon / 45) + 4;
    else a = lon / 45;
    if (lat < 0) c = (-lat / 30) + 3;
    else c = lat / 30;

    return mat[a][c];
}
 */
Ip UserAddress::GetIp() {
    return Id;
}
int UserAddress::GetAreaID() {


    return int(Id.seg3);

}

void UserAddress::GetUserAddress() {

    cout<<"UserAddress:"<<int(Id.seg3)<<"."<<int(Id.seg2)<<"."<<int(Id.seg1)<<"."<<int(Id.seg0)<<endl;


}

void UserAddress::GetUserID() {

    cout<<"UserID:"<<int(Id.seg2)<<"."<<int(Id.seg1)<<"."<<int(Id.seg0)<<endl;

}
void UserAddress::SetAreaidTest(int n){
    Id.seg3 = n;
}

void UserAddress::SetUserID(vector<int> random,int order) {
    int temp = random[order];
    Id.seg2 = temp>>16;
    Id.seg1 = (temp&65280)>>8;     //1111111100000000
    Id.seg0 = temp&255;       //11111111

}
void UserAddress::SetAreaID(double longtitude,double latitude) {

    //Id.seg3 = GetAddressID(longtitude,latitude);
}
int UserAddress::GetSeg2()
{
    return int(Id.seg2);
}
int UserAddress::GetSeg1()
{
    return int(Id.seg1);
}
int UserAddress::GetSeg0()
{
    return int(Id.seg0);
}
void SatAddress::DisplayIp() {
    cout<<"Sat_Ip:"<<int(area_id.seg3)<<"."<<int(area_id.seg2)<<"."<<int(area_id.seg1)<<"."<<int(area_id.seg0);

}
void SatAddress::SetSatAddressIp(Ip a) {
    area_id = a;
}
void SatAddress::SetSatAddressSatId(int b) {
    satid = b;
}

void SatAddress::SetSatAddressToGround(int area[]) {
    //区域号需调用函数
    for(int i = 0;i <48;i++)
    {
        if(area[i] == satid) {
            area_id.seg3 = char(i);
            break;
        }

    }
    area_id.seg2 = 0;
    area_id.seg1 = 0;
    area_id.seg0 = 1;
}
Ip SatAddress::GetSatAddressToGround(){
    return area_id;
}
int SatAddress::GetSatID(){
    return satid;
}
void SatAddress::SetSatID() {
    area_id.seg2 = 0;
    area_id.seg1 = 0;
    area_id.seg0 = 1;
}
void SatAddress::SetAreaID(int area[]) {
    for(int i = 0;i <48;i++)
    {
        if(area[i] == satid) {
            area_id.seg3 = i;
            break;
        }

    }
}
void SatAddress::SetAreaID_test(int n){
    area_id.seg3 = n;
}
void SatAddress::SetSatid(int n){
    satid = n;
}

int SatAddress::GetAreaID() {
    return area_id.seg3;
}
/*
void SatAddress::SetNum(sat_id sateid) {
    //调用STK的信息
    satid.num = sateid.num;
}

int SatAddress::GetNum() {
    return satid.num;
}

void SatAddress::SetOrbitNum(sat_id sateid) {
    //调用STK的信息
    satid.orbit_num = sateid.orbit_num;
}
int SatAddress::GetOrbitNum() {
    return satid.orbit_num;
}
 */
pos Packet::GetSatPos()
{
    return satpos;
}
void Packet::SetSAddr(Ip s_address) {
    s_addr = s_address;
}
void Packet::SetDAddr(Ip d_address) {
    d_addr = d_address;
}
void Packet::SetNextHop(SatAddress n_hop) {
    next_hop = n_hop;
}
void Packet::SetRelaySatID(SatAddress relaysatid) {
    relay_sat_id = relaysatid;
}
void Packet::SetRegist(bool n){
    registornot = n;
}

pkt_type Packet::GetType() {
    return type;
}
Ip Packet::GetS_ip() {
    return s_addr;
}
Ip Packet::GetD_ip() {
    return d_addr;
}
SatAddress Packet::GetNextHop() {
    return next_hop;
}
SatAddress Packet::GetRelaySatId() {
    return relay_sat_id;
}
bool Packet::GetRegist() {
    return registornot;
}
void Packet::CreatePkt(int n,Ip s_addr,Ip d_address,SatAddress n_hop,SatAddress relaysatid,bool regist,pos sat_pos) {
    if(n>=3||n<0)
        cout<<"n should be 0 or 1 or 2 , Please recreate.";
    type = (enum pkt_type)n;//此处有错
    SetSAddr(s_addr);
    SetDAddr(d_address);
    SetNextHop(n_hop);
    SetRelaySatID(relaysatid);
    satpos = sat_pos;
    registornot = regist;
}
