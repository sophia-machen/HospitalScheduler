#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <stdio.h>

using namespace std;

int main()
{
	/*time_t timer;
	struct tm current = {0};
	double seconds;
	current.tm_hour=0;
	current.tm_min=0;
	current.tm_sec=0;
	current.tm_year=100;
	current.tm_mon=0;
	current.tm_mday=1;

	time(&timer);

	seconds=difftime(timer, mktime(&current));

	printf("%.f seconds since January 1, 2000 in the current timezone", seconds);

	return 0;*/
	/*time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  printf ("Current intake time: %s", asctime(timeinfo));

	int treatmenttime;
	int hours, minutes;
	printf ("Enter treatment time in minutes: "); fflush(stdout); scanf ("%d",&treatmenttime);
	hours=(treatmenttime%60);
	minutes=(treatmenttime)-(hours*60);
	timeinfo->tm_hour=timeinfo->tm_hour+hours;
	timeinfo->tm_min=timeinfo->tm_min+minutes;
	mktime(timeinfo);
	printf ("Current discharge time: %s", asctime(timeinfo));

  return 0;*/
	/*time_t rawtime1;
  struct tm * timeinfo1;

  time (&rawtime1);
  timeinfo1 = localtime (&rawtime1);

	time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

	char * time1=asctime(timeinfo1);
	cout<<time1<<endl;
  printf ("Current intake time: %s", asctime(timeinfo1));
	int treatmenttime;
	int hours, minutes;
	printf ("Enter treatment time in minutes: "); fflush(stdout); scanf ("%d",&treatmenttime);
	hours=(treatmenttime%60);
	minutes=(treatmenttime)-(hours*60);
	timeinfo->tm_hour=timeinfo->tm_hour+hours;
	timeinfo->tm_min=timeinfo->tm_min+minutes;
	mktime(timeinfo);
	printf ("Current discharge time: %s", asctime(timeinfo));
	char * time2 = asctime(timeinfo);
	cout<<time1<<endl;
	cout<<time2<<endl;
	if (time1==time2)
	{
		cout<<time1<<endl;
		cout<<time2<<endl;
		cout<<"time to discharge!"<<endl;
	}*/

	// get current time

	time_t result;
  result = time(NULL);

	cout<<result<<endl;

  struct tm* brokentime = new tm();
	char mybuf[50];

	// putting the current time into the tm struct
  localtime_r(&result, brokentime);

	cout<<asctime_r(brokentime, mybuf)<<endl;

	// get a new time

	time_t result1;
  result1 = time(NULL);

  struct tm* brokentime1 = new tm();
  localtime_r(&result1, brokentime1);

	int treatmenttime=63;
	int hours, minutes;

	hours=(treatmenttime%60);
	minutes=(treatmenttime)-(hours*60);

	brokentime1->tm_hour=brokentime1->tm_hour+hours;
	brokentime1->tm_min=brokentime1->tm_min+minutes;
	mktime(brokentime1);

	cout<<asctime_r(brokentime,mybuf)<<endl;
	cout<<asctime_r(brokentime1,mybuf)<<endl;
	int hour=mybuf[11]+mybuf[12];
	cout<<mybuf[11]<<endl;
	cout<<mybuf[12]<<endl;
	cout<<mybuf[14]<<endl;
	cout<<mybuf[15]<<endl;
	cout<<mybuf[17]<<endl;
	cout<<mybuf[18]<<endl;
	int hour1=(int)mybuf[11]-48;
	int hour2=(int)mybuf[12]-48;
	int min1=(int)mybuf[14]-48;
	int min2=(int)mybuf[15]-48;
	int sec1=(int)mybuf[17]-48;
	int sec2=(int)mybuf[18]-48;
	int _hour=(hour1*10)+(hour2);
	int _min=(min1*10)+(min2);
	int _sec=(sec1*10)+(sec2);
	cout<<_hour<<endl;
	cout<<_min<<endl;
	cout<<_sec<<endl;

  delete(brokentime);
  return(0);




}
