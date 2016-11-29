//User.cpp
//
// Created by 苏思睿 on 16/6/14.
//
//#include "Satelite.h"
#include "User.h"
const int SAT_NUM = 48;
double radian(double d)
{
    return d * PI / 180.0;   //角度1˚ = π / 180][{
}

double GetDistance(double lon1,double lat1,double lon2,double lat2)            //东经为正，北纬为正
{
    double radLat1 = radian(lat1);
    double radLat2 = radian(lat2);
    double a = radLat1 - radLat2;
    double b = radian(lon1) - radian(lon2);

    double dst = 2 * asin((sqrt(pow(sin(a / 2), 2) + cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2) )));

    dst = dst * EARTH_RADIUS;
    dst= round(dst * 10000) / 10000;
    return dst;
}


void User::SetPos(pos n) {
    user_pos = n;
}
pos* User::GetPUserPos(){
    return &user_pos;
}
UserAddress* User::GetPUserAddr(){
    return &u_add;
}
vector<AdInfo>* User::GetPAdInfo(){
    return &ad_info;
}

queue<Packet>* User::GetPacketQueue(){
    return &pkt;
}
void User::SetUserOrder(int a){
    order = a;
}
void User::ProcessDataPkt(){
    while(!pkt.empty())
    {
        ReceivePkt(&(pkt.front()));
        pkt.pop();

    }
}
void User::DisplayRegistAndRelaySat(int stoa[],int atos[]){
    for(int i=0;i<ad_info.size();i++)
    {
        if(ad_info[i].regist==1)
        {
            if(stoa[ad_info[i].sat_addr.GetSatID()]==GetPUserAddr()->GetAreaID())
            {
                cout<<"User"<<order<<"\t"<<"RegistSat:"<<ad_info[i].sat_addr.GetSatID()<<"   without RelaySat"<<endl;
            }
            else
            {
                cout<<"User"<<order<<"\t"<<"RegistSat:"<<atos[GetPUserAddr()->GetAreaID()]<<"\t"<<"RelaySat:"<<ad_info[i].sat_addr.GetSatID()<<endl;
            }
            break;
        }
    }
}

void User::ReceivePkt(Packet *pkt)
{
    switch(pkt->GetType())
    {
        case DataPkt:
            //  pkt->DropPkt();
            cout<<"User"<<order<<"'ve received a Datapkt!!"<<endl;
            cout<<"Pacet Path:";
            for(int i = 0;i<pkt->path.size();++i)
            {
                cout<<pkt->path[i]<<"\t";
            }
            cout<<endl<<endl;
            break;
        case AdPkt:
            //  pkt->DropPkt();
            break;
        case RegistPkt:
            //  pkt->DropPkt();
            break;
    }
}
/*************************************用户发送注册包的整个过程*****************************************************************/
void User::FindInSatCoverage(Sat *sat[])//发生变化了才重新注册!!!!!改!!!
{
    //清空注册表

    ad_info.clear();

    for(int i=0;i<SAT_NUM;i++){
        double d=GetDistance(user_pos.lon,user_pos.lat,sat[i]->GetPos().lon,sat[i]->GetPos().lat);
        if(d<R)
        {
            Packet q = sat[i]->GetAdpkt();
            Packet* p = &q;
            UpdateAdInfo(p,sat);  //表示在卫星范围内，这里的调用需要参考ssr的卫星类中的广播包队列如何实现
        }
        else continue;
    }

}
void User::UpdateAdInfo(Packet *pkt,Sat* sat[])//(int sat_id,Pos satPos,SatAddress sat_addr,bool regist)
{   //收到广播包，（广播包要先计算用户所处哪些卫星范围，记录下这些卫星信息，然后到相应卫星的广播包队列中去取包）
    AdInfo ad;
    ad.sat_addr.SetSatAddressIp(pkt->GetS_ip());
    ad.satPos=pkt->GetSatPos();
    ad.regist=pkt->GetRegist();
    for(int i = 0;i<SAT_NUM;i++)
    {
        if((sat[i]->GetPos().lat==pkt->GetSatPos().lat)&&(sat[i]->GetPos().lon==pkt->GetSatPos().lon))
        {
            ad.sat_addr.SetSatAddressSatId(sat[i]->GetAdd().GetSatID());
            break;
        }
    }
    ad_info.push_back(ad);
}

AdInfo* User::ChooseSat()
{
    double x[ad_info.size()];
    double x_min=R;  ////R为卫星覆盖半径
    int i_min=0;

    for(int i=0;i<ad_info.size();i++)
    {
        x[i]=GetDistance(user_pos.lon,user_pos.lat,ad_info[i].satPos.lon,ad_info[i].satPos.lat);
        if(x[i]<x_min){ i_min=i;}            //随意选择一颗通信卫星
        else{continue;}
    }
    int flag=0;//表示没有分区对应卫星
    int i_regist;//用于表示用户发包的对应卫星广播
    for(int i=0;i<ad_info.size();i++)
    {
        if(ad_info[i].sat_addr.GetAreaID()!=u_add.GetAreaID()){continue;}
        else{                               //找到覆盖并且能够
            flag=1;
            i_regist=i;
            break;
        }
    }
    if(flag==1){ad_info[i_regist].regist=1;}            //有可以直接注册的卫星
    else {ad_info[i_min].regist=1;}{}                   //只能找到所谓的中继卫星来通信(特殊用户)

    for(int i=0;i<ad_info.size();i++)
    {   //缺少卫星Ip的显示
 /*      cout<<"             User"<<order<<"   ChooseSatTable"<<endl;
        ad_info[i].sat_addr.DisplayIp();
        cout<<"\t"<<"Sat_Id:"<<ad_info[i].sat_addr.GetSatID()<<"\t"<<"Sat_Pos:"<<ad_info[i].satPos.lat<<","<<ad_info[i].satPos.lon<<"\t"<<"Regist:"<<ad_info[i].regist<<"\t"<<"Distance:"<<x[i];

        cout<<endl<<endl;
*/
    }
    for(int i=0;i<ad_info.size();i++)
    {
        if(ad_info[i].regist==1) return &ad_info[i];
        else continue;
    }
}
Packet* User::CreateRegistPkt()
{
    Packet *pkt;
    pkt = new Packet;
    int i=0;
    for(i;i<ad_info.size();i++)
    {
        if(ad_info[i].regist==1) break;
        else continue;
    }
    Ip s_addr=u_add.GetIp();
    Ip d_addr={1,0,0,u_add.GetAreaID()};
    pos sat_pos=ad_info[i].satPos;
    SatAddress n_hop=ad_info[i].sat_addr;

    SatAddress relaysatid;
    bool relayornot;

    if(ad_info[i].sat_addr.GetAreaID()==u_add.GetAreaID())
    {
        relaysatid.SetSatAddressIp({0,0,0,0});
        relaysatid.SetSatid(-1);//运算符重载问题
        relayornot=0;
    }
    else{
        relaysatid=ad_info[i].sat_addr;
        relayornot=1;
    }
    pkt->CreatePkt(1,s_addr,d_addr,n_hop,relaysatid,relayornot,sat_pos);
    return pkt;
}
void User::SendPkt(Packet* pkt,Sat* sat[])
{
    for(int i = 0;i<SAT_NUM;i++)
    {
        if(sat[i]->GetAdd().GetAreaID()!=pkt->GetNextHop().GetSatAddressToGround().seg3){continue;}//Problem
        else{                                           //发给下一跳的卫星
            sat[i]->GetPacketQueue()->push(*pkt);
           // cout<<"SEND A REGIST to"<<"sat"<<sat[i]->GetAdd().GetSatID()<<endl;
            delete pkt;
            //cout<<"sat"<<i<<"   received a pkt";
        }
    }
}
void User::DisplayAd(){
    for(int i=0;i<ad_info.size();i++)
    {
        cout<<"             User1"<<"   ReceiveAdTable"<<endl;
        cout<<"Sat_Ip:"<<ad_info[i].sat_addr.GetSatAddressToGround().seg3<<"."<<ad_info[i].sat_addr.GetSatAddressToGround().seg2<<"."<<ad_info[i].sat_addr.GetSatAddressToGround().seg1<<"."<<ad_info[i].sat_addr.GetSatAddressToGround().seg0;
        cout<<"\t"<<"Sat_Id:"<<ad_info[i].sat_addr.GetSatID()<<"\t";
        cout<<"Sat_Pos:"<<ad_info[i].satPos.lat<<","<<ad_info[i].satPos.lon<<"\t";
        cout<<"Regist:"<<ad_info[i].regist<<endl<<endl;
    }
}
/*************************************用户发送注册包的整个过程*****************************************************************/


//2016/06/16 补充创建数据包的过程 by_sherry
Packet* User::CreateDataPkt(Ip d_ip)      //数据包有的内容1.包类型 2.用户地址 3.下一跳
{
	Packet *pkt;
    pkt = new Packet;
    int i=0;
    for(i;i<ad_info.size();i++)
    {
        if(ad_info[i].regist==1) break;
        else continue;
    }
    Ip s_addr=u_add.GetIp();
    Ip d_addr=d_ip;  //目的地址为另一个用户的地址，这里先自己设一个
    pos sat_pos=ad_info[i].satPos;  //数据包中不需要卫星地址信息，这里没有动，也可以赋值为0
    SatAddress n_hop=ad_info[i].sat_addr;  

	SatAddress relaysatid;
    relaysatid.SetSatAddressIp({-1,-1,-1,-1});  //数据包中的中继卫星地址是由目的地址对应卫星来填的，不是用户，故生成数据包时先把该值赋为0
    relaysatid.SetSatid(-1);
	bool relayornot=0; //数据包并不使用该标志位，因此均设为0

    pkt->CreatePkt(2,s_addr,d_addr,n_hop,relaysatid,relayornot,sat_pos);
    return pkt;
};