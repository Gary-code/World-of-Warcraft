#include <iostream>//备战
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#pragma warning (disable:4996)
/*
char * CWarrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。
蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。
武器编号为 sword bomb arrow 0，1，2
*/
#define Warrior_NUM 5
#define WEAPON_NUM 3
#define MAX_WARRIOR 1000
enum {DRAGON,NINJA,ICEMAN,LION,WOLF};
using namespace std;
//先定义CHeadquarter类，方便后面使用
class CHeadquarter;

class CWeapon
{
public:
	int nKindNo;//武器的编号
	int nForce;//攻击力，本题中没有，为下一题做铺垫
	static int InitialForce[WEAPON_NUM];
	static const char* Names[WEAPON_NUM];
};
class CWarrior
{
protected:
	CHeadquarter* pHeadquarter;
	int nNo;//武士的编号
public:
	static const char* names[Warrior_NUM];
	static int InitialLifeValue[Warrior_NUM];
	CWarrior(CHeadquarter* p, int nNo_);
	virtual void PrintResult(int nTime,int nKindNo);//像第一题那样的输出
	virtual void PrintResult(int nTime) = 0;
	virtual ~CWarrior() {}
};
class CDragon;
class CNinja;
class CIceman;
class CLion;
class CWolf;
class CHeadquarter
{
	int totalLifeValue;
	bool bStopped;
	int color;//0 for red
	int curMakingSeqIdx;//当前制作的勇士为制作中的第几个
	int warriorNum[Warrior_NUM];//每种勇士的个数
	int totalWarriorNum;//勇士总数
	CWarrior* pWarrior[MAX_WARRIOR];//相互指，知道的关系
public:
	friend class CWarrior;//方便访问
	static int makingSeq[2][Warrior_NUM];
	void Init(int color_, int lv);
	~CHeadquarter();//消除pwarrior
	int Produce(int nTime);//返回0或1，看看是否produce下去
	void GetColor(char* szColor);
	int GetTotalLifeValue() { return totalLifeValue; }
};
class CDragon :public CWarrior
{
	CWeapon wp;
	double fmorale;//士气
public:
	void Countmorale()
	{
		fmorale = pHeadquarter->GetTotalLifeValue()
			/ (double)CWarrior::InitialLifeValue[0];
	}
	CDragon(CHeadquarter* p, int nNo_) :
		CWarrior(p, nNo_)
	{
		wp.nKindNo = nNo % WEAPON_NUM;
		wp.nForce = CWeapon::InitialForce[wp.nKindNo];
		Countmorale();
	}
	void PrintResult(int nTime)
	{
		CWarrior::PrintResult(nTime, DRAGON);
		printf("It has a %s,and it's morale is %.2f\n",
			CWeapon::Names[wp.nKindNo], fmorale);
	}
};

class CNinja :public CWarrior
{
	CWeapon wps[2];
public:
	CNinja(CHeadquarter* p, int nNo_) :CWarrior(p, nNo_)
	{
		wps[0].nKindNo = nNo % WEAPON_NUM;
		wps[0].nForce = CWeapon::InitialForce[wps[0].nKindNo];
		wps[1].nKindNo = (nNo+1) % WEAPON_NUM;
		wps[1].nForce = CWeapon::InitialForce[wps[1].nKindNo];
	}
	void PrintResult(int nTime)
	{
		CWarrior::PrintResult(nTime, NINJA);
		printf("It has a %s and a %s\n",
			CWeapon::Names[wps[0].nKindNo],
			CWeapon::Names[wps[1].nKindNo]);
	}
};

class CIceman:public CWarrior
{
	CWeapon wp;
public:
	CIceman(CHeadquarter* p, int nNo_) :
		CWarrior(p, nNo_)
	{
		wp.nKindNo = nNo % WEAPON_NUM;
		wp.nForce = CWeapon::InitialForce[wp.nKindNo];
	}
	void PrintResult(int nTime)
	{
		CWarrior::PrintResult(nTime, ICEMAN);
		printf("It has a %s\n",
			CWeapon::Names[wp.nKindNo]);
	}
};

class CLion :public CWarrior
{
	int nLoyalty;
public:
	void CountLoyalty()
	{
		nLoyalty = pHeadquarter->GetTotalLifeValue();
	}
	CLion(CHeadquarter* p, int nNo_) :
		CWarrior(p, nNo_)
	{
		CountLoyalty();
	}
	void PrintResult(int nTime)
	{
		CWarrior::PrintResult(nTime, LION);
		printf("It's loyalty is %d\n",
		nLoyalty);
	}
};

class CWolf :public CWarrior
{
public:
	CWolf(CHeadquarter* p, int nNo_) :
		CWarrior(p, nNo_) {}
	void PrintResult(int nTime)
	{
		CWarrior::PrintResult(nTime, WOLF);
		
	}
};
CWarrior::CWarrior(CHeadquarter* p, int nNo_) :pHeadquarter(p), nNo(nNo_) {}

void CWarrior::PrintResult(int nTime,int nKindNo)
{
	char szColor[20];
	pHeadquarter->GetColor(szColor);
	printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n", nTime, szColor, names[nKindNo], nNo, InitialLifeValue[nKindNo],
		pHeadquarter->warriorNum[nKindNo], names[nKindNo], szColor);
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

void CHeadquarter::GetColor(char* szColor)
{
	if (color == 0)
		strcpy(szColor, "red");
	else
		strcpy(szColor, "blue");
}

//最困难的Produce函数
int CHeadquarter::Produce(int nTime)
{
	if (bStopped)//一定要写，避免多次停止生产
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
	if (nSearchingTimes == Warrior_NUM) {
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
	switch (kindNo)
	{
	case DRAGON:
		pWarrior[totalWarriorNum] = new CDragon(this, totalWarriorNum + 1);
		break;
	case NINJA:
		pWarrior[totalWarriorNum] = new CNinja(this, totalWarriorNum + 1);
		break;
	case ICEMAN:
		pWarrior[totalWarriorNum] = new CIceman(this, totalWarriorNum + 1);
		break;
	case LION:
		pWarrior[totalWarriorNum] = new CLion(this, totalWarriorNum + 1);
		break;
	case WOLF:
		pWarrior[totalWarriorNum] = new CWolf(this, totalWarriorNum + 1);
		break;

	}
	pWarrior[totalWarriorNum]->PrintResult(nTime);
	totalWarriorNum++;
	return 1;
}

const char* CWarrior::names[Warrior_NUM] = { "dragon","ninja","iceman","lion","wolf" };
int CWarrior::InitialLifeValue[Warrior_NUM];
int CHeadquarter::makingSeq[2][Warrior_NUM] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //两个司令部武士的制作顺序序列
const char* CWeapon::Names[WEAPON_NUM] = { "sword","bomb","arrow" };
int CWeapon::InitialForce[WEAPON_NUM];

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

