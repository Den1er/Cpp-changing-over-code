//main.cpp
#include <iostream>
#include <algorithm>
//#include <vector>
#include "math.h"
#include "User.h"
#include "Area2Sat.h"
#include "string.h"
#define r  6378000
#define h  14450000
#define DATA                    69168
using namespace std;
#define Max_Num = 2^23;
const bool DebugUserIp =0;
vector<int> randperm(int Num)
{
    vector<int> temp;
    for (int i = pow(2,23); i < Num; ++i)
    {      //NUM应小于2^24
        temp.push_back(i + 1);
    }

    random_shuffle(temp.begin(), temp.end());

    return temp;

}
int Fin1(int t,pos mat[]){//读入2进制位置信息
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
        infile.seekg(sizeof(pos) * start_time,ios_base::beg);
        infile.read((char*)(mat+i), sizeof(pos));
    }
    infile.close();
    return 0;
}

int main() {
    int StoA[48] = {99,99,99,99,99,99,99,99,                        //映射表
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99
    };
    int AtoS[48] = {99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99,
                    99,99,99,99,99,99,99,99
    };
    pos mat_pos[48];


    string filename[48];
    filename[0] = "/Users/Den1er/ClionProjects/nexxxxthop/0.txt";
    filename[1] = "/Users/Den1er/ClionProjects/nexxxxthop/1.txt";
    filename[2] = "/Users/Den1er/ClionProjects/nexxxxthop/2.txt";
    filename[3] = "/Users/Den1er/ClionProjects/nexxxxthop/3.txt";
    filename[4] = "/Users/Den1er/ClionProjects/nexxxxthop/4.txt";
    filename[5] = "/Users/Den1er/ClionProjects/nexxxxthop/5.txt";
    filename[6] = "/Users/Den1er/ClionProjects/nexxxxthop/6.txt";
    filename[7] = "/Users/Den1er/ClionProjects/nexxxxthop/7.txt";
    filename[8] = "/Users/Den1er/ClionProjects/nexxxxthop/8.txt";
    filename[9] = "/Users/Den1er/ClionProjects/nexxxxthop/9.txt";
    filename[10] = "/Users/Den1er/ClionProjects/nexxxxthop/10.txt";
    filename[11] = "/Users/Den1er/ClionProjects/nexxxxthop/11.txt";
    filename[12] = "/Users/Den1er/ClionProjects/nexxxxthop/12.txt";
    filename[13] = "/Users/Den1er/ClionProjects/nexxxxthop/13.txt";
    filename[14] = "/Users/Den1er/ClionProjects/nexxxxthop/14.txt";
    filename[15] = "/Users/Den1er/ClionProjects/nexxxxthop/15.txt";
    filename[16] = "/Users/Den1er/ClionProjects/nexxxxthop/16.txt";
    filename[17] = "/Users/Den1er/ClionProjects/nexxxxthop/17.txt";
    filename[18] = "/Users/Den1er/ClionProjects/nexxxxthop/18.txt";
    filename[19] = "/Users/Den1er/ClionProjects/nexxxxthop/19.txt";
    filename[20] = "/Users/Den1er/ClionProjects/nexxxxthop/20.txt";
    filename[21] = "/Users/Den1er/ClionProjects/nexxxxthop/21.txt";
    filename[22] = "/Users/Den1er/ClionProjects/nexxxxthop/22.txt";
    filename[23] = "/Users/Den1er/ClionProjects/nexxxxthop/23.txt";
    filename[24] = "/Users/Den1er/ClionProjects/nexxxxthop/24.txt";
    filename[25] = "/Users/Den1er/ClionProjects/nexxxxthop/25.txt";
    filename[26] = "/Users/Den1er/ClionProjects/nexxxxthop/26.txt";
    filename[27] = "/Users/Den1er/ClionProjects/nexxxxthop/27.txt";
    filename[28] = "/Users/Den1er/ClionProjects/nexxxxthop/28.txt";
    filename[29] = "/Users/Den1er/ClionProjects/nexxxxthop/29.txt";
    filename[30] = "/Users/Den1er/ClionProjects/nexxxxthop/30.txt";
    filename[31] = "/Users/Den1er/ClionProjects/nexxxxthop/31.txt";
    filename[32] = "/Users/Den1er/ClionProjects/nexxxxthop/32.txt";
    filename[33] = "/Users/Den1er/ClionProjects/nexxxxthop/33.txt";
    filename[34] = "/Users/Den1er/ClionProjects/nexxxxthop/34.txt";
    filename[35] = "/Users/Den1er/ClionProjects/nexxxxthop/35.txt";
    filename[36] = "/Users/Den1er/ClionProjects/nexxxxthop/36.txt";
    filename[37] = "/Users/Den1er/ClionProjects/nexxxxthop/37.txt";
    filename[38] = "/Users/Den1er/ClionProjects/nexxxxthop/38.txt";
    filename[39] = "/Users/Den1er/ClionProjects/nexxxxthop/39.txt";
    filename[40] = "/Users/Den1er/ClionProjects/nexxxxthop/40.txt";
    filename[41] = "/Users/Den1er/ClionProjects/nexxxxthop/41.txt";
    filename[42] = "/Users/Den1er/ClionProjects/nexxxxthop/42.txt";
    filename[43] = "/Users/Den1er/ClionProjects/nexxxxthop/43.txt";
    filename[44] = "/Users/Den1er/ClionProjects/nexxxxthop/44.txt";
    filename[45] = "/Users/Den1er/ClionProjects/nexxxxthop/45.txt";
    filename[46] = "/Users/Den1er/ClionProjects/nexxxxthop/46.txt";
    filename[47] = "/Users/Den1er/ClionProjects/nexxxxthop/47.txt";
    int routte[48];

    Sat sat0;Sat sat1;Sat sat2;Sat sat3;Sat sat4;Sat sat5;Sat sat6;Sat sat7;
    Sat sat8;Sat sat9;Sat sat10;Sat sat11;Sat sat12;Sat sat13;Sat sat14;Sat sat15;
    Sat sat16;Sat sat17;Sat sat18;Sat sat19;Sat sat20;Sat sat21;Sat sat22;Sat sat23;
    Sat sat24;Sat sat25;Sat sat26;Sat sat27;Sat sat28;Sat sat29;Sat sat30;Sat sat31;
    Sat sat32;Sat sat33;Sat sat34;Sat sat35;Sat sat36;Sat sat37;Sat sat38;Sat sat39;
    Sat sat40;Sat sat41;Sat sat42;Sat sat43;Sat sat44;Sat sat45;Sat sat46;Sat sat47;
    Sat* sat[48];
    sat[0] = &sat0;sat[1] = &sat1;sat[2] = &sat2;sat[3] = &sat3;sat[4] = &sat4;sat[5] = &sat5;sat[6] = &sat6;sat[7] = &sat7;
    sat[8] = &sat8;sat[9] = &sat9;sat[10] = &sat10;sat[11] = &sat11;sat[12] = &sat12;sat[13] = &sat13;sat[14] = &sat14;sat[15] = &sat15;
    sat[16] = &sat16;sat[17] = &sat17;sat[18] = &sat18;sat[19] = &sat19;sat[20] = &sat20;sat[21] = &sat21;sat[22] = &sat22;sat[23] = &sat23;
    sat[24] = &sat24;sat[25] = &sat25;sat[26] = &sat26;sat[27] = &sat27;sat[28] = &sat28;sat[29] = &sat29;sat[30] = &sat30;sat[31] = &sat31;
    sat[32] = &sat32;sat[33] = &sat33;sat[34] = &sat34;sat[35] = &sat35;sat[36] = &sat36;sat[37] = &sat37;sat[38] = &sat38;sat[39] = &sat39;
    sat[40] = &sat40;sat[41] = &sat41;sat[42] = &sat42;sat[43] = &sat43;sat[44] = &sat44;sat[45] = &sat45;sat[46] = &sat46;sat[47] = &sat47;
    for(int i = 0;i<48;++i)
    {
        sat[i]->ReadFileAndApdate(filename[i],routte);                          //初始化路由表,路由表不会变更,已写死

    }
    int Num = pow(2,22)+pow(2,23);
    vector<int> Id = randperm(Num);
    User user1;
    user1.SetPos({33.0,159.1});                                 //纬度经度
    user1.SetUserOrder(1);
    user1.GetPUserAddr()->SetUserID(Id,1);
    user1.GetPUserAddr()->SetAreaidTest(GetAddressID(159.1,33.0));              //先经度后纬度


    User user2;
    user2.SetPos({85.6,30.0});
    user2.SetUserOrder(2);
    user2.GetPUserAddr()->SetUserID(Id,2);
    user2.GetPUserAddr()->SetAreaidTest(GetAddressID(30.0,85.6));


    User* user[2];
    user[0] = &user1;
    user[1] = &user2;
    if(DebugUserIp==1)
    {
        cout << user1.GetPUserAddr()->GetIp().seg3 << "." << user1.GetPUserAddr()->GetIp().seg2 << "." <<
        user1.GetPUserAddr()->GetIp().seg1 << "." << user1.GetPUserAddr()->GetIp().seg0 << endl;
        cout << user2.GetPUserAddr()->GetIp().seg3 << "." << user2.GetPUserAddr()->GetIp().seg2 << "." <<
        user2.GetPUserAddr()->GetIp().seg1 << "." << user2.GetPUserAddr()->GetIp().seg0 << endl;
    }
    for(int i = 0;i<3000;++i)
    {
        //cout<<"                Second "<<i<<endl;
        int n = i/431;
        FindSat((220+n*431),StoA,AtoS);                               //更新映射表
        Fin1(i,mat_pos);                                    //每个卫星的位置存在
        for(int j = 0;j<48;++j)
        {
            sat[j]->SetSatPos(mat_pos[j]);                  //卫星经纬度
            sat[j]->GetPAdd()->SetSatID();                  //卫星Ip后三位
            sat[j]->GetPAdd()->SetAreaID_test(StoA[j]);       //卫星的区域号
            sat[j]->GetPAdd()->SetSatid(j);                 //卫星的标识
            sat[j]->SendAdPacket();                         //发送广播
            //sat[j]->DisplayAdpkt();                         //显示广播

        }
        user1.FindInSatCoverage(sat);
        //user1.DisplayAd();
        user1.ChooseSat();

        user2.FindInSatCoverage(sat);
        //user2.DisplayAd();
        user2.ChooseSat();

        user1.SendPkt(user1.CreateRegistPkt(),sat);
        user2.SendPkt(user2.CreateRegistPkt(),sat);
        for(int j = 0;j<48;++j) {
            sat[j]->ProcessPacketQueue(sat, user, StoA);
        }


        if(i%10==0)
        {
            user1.SendPkt(user1.CreateDataPkt(user2.GetPUserAddr()->GetIp()),sat);
            user1.DisplayRegistAndRelaySat(StoA,AtoS);
            user2.DisplayRegistAndRelaySat(StoA,AtoS);
            //user2.SendPkt(user1.CreateDataPkt(user1.GetPUserAddr()->GetIp()),sat);
            cout<<"user1 sent a pkt to user2"<<"   "<<i<<endl;
        }
        user2.ProcessDataPkt();
        //user1.ProcessDataPkt();

    }




    return 0;
}