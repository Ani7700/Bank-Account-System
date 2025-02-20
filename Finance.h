
typedef struct
{
	int nYear,nMonth,nDay; //年月日
	double dlIncome,dlOutput; //收入，支出数目
	char szPurpose[20];//用途，目的
}INFO;


class CFinance  //财务数据类
{
private:

	double dlBalance;// 结余
	static double dlPurpose; //目标
	double dlGap; //据目标的差额

public:
    INFO financeInfo;
	CFinance()
	{
		financeInfo.dlIncome = financeInfo.dlOutput = dlBalance =0.0;
	}
	CFinance(int,int,int);  //根据日期生成节点，查找用
	int Compare(CFinance &) ; //比较日期，大于返回1，小于返回-1，等于返回0
	void Show();  //输出记录
	void SetFinance(int ,int ,int ,double,double, char *); //对象赋值
    static void GerPurpose(double purs); //赋值目标
	double ShowPur(){ return dlPurpose;}; //返回目标
	void Calculate(double dlBal); //计算节点的盈余情况
	double GetBalance(); // 返回节点的盈余情况，计算下一节点用
    void ModifyData(); //修改节点记录
};



double CFinance::dlPurpose = 0.0;

class CNode //定义节点类
{
private:
    CFinance *pData; // 用于指向数据类的指针
    CNode *pNext; //指向链表的下一个节点的指针
public:
    CNode(){pData=0;pNext=0;} //节点构造函数
    CNode(CNode &node);//用于拷贝的构造函数
    void InputData(CFinance *pdata);
    void ShowNode(){pData->Show();};
    CFinance *GetData(){return pData;};
    void writeFile();
    friend class CList; //定义链表类为友元
};

class CList// 定义链表类
{
protected:
    CNode *pHead; //链表头节点指针
    double dlbalance;
public:
    CList(){pHead = 0; dlbalance = 0;};
    //~CList{DeleteList();};
    void AddNode(CNode *pnode); //在首部添加节点
    CNode *DeleteNode(CNode *); //删除一个指定的节点，返回该节点的指针
    CNode *LookUp(CFinance &); //查找一个指定的数据，返回改数据所在节点在链表中的指针，若未找到返回0
    void ShowList(int iChoice);//打印整个链表
    void DeleteList() ; //删除整个链表
    CNode *GetListHead(){return pHead;} ;// 返回首节点
    CNode *GetListNextNode(CNode *); //返回链表指定节点的下一个节点
    void Insert(CNode *); //插入排序
    CNode *GetLastNode(); //返回最后一个节点，看看最后的盈余有无赤字
    CNode *SearchListByDate(int nYear, int nMonth, int nDay);
    void readFile();
    void writeFile();

};

CList *list = NULL;

void initList();  //初始化列表
CNode *AddNewNode(int bDeposit); //输入新的node
void Deposit(); //存款
void Withdraw();//取款
int Choice();
void ViewTotal(int iChoice);
void SearchByDate();
void UpdateByDate();
void DeleteByDate();
int getMaxDay(int nYear,int nMonth);

void SetGoal();
int InPutDate(int &nYear,int &nMonth, int &nDay);

