#include <iostream>//��ս
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#pragma warning (disable:4996)
/*
char * CWarrior::names[WARRIOR_NUM] = { "dragon","ninja","iceman","lion","wolf" };
�췽˾����� iceman��lion��wolf��ninja��dragon ��˳��������ʿ��
����˾����� lion��dragon��ninja��iceman��wolf ��˳��������ʿ��
*/
#define Warrior_NUM 5
using namespace std;
//�ȶ���CHeadquarter�࣬�������ʹ��
class CHeadquarter;
class CWarrior
{
	CHeadquarter* pHeadquarter;
	int kindNo;////��ʿ�������� 0 dragon 1 ninja 2 iceman 3 lion 4 wolf
	int nNo;//��ʿ�ı��
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
	int curMakingSeqIdx;//��ǰ��������ʿΪ�����еĵڼ���
	int warriorNum[Warrior_NUM];//ÿ����ʿ�ĸ���
	int totalWarriorNum;//��ʿ����
	CWarrior* pWarrior[1000];//�໥ָ��֪���Ĺ�ϵ
public:
	friend class CWarrior;//�������
	static int makingSeq[2][Warrior_NUM];
	void Init(int color_, int lv);
	~CHeadquarter();//����pwarrior
	int Produce(int nTime);//����0��1�������Ƿ�produce��ȥ
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

CHeadquarter::~CHeadquarter()//�����������Ҫд�ϣ���Ϊ��1000��Ԫ�أ����ܴܺ�
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

//�����ѵ�Produce����
int CHeadquarter::Produce(int nTime)
{
	if(bStopped)//һ��Ҫд��������ֹͣ����
	return 0;
	//����Ҫȷ�����ĸ���ʼ����
	int nSearchingTimes = 0;//ɨ��
	while (CWarrior::InitialLifeValue[makingSeq[color][curMakingSeqIdx]] > totalLifeValue
		&& nSearchingTimes < Warrior_NUM)
	{
		curMakingSeqIdx = (curMakingSeqIdx + 1) % Warrior_NUM;
		nSearchingTimes++;
	}
	int kindNo = makingSeq[color][curMakingSeqIdx];
	//ȷ��λ�ú�ʼ������
	//���޷���������ʱ
	if(nSearchingTimes==Warrior_NUM) {
		bStopped = true;
		if (color == 0)
			printf("%03d red headquarter stops making warriors\n", nTime);
		else
			printf("%03d blue headquarter stops making warriors\n", nTime);
		return 0;
	}
	//���Լ�������
	totalLifeValue -= CWarrior::InitialLifeValue[kindNo];
	curMakingSeqIdx = (curMakingSeqIdx + 1) % Warrior_NUM;//ָ���´�Ҫ��������ʿ
	warriorNum[kindNo]++;
	//�㶨һЩ��β����
	pWarrior[totalWarriorNum] = new CWarrior(this,kindNo ,totalWarriorNum + 1 );
	pWarrior[totalWarriorNum]->PrintResult(nTime);
	totalWarriorNum++;
	return 1;
}

const char* CWarrior::names[Warrior_NUM] = { "dragon","ninja","iceman","lion","wolf" };
int CWarrior::InitialLifeValue[Warrior_NUM];
int CHeadquarter::makingSeq[2][Warrior_NUM] = { { 2,3,4,1,0 },{3,0,1,2,4} }; //����˾���ʿ������˳������

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