#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>
using namespace std;

typedef double T_MONEY;
//游戏模式
long long PlayMode;
//鸿昊赌徒系数,乘入mProfitRate可以减少赌局次数,增大盈利率和盈利金额
const float GambleRateFault=0.01;
float GambleRate=1;
//失败是否重新设置购买倍数
bool FailReset=false;
//程序运行多久提示用户一次程序仍在运行,单位秒
const int IntervalRunTip=3;
class Player{
    //TODO:溢出检测
private:
    //用户起始资产
    T_MONEY mPropertyInital;
    //用户现有资产
    T_MONEY mProperty;
    //用户编号5
    long long mOrder;
    //用户游戏次数
    long long mPlayTimes=0;
    //以下变量用于计算翻倍后的下注金额
    //用户起始下注倍数
    long long mTimesInital;
    //用户下次下注倍数，如果中奖重置为用户起始下注倍数
    long long mTimes;
    //用户连续赔钱总额，中奖后重置
    long long mLossMoney=0;
    //用户 中奖金额后去除本金所能获得的收益 单次
    double mProfitRate;
    //以下变量用于下注
    long long mNumCount;
    long long mBetArr[10]{2,4,6,8,9,10,1,3,5,7};
    //计算盈利金额,盈利金额=倍数*timesRate
    float mTimesRate;
    long long mMaxTimes=1;
    T_MONEY mMaxProperty;
    T_MONEY mMinProperty;
    long long mWinTimes=0;
    long long mLossTimes=0;
    long long mMaxLossTimes=0;
    bool mDead=false;
    void fail(){
        T_MONEY money=mNumCount*mTimes;
        mLossMoney+=money;
        mTimes=(mLossMoney/mProfitRate)/10+1+10;
        mLossTimes++;
        mMaxLossTimes++;
        if(mTimes>mMaxTimes)
            mMaxTimes=mTimes;
        if(mProperty<mMinProperty)
            mMinProperty=mProperty;
    }
    void succ(){
        T_MONEY money=mTimesRate*mTimes;
        mProperty+=money;
        mTimes=mTimesInital;
        mLossMoney=0;
        mMaxLossTimes=0;
        mWinTimes++;
        if(mProperty<mMinProperty)
            mMinProperty=mProperty;
        if(mProperty>mMaxProperty)
            mMaxProperty=mProperty;
    }
public:
    Player(long long order,T_MONEY propertyInital,long long timesInital=1,long long numCount=6,float timesRate=9.8){
        mOrder=order;
        mPropertyInital=propertyInital;
        mProperty=mPropertyInital;
        mTimesInital=timesInital;
        mTimes=mTimesInital;
        mNumCount=numCount;
        mTimesRate=timesRate;
        mProfitRate=(((10-mNumCount)/10.0)*(mTimesRate/10))*GambleRate;
        mMaxProperty=mMinProperty=mProperty;
    }
    T_MONEY getPropertyInital(){
        return mPropertyInital;
    }
    T_MONEY getProperty(){
        return mProperty;
    }
    T_MONEY getPlayTimes(){
        return mPlayTimes;
    }
    T_MONEY getTimes(){
        return mTimes;
    }
    T_MONEY getProfit(){
        return mProperty-mPropertyInital;
    }
    T_MONEY getMaxProperty(){
        return mMaxProperty;
    }
    void gameOver(){
        if(PlayMode!=3){
            cout<<"游戏结束,资金不足!"<<endl;
            cout<<"用户编号:"<<mOrder<<endl;
            cout<<"游戏轮数:"<<mPlayTimes<<endl;
            cout<<"起始资金:"<<getPropertyInital()<<endl;
            cout<<"现有资金:"<<getProperty()<<endl;
            cout<<"总盈利:"<<getProfit()<<endl;
            cout<<"最高连输次数:"<<mMaxLossTimes<<" 最高下注倍数:"<<mMaxTimes<<" "<<"金额:"<<mMaxTimes*6<<endl;
            cout<<"最高所需下注倍数:"<<mMaxTimes<<" "<<"金额:"<<mMaxTimes*6<<endl;
            cout<<"最高历史资金:"<<mMaxProperty<<" "<<"最低历史资金:"<<mMinProperty<<" 最高历史收益率:"<<(mMaxProperty/mPropertyInital-1)*100<<"%"<<endl;
            cout<<"盈利次数:"<<mWinTimes<<" "<<"亏损次数:"<<mLossTimes<<"盈利频率:"<<(float)(mWinTimes)/(mLossTimes+mWinTimes)<<endl;
            cout<<endl;
        }
        mDead=true;
    }
    
    void opening(long long num){//开奖号码
        //买入,扣款
        if(mPlayTimes<0){
            cout<<"程序溢出!";
            mDead=true;
        }

        T_MONEY money=mNumCount*mTimes;
        mProperty-=money;
        if(mProperty<0){
            mProperty+=money;
            if(FailReset){
                if (mTimes==1){
                    gameOver();
                    return;
                }
                else{
                    mTimes=1;
                    mLossMoney=0;
                    return;
                }
                    
            }
            else{
                gameOver();
                return;
            }

        }
            
        
        bool flag=false;
        for(long long i=0;i<mNumCount;i++)
            if(mBetArr[i]==num)
                flag=true;
        long long lTimes=mTimes;
        mPlayTimes++;
        if(flag)
            succ();
        else
            fail();
        if(PlayMode==1||PlayMode==4){
            cout<<"用户编号:"<<mOrder<<endl;
            cout<<"游戏轮数:"<<mPlayTimes<<endl;
            cout<<"起始资金:"<<getPropertyInital()<<endl;
            cout<<"现有资金"<<getProperty()<<endl;
            cout<<"总盈利"<<getProfit()<<endl;
            cout<<"中奖次数:"<<mWinTimes<<" "<<"亏损次数:"<<mLossTimes<<" 中奖频率:"<<(float)(mWinTimes)/(mLossTimes+mWinTimes)<<endl;
            cout<<"本次下注倍率:"<<lTimes<<endl<<"本次下注金额:"<<mNumCount*lTimes<<endl;
            cout<<"本次盈利:";
            if(flag)
                cout<<mTimesRate*lTimes<<endl;
            else
                cout<<"-"<<mNumCount*lTimes<<endl;
            cout<<"本期购买号码:";
            for(long long i=0;i<mNumCount;i++){
                cout<<mBetArr[i]<<" ";
            }
            cout<<"本期开奖号码:"<<num<<endl;
            cout<<endl;
            
        }

    }
    bool getDead(){
        return mDead;
    }

};

class Plate{
private:
    vector<Player> mPlayerVec;
    long long mTime=0;
    long long mInterval;
public:
    Plate(long long playerNum       //用户数量
    ,long long interval             //开盘间隔时间,0为不等待，单位秒
    ,T_MONEY propertyInital         //用户初始资产,单位元
    ,long long timesInital=1        //用户初始倍数,单位元
    ,long long numCount=6           //用户购买号码数量
    ,float timesRate=9.8            //收益倍率 收益=timesRate*收益倍数
    ){
        mInterval=interval;
        for(long long i=0;i<playerNum;i++){
            Player Player(i+1,propertyInital,timesInital,numCount,timesRate);
            mPlayerVec.push_back(Player);
        }
    }
    void gameOver(float maxProfitRateHis,T_MONEY maxProfitHis,long long playTimes){
        long long winPeople=0;
        long long lossPeople=0;
        T_MONEY allProfit=0;
        T_MONEY allDeficit=0;
        T_MONEY averProfit=0;
        T_MONEY averDeficit=0;
        T_MONEY maxProfit=0;
        long long maxProfitRate=0;
        long long profitBigger10p=0;
        long long profitBigger100p=0;
        long long profitBigger1000p=0;
        for(Player & player:mPlayerVec){
            if(player.getProfit()>0){
                winPeople++;
                allProfit+=player.getProfit();
            }
            if(player.getProfit()<=0){
                lossPeople++;
                allDeficit+=player.getProfit();
            }
            if(player.getMaxProperty()/player.getPropertyInital()>=1.1)
                profitBigger10p++;
            if(player.getMaxProperty()/player.getPropertyInital()>=10)
                profitBigger100p++;
            if(player.getMaxProperty()/player.getPropertyInital()>=100)
                profitBigger1000p++;
            if(player.getProfit()>maxProfit){
                maxProfit=player.getProfit();
                maxProfitRate=maxProfit/player.getPropertyInital();
            }
                
        }


        if(winPeople)
            averProfit=allProfit/winPeople;
        else
            averProfit=0;
        if(lossPeople)
            averDeficit=allDeficit/-lossPeople;
        else
            averDeficit=0;
        cout<<"游戏结束,所有玩家失败!"<<endl;
        if(GambleRate==1)
            cout<<"鸿昊赌徒系数(关)"<<endl;
        else
            cout<<"鸿昊赌徒系数(开)"<<endl;
        if(FailReset)
            cout<<"资金不足是否重置购买倍数(重置)"<<endl;
        else
            cout<<"资金不足是否重置购买倍数(不重置)"<<endl;
        cout<<"盈利人数:"<<winPeople<<" 平均盈利金额:"<<averProfit<<" 总盈利金额:"<<allProfit<<endl;
        cout<<"亏损人数:"<<lossPeople<<" 平均亏损金额:"<<averDeficit<<" 总亏损金额:"<<-allDeficit<<endl;
        cout<<"盈利曾经超过10%的用户数:"<<profitBigger10p<<endl;
        cout<<"盈利超过100%的用户数:"<<profitBigger100p<<endl;
        cout<<"盈利超过1000%的用户数:"<<profitBigger1000p<<endl;
        cout<<"最高盈利倍率:"<< maxProfitRate*100<< "%" <<"最高盈利金额:"<< maxProfit <<endl;
        cout<<"历史最高盈利倍率:"<< maxProfitRateHis*100<< "%" <<"历史最高盈利金额:"<< maxProfitHis <<
            "游戏轮数:"<<playTimes<<endl;
        cout<<"输入任意内容并回车，重新开始游戏!";
        char a;
        cin>>a;
        cout<<endl;
    }
    void run(){
        time_t timeLast=time(NULL);
        time_t timeLastRunTip=time(NULL);
        T_MONEY maxProfit=0;
        float maxProfitRate=0;
        long long playTimes=0;
        while(true){
            if(!mInterval?true:((time(NULL)-timeLast>=mInterval))){
                bool AllDead=true;
                for(Player & player:mPlayerVec){
                    if(!player.getDead()){
                        AllDead=false;
                        player.opening(rand()%10+1);
                        if(player.getProfit()>maxProfit){
                            maxProfit=player.getProfit();
                            maxProfitRate=maxProfit/player.getPropertyInital();
                            playTimes=player.getPlayTimes();
                        }
                    }
                    if(time(NULL)-timeLastRunTip>=IntervalRunTip){
                        timeLastRunTip=time(NULL);
                        cout<<"程序仍在运行中... 目前最高盈利倍率:"<< maxProfitRate*100<< "%" <<" 最高盈利金额:"<< maxProfit <<
                            "游戏轮数:"<<playTimes<<endl;
                    }
                }
                timeLast=time(NULL);
                if(AllDead==true){
                    gameOver(maxProfitRate,maxProfit,playTimes);
                    return;
                }
            }
        }
    }
};
void clearScreen(){
    system("cls");
}
void help(){
    cout<<"本游戏是模拟赌博游戏，帮助大家认清赌博里的数学。\n";
    cout<<"每局开始，用户从1~10选择需要购买的号码，1个号码1块钱。\n";
    cout<<"可以选择加倍购买，2倍就是一个号码2块钱。\n";
    cout<<"每局开盘会从1~10中随机抽取一个号码作为开奖结果。\n";
    cout<<"如果购买的号码中包含开奖结果则中奖,否则不中奖。中奖金额为 “中奖系数*倍数” 一般中奖倍数设置为9.8\n";
    cout<<"比如购买1倍率，6个号码花费6块钱，中奖后获得9.8。\n";
    cout<<"很多用户认为，使用翻倍购买法可以稳赚，即第一次购买1倍率，如果输了第二次购买2倍率，如果再输了购买5倍率\n";
    cout<<"只要赢一次就可以将本金赚回还能再赚几块，从而永不亏损，本程序模拟了此行为。\n";
    cout<<"鸿昊赌徒系数可以极大的减少用户游戏轮次，提高最终盈利用户比例！但会减少历史最高盈利率！\n";
    cout<<"输入任意内容并回车，返回主菜单!";
    char a;
    cin>>a;
}

int main(){
    while(true){
        srand((unsigned)time(NULL)); 
        cout<<"欢迎来到模拟5分钟短线交易游戏!本游戏十分有趣，第一次使用请查看帮助文档。"<<endl;
        cout<<"请输入游戏模式序号并回车:"<<endl;
        cout<<"1.模拟单用户模式(可以看到每一步交易结果)\n2.模拟多用户模式\n3.模拟超多用户模式(如果多用户模式长时间无响应请使用此模式)\n4.全真模拟"
        <<"\n5.帮助\n6.退出"<<endl;
        if(GambleRate==1)
            cout<<"7.切换鸿昊赌徒系数,目前状态(关)"<<endl;
        else
            cout<<"7.切换鸿昊赌徒系数,目前状态(开)"<<endl;
        if(FailReset)
            cout<<"8.切换资金不足是否重置购买倍数,目前状态(重置)"<<endl;
        else
            cout<<"8.切换资金不足是否重置购买倍数,目前状态(不重置)"<<endl;
        cout<<"请选择:";
        cin>>PlayMode;
        if(PlayMode>8||PlayMode<1){
            clearScreen();
            cout<<"输入有误请重新输入!"<<endl<<endl;
            continue;
        }
        long long propertyInitial;
        long long numCount;
        long long timesInitial;
        long long numPeople;
        if(PlayMode<5){
            cout<<"请输入购买多少个号码(1~9)，默认按照2,4,6,8,9,10,1,3,5,7顺序购买代码:";
            cin>>numCount;
            cout<<"请输入用户的起步资产:";
            cin>>propertyInitial;
            cout<<"请输入用户起始购买倍数:";
            cin>>timesInitial;
        }
        switch(PlayMode){
            case 5:help();break;
            case 6:exit(0);
            case 7:{
                if(GambleRate==1)
                    GambleRate=GambleRateFault;
                else 
                    GambleRate=1;
                clearScreen();
                continue;
            }
            case 8:{
                FailReset=!FailReset;
                clearScreen();
                continue;
            }
            case 4:{
                long long interval;
                cout<<"请输入开盘间隔时间(单位/秒):";
                cin>>interval;
                cout<<endl;
                Plate plate(1,interval,propertyInitial,timesInitial,numCount);
                plate.run();
                break;
            }
            case 1:{
                Plate plate(1,0,propertyInitial,timesInitial,numCount);plate.run();
                break;
            }
            default:{
                cout<<"请输入用户数目:";
                cin>>numPeople;
                Plate plate(numPeople,0,propertyInitial,timesInitial,numCount);plate.run();
            }
        }
        clearScreen();
    }
    
    return 0;
}