#include <iostream>//备战
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#pragma warning (disable:4996)
/*
char * CWarrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
*/
#define Warrior_NUM 5
using namespace std;
//先定义CHeadquarter类，方便后面使用
class CHeadquarter;
class CWarrior
{
	CHeadquarter* pHeadquarter;
	int kindNo;////武士的种类编号 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
	int nNo;//武士的编号
public:
	static const char* names[Warrior_NUM];
	static int InitialLifeValue[Warrior_NUM];
	CWarrior(CHeadquarter* p, int kindNo_, int nNo_);
	void PrintResult(int nTime);
};

class CHeadquarter
{
	int totalLifeValue;
	bool bStopped;
	int color;//0 for red
	int curMakingSeqIdx;//当前制作的勇士为制作中的第几个
	int warriorNum[Warrior_NUM];//每种勇士的个数
	int totalWarriorNum;//勇士总数
	CWarrior* pWarrior[1000];//相互指，知道的关系
public:
	friend class CWarrior;//方便访问
	static int makingSeq[2][Warrior_NUM];
	void Init(int color_, int lv);
	~CHeadquarter();//消除pwarrior
	int Produce(int nTime);//返回0或1，看看是否produce下去
	void GetColor(char* szColor);

};

CWarrior::CWarrior(CHeadquarter* p, int kindNo_, int nNo_) :pHeadquarter(p), kindNo(kindNo_), nNo(nNo_) {}

void CWarrior::PrintResult(int nTime)
{
	char szColor[20];
	pHeadquarter->GetColor(szColor);
	printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n", nTime, szColor, names[kindNo],nNo, InitialLifeValue[kindNo],
		pHeadquarter->warriorNum[kindNo], names[kindNo], szColor);
}

void CHeadquarter::Init(int color_, int lv)
{
	totalLifeValue = lv;
	color = color_;
	curMakingSeqIdx = 0;
	bStopped = false;
	totalWarriorNum = 0;
	for (int i = 0; i < Warrior_NUM; i++)
		warriorNum[i] = 0;
}

CHeadquarter::~CHeadquarter()//析构函数最好要写上，因为有1000个元素，可能很大
{
	for (int i = 0; i < totalWarriorNum; i++)
		delete pWarrior[i];
}

void CHeadquarter::GetColor(char * szColor)
{
	if (color == 0)
		strcpy(szColor, "red");
	else 
		strcpy(szColor, "blue");
}

//最困难的Produce函数
int CHeadquarter::Produce(int nTime)
{
	if(bStopped)//一定要写，避免多次停止生产
	return 0;
	//下面要确定从哪个开始制造
	int nSearchingTimes = 0;//扫描
	while (CWarrior::InitialLifeValue[makingSeq[color][curMakingSeqIdx]] > totalLifeValue
		&& nSearchingTimes < Warrior_NUM)
	{
		curMakingSeqIdx = (curMakingSeqIdx + 1) % Warrior_NUM;
		nSearchingTimes++;
	}
	int kindNo = makingSeq[color][curMakingSeqIdx];
	//确定位置后开始生产了
	//当无法继续生产时
	if(nSearchingTimes==Warrior_NUM) {
		bStopped = true;
		if (color == 0)
			printf("%03d red headquarter stops making warriors\n", nTime);
		else
			printf("%03d blue headquarter stops making warriors\n", nTime);
		return 0;
	}
	//可以继续生产
	totalLifeValue -= CWarrior::InitialLifeValue[kindNo];
	curMakingSeqIdx = (curMakingSeqIdx + 1) % Warrior_NUM;//指向下次要生产的武士
	warriorNum[kindNo]++;
	//搞定一些收尾工作
	pWarrior[totalWarriorNum] = new CWarrior(this,kindNo ,totalWarriorNum + 1 );
	pWarrior[totalWarriorNum]->PrintResult(nTime);
	totalWarriorNum++;
	return 1;
}

const char* CWarrior::names[Warrior_NUM] = { "dragon","ninja","iceman","lion","wolf" };
int CWarrior::InitialLifeValue[Warrior_NUM];
int CHeadquarter::makingSeq[2][Warrior_NUM] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列

int main()
{
	int t;
	int m;
	CHeadquarter RedHead, BlueHead;
	scanf("%d", &t);
	int nCaseNo = 1;
	while (t--) {
		scanf("%d", &m);
	
		for (int i = 0; i < Warrior_NUM; i++)
			scanf("%d", &CWarrior::InitialLifeValue[i]);
		RedHead.Init(0, m);
		BlueHead.Init(1, m);
		int nTime = 0;
		printf("Case:%d\n", nCaseNo++);
		while (true) {
			int tmp1 = RedHead.Produce(nTime);
			int tmp2 = BlueHead.Produce(nTime);
			if (tmp1 == 0 && tmp2 == 0)
				break;
			nTime++;
		}
	}
	return 0;
}