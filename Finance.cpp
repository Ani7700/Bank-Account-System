// Finance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Finance.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;
//�����������ɽڵ㣬������
CFinance::CFinance(int nYear,int nMonth,int nDay)
{
    this->financeInfo.nDay = nDay;
    this->financeInfo.nMonth = nMonth;
    this->financeInfo.nYear = nYear;
    financeInfo.dlIncome = financeInfo.dlOutput = dlBalance =0.0;
}
//�Ƚ����ڣ����ڷ���1��С�ڷ���-1�����ڷ���0
int CFinance::Compare(CFinance &finance)
{
    //�ȱȽ���
    if(financeInfo.nYear > finance.financeInfo.nYear)
        return 1;
    else if (financeInfo.nYear < finance.financeInfo.nYear)
        return -1;

    //�ٱȽ���
    if(financeInfo.nMonth> finance.financeInfo.nMonth)
        return 1;
    else if (financeInfo.nMonth < finance.financeInfo.nMonth)
        return -1;

    //���Ƚ���
    if(financeInfo.nDay> finance.financeInfo.nDay)
        return 1;
    else if (financeInfo.nDay < finance.financeInfo.nDay)
        return -1;
    else
        return 0;
}

//�����¼
void CFinance::Show()
{
    printf("%04dyear%02dmonth%02dday Income and Expenditure:\n",financeInfo.nYear,financeInfo.nMonth,financeInfo.nDay);
    printf("Income:%lf Expenditure:%lf Balance:%lf\n",financeInfo.dlIncome,financeInfo.dlOutput,dlBalance);
    printf("Purpose:%s\n",financeInfo.szPurpose);
}

double CFinance::GetBalance() // ���ؽڵ��ӯ�������������һ�ڵ���
{
    return dlBalance;
}
//����ֵ
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
//��ֵĿ��
void CFinance::GerPurpose(double purs)
{
    CFinance::dlPurpose = purs;
}

//����ڵ��ӯ�����
void CFinance::Calculate(double dlBal)
{
    dlBalance = dlBal + financeInfo.dlIncome - financeInfo.dlOutput;
    dlGap = CFinance::dlPurpose - dlBalance;
}
CNode::CNode(CNode &node)//���ڿ����Ĺ��캯��
{
    //this->pData = new CFinance();
    this->pData = node.pData;
    //this->pData->SetFinance(node.pData->nYear, node.pData->nMonth, node.pData->nDay, node.pData->dlIncome, node.pData->dlOutput, node.pData->szPurpose);
    this->pNext = node.pNext; //ָ���������һ���ڵ��ָ��
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

//���ײ���ӽڵ�
void CList::AddNode(CNode *pnode)
{
    //��������ӿսڵ� ֱ�ӷ���
    if (pnode == NULL)
        return;

    //�������ͷ�ڵ�Ϊ��ֱ�Ӹ�ֵ
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
//ɾ��һ��ָ���Ľڵ㣬���ظýڵ��ָ��
CNode *CList::DeleteNode(CNode *pnode)
{
    //�����ɾ���սڵ� ֱ�ӷ���
    if (pnode == NULL)
        return NULL;

    if (pHead == NULL)
        return NULL;

    //���ҵ�������һ�ڵ�
    CNode *tempNode = pHead;
    while(tempNode->pNext != NULL)
    {
        if (tempNode->pNext == pnode)
            break;

        tempNode= tempNode->pNext;
    }

    tempNode->pNext = pnode->pNext;
    pnode->pNext = NULL;


    //�����¸�ֵ���
    dlbalance = tempNode->pData->GetBalance();
    //�Ӵ˽ڵ㿪ʼ���¼������
    tempNode = GetListNextNode(tempNode);
    while (tempNode!= NULL)
    {
        tempNode->pData->Calculate(dlbalance);
        dlbalance = tempNode->pData->GetBalance();
		tempNode = GetListNextNode(tempNode);
    }
    return pnode;
}

//����һ��ָ�������ݣ����ظ��������ڽڵ��������е�ָ�룬��δ�ҵ�����0
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

//��ӡ��������
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

//ɾ����������
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

//��������ָ���ڵ����һ���ڵ�
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

//��������
void CList::Insert(CNode *node)
{
    if (node == NULL)
        return;

    if (node->pData == NULL)
        return;

    //�����ͷ����
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

			//�����Ƚ���ǰ���ำֵ
			dlbalance = tempNode->pData->GetBalance();

            //����ڵ�
            node->pNext = tempNode->pNext;
            tempNode->pNext = node;

            //�Ӵ˽ڵ㿪ʼ���¼������
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

    //���û���ҵ��ȸýڵ������Ҫ���ڶ�β
    tempNode->pNext = node;
    node->pNext = NULL;
    node->pData->Calculate(dlbalance);
    dlbalance = node->pData->GetBalance();
}

//�������һ���ڵ㣬��������ӯ�����޳���
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

//ѡ�����
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


//¼���µĽڵ�
CNode *AddNewNode(int bDeposit)
{
    //����������
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
    cin >> dAmt; //����Ԥ���Ǯ��
    if (dAmt <= 0)
    {
        cout << "\n\n\n\n\nThat was an invalid amount.." << endl;
        return NULL;
    }

    cout << "\n\n\tEnter  Purpose: ";
    cin >> szPurpose; //����Ŀ��
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

//���
void Deposit()
{
    double dAmt = 0;
    char ans;
    double num = 0 ;
    //ifstream get("Deposit.txt"); //�򿪴���ļ�
    //get>>num;
    //get.close;

    CNode *node = AddNewNode(true);
    if (node == NULL)
        return;


    cout << "\n\tContinue with deposit <y/n>:";
    cin >> ans;
    if (ans == 'y'|| ans == 'Y')
    {
        dAmt = dAmt + num;  //���µĴ������
        list->Insert(node);
        //ofstream save("Deposit.txt");
        //save << dAmt << endl;
        //save.close();
        cout << "\n\n\n\n\n            Your deposit was successful..." << endl;

    }

}

//ȡ��
void Withdraw()
{
    double dAmt = 0;
    char ans;
    double num = 0 ;
    //ifstream get("Deposit.txt"); //�򿪴���ļ�
    //get>>num;
    //get.close;


    CNode *node = AddNewNode(false);
    if (node == NULL)
        return;


    cout << "\n\tContinue with Withdraw <y/n>:";
    cin >> ans;
    if (ans == 'y'|| ans == 'Y')
    {
        dAmt = dAmt + num;  //���µĴ������
        list->Insert(node);
        //ofstream save("Deposit.txt");
        //save << dAmt << endl;
        //save.close();
        cout << "\n\n\n\n\n            Your Withdraw was successful..." << endl;

    }

}

//�鿴������֧ ֻ��֧��:-1 ֻ������:1 ���ж���:0
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

//�������
int InPutDate(int &nYear,int &nMonth, int &nDay)
{
    cout << "\n\n\tEnter year:  ";
    cin >> nYear; //�������
    if ((nYear <= 1900) || (nYear>2100))
    {
        cout << "\n\n\n\n\n           That was an invalid year..(1900-2100)" << endl;
        return -1;
    }

    cout << "\n\n\tEnter month:  ";
    cin >> nMonth; //�����·�
    if ((nMonth <= 0) || (nMonth>12))
    {
        cout << "\n\n\n\n\n           That was an invalid Month.." << endl;
        return -1;
    }

    cout << "\n\n\tEnter day: ";
    cin >> nDay; //��������

    if ((nDay <= 0) || (nDay>getMaxDay(nYear,nMonth)))
    {
        cout << "\n\n\n\n\n           That was an invalid Day.." << endl;
        return -1;
    }


    return 0;
}

//��ȡÿ���µ����ֵ 1,3,5,7,8,10,12 Ϊ31��  4,6,9,11Ϊ30�� ����2Ϊ29�� ����Ϊ28��
int getMaxDay(int nYear,int nMonth)
{
    if ((nMonth == 1) || (nMonth == 3) || (nMonth == 5)||(nMonth == 7) ||(nMonth == 8)  ||(nMonth == 10) ||(nMonth == 12))
        return 31;

    if ((nMonth == 4) || (nMonth == 6) || (nMonth == 9)||(nMonth == 11))
        return 30;


    bool bSpecialYear = false;
    //������100 Ҫ����00
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

//�������ڲ�������
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


//��������ɾ������
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


//���������޸�����
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
    cin >> dlIncome; //����Ԥ���Ǯ��
    if (dlIncome < 0)
    {
        cout << "\n\n\n\n\n           That was an invalid amount.." << endl;
        return;
    }

    cout << "\n\n\tEnter  dlOutput: $";
    cin >> dlOutput; //����Ԥ���Ǯ��
    if (dlOutput < 0)
    {
        cout << "\n\n\n\n\n           That was an invalid amount.." << endl;
        return;
    }


    cout << "\n\n\tEnter  Purpose: ";
    cin >> szPurpose; //����Ŀ��

    //��ɾ������
    node = list->DeleteNode(node);

    CFinance * pData = node->GetData();
    pData->SetFinance(nYear, nMonth, nDay, dlIncome, dlOutput, szPurpose);

    list->AddNode(node);
    cout << "\n\n\n\n\n Successfully altered. Please search again. " << endl;
}

