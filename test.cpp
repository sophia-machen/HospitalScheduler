#include "HospitalScheduler.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>

using namespace std;

int main()
{
	HospitalScheduler x(30,10);
	x.DocAdd("Sophie", 60, 1);
	x.DocAdd("Silverman",5,1);
	x.intakePatient();
}
