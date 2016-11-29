//User.h
//
// Created by 苏思睿 on 16/6/14.
//

#ifndef SATELITE_USER_H
#define SATELITE_USER_H
#include "Sat.h"
class User{
private:
    pos user_pos;
    queue<Packet> pkt;
    vector<AdInfo>ad_info;
    UserAddress u_add;
    int order;
public:
//  int CalculateUserArea(pos* user_pos);
    pos* GetPUserPos();
    vector<AdInfo>* GetPAdInfo();
    UserAddress* GetPUserAddr();
    void SetPos(pos n);
    void SetUserAddress();
    void UpdateAdInfo(Packet *pkt,Sat* sat[]);//
    AdInfo* ChooseSat();
    queue<Packet>* GetPacketQueue();
    void SendPkt(Packet* pkt,Sat* sat[]);
    void FindInSatCoverage(Sat *sat[]);
    void ReceivePkt(Packet *pkt);//除广播包外的其他包
    Packet* CreateRegistPkt();
    void DisplayAd();
    void SetUserOrder(int a);
    void ProcessDataPkt();
    void DisplayRegistAndRelaySat(int stoa[],int atos[]);
	//2016/06/16 补充 by_sherry
	Packet* CreateDataPkt(Ip d_ip);

};
#endif //SATELITE_USER_H
//#include "Satelite.h"
