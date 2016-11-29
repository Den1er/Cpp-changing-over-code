//Sat.cpp
//
// Created by 苏思睿 on 16/6/15.
//

//
// Created by 苏思睿 on 16/6/14.
//
//#include"Satelite.h"
//#include "Sat.h"
#include "User.h"
#include <fstream>
const int USER_NUM = 2;

void Sat::DisplayRegistUser(){
    cout<<"             Sat"<<GetAdd().GetSatID()<<"     RegistUserTable"<<endl;
    for(int i = 0;i<usertable.size();i++)
    {
        cout<<"User_Ip:"<<usertable[i].user_id.seg3<<"."<<usertable[i].user_id.seg2<<"."<<usertable[i].user_id.seg1<<"."<<usertable[i].user_id.seg0<<"\t";
        cout<<"RelayOrNot:"<<usertable[i].regist<<"\t";
        cout<<"Relay_Ip:"<<usertable[i].relay_sat.GetSatAddressToGround().seg3<<"."<<usertable[i].relay_sat.GetSatAddressToGround().seg2<<"."<<usertable[i].relay_sat.GetSatAddressToGround().seg1<<"."<<usertable[i].relay_sat.GetSatAddressToGround().seg0<<endl;
        cout<<endl;
    }
}

void Sat::DisplayAdpkt() {
    cout<<"             Sat"<<GetAdd().GetSatID()<<"     AdpktTble"<<endl;
    cout<<"Sat_Ip:"<<sat_addr.GetSatAddressToGround().seg3<<"."<<sat_addr.GetSatAddressToGround().seg2<<"."<<sat_addr.GetSatAddressToGround().seg1<<"."<<sat_addr.GetSatAddressToGround().seg0;
    cout<<"\t"<<"Sat_Id:"<<GetAdd().GetSatID()<<"\t";
    cout<<"Sat_Pos:"<<sat.lat<<","<<sat.lon<<"\t";
    cout<<"Regist:"<<adpkt.GetRegist()<<endl<<endl;
}
void Sat::SetSatPos(pos satpos)
{
    sat = satpos;
}
void Sat::UpdateRoutingTable(int d_Ip,Sat* sat[]) {             //暂时未用到这个函数
    RoutingTable rtable;
    rtable.des_Ip = d_Ip;

    rt.push_back(rtable);
}
void Sat::UpdateRoutingTableTest(int d_Ip,int next_hop){//0->1->2
    RoutingTable rtable;
    rtable.des_Ip = d_Ip;
    rtable.n_hop = next_hop;
    rt.push_back(rtable);
}

Packet Sat::GetAdpkt() {
    return adpkt;
}
void Sat::ReadFileAndApdate(string filename,int route[]) {             //初始化路由表

    ifstream infile;
    infile.open(filename);
    if(!infile)
    {
        cerr<<"open error!"<<endl;
        exit(1);
    }
    int i = 0;
    while(!infile.eof())
    {

        infile>>route[i];
        //cout<<data[i]<<"\t"<<i<<endl;
        ++i;
    }
    for(int i = 0;i<48;i++)
    {
        RoutingTable rou;
        rou.des_Ip = i;
        rou.n_hop = route[i];
        rt.push_back(rou);
    }
}
/*===================================卫星处理队列函数=====================================================*/
void Sat::ProcessPacketQueue(Sat *sat[],User *user[],int reflect[]) {
    while(!pkt.empty())
    {
        ReceivePacket(&(pkt.front()),sat,user,reflect);
        pkt.pop();
    }
}
void Sat::ReceivePacket(Packet *pkt,Sat *sat[],User *user[],int reflect[]) {
    switch(pkt->GetType()){
        case RegistPkt:
            //cout<<"Sat"<<GetAdd().GetSatID()<<":RECEIVE"<<endl;

            if(pkt->GetD_ip().seg3==sat_addr.GetAreaID())
            {
                if(pkt->GetRegist()==0)                                 //没有中继的情况
                {
                    vector<RegistUser>::iterator iter = usertable.begin();
                    for (; iter != usertable.end( ); ++iter)
                    {
                        if(iter->user_id==pkt->GetS_ip())               //若找到了原有的用户,则删除原有项,并且更新现有的
                        {
                            usertable.erase(iter);
                            //cout<<"I delete U"<<endl<<endl;

                            break;
                        }
                    }
                    RegistUser User;                                    //未找到则直接更新
                    User.user_id = pkt->GetS_ip();
                    User.regist = 0;
                    User.relay_sat.SetSatAddressIp({0,0,0,0});
                    //cout<<"Sat"<<GetAdd().GetSatID()<<"\t"<<"user_id:"<<User.user_id.seg3<<"."<<User.user_id.seg2<<"."<<User.user_id.seg1<<"."<<User.user_id.seg0<<endl<<endl;
//                    User.relay_sat.SetAreaID();
                    usertable.push_back(User);
                }
                else                                                    //有中继的情况
                {
                    bool flag = 0;        //判断是否存在用户信息
                    vector<RegistUser>::iterator iter = usertable.begin();
                    for (; iter != usertable.end( ); ++iter)
                    {
                        if(iter->user_id==pkt->GetS_ip())               //如果原有的用户表里有该用户
                        {
                            if(iter->regist==1){                        //原用户表中中继信息为1的时候,更新用户表
                                usertable.erase(iter);
                                RegistUser User;
                                User.user_id = pkt->GetS_ip();
                                User.regist = 1;
                                User.relay_sat = pkt->GetRelaySatId();
                                usertable.push_back(User);
                                flag = 1;
                            }//表中的用户中继信息若为0,则丢弃该包;若为1,则更新
                            else {                              //未测试
                                flag = 1;
                                break;
                            }
                        }
                        //cout<<"Sat"<<GetAdd().GetSatID()<<":Not me"<<endl;
                    }
                    if(flag==0)                                         //若原表里没有用户的信息,则加上
                    {
                        RegistUser User;
                        User.user_id = pkt->GetS_ip();
                        User.regist = 1;
                        User.relay_sat = pkt->GetRelaySatId();
                        usertable.push_back(User);
                        //cout<<"Sat"<<GetAdd().GetSatID()<<":I've added U"<<endl;
                    }
                }
            }
            else                                                        //目的地址的区域号与当前卫星的区域号不同,
            {
                if(pkt->GetNextHop().GetAreaID()==sat_addr.GetAreaID())
                {
                    //给目的卫星发包
                    //找到目的卫星
                    SendPacket2Sat(pkt,FindSat(sat,pkt));               //应该还需要路由表     !!!!!!!!!!!!!!
                    //cout<<endl<<"\t sat"<<GetAdd().GetSatID()<<" send Relay_Regist_Pkt to sat"<<FindSat(sat,pkt)->GetAdd().GetSatID()<<endl<<endl;
                }
            }
            break;
        case DataPkt:
            pkt->path.push_back(GetAdd().GetSatID());
            //cout<<"Sat"<<GetPAdd()->GetSatID()<<"'ve reveived a PKT"<<endl<<endl;
			if(pkt->GetD_ip().seg3==sat_addr.GetAreaID())
			{  //数据包的目的地址的分区与自己的分区相同
				bool flag=0; //用于表示是否在自己的用户表里找到该目的用户的注册信息
				vector<RegistUser>::iterator iter = usertable.begin();
                for (; iter != usertable.end( ); ++iter)
				{
					if(iter->user_id==pkt->GetD_ip())
					{//在自己的注册用户表找到目的用户信息，判断是否有中继位
						flag=1; 
						if(iter->regist==0)
						{//无中继位，则遍历全局的用户数组，找到用户地址为目的地的用户，向其收包队列中加入该包
							SendPacket2User(pkt,FindUser(user,pkt));
                            //cout<<"user I'm coming!!"<<endl<<endl;
						}
						else{//有中继位，则将包中的relay_sat_id信息填为相应usertable中的中继卫星地址
							if(pkt->GetRegist()==0)
                            {
                                pkt->SetRegist(1);
                                //Ip relayip = iter->relay_sat.GetSatAddressToGround();
                                pkt->SetRelaySatID(iter->relay_sat);//从用户信息表中找到该用户的中继卫星,并且将PKT发送给中继卫星
                                SendPacket2Sat(pkt, LookUpToRoutingTable(sat, pkt,reflect));//遍历全局的卫星数组，找到卫星地址为该中继卫星的卫星，向其收包队列中加入该包？？该实现的假设是目的卫星和中继卫星之间只有1跳
                                //还是根据中继卫星地址来查找路由表的下一跳卫星？？该实现目前还不太会，因为没有实现路由表
                                //做法2：cout<<"Looking Up to RoutingTable and S-A Table use Relay_Sat_ID"<<endl<<endl;
                            }
                            else
                            {
                                SendPacket2Sat(pkt, LookUpToRoutingTable(sat, pkt,reflect));//因为路由信息转到了这颗卫星,继续查找中继卫星
                            }
						}

					}
					else{ continue;	}
				}
				if(flag==0)
				{
					//在自己的注册用户表未找到目的用户信息，报错
					cout<<"Sat"<<GetPAdd()->GetSatID()<<"   Error:can't find Dest_User"<<endl<<endl;
				}
				//else {/*不操作*/}

			}
			else{//说明还未到目的地址对应的卫星，要判断是中间的路由过程or由目的卫星向中继卫星的转发？？
				//Ip PktIp;
                //PktIp = pkt->GetRelaySatId().GetSatAddressToGround();
                bool reg;
                reg = pkt->GetRegist();
                //Ip EmptyPkt;
                //EmptyPkt = {0,0,0,0};//若数据包中的中继卫星地址
                if(reg==1)  //数据包内的信息被修改过,意味着中继卫星应该向用户直接发包
                    //判断包中的中继卫星地址是否为0，这里判断语句不确定是否正确？？
				{//非0，表示该包此前已到达过目的卫星，又由目的卫星向中继卫星转发
                    //cout<<"Looking Up to RoutingTable and S-A Table"<<endl<<endl;
                    if(pkt->GetRelaySatId().GetSatAddressToGround()==sat_addr.GetSatAddressToGround() )
                    {
                        SendPacket2User(pkt, FindUser(user, pkt));
                    }
                    //如果不止一跳,那么还需要找到目的用户的中继卫星,并查出到达中继卫星的方法
                    //疑问,若用户此时不处于卫星的覆盖范围了,是不是应该输出丢包
                    else
                    {
                        SendPacket2Sat(pkt, LookUpToRoutingTable(sat, pkt,reflect));//以中继卫星为目的地址查路由表,并且转发给中继卫星
                    }

                }
				else{  //表示为中间的路由过程，则继续查找映射表和路由表进行转发
                    SendPacket2Sat(pkt, LookUpToRoutingTable(sat, pkt,reflect));
                    //SendPacket2User(pkt,FindUser(user,pkt));   //这里有待讨论，该做法假设前提为注册卫星和中继卫星间只有1跳；否则应为根据中继卫星信息查找路由表的过程
                    //做法2：cout<<"Looking Up to RoutingTable and S-A Table use Relay_Sat_ID"<<endl<<endl;
                    //查路由表
				}
			}
            break;
        case AdPkt:
            break;
    }

}
Sat* Sat::LookUpToRoutingTable(Sat* sat[],Packet *pkt,int reflect[]){
    if(pkt->GetRegist())                                            //如果数据包的标记为被置成1,说明需要找到中继卫星,并查表转发
    {
        vector<RoutingTable>::iterator iter = rt.begin();
        for(; iter != rt.end( ); ++iter)
        {
            if(iter->des_Ip==pkt->GetRelaySatId().GetSatID())
            {
                  for(int i = 0;i<48;i++)
                  {
                      if(sat[i]->GetAdd().GetSatID()==iter->n_hop)
                          return sat[i];
                  }
            }
        }
    }
    else                                                            //查路由表,找到目的卫星
    {
        vector<RoutingTable>::iterator iter = rt.begin();
        for(; iter != rt.end( ); ++iter)
        {
            if(reflect[iter->des_Ip]==pkt->GetD_ip().seg3)
            {
                for(int i = 0;i<48;i++)
                {
                    if(sat[i]->GetAdd().GetSatID()==iter->n_hop)
                        return sat[i];
                }
            }
        }
    }
}
Sat* Sat::FindNextHop(Sat *sat[], Packet *pkt,int reflect[]) {
    vector<RoutingTable>::iterator iter = rt.begin();
    for (; iter != rt.end( ); ++iter)
    {
        if(reflect[iter->n_hop]==pkt->GetD_ip().seg3)
        {
            for(int i = 0;i<48;i++)
            {
                if(sat[i]->GetAdd().GetSatID()==iter->des_Ip)
                    return sat[i];
            }

        }
    }
}
Sat* Sat::FindSat(Sat *sat[], Packet *pkt) {
    for(int i = 0;i<48;i++)
    {
        if(sat[i]->GetAdd().GetSatAddressToGround()==pkt->GetD_ip()){
            return sat[i];
        }

    }
    return NULL;
}
Sat* Sat::FindRelaySat(Sat *sat[],Packet *pkt)
{
	for(int i = 0;i<48;i++)
    {
		if(sat[i]->GetAdd().GetSatAddressToGround()==pkt->GetRelaySatId().GetSatAddressToGround()){
            return sat[i];
        }

    }
    return NULL;
}
User* Sat::FindUser(User *user[],Packet *pkt)
{
	for(int i = 0;i<USER_NUM;i++)
    {
		if(user[i]->GetPUserAddr()->GetIp()==pkt->GetD_ip()){
            return user[i];
        }

    }
    return NULL;
}
/*===================================卫星处理队列函数=====================================================*/
void Sat::SendPacket2Sat(Packet *pkt, Sat *sat) {
    sat->GetPacketQueue()->push(*pkt);
}
void Sat::SendPacket2User(Packet *pkt, User *user) {
    user->GetPacketQueue()->push(*pkt);
}
void Sat::SendAdPacket() {//10S发一次广播,,,,,,,,,改成每秒发一次广播

    adpkt.CreatePkt(0,sat_addr.GetSatAddressToGround(),{0,0,0,0},sat_addr,sat_addr,0,sat);      //构造函数的作用

}
double Sat::SatCover(double h,double B){
    double radB = B*PI/180.0;

    double a = 1+1/pow(tan(radB),2);
    double b = -2*(EARTH_RADIUS+h)/tan(radB);
    double c = pow(EARTH_RADIUS+h,2)-pow(EARTH_RADIUS,2);
    double p = -b/(2*a);
    double q = sqrt(b*b-4*a*c)/(2*a);

    double x = p-q;               //还需讨论？

    double A = asin(x/EARTH_RADIUS);
    double R = EARTH_RADIUS*A;
    return R;
}
pos Sat::GetPos() {
    return sat;
}
SatAddress Sat::GetAdd() {
    return sat_addr;
}
SatAddress* Sat::GetPAdd(){
    return &sat_addr;
}
double Sat::GetRadius() {
    return  r;
}
queue<Packet>* Sat::GetPacketQueue(){
    return &pkt;
}
