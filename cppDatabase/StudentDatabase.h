#define _CRT_SECURE_NO_WARNINGS
#define BASE 5

#include <iostream>
#include <iomanip>
#include <fstream>

#include "Student.h" 
using namespace std;

static Student student;
static Student temp_stud;
static Student emptyStud;
static fstream database;
static fstream tmp;
static long position1;
static long position2;
static int sizeOf = sizeof(student);
static int numberOfRecords = 3;
static int sizeOfFile = numberOfRecords * BASE;


class StudentDatabase
{
public:
	static void CreateDatabase();
	static void Insert();
	static void Edit();
	static void Delete();
	static void Search();
	static void PrintToFile();
private:
	static long Exist(int);
	static void PrintExact(Student);
	static int Hash(int);
	static void InsertToBlock(long);
	static long AddEmptyBlock();
	static long isInBlock(long, int);
	static void GetLastRecord(long);
	static void RemoveEmptyBlock();
	static void CheckNextBlocks(long);
	static void CheckNextBlocks(long, int, int);
};

