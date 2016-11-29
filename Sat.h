//Sat.h
//
// Created by 苏思睿 on 16/6/15.
//

#ifndef SATELITE_SAT_H
#define SATELITE_SAT_H


#include "Satelite.h"
class User;
class Sat{//卫星需要管理1.自己的用户表 2.卫星与地面分区的映射表 3.路由表、转发表 4.
private:
    pos sat;
    SatAddress sat_addr;
    queue<Packet> pkt;
    Packet adpkt;
    //SatToArea sat_to_area;
    vector<RegistUser> usertable;
    vector<RoutingTable> rt;
    int adClock = 0;
    double r;
public:
    //void UpdatePos();
    //void Calculate(pos sat,double open_angle);
    //void ProcessHandoffClock(int handoffclock);
    //void ProcessHAdClock(int AdClock);
    double SatCover(double h,double B);
    RegistUser Regist(int user_area,int user_id);
    void ReadFileAndApdate(string filename,int route[]);
    pos GetPos();
    SatAddress GetAdd();
    SatAddress* GetPAdd();
    double GetRadius();
    queue<Packet>* GetPacketQueue();
    void ReceivePacket(Packet* pkt,Sat *sat[],User *user[],int reflect[]);
    void SendPacket2Sat(Packet* pkt,Sat* sat);
    void SendPacket2User(Packet* pkt,User* user);
    void SendAdPacket();
    Sat* FindSat(Sat *sat[],Packet *pkt);
    void ProcessPacketQueue(Sat *sat[],User *user[],int reflect[]);
    Packet GetAdpkt();
    void SetSatPos(pos satpos);
   // void SetSatId(sat_id sat)
    void DisplayAdpkt();
    void DisplayRegistUser();
	void UpdateRoutingTable(int d_Ip,Sat* sat[]);
	void UpdateRoutingTableTest(int d_Ip,int next_hop);
	Sat* FindNextHop(Sat *sat[], Packet *pkt,int reflect[]);
	Sat* LookUpToRoutingTable(Sat* sat[],Packet *pkt,int reflect[]);

	//2016/06/16 补充 by_sherry
	User* FindUser(User *user[],Packet *pkt);
	Sat* FindRelaySat(Sat *sat[],Packet *pkt);
};
#endif //SATELITE_SAT_H
