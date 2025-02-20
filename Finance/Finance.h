
typedef struct
{
	int nYear,nMonth,nDay; //������
	double dlIncome,dlOutput; //���룬֧����Ŀ
	char szPurpose[20];//��;��Ŀ��
}INFO;


class CFinance  //����������
{
private:

	double dlBalance;// ����
	static double dlPurpose; //Ŀ��
	double dlGap; //��Ŀ��Ĳ��

public:
    INFO financeInfo;
	CFinance()
	{
		financeInfo.dlIncome = financeInfo.dlOutput = dlBalance =0.0;
	}
	CFinance(int,int,int);  //�����������ɽڵ㣬������
	int Compare(CFinance &) ; //�Ƚ����ڣ����ڷ���1��С�ڷ���-1�����ڷ���0
	void Show();  //�����¼
	void SetFinance(int ,int ,int ,double,double, char *); //����ֵ
    static void GerPurpose(double purs); //��ֵĿ��
	double ShowPur(){ return dlPurpose;}; //����Ŀ��
	void Calculate(double dlBal); //����ڵ��ӯ�����
	double GetBalance(); // ���ؽڵ��ӯ�������������һ�ڵ���
    void ModifyData(); //�޸Ľڵ��¼
};



double CFinance::dlPurpose = 0.0;

class CNode //����ڵ���
{
private:
    CFinance *pData; // ����ָ���������ָ��
    CNode *pNext; //ָ���������һ���ڵ��ָ��
public:
    CNode(){pData=0;pNext=0;} //�ڵ㹹�캯��
    CNode(CNode &node);//���ڿ����Ĺ��캯��
    void InputData(CFinance *pdata);
    void ShowNode(){pData->Show();};
    CFinance *GetData(){return pData;};
    void writeFile();
    friend class CList; //����������Ϊ��Ԫ
};

class CList// ����������
{
protected:
    CNode *pHead; //����ͷ�ڵ�ָ��
    double dlbalance;
public:
    CList(){pHead = 0; dlbalance = 0;};
    //~CList{DeleteList();};
    void AddNode(CNode *pnode); //���ײ���ӽڵ�
    CNode *DeleteNode(CNode *); //ɾ��һ��ָ���Ľڵ㣬���ظýڵ��ָ��
    CNode *LookUp(CFinance &); //����һ��ָ�������ݣ����ظ��������ڽڵ��������е�ָ�룬��δ�ҵ�����0
    void ShowList(int iChoice);//��ӡ��������
    void DeleteList() ; //ɾ����������
    CNode *GetListHead(){return pHead;} ;// �����׽ڵ�
    CNode *GetListNextNode(CNode *); //��������ָ���ڵ����һ���ڵ�
    void Insert(CNode *); //��������
    CNode *GetLastNode(); //�������һ���ڵ㣬��������ӯ�����޳���
    CNode *SearchListByDate(int nYear, int nMonth, int nDay);
    void readFile();
    void writeFile();

};

CList *list = NULL;

void initList();  //��ʼ���б�
CNode *AddNewNode(int bDeposit); //�����µ�node
void Deposit(); //���
void Withdraw();//ȡ��
int Choice();
void ViewTotal(int iChoice);
void SearchByDate();
void UpdateByDate();
void DeleteByDate();
int getMaxDay(int nYear,int nMonth);

void SetGoal();
int InPutDate(int &nYear,int &nMonth, int &nDay);

