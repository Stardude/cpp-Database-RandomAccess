#include "StudentDatabase.h"

void StudentDatabase::CreateDatabase()
{
	char p;
	system("cls");
	cout << "\nВи впевнені, що хочете створити нову базу даних? Стара база даних буде знищена.";
	cout << "\n\n (1 - ТАК, 0 - НІ) -> "; cin >> p;
	if (p == '1')
	{
		database.open("database", ios_base::out | ios_base::binary);

		for (int i = 0; i < sizeOfFile; i++)
			database.write((char*)&emptyStud, sizeOf);

		database.close();
	}
}

void StudentDatabase::Insert()
{
	system("cls");
	cout << "Будь ласка, введіть інформацію про нового студента...";
	cout << "\nID -> "; cin >> student.id;
	if (Exist(student.id) != -1)
	{
		cout << "\nСтудент з таким ID вже існує...\n\n";
		system("pause");
		return;
	}
	cout << "\nПрізвище -> "; cin >> student.surname;
	cout << "Ім\'я -> "; cin >> student.name;
	cout << "По-батькові -> "; cin >> student.father;
	cout << "\nДень народження (ДД.ММ.РРРР) -> "; cin >> student.birthday;
	getchar();
	cout << "Адреса -> "; gets_s(student.address);
	cout << "Телефон -> "; cin >> student.telephone;
	cout << "E-mail -> "; cin >> student.email;
	cout << "\nГрупа -> "; cin >> student.group;
	cout << "\nЧи має він/вона стипендію? (1 - ТАК, 0 - НІ) -> "; cin >> student.hasScolarship;
	student.nextBlock = 0;

	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	InsertToBlock(Hash(student.id) * sizeOf * numberOfRecords);
	database.close();
}

void StudentDatabase::InsertToBlock(long startPosition)
{
	int nextBlock;
	database.clear();
	database.seekg(startPosition, ios_base::beg);
	bool isNext = false;
	for (int i = 0; i < numberOfRecords; i++)
	{
		temp_stud.id = 0;
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.nextBlock != 0)
			isNext = true;
		if (temp_stud.id == 0)
		{
			database.clear();
			database.seekp(startPosition + i * sizeOf, ios_base::beg);
			database.write((char*)&student, sizeOf);
			return;
		}
	}
	if (isNext)
	{
		database.clear();
		database.seekg(startPosition, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		InsertToBlock(temp_stud.nextBlock * sizeOf);
	}
	else
	{
		nextBlock = AddEmptyBlock() / sizeOf;
		database.clear();
		database.seekg(startPosition, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		temp_stud.nextBlock = nextBlock;
		database.clear();
		database.seekp(startPosition, ios_base::beg);
		database.write((char*)&temp_stud, sizeOf);
		InsertToBlock(temp_stud.nextBlock * sizeOf);
	}
}

long StudentDatabase::AddEmptyBlock()
{
	database.clear();
	database.seekg(-(numberOfRecords * sizeOf), ios_base::end);
	database.read((char*)&temp_stud, sizeOf);
	position1 = database.tellg();
	position1 -= sizeOf;
	if (temp_stud.id == 0 && position1 >= BASE * numberOfRecords * sizeOf)
	{
		return position1;
	}
	else
	{
		database.clear();
		database.seekp(0, ios_base::end);
		position1 = database.tellp();
		for (int i = 0; i < numberOfRecords; i++)
			database.write((char*)&emptyStud, sizeOf);
		return position1;
	}
}

void StudentDatabase::Edit()
{
	system("cls");
	cout << "Введіть ID студента, дані про якого ви хочете редагувати...";
	cout << "\n\nID -> "; cin >> student.id;
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	cout << "\nБудь ласка, введіть нову інформацію про студента...";
	cout << "\n\nПрізвище -> "; cin >> student.surname;
	cout << "Ім\'я -> "; cin >> student.name;
	cout << "По-батькові -> "; cin >> student.father;
	cout << "\nДень народження (ДД.ММ.РРРР) -> "; cin >> student.birthday;
	getchar();
	cout << "Адреса -> "; gets_s(student.address);
	cout << "Телефон -> "; cin >> student.telephone;
	cout << "E-mail -> "; cin >> student.email;
	cout << "\nГрупа -> "; cin >> student.group;
	cout << "\nЧи має він/вона стипендію? (1 - ТАК, 0 - НІ) -> "; cin >> student.hasScolarship;

	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	student.nextBlock = temp_stud.nextBlock;
	database.clear();
	database.seekp(position1, ios_base::beg);
	database.write((char*)&student, sizeOf);
	database.close();
}

void StudentDatabase::Delete()
{
	system("cls");
	cout << "Введіть ID студента, дані про якого ви хочете видалити...";
	cout << "\n\nID -> "; cin >> student.id;
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	int nextBlock, id = student.id;
	database.open("database", ios_base::in | ios_base::out | ios_base::binary);
	database.clear();
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);

	database.clear();
	database.seekp(position1, ios_base::beg);
	database.write((char*)&emptyStud, sizeOf);
	if (temp_stud.nextBlock == 0)
	{
		GetLastRecord(Hash(id) * sizeOf * numberOfRecords);
		InsertToBlock(Hash(id) * sizeOf * numberOfRecords);
		CheckNextBlocks(Hash(id) * sizeOf * numberOfRecords);
		RemoveEmptyBlock();
	}
	else
	{
		nextBlock = temp_stud.nextBlock;
		GetLastRecord(temp_stud.nextBlock * sizeOf);
		student.nextBlock = nextBlock;
		InsertToBlock(Hash(id) * sizeOf * numberOfRecords);
		CheckNextBlocks(Hash(id) * sizeOf * numberOfRecords);
		RemoveEmptyBlock();
	}
	database.close();
}

void StudentDatabase::GetLastRecord(long startPosition)
{
	database.clear();
	database.seekg(startPosition, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	if (temp_stud.nextBlock != 0)
		GetLastRecord(temp_stud.nextBlock * sizeOf);
	else
	{
		student = temp_stud;
		for (int i = 1; i < numberOfRecords; i++)
		{
			database.read((char*)&temp_stud, sizeOf);
			if (temp_stud.id != 0)
				student = temp_stud;
		}
		database.clear();
		database.seekg(startPosition, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		while (temp_stud.id != student.id)
		{
			database.read((char*)&temp_stud, sizeOf);
		}
		position2 = database.tellg();
		position2 -= sizeOf;
		database.clear();
		database.seekp(position2, ios_base::beg);
		database.write((char*)&emptyStud, sizeOf);
	}
}

void StudentDatabase::RemoveEmptyBlock()
{
	database.clear();
	database.seekg(-(numberOfRecords * sizeOf), ios_base::end);
	position1 = database.tellg();
	if (position1 <= BASE * numberOfRecords * sizeOf)
		return;
	database.clear();
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	if (temp_stud.id == 0)
		return;
	position2 = position1 - (numberOfRecords * sizeOf);
	while (position2 >= BASE * numberOfRecords * sizeOf)
	{
		database.clear();
		database.seekg(position2, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.id == 0)
			break;
		else
			position2 -= (numberOfRecords * sizeOf);
	}
	if (temp_stud.id != 0) return;
	database.clear();
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	CheckNextBlocks(Hash(temp_stud.id) * sizeOf * numberOfRecords, position1 / sizeOf, position2 / sizeOf);
	for (int i = 0; i < numberOfRecords; i++)
	{
		database.clear();
		database.seekg(position1 + i * sizeOf, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		database.seekp(position2 + i * sizeOf, ios_base::beg);
		database.write((char*)&temp_stud, sizeOf);
	}
	database.clear();
	database.seekp(position1, ios_base::beg);
	for (int i = 0; i < numberOfRecords; i++)
		database.write((char*)&emptyStud, sizeOf);
}

void StudentDatabase::CheckNextBlocks(long startPosition)
{
	database.clear();
	database.seekg(startPosition, ios_base::beg);
	position1 = database.tellg();
	database.read((char*)&temp_stud, sizeOf);
	while (temp_stud.nextBlock != 0)
	{
		database.clear();
		database.seekg(temp_stud.nextBlock * sizeOf, ios_base::beg);
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.id == 0)
		{
			database.clear();
			database.seekg(position1, ios_base::beg);
			database.read((char*)&temp_stud, sizeOf);
			temp_stud.nextBlock = 0;
			database.clear();
			database.seekp(position1, ios_base::beg);
			database.write((char*)&temp_stud, sizeOf);
			return;
		}
		else
		{
			position1 = database.tellg();
			position1 -= sizeOf;
		}
	}
}

void StudentDatabase::CheckNextBlocks(long startPosition, int oldBlock, int newBlock)
{
	database.clear();
	database.seekg(startPosition, ios_base::beg);
	database.read((char*)&temp_stud, sizeOf);
	if (temp_stud.nextBlock == oldBlock)
	{
		temp_stud.nextBlock = newBlock;
		database.seekp(startPosition, ios_base::beg);
		database.write((char*)&temp_stud, sizeOf);
		return;
	}
	else if (temp_stud.nextBlock != 0)
		CheckNextBlocks(temp_stud.nextBlock * sizeOf, oldBlock, newBlock);
	else return;
}

int StudentDatabase::Hash(int id)
{
	return id % BASE;
}

void StudentDatabase::Search()
{
	system("cls");
	cout << "\nВведіть ID студента, якого хочете знайти -> "; cin >> student.id;
	database.open("database", ios_base::in | ios_base::binary);
	if ((position1 = Exist(student.id)) == -1)
	{
		cout << "\nСтудента з таким ID в базі даних не виявлено...\n\n";
		system("pause");
		return;
	}
	database.seekg(position1, ios_base::beg);
	database.read((char*)&temp_stud, ios_base::beg);
	PrintExact(temp_stud);
	database.close();
	system("pause");
	
}

long StudentDatabase::Exist(int id)
{
	position1 = -1;
	database.open("database", ios_base::in | ios_base::binary);
	if ((position1 = isInBlock(Hash(id) * sizeOf * numberOfRecords, id)) != -1)
	{
		database.close();
		return position1;
	}
	database.clear();
	database.seekg(Hash(id) * sizeOf * numberOfRecords, ios_base::beg);
	position2 = database.tellg();
	database.read((char*)&temp_stud, sizeOf);
	while (1)
	{
		if (temp_stud.nextBlock != 0)
		{
			if ((position1 = isInBlock(temp_stud.nextBlock * sizeOf, id)) != -1)
			{
				database.close();
				return position1;
			}
			database.clear();
			database.seekg(position2, ios_base::beg);
			database.read((char*)&temp_stud, sizeOf);
			database.seekg(temp_stud.nextBlock * sizeOf, ios_base::beg);
			position2 = database.tellg();
			database.read((char*)&temp_stud, sizeOf);
		}
		else
			break;
	}
	database.close();
	return position1;
}

long StudentDatabase::isInBlock(long startPosition, int id)
{
	position1 = -1;
	database.clear();
	database.seekg(startPosition, ios_base::beg);
	for (int i = 0; i < numberOfRecords; i++)
	{
		database.read((char*)&temp_stud, sizeOf);
		if (temp_stud.id == id)
		{
			position1 = database.tellg();
			position1 -= sizeOf;
			return position1;
		}
	}
	return position1;
}

void StudentDatabase::PrintToFile()
{
	tmp.open("database.txt", ios_base::out);
	tmp << left << setw(4) << "ID"
		<< left << setw(20) << "Прізвище"
		<< left << setw(20) << "Ім\'я"
		<< left << setw(20) << "По-батькові"
		<< left << setw(17) << "День народження"
		<< left << setw(40) << "Адреса"
		<< left << setw(16) << "Телефон"
		<< left << setw(25) << "E-mail"
		<< left << setw(10) << "Група"
		<< left << setw(10) << "Стипендія"
		<< left << setw(10) << "NextBlock\n\n";
	database.open("database", ios_base::in | ios_base::binary);
	database.read((char*)&student, sizeOf);
	while (!database.eof())
	{
		tmp << left << setw(4) << student.id
			<< left << setw(20) << student.surname
			<< left << setw(20) << student.name
			<< left << setw(20) << student.father
			<< left << setw(17) << student.birthday
			<< left << setw(40) << student.address
			<< left << setw(16) << student.telephone
			<< left << setw(25) << student.email
			<< left << setw(10) << student.group
			<< left << setw(10) << student.hasScolarship 
			<< left << setw(10) << student.nextBlock << "\n";
		database.read((char*)&student, sizeOf);
	}
	tmp.close();
	database.close();
	system("explorer.exe database.txt");
}

void StudentDatabase::PrintExact(Student student)
{
	cout << "\nID:\t\t" << student.id
		<< "\nПрізвище:\t" << student.surname
		<< "\nІм\'я:\t\t" << student.name
		<< "\nПо-батькові:\t" << student.father
		<< "\nДень народження:" << student.birthday
		<< "\nАдреса:\t\t"; puts(student.address);
	cout << "Телефон:\t" << student.telephone
		<< "\nE-mail:\t\t" << student.email
		<< "\nГрупа:\t\t" << student.group
		<< "\nСтипендія:\t" << student.hasScolarship
		<< "\nNextBlock:\t" << student.nextBlock
		<< "\n";
}