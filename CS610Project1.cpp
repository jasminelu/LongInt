// LongInt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "conio.h"
#include <cstring>
#include "math.h"

using namespace std;

const int EVERY_NODE_LEN = 4; //the digits in every node


struct LNode //node
{
	int value;
	LNode* prev;
	LNode* next;
};

class LList //long integer list
{
public:
	LNode* head; //the head node
	int count; //the number of nodes

	LList() //constructor
	{
		head = (LNode*) malloc(sizeof(LNode));
		head->value = 0;
		head->prev = NULL;
		head->next = NULL;
		count = 0;

	}

	~LList(){} //destructor

	LNode* FirstRight()
	{
		LNode* tempNode = head;
		while (tempNode->next != NULL)
			tempNode = tempNode->next;
		return tempNode;
	}

	LNode* FirstLeft()
	{
		return head->next;
	}

	LNode* NextRight(LNode* thisNode)
	{
		return thisNode->next;
	}

	LNode* NextLeft(LNode* thisNode)
	{
		return thisNode->prev;
	}

	LNode* CountFromRight(int N) //return the no.N node from right
	{
		if(N > count)
			return NULL;
		LNode* temp = FirstRight();
		for(int i = 0; i < N - 1; i++)
		{
			temp = temp->prev;
		}
		return temp;
	}

	void InsertRight(LNode* insertNode)
	{
		this->FirstRight()->next = insertNode;
		insertNode->prev = this->FirstRight();
		insertNode->next = NULL;
		count++;
	}

	void InsertLeft(LNode* insertNode)
	{

		if(head->next != NULL)
		{
			insertNode->next = head->next;
			head->next->prev = insertNode;
			head->next = insertNode;
			insertNode->prev = head;
		}
		else
		{
			head->next = insertNode;
			insertNode->prev = head;
			insertNode->next = NULL;
		}
		count++;
	}


};


class LongInt
{
public:
	LList* intData; //long integer list

	LongInt() //constructor
	{
		intData = new LList();
		intData->head->value = 0;
	}

	~LongInt(){} //destructor

	void Clear()
	{
		delete intData;
		intData = NULL;
		intData = new LList();
		intData->head->value = 0;

		LNode* zeroNode = (LNode*) malloc(sizeof(LNode));
		zeroNode->value = 0;
		zeroNode->prev = intData->head;
		zeroNode->next = NULL;
		intData->head->next = zeroNode;

		intData->count++;
	}

	void Delete()
	{
		this->~LongInt();
	}

	int GetSign() //get the sigh of the number
	{
		return intData->head->value;
	}

	void SetSign(int sign) //set sign
	{
		intData->head->value = sign;
	}

	int Digits() //return the digits
	{
		if (intData->head->next == NULL)
			return 0;
		if (intData->head->next->value == 0)
			return 1;

		LNode* temp = intData->head->next;
		int result = 0;
		result += (int)log10((double)temp->value) + 1;
		while(temp->next != NULL)
		{
			result += EVERY_NODE_LEN;
			temp = temp->next;
		}

		return result;
	}

	void SetData(char* longNum) //set long integer
	{
		delete intData;
		intData = NULL;
		intData = new LList();
		intData->head->value = 0;

		int numLen = strlen(longNum);
		char* tempStr = NULL;
		while(numLen > 0) //get the last four digits everytime
		{
			tempStr = new char[EVERY_NODE_LEN + 1];
			tempStr[EVERY_NODE_LEN] = '\0';
			for(int i = EVERY_NODE_LEN-1, j = 0; i >= 0; i--, j++)
			{
				if (numLen-1-j >= 0)
					tempStr[i] = longNum[numLen-1-j];
				else
					tempStr[i] = '0';
			}

			LNode* tempNode = (LNode*) malloc(sizeof(LNode));
			tempNode->value = atoi(tempStr); //turn into integer
			tempNode->next = tempNode->prev = NULL;
			intData->InsertLeft(tempNode);

			tempNode = NULL;
			tempStr = NULL;
			numLen -= EVERY_NODE_LEN;
		}

	}

	char* GetData() //get the long integer,return the string
	{
		LNode* tempNode = intData->head;
		char* result = new char[this->Digits()+2]; //the length of the result=sign+digits+ '\0'

		if(tempNode->value == 0)
			result[0] = '+';
		else
			result[0] = '-';
		result[1] = '\0';

		while (tempNode->next != NULL)
		{
			tempNode = tempNode->next;
			char* tempstr = new char[EVERY_NODE_LEN+1];
			itoa(tempNode->value, tempstr, 10); //turn into srings

			//if there is a '0',then the length is not required EVERY_NODE_LEN£¬we need add 0
			if(tempNode->prev != intData->head && strlen(tempstr) < EVERY_NODE_LEN)
			{
				for(int i = strlen(tempstr); i < EVERY_NODE_LEN; i++)
					strcat(result, "0");
			}

			strcat(result, tempstr);
		}
		return result;
	}

	int CompAbs(LongInt* otherInt) //compare the absolute value:> return 1, < return -1
	{
		if (Digits() > otherInt->Digits())
			return 1;
		else if(Digits() < otherInt->Digits())
			return -1;
		else
		{

			LNode* myNode = intData->head->next;
			LNode* otherNode = otherInt->intData->head->next;
			while(myNode != NULL)
			{
				if(myNode->value > otherNode->value)
					return 1;
				else if (myNode->value < otherNode->value)
					return -1;
				else //compare next node
				{
					myNode = myNode->next;
					otherNode = otherNode->next;
				}
			}
			return 0;
		}
	}

	int OverFlow(int testInt)
	{
		return testInt / (int)pow(10.0, (double)EVERY_NODE_LEN);
	}


	LongInt* Add(LongInt* otherInt) //addition
	{
		LongInt *result = new LongInt();
		LList* otherList = otherInt->intData;
		LList* resultList = result->intData;
		LNode* myNode = intData->FirstRight();
		LNode* otherNode = otherList->FirstRight();

		int addition = 0;
		int carry = 0;
		LNode* tempNode;

		while(intData->count != otherList->count)
		{
			LNode* zeroNode = new LNode();
			zeroNode->value = 0;
			zeroNode->next = zeroNode->prev = NULL;
			if(intData->count > otherList->count)
				otherList->InsertLeft(zeroNode);
			else
				intData->InsertLeft(zeroNode);
			zeroNode = NULL;
		}

		if(intData->head->value == otherList->head->value)
		{
			resultList->head->value = intData->head->value;

			while(myNode != intData->head)
			{
				addition = myNode->value + otherNode->value + carry;
				carry = OverFlow(addition);
				addition = addition - carry * (int)pow(10.0, (double)EVERY_NODE_LEN);

				tempNode = new LNode();
				tempNode->value = addition;
				tempNode->prev = tempNode->next = NULL;
				resultList->InsertLeft(tempNode);
				tempNode = NULL;
				myNode = intData->NextLeft(myNode);
				otherNode = otherList->NextLeft(otherNode);
			}

			if(carry != 0)
			{
				LNode* carryNode = new LNode();
				carryNode->value = carry;
				carryNode->next = carryNode->prev = NULL;
				resultList->InsertLeft(carryNode);
			}
		}
		else //opposite sign
		{
			LList *leftList, *rightList;

			int abs = CompAbs(otherInt);
			if(abs == 0)
			{
				LNode* zeroNode = new LNode();
				zeroNode->value = 0;
				zeroNode->next = zeroNode->prev = NULL;
				resultList->InsertLeft(zeroNode);
				return result;
			}
			else if(abs == 1)
			{
				resultList->head->value = intData->head->value;
				leftList = intData;
				rightList = otherInt->intData;
			}
			else
			{
				resultList->head->value = otherInt->intData->head->value;
				leftList = otherInt->intData;
				rightList = intData;
			}

			myNode = leftList->FirstRight();
			otherNode = rightList->FirstRight();
			carry = 0;

			while(myNode != leftList->head)
			{

				addition = myNode->value - otherNode->value + carry;
				if(addition < 0)
				{
					carry = -1;
					addition = addition + (int)pow(10.0, (double)EVERY_NODE_LEN);
				}
				else
					carry = 0;

				tempNode = new LNode();
				tempNode->value = addition;
				tempNode->prev = tempNode->next = NULL;
				resultList->InsertLeft(tempNode);
				tempNode = NULL;
				myNode = leftList->NextLeft(myNode);
				otherNode = rightList->NextLeft(otherNode);
			}
		}

		return result;
	}

	LongInt* Minus(LongInt* otherInt) //subtraction
	{
		int oldsign = otherInt->GetSign();
		if (oldsign == 0)
			otherInt->SetSign(1);
		else
			otherInt->SetSign(0);

		LongInt* result = new LongInt();
		result = Add(otherInt);

		otherInt->SetSign(oldsign); //old sigh

		return result;
	}

	LongInt* Multi(LongInt* otherInt) //multiplication
	{
		LongInt *result = new LongInt();
		//sign
		if(intData->head->value == otherInt->intData->head->value)
			result->intData->head->value = 0;
		else
			result->intData->head->value = 1;

		LList* myList = intData;
        LNode *myNode = intData->FirstRight();
		LList *otherList = otherInt->intData;

        //the length of the result
		for(int i = 0; i < myList->count + otherList->count; i++)
		{
			LNode* zeroNode = new LNode();
			zeroNode->value = 0;
			zeroNode->next = zeroNode->prev = NULL;
			result->intData->InsertLeft(zeroNode);
			zeroNode = NULL;
		}


		for(int i = 1; myNode != myList->head; myNode = myNode->prev, i++)
		{
			LNode* otherNode = otherList->FirstRight();
			for(int j = 1; otherNode != otherList->head; otherNode = otherNode->prev, j++)
			{
				int myValue =  myNode->value;
				int otherValue = otherNode->value;
				int tempResult = myValue * otherValue;
				int carry = OverFlow(tempResult);
				tempResult = tempResult - carry * (int)pow(10.0, (double)EVERY_NODE_LEN);

				LNode* resultNode = result->intData->CountFromRight(i + j - 1);
				LNode* carryNode = result->intData->CountFromRight(i + j);

				resultNode->value += tempResult; //may carry
				int carry2 = OverFlow(resultNode->value);
				resultNode->value = resultNode->value - carry2 * (int)pow(10.0, (double)EVERY_NODE_LEN);

				carryNode->value += carry + carry2; //may carry
				int carry3 = OverFlow(carryNode->value);
				carryNode->value = carryNode->value - carry3 * (int)pow(10.0, (double)EVERY_NODE_LEN);

				carryNode->prev->value += carry3;


			}
		}

		LNode* delNode = result->intData->head->next;
		while(delNode->value == 0)
		{
			result->intData->head->next = result->intData->head->next->next;
			result->intData->head->next->prev = result->intData->head;
			delete delNode;
			delNode = result->intData->head->next;
		}

		return result;

	}

	LongInt* Pow(int N) //power
	{
		if(N == 1)
			return this;
		if(N == 0)
		{
			LongInt* result = new LongInt();
			result->Clear();
			result->intData->head->next->value = 1;
			return result;
		}
		if(N > 1)
		{
			LongInt* result;
			result = this->Multi(this);
			for(int i = 2; i < N; i++)
			{
				result = result->Multi(this);
			}

			return result;
		}
	}
/*LongInt* Divide(LongInt* otherInt)//division: I didn't figure it out
/{

        LongInt *result = new LongInt();
        LList *QList =  result->intData;

        LongInt *R= new LongInt();
        LList *RList =  R->intData;

        LNode *myNode = intData->FirstLeft();

	for(; myNode != NULL; myNode = myNode->next)
	{
		QList->InsertRight(0);
		RList->InsertRight(myNode->value);
        LongInt *tmpList = R->Minus(otherInt);

		return result;
	}
*/
};



int _tmain(int argc, _TCHAR* argv[])
{
	LongInt* A = new LongInt();
	A->SetData("2222");

	LongInt* B = new LongInt();
	B->SetData("99999999");

	LongInt* C = new LongInt();
	C->SetData("246813575732");

	LongInt* D = new LongInt();
	D->SetData("180270361023456789");

	cout<<"A = "<<A->GetData()<<endl;
	cout<<"B = "<<B->GetData()<<endl;
	cout<<"C = "<<C->GetData()<<endl;
	cout<<"D = "<<D->GetData()<<endl<<endl;

	LongInt *E, *F, *G, *H, *I, *J, *K, *L, *M, *N;

	cout<<"E = A * D = "<<(E = A->Multi(D))->GetData()<<endl;
	cout<<"F = A^2 = "<<(F = A->Pow(2))->GetData()<<endl;
	cout<<"G = D^2 = "<<(G = D->Pow(2))->GetData()<<endl;
	cout<<"H = B * C = "<<(H = B->Multi(C))->GetData()<<endl;
	cout<<"I = A + D = "<<(I = A->Add(D))->GetData()<<endl;
	cout<<"J = I^2 = "<<(J = I->Pow(2))->GetData()<<endl;
	cout<<"K = J - F = "<<(K = J->Minus(F))->GetData()<<endl;
	cout<<"L = K - G = "<<(L = K->Minus(G))->GetData()<<endl;
	cout<<"M = L / E = "<<""<<endl;
	cout<<"N = E^5 = "<<(N = E->Pow(5))->GetData()<<endl;
	//cout<<"O = H^3 = "<<""<<endl;
	//cout<<"P = N - O = "<<""<<endl;
	//cout<<"Q = N^2 = "<<""<<endl;
	//cout<<"R = O^2 = "<<""<<endl;
	//cout<<"S = Q - R = "<<""<<endl;
	//cout<<"T = S / P = "<<""<<endl;
	//cout<<"U = T - O = "<<""<<endl;
	//cout<<"V = U - N = "<<""<<endl;
	//cout<<"W = C^2 = "<<""<<endl;
	//cout<<"X = B^2 = "<<""<<endl;
	//cout<<"Y = W - X = "<<""<<endl;
	//cout<<"Z = C + B = "<<""<<endl;
	//cout<<"H = Y / Z = "<<""<<endl;
	//cout<<"F = N^5 = "<<""<<endl;
	//cout<<"G = E^25 = "<<""<<endl;
	//cout<<"I = F / N = "<<""<<endl;
	//cout<<"J = G / I = "<<""<<endl;
	//cout<<"M = N^10 = "<<""<<endl;
	//cout<<"P = G^2 = "<<""<<endl;
	//cout<<"Q = P - M = "<<""<<endl;
	//cout<<"R = J - M = "<<""<<endl;

	getchar();
	return 0;
}

