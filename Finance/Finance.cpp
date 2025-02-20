// Finance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Finance.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;
//根据日期生成节点，查找用
CFinance::CFinance(int nYear,int nMonth,int nDay)
{
    this->financeInfo.nDay = nDay;
    this->financeInfo.nMonth = nMonth;
    this->financeInfo.nYear = nYear;
    financeInfo.dlIncome = financeInfo.dlOutput = dlBalance =0.0;
}
//比较日期，大于返回1，小于返回-1，等于返回0
int CFinance::Compare(CFinance &finance)
{
    //先比较年
    if(financeInfo.nYear > finance.financeInfo.nYear)
        return 1;
    else if (financeInfo.nYear < finance.financeInfo.nYear)
        return -1;

    //再比较月
    if(financeInfo.nMonth> finance.financeInfo.nMonth)
        return 1;
    else if (financeInfo.nMonth < finance.financeInfo.nMonth)
        return -1;

    //最后比较天
    if(financeInfo.nDay> finance.financeInfo.nDay)
        return 1;
    else if (financeInfo.nDay < finance.financeInfo.nDay)
        return -1;
    else
        return 0;
}

//输出记录
void CFinance::Show()
{
    printf("%04dyear%02dmonth%02dday Income and Expenditure:\n",financeInfo.nYear,financeInfo.nMonth,financeInfo.nDay);
    printf("Income:%lf Expenditure:%lf Balance:%lf\n",financeInfo.dlIncome,financeInfo.dlOutput,dlBalance);
    printf("Purpose:%s\n",financeInfo.szPurpose);
}

double CFinance::GetBalance() // 返回节点的盈余情况，计算下一节点用
{
    return dlBalance;
}
//对象赋值
void CFinance::SetFinance(int nYear,int nMonth,int nDay,double dlIncome,double dlOutput, char *szPurpose)
{
    this->financeInfo.nDay = nDay;
    this->financeInfo.nMonth = nMonth;
    this->financeInfo.nYear = nYear;
    this->financeInfo.dlIncome = dlIncome;
    this->financeInfo.dlOutput = dlOutput;
    memcpy(this->financeInfo.szPurpose,szPurpose,20);
    this->financeInfo.szPurpose[19] = '\0';
}
//赋值目标
void CFinance::GerPurpose(double purs)
{
    CFinance::dlPurpose = purs;
}

//计算节点的盈余情况
void CFinance::Calculate(double dlBal)
{
    dlBalance = dlBal + financeInfo.dlIncome - financeInfo.dlOutput;
    dlGap = CFinance::dlPurpose - dlBalance;
}
CNode::CNode(CNode &node)//用于拷贝的构造函数
{
    //this->pData = new CFinance();
    this->pData = node.pData;
    //this->pData->SetFinance(node.pData->nYear, node.pData->nMonth, node.pData->nDay, node.pData->dlIncome, node.pData->dlOutput, node.pData->szPurpose);
    this->pNext = node.pNext; //指向链表的下一个节点的指针
}

void CNode::InputData(CFinance *pData)
{
    this->pData = pData;
    this->pNext = NULL;
}
void CNode::writeFile()
{
    FILE *fp;
    fp = fopen("st.txt", "a+");
    if (!fp)
    {
        return;
    }
    fwrite(&pData->financeInfo, sizeof(INFO) , 1 ,fp);
    fclose(fp);
}


void CList::readFile()
{
    FILE *fp;
    INFO financeInfo;
    fp = fopen("st.txt", "r");
    if (!fp)
    {
        return;
    }

    while(fread(&financeInfo , sizeof(INFO) , 1 ,fp) != 0)
    {
        CFinance *finance= new CFinance();
        finance->SetFinance(financeInfo.nYear, financeInfo.nMonth, financeInfo.nDay, financeInfo.dlIncome, financeInfo.dlOutput, financeInfo.szPurpose);
        //finance->Show();
        CNode *node = new CNode();
        node->InputData(finance);
        Insert(node);
    }
    fclose(fp);

}

void CList::writeFile()
{
    remove("st.txt");

    if (pHead == NULL)
        return;
    FILE *fp;
    fp = fopen("st.txt", "a+");
    if (!fp)
    {
        return;
    }
    int num = 0;

    CNode *node = pHead->pNext;
    CFinance *pData = NULL;
    while (node!= NULL)
    {
        pData = node->GetData();
        if (pData != NULL)
        {
            fwrite(&pData->financeInfo, sizeof(INFO) , 1 ,fp);
            num ++;
        }

        node = node->pNext;
    }
    fclose(fp);

    cout << "Save" << num <<"records";
}

//在首部添加节点
void CList::AddNode(CNode *pnode)
{
    //如果是增加空节点 直接返回
    if (pnode == NULL)
        return;

    //如果链表头节点为空直接赋值
    if (pHead == NULL)
    {
		pHead = new CNode();
        CFinance *finance = new CFinance();
        pHead->InputData(finance);
        pHead->pNext = pnode;
        return;
    }

    pnode->pNext = pHead->pNext;
    pHead->pNext = pnode;
}
//删除一个指定的节点，返回该节点的指针
CNode *CList::DeleteNode(CNode *pnode)
{
    //如果是删除空节点 直接返回
    if (pnode == NULL)
        return NULL;

    if (pHead == NULL)
        return NULL;

    //先找到链表上一节点
    CNode *tempNode = pHead;
    while(tempNode->pNext != NULL)
    {
        if (tempNode->pNext == pnode)
            break;

        tempNode= tempNode->pNext;
    }

    tempNode->pNext = pnode->pNext;
    pnode->pNext = NULL;


    //先重新赋值余额
    dlbalance = tempNode->pData->GetBalance();
    //从此节点开始从新计算余额
    tempNode = GetListNextNode(tempNode);
    while (tempNode!= NULL)
    {
        tempNode->pData->Calculate(dlbalance);
        dlbalance = tempNode->pData->GetBalance();
		tempNode = GetListNextNode(tempNode);
    }
    return pnode;
}

//查找一个指定的数据，返回改数据所在节点在链表中的指针，若未找到返回0
CNode *CList::LookUp(CFinance &finance)
{
    CNode *tempNode = pHead->pNext;
    while(tempNode != NULL)
    {
        if (tempNode->pData != NULL)
        {
            if (tempNode->pData->Compare(finance) == 0)
            {
                return tempNode;
            }
        }
        tempNode= tempNode->pNext;
    }

    return NULL;
}

//打印整个链表
void CList::ShowList(int iChoice)
{
    int iNum = 0;
    if (pHead == NULL)
    {
        cout << "\n  0 records are searched in total \n\n" << endl;
        return;
    }
    CNode *tempNode = pHead->pNext;
    while(tempNode != NULL)
    {
        if (tempNode->pData != NULL)
        {
            if (((iChoice >= 0) && (tempNode->pData->financeInfo.dlIncome > 0))
                || ((iChoice <= 0) && (tempNode->pData->financeInfo.dlOutput > 0)))
            {
                tempNode->pData->Show();
                iNum++;
            }
        }
        tempNode= tempNode->pNext;
    }

    cout << iNum<< "records are searched in total \n\n" << endl;
}

CNode *CList::SearchListByDate(int nYear, int nMonth, int nDay)
{
    int iNum = 0;
    if (pHead == NULL)
        return NULL;

    if (pHead->pNext == NULL)
        return NULL;

    CNode *tempNode = pHead->pNext;
    while(tempNode != NULL)
    {
        if (tempNode->pData != NULL)
        {
            if ((tempNode->pData->financeInfo.nYear == nYear)
                && (tempNode->pData->financeInfo.nMonth == nMonth)
                && (tempNode->pData->financeInfo.nDay == nDay))
            {
                //tempNode->pData->Show();
                return tempNode;
            }
        }
        tempNode= tempNode->pNext;
    }

    return NULL;
}

//删除整个链表
void CList::DeleteList()
{
    CNode *tempNode = pHead->pNext;
    CNode *delNode = NULL;
    pHead->pNext = NULL;
    while(tempNode != NULL)
    {
        delNode = tempNode;
        tempNode= tempNode->pNext;
        delete delNode;
    }
}

//返回链表指定节点的下一个节点
CNode *CList::GetListNextNode(CNode *node)
{
    if (node == NULL)
        return NULL;

    if (pHead == NULL)
        return NULL;

    CNode *tempNode = pHead->pNext;
    while(tempNode != NULL)
    {

        if (tempNode == node)
        {
            return tempNode->pNext;
        }

        tempNode= tempNode->pNext;
    }

    return NULL;
}

//插入排序
void CList::Insert(CNode *node)
{
    if (node == NULL)
        return;

    if (node->pData == NULL)
        return;

    //如果在头插入
    if (pHead== NULL)
    {
		pHead = new CNode();
        CFinance *finance = new CFinance();
        pHead->InputData(finance);
        pHead->pNext = node;

        node->pData->Calculate(dlbalance);
        dlbalance = node->pData->GetBalance();
        return;
    }

    CNode *tempNode = pHead, *pNode = NULL ;
    while(tempNode->pNext != NULL)
    {
        if (tempNode->pNext->pData == NULL)
		{
			tempNode= tempNode->pNext;
            continue;
		}
        if (tempNode->pNext->pData->Compare(*(node->pData)) > 0)
        {

			//首先先将当前结余赋值
			dlbalance = tempNode->pData->GetBalance();

            //插入节点
            node->pNext = tempNode->pNext;
            tempNode->pNext = node;

            //从此节点开始从新计算余额
            node->pData->Calculate(dlbalance);
            dlbalance = node->pData->GetBalance();
            pNode = GetListNextNode(node);
            while (pNode!= NULL)
            {
                pNode->pData->Calculate(dlbalance);
                dlbalance = pNode->pData->GetBalance();
				pNode = GetListNextNode(pNode);
            }
            return;
        }

        tempNode= tempNode->pNext;
    }

    //如果没有找到比该节点大，则需要插在队尾
    tempNode->pNext = node;
    node->pNext = NULL;
    node->pData->Calculate(dlbalance);
    dlbalance = node->pData->GetBalance();
}

//返回最后一个节点，看看最后的盈余有无赤字
CNode *CList::GetLastNode()
{
    CNode *tempNode = pHead;
    if (pHead == NULL)
        return NULL;

    while(tempNode->pNext != NULL)
    {
        tempNode= tempNode->pNext;
    }

    return tempNode;
}


int main()
{
    list = new CList();
    list->readFile();
	char ans;
    while (Choice() != 0)
	{
		cout << "\n\t exit? <y/n>:";
		cin >> ans;
		if (ans == 'y'|| ans == 'Y')
		{
			break;
		}
	}

	return 0;
}

//选择界面
int Choice()
{
	char select;
    cout << "Money Manager    ";
    cout << "\n\n\t<1> Insert income records" << endl;
    cout << "\n\t<2> Insert expenditure records" << endl;
    cout << "\n\t<3> Display income records" << endl;
    cout << "\n\t<4> Display expenditure records" << endl;
    cout << "\n\t<5> Display balance" << endl;
    cout << "\n\t<6> Search records by date" << endl;
    cout << "\n\t<7> Change records by date" << endl;
    cout << "\n\t<8> Selete records by date" << endl;
    cout << "\n\t<0> Exit" << endl;
    cout << "\n\n\tEnter Selection:  ";
    cin >> select;
    if (select == '1')
    {
        cout << "\n\n\n\n\nStart to insert income records  \n\n";
        Deposit();
    }
    else if (select == '2')
    {
        cout << "\n\n\n\n\nStart to insert expenditure records  \n\n";
        Withdraw();
    }
    else if (select == '3')
    {
        cout << "\n\n\n\n\nDisplay all income records  \n\n";
        ViewTotal(1);
    }
    else if (select == '4')
    {
        cout << "\n\n\n\n\nDisplay all expenditure records  \n\n";
        ViewTotal(-1);
    }
    else if (select == '5')
    {
        cout << "\n\n\n\n\nDisplay all balance  \n\n";
        ViewTotal(0);
    }
    else if (select == '6')
    {
        cout << "\n\n\n\n\nSearch records by date  \n\n";
        SearchByDate();
    }
    else if (select == '7')
    {
        cout << "\n\n\n\n\nChange records by date  \n\n";
        UpdateByDate();
    }
    else if (select == '8')
    {
        cout << "\n\n\n\n\nDelete records by date  \n\n";
        DeleteByDate();
    }
    else if (select == '0')
    {
        list->writeFile();
        return 0;
    }

	return -1;
}


//录入新的节点
CNode *AddNewNode(int bDeposit)
{
    //输入年月日
    int nYear,nMonth,nDay;
    double dAmt;
    char szPurpose[255];
    memset(szPurpose,0,20);
    if (InPutDate(nYear, nMonth, nDay) == -1)
        return NULL;

    CNode *node = NULL;
    node = list->SearchListByDate(nYear, nMonth, nDay);
    if (node != NULL)
    {
        cout << "\n\n\n\n\nThis date already has records, please change..." << endl;
        return NULL;
    }

    cout << "\n\n\tEnter  amount: $";
    cin >> dAmt; //输入预存的钱数
    if (dAmt <= 0)
    {
        cout << "\n\n\n\n\nThat was an invalid amount.." << endl;
        return NULL;
    }

    cout << "\n\n\tEnter  Purpose: ";
    cin >> szPurpose; //输入目的
    szPurpose[19] = '\0';
    CFinance *finance = new CFinance();
    if (bDeposit)
        finance->SetFinance(nYear, nMonth,nDay, dAmt, 0, szPurpose);
    else
        finance->SetFinance(nYear, nMonth,nDay, 0, dAmt, szPurpose);

    node = new CNode();
    node->InputData(finance);
    return node;
}

//存款
void Deposit()
{
    double dAmt = 0;
    char ans;
    double num = 0 ;
    //ifstream get("Deposit.txt"); //打开存款文件
    //get>>num;
    //get.close;

    CNode *node = AddNewNode(true);
    if (node == NULL)
        return;


    cout << "\n\tContinue with deposit <y/n>:";
    cin >> ans;
    if (ans == 'y'|| ans == 'Y')
    {
        dAmt = dAmt + num;  //更新的存款数额
        list->Insert(node);
        //ofstream save("Deposit.txt");
        //save << dAmt << endl;
        //save.close();
        cout << "\n\n\n\n\n            Your deposit was successful..." << endl;

    }

}

//取款
void Withdraw()
{
    double dAmt = 0;
    char ans;
    double num = 0 ;
    //ifstream get("Deposit.txt"); //打开存款文件
    //get>>num;
    //get.close;


    CNode *node = AddNewNode(false);
    if (node == NULL)
        return;


    cout << "\n\tContinue with Withdraw <y/n>:";
    cin >> ans;
    if (ans == 'y'|| ans == 'Y')
    {
        dAmt = dAmt + num;  //更新的存款数额
        list->Insert(node);
        //ofstream save("Deposit.txt");
        //save << dAmt << endl;
        //save.close();
        cout << "\n\n\n\n\n            Your Withdraw was successful..." << endl;

    }

}

//查看所有收支 只查支出:-1 只查收入:1 所有都查:0
void ViewTotal(int iChoice)
{
    list->ShowList(iChoice);

    double dlbalance = 0;
    CNode *node = list->GetLastNode();
    if (node != NULL)
    {
        CFinance *finance = node->GetData();
        if (finance != NULL)
            dlbalance = finance->GetBalance();
    }

    cout << "Currently all the balance is: " << dlbalance << endl;

    if (dlbalance < 0)
        cout << "The balance is negative, please be cautious!" << endl;
}

void SetGoal()
{
    double dGoal = 0.0;
    cout << "Set Goal Total Balance left \n\n";
    cin >> dGoal;
    if (dGoal <= 0)
    {
        cout << "\n\n\n\n\n           That was an invalid amount.." << endl;
        return;
    }

    CFinance::GerPurpose(dGoal);
}

//输入年份
int InPutDate(int &nYear,int &nMonth, int &nDay)
{
    cout << "\n\n\tEnter year:  ";
    cin >> nYear; //输入年份
    if ((nYear <= 1900) || (nYear>2100))
    {
        cout << "\n\n\n\n\n           That was an invalid year..(1900-2100)" << endl;
        return -1;
    }

    cout << "\n\n\tEnter month:  ";
    cin >> nMonth; //输入月份
    if ((nMonth <= 0) || (nMonth>12))
    {
        cout << "\n\n\n\n\n           That was an invalid Month.." << endl;
        return -1;
    }

    cout << "\n\n\tEnter day: ";
    cin >> nDay; //输入日期

    if ((nDay <= 0) || (nDay>getMaxDay(nYear,nMonth)))
    {
        cout << "\n\n\n\n\n           That was an invalid Day.." << endl;
        return -1;
    }


    return 0;
}

//获取每个月的最大值 1,3,5,7,8,10,12 为31天  4,6,9,11为30天 闰年2为29天 其他为28天
int getMaxDay(int nYear,int nMonth)
{
    if ((nMonth == 1) || (nMonth == 3) || (nMonth == 5)||(nMonth == 7) ||(nMonth == 8)  ||(nMonth == 10) ||(nMonth == 12))
        return 31;

    if ((nMonth == 4) || (nMonth == 6) || (nMonth == 9)||(nMonth == 11))
        return 30;


    bool bSpecialYear = false;
    //闰年满100 要除以00
    if (nYear % 100 == 0)
    {
        if (nYear % 400 == 0)
            bSpecialYear = true;
    }
    else
    {
        if (nYear % 4 == 0)
            bSpecialYear = true;
    }

    if (nMonth == 2)
    {
        if (bSpecialYear)
            return 29;
        else
            return 28;
    }

	return 0;
}

//根据日期查找数据
void SearchByDate()
{
    int nYear,nMonth, nDay;
    if (InPutDate(nYear, nMonth, nDay) == -1)
        return;

    CNode *node = NULL;
    node = list->SearchListByDate(nYear, nMonth, nDay);
    if (node != NULL)
        node->ShowNode();
    else
    {
        cout << "\n\n\n\n\n  This date doesn't have a record." << endl;

    }
}


//根据日期删除数据
void DeleteByDate()
{
    int nYear,nMonth, nDay;
    if (InPutDate(nYear, nMonth, nDay) == -1)
        return;

    CNode *node = NULL;
    node = list->SearchListByDate(nYear, nMonth, nDay);
    if (node == NULL)
    {
        cout << "\n\n\n\n\n  This date doesn't have a record." << endl;
        return;
    }

    node = list->DeleteNode(node);
    CFinance *pData = node->GetData();
    delete pData;
    delete node;
    cout << "\n\n\n\n\n  Successfully deleted. Please search again. " << endl;
}


//根据日期修改数据
void UpdateByDate()
{
    int nYear,nMonth, nDay;
    if (InPutDate(nYear, nMonth, nDay) == -1)
        return;
    double dlIncome,dlOutput;
    char szPurpose[20];
    CNode *node = NULL;
    node = list->SearchListByDate(nYear, nMonth, nDay);
    if (node == NULL)
    {
        cout << "\n\n\n\n\n This date doesn't have a record. " << endl;
        return;
    }

    cout << "\n\n\tEnter  dlIncome: $";
    cin >> dlIncome; //输入预存的钱数
    if (dlIncome < 0)
    {
        cout << "\n\n\n\n\n           That was an invalid amount.." << endl;
        return;
    }

    cout << "\n\n\tEnter  dlOutput: $";
    cin >> dlOutput; //输入预存的钱数
    if (dlOutput < 0)
    {
        cout << "\n\n\n\n\n           That was an invalid amount.." << endl;
        return;
    }


    cout << "\n\n\tEnter  Purpose: ";
    cin >> szPurpose; //输入目的

    //先删除数据
    node = list->DeleteNode(node);

    CFinance * pData = node->GetData();
    pData->SetFinance(nYear, nMonth, nDay, dlIncome, dlOutput, szPurpose);

    list->AddNode(node);
    cout << "\n\n\n\n\n Successfully altered. Please search again. " << endl;
}

