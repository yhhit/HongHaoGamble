#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

//操作系统,WINDWOS LINUX ANDROID
#define WINDOWS

using namespace std;

typedef double T_MONEY;
//游戏模式
long long PlayMode;
//鸿昊赌徒系数,乘入mProfitRate可以减少赌局次数,增大盈利率和盈利金额
const float GambleRateFault=0.01;
float GambleRate=1;
//失败是否重新设置购买倍数
bool FailReset=false;
//程序运行多久提示玩家一次程序仍在运行,单位秒
const int IntervalRunTip=3;
class Player{
    //TODO:溢出检测
private:
    //玩家起始资产
    T_MONEY mPropertyInitial;
    //玩家现有资产
    T_MONEY mProperty;
    //玩家编号5
    long long mOrder;
    //玩家游戏次数
    long long mPlayTimes=0;
    //以下变量用于计算翻倍后的下注金额
    //玩家起始下注倍数
    long long mTimesInitial;
    //玩家下次下注倍数,如果中奖重置为玩家起始下注倍数
    long long mTimes;
    //玩家连续赔钱总额,中奖后重置
    long long mLossMoney=0;
    //玩家 中奖金额后去除本金所能获得的收益 单次
    double mProfitRate;
    //以下变量用于下注
    long long mNumCount;
    long long mBetArrInitial[10]{2,4,6,8,9,10,1,3,5,7};
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
        mTimes=mTimesInitial;
        mLossMoney=0;
        mMaxLossTimes=0;
        mWinTimes++;
        if(mProperty<mMinProperty)
            mMinProperty=mProperty;
        if(mProperty>mMaxProperty)
            mMaxProperty=mProperty;
    }
public:
    Player(long long order,T_MONEY propertyInitial,long long timesInitial=1,long long numCount=6,float timesRate=9.8){
        mOrder=order;
        mPropertyInitial=propertyInitial;
        mProperty=mPropertyInitial;
        mTimesInitial=timesInitial;
        mTimes=mTimesInitial;
        mNumCount=numCount;
        mTimesRate=timesRate;
        mProfitRate=(((10-mNumCount)/10.0)*(mTimesRate/10))*GambleRate;
        mMaxProperty=mMinProperty=mProperty;
    }
    //从文件中还原Player
    Player(string path){

    }
    void save(){
        
        //玩家起始资产
        T_MONEY mPropertyInitial;
        //玩家现有资产
        T_MONEY mProperty;
        //玩家编号5
        long long mOrder;
        //玩家游戏次数
        long long mPlayTimes=0;
        //以下变量用于计算翻倍后的下注金额
        //玩家起始下注倍数
        long long mTimesInitial;
        //玩家下次下注倍数,如果中奖重置为玩家起始下注倍数
        long long mTimes;
        //玩家连续赔钱总额,中奖后重置
        long long mLossMoney=0;
        //玩家 中奖金额后去除本金所能获得的收益 单次
        double mProfitRate;
        //以下变量用于下注
        long long mNumCount;
        long long mBetArrInitial[10]{2,4,6,8,9,10,1,3,5,7};
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
    }
    T_MONEY getPropertyInitial(){
        return mPropertyInitial;
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
        return mProperty-mPropertyInitial;
    }
    T_MONEY getMaxProperty(){
        return mMaxProperty;
    }
    void setBetArr(int offset,long long val){
        mBetArr[offset]=val;
    }
    void setNumCount(long long val){
        mNumCount=val;
    }
    void setTimes(long long times){
        mTimes=times;
    }
    void gameOver(){
        if(PlayMode!=3){
            cout<<endl;
            cout<<"游戏结束,资金不足!"<<endl;
            cout<<"玩家编号:"<<mOrder<<endl;
            cout<<"游戏期数:"<<mPlayTimes<<endl;
            cout<<"起始资金:"<<getPropertyInitial()<<endl;
            cout<<"现有资金:"<<getProperty()<<endl;
            cout<<"总盈利:"<<getProfit()<<endl;
            cout<<"最高连输次数:"<<mMaxLossTimes<<" 最高下注倍数:"<<mMaxTimes<<" "<<"金额:"<<mMaxTimes*6<<endl;
            cout<<"最高所需下注倍数:"<<mMaxTimes<<" "<<"金额:"<<mMaxTimes*6<<endl;
            cout<<"最高历史资金:"<<mMaxProperty<<" "<<"最低历史资金:"<<mMinProperty<<" 最高历史收益率:"<<(mMaxProperty/mPropertyInitial-1)*100<<"%"<<endl;
            cout<<"盈利次数:"<<mWinTimes<<" "<<"未中奖次数:"<<mLossTimes<<"盈利频率:"<<(float)(mWinTimes)/(mLossTimes+mWinTimes)<<endl;
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
        if(PlayMode!=4&&mProperty<0){
            mProperty+=money;
            //判断是否资金不足需要重置购买倍数
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
            cout<<endl;
            
            if(flag){
                cout<<"恭喜中奖";
                cout<<mTimesRate*lTimes<<"元!";
            }else{
                cout<<"未中奖!";
            }
            cout<<"购买号码花费:"<<mNumCount*lTimes<<"元"<<endl;
            cout<<"玩家编号:"<<mOrder<<endl;
            cout<<"游戏期数:"<<mPlayTimes<<endl;
            cout<<"起始资金:"<<getPropertyInitial()<<endl;
            cout<<"现有资金"<<getProperty()<<endl;
            cout<<"总盈利"<<getProfit()<<endl;
            cout<<"中奖次数:"<<mWinTimes<<" "<<"未中奖次数:"<<mLossTimes<<" 中奖频率:"<<(float)(mWinTimes)/(mLossTimes+mWinTimes)<<endl;
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
    Plate(long long playerNum       //玩家数量
    ,long long interval             //开盘间隔时间,0为不等待,单位秒
    ,T_MONEY propertyInitial         //玩家初始资产,单位元
    ,long long timesInitial=1        //玩家初始倍数,单位元
    ,long long numCount=6           //玩家购买号码数量
    ,float timesRate=9.8            //收益倍率 收益=timesRate*收益倍数
    ){
        mInterval=interval;
        for(long long i=0;i<playerNum;i++){
            Player Player(i+1,propertyInitial,timesInitial,numCount,timesRate);
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
            if(player.getMaxProperty()/player.getPropertyInitial()>=1.1)
                profitBigger10p++;
            if(player.getMaxProperty()/player.getPropertyInitial()>=10)
                profitBigger100p++;
            if(player.getMaxProperty()/player.getPropertyInitial()>=100)
                profitBigger1000p++;
            if(player.getProfit()>maxProfit){
                maxProfit=player.getProfit();
                maxProfitRate=maxProfit/player.getPropertyInitial();
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
        cout<<endl;
        cout<<"所有玩家退出游戏!"<<endl;
        cout<<"游戏模式“";
        switch (PlayMode)
        {
        case 1:cout<<"模拟单玩家模式(自动)”"<<endl;break;
        case 2:cout<<"模拟多玩家模式(自动)”"<<endl;break;
        case 3:cout<<"模拟超多玩家模式(自动)”"<<endl;break;
        case 4:cout<<"全真模拟(手动)”"<<endl;break;
        default:
            break;
        }
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
        cout<<"盈利曾经超过10%的玩家数:"<<profitBigger10p<<endl;
        cout<<"盈利超过100%的玩家数:"<<profitBigger100p<<endl;
        cout<<"盈利超过1000%的玩家数:"<<profitBigger1000p<<endl;
        cout<<"最高盈利倍率:"<< maxProfitRate*100<< "%" <<"最高盈利金额:"<< maxProfit <<endl;
        cout<<"历史最高盈利倍率:"<< maxProfitRateHis*100<< "%" <<"历史最高盈利金额:"<< maxProfitHis <<
            "最高收益的游戏期数:"<<playTimes<<endl;
        cout<<"输入任意内容并回车,重新开始游戏!\n";
        char a;
        cin>>a;
    }
    void run(){
        time_t timeLast=time(NULL);
        time_t timeLastRunTip=time(NULL);
        T_MONEY maxProfit=0;
        float maxProfitRate=0;
        long long playTimes=0;
        bool inputBetArr=false;
        int numCount=0;
        long long betArr[10]{};
        long long times=1;
        time_t timeLastTip=time(NULL);
        while(true){
            if(PlayMode==4&&!inputBetArr){
                do{
                    numCount=10;
                    cout<<endl;
                    cout<<"请输入您本期购买的号码(1~10任选),号码之间以空格分隔,以@号结束(不输入为不购买)\n(例:1 2 4 5 6@)(认输请输入q按回车)：";
                    for(int i=0;i<11;i++){
                        cin>>betArr[i];
                        if(cin.fail()){
                            char a;
                            cin.clear();
                            cin>>a;
                            if(a=='@'){
                                numCount=i;
                                break;
                            }else if(a=='q'){
                                gameOver(maxProfitRate,maxProfit,playTimes);
                                return;
                            }else
                                i--;
                        }
                    }
                    cout<<"请输入购买倍数(例:1):";
                    cin>>times;
                    T_MONEY property=(*mPlayerVec.begin()).getProperty();
                    if(property<times*numCount){
                        cout<<endl;
                        cout<<"错误:购买失败，资金不足!"<<numCount<<"个号码您最多只能购买"<<property/numCount<<"倍!"<<endl<<endl;
                        continue;
                    }else
                        break;
                }while(true);
                

                timeLast=time(NULL);
                inputBetArr=true;
                cout<<"等待开盘中..."<<endl;
                
            }
            if(PlayMode==4&&(time(NULL)-timeLastTip>=1)){
                cout<<"\r";
                cout<<"距开盘还剩"<<(mInterval-(time(NULL)-timeLast))<<"秒  ";
                //由于未知原因,安卓系统添加此代码会无法输出开盘时间
                #ifndef ANDROID
                timeLastTip=time(NULL);
                #endif
            }
            
            if(!mInterval?true:((time(NULL)-timeLast>=mInterval))){
                bool AllDead=true;
                for(Player & player:mPlayerVec){
                    if(!player.getDead()){
                        AllDead=false;
                        if(PlayMode==4){
                            for(int i=0;i<10;i++)
                                player.setBetArr(i,betArr[i]);
                            player.setNumCount(numCount);
                            player.setTimes(times);
                            cout<<endl;
                        }
                        
                        player.opening(rand()%10+1);
                        if(player.getProfit()>maxProfit){
                            maxProfit=player.getProfit();
                            maxProfitRate=maxProfit/player.getPropertyInitial();
                            playTimes=player.getPlayTimes();
                        }
                    }
                    if(time(NULL)-timeLastRunTip>=IntervalRunTip){
                        timeLastRunTip=time(NULL);
                        cout<<"程序仍在运行中... 目前最高盈利倍率:"<< maxProfitRate*100<< "%" <<" 最高盈利金额:"<< maxProfit <<
                            "游戏期数:"<<playTimes<<endl;
                    }
                }
                timeLast=time(NULL);
                if(AllDead==true){
                    gameOver(maxProfitRate,maxProfit,playTimes);
                    return;
                }
                inputBetArr=false;
            }
        }
    }
};

void clearScreen(){
//windows下用cls命令清屏
#ifdef WINDOWS
    system("cls");
#endif
#ifdef LINUX
    system("clear");
#endif
#ifdef ANDROID
    system("clear");
#endif
}

void help(){
    clearScreen();
    cout<<"这是一个赌博游戏,程序通过你设定的购买方式,计算机快速购买计算出参与赌博的收益率,多少玩家可以赚钱,可以赚多少钱,本程序还可以完全模拟赌博游戏。\n";
    cout<<"每局开始,玩家从1~10选择需要购买的号码,1个号码1块钱。\n";
    cout<<"可以选择加倍购买,2倍就是一个号码2块钱。\n";
    cout<<"每局开盘会从1~10中随机抽取一个号码作为开奖结果。\n";
    cout<<"如果购买的号码中包含开奖结果则中奖,否则不中奖。奖金额为 “中奖系数*购买倍数” 一般中奖系数设置为9.8。\n";
    cout<<"比如购买号码2,3两个号码3倍率,花费2*3=6元,若开奖结果为2或3则中奖后获得9.8*3=29.4元,净收入23.4元。\n";
    cout<<"很多玩家认为,使用翻倍购买法可以稳赚,即第一次购买1倍率,如果输了第二次购买2倍率,如果再输了购买5倍率。\n";
    cout<<"只要赢一次就可以将本金赚回还能再赚一小部分,从而永不亏损,本程序模拟了此行为。\n";
    cout<<"鸿昊赌徒系数适量的增大了输掉后翻倍的购买倍数,可以极大的减少玩家游戏轮次,但对于玩家平均盈利率与玩家最高盈利率会产生一定影响!\n";
    cout<<"资金不足是否重置购买倍数,表示如果使用翻倍购买法资金不足以进行下一次购买,是否重新从最小倍数开始购买。\n"<<endl;
    cout<<"输入任意内容并回车,返回主菜单!\n";
    char a;
    cin>>a;
}

int main(){
    while(true){
        srand((unsigned)time(NULL)); 
            cout<<"欢迎来到“赌博中的数学”,第一次使用请查看帮助文档。"<<endl;
        cout<<"请输入游戏模式序号并回车:"<<endl;
        cout<<"1.模拟单玩家模式(自动)(可以看到每一步交易结果)\n2.模拟多玩家模式(自动)\n3.模拟超多玩家模式(自动)(如果多玩家模式长时间无响应请使用此模式)\n4.全真模拟(手动)(模拟真实赌博体验)"
        <<"\n5.帮助\n6.退出"<<endl;
        if(GambleRate==1)
            cout<<"7.切换鸿昊赌徒系数,目前状态(关)(自动模式的配置)"<<endl;
        else
            cout<<"7.切换鸿昊赌徒系数,目前状态(开)(自动模式的配置)"<<endl;
        if(FailReset)
            cout<<"8.切换资金不足是否重置购买倍数,目前状态(重置)(自动模式的配置)"<<endl;
        else
            cout<<"8.切换资金不足是否重置购买倍数,目前状态(不重置)(自动模式的配置)"<<endl;
#ifdef ANDROID
        cout<<"\n提示：由于安卓系统限制，应用在锁屏或后台情况下可能会终止，请确保手机不会锁屏或在后台停留太久，建议使用电脑版！可访问www.yhhit.xyz下载!\n\n";
        cout<<"提示：更新应用必须卸载之前版本，否则虽然安装成功但是软件内容并不会更新!\n\n";
#endif
        static bool faultFlag=false;
        if(faultFlag){
            cout<<"输入有误请重新输入:";
            faultFlag=false;
        }else
            cout<<"请选择:";
        cin>>PlayMode;
        if(PlayMode>8||PlayMode<1){
            faultFlag=true;
            clearScreen();
            continue;
        }
        long long propertyInitial;
        long long numCount;
        long long timesInitial;
        long long numPeople;
        if(PlayMode<4){
            clearScreen();
            cout<<"请输入每次购买的号码的数目(1~9),系统自动按照2,4,6,8,9,10,1,3,5,7顺序购买代码(例:6):";
            cin>>numCount;
            cout<<"请输入玩家的起步资产(例:1000):";
            cin>>propertyInitial;
            cout<<"请输入玩家起始购买倍数(例:1):";
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
                clearScreen();
                numCount=-1;
                cout<<"请输入玩家的起步资产,单位元(例:1000):";
                cin>>propertyInitial;
                long long interval;
                cout<<"请输入开盘间隔时间(单位/分钟)(例:1):";
                cin>>interval;
                cout<<endl;
                Plate plate(1,interval*60,propertyInitial,timesInitial,numCount);
                plate.run();
                break;
            }
            case 1:{
                Plate plate(1,0,propertyInitial,timesInitial,numCount);plate.run();
                break;
            }
            default:{
                cout<<"请输入玩家数目(例:100):";
                cin>>numPeople;
                Plate plate(numPeople,0,propertyInitial,timesInitial,numCount);plate.run();
            }
        }
        clearScreen();
    }
    
    return 0;
}