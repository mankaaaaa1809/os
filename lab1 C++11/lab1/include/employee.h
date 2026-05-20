#pragma once
#include <string>

struct employee
{
	int num;
	char name[10];
	double hours;
};

double countWage(double, double);
int validateNum();
std::string validateName();
double validateHours();