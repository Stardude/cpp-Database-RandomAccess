#include<iostream>
#include<Windows.h>
#include"StudentDatabase.h"
using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	short choice, M;
	while (1)
	{
		/*������� ����*/
		system("cls");
		cout << "\n\t1.�������� ���� �����"
			<< "\n\n\t2.������ ��� ��� ������ ��������"
			<< "\n\t3.���������� ��� ��� ��������"
			<< "\n\t4.�������� ��� ��� ��������"
			<< "\n\n\t5.�����"
			<< "\n\t6.��������� � ����"
			<< "\n\n\t0.�����";
		cout << "\n\n\t������ ��������:\n";
		M:	cout << "\t -> "; cin >> choice;
		switch (choice)
		{
		case 1: StudentDatabase::CreateDatabase(); break;
		case 2:	StudentDatabase::Insert(); break;
		case 3:	StudentDatabase::Edit(); break;
		case 4:	StudentDatabase::Delete(); break;
		case 5:	StudentDatabase::Search(); break;
		case 6:	StudentDatabase::PrintToFile(); break;
		case 0:	exit(1); break;
		default: cout << "\n���� �����, �������� ������...\n"; goto M; break;
		}
	}
	cout << "\n\n";
	return 1;
}