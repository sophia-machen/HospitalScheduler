#include "HospitalScheduler.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>

using namespace std;

HospitalScheduler::HospitalScheduler(int ERsize, int docSize)
{
	ERcapacity=ERsize;
	docCapacity=docSize;
	intakeCapacity=docCapacity*3;
	ERQueue=new Patient[ERcapacity];
	ERcurrentSize=0;
	docCurrentSize=0;
	intakeCurrentSize=0;
	for (int i=0; i<ERcapacity; i++)
	{
		ERQueue[i].name="";
		ERQueue[i].severity=0;
		ERQueue[i].waittime=0;
		ERQueue[i].treatmenttime=0;
		ERQueue[i].priorityValue=0;
	}
}
HospitalScheduler::~HospitalScheduler()
{
	ERQueue=NULL;
}
void HospitalScheduler::ERenqueue(std::string _name, int _severity, int _waittime, int _treatmenttime, int _docType)
{
	if (ERcurrentSize<ERcapacity)
	{
		Patient temp;
		temp.name=_name;
		temp.severity=_severity;
		temp.waittime=_waittime;
		temp.treatmenttime=_treatmenttime;
		temp.docType=_docType;
		temp.priorityValue=createPriorityValue(temp);
		cout<<"Adding: "<<temp.name<<" to the queue of waiting patients"<<endl;
		ERQueue[ERcurrentSize]=temp;
		assignPtoD(ERcurrentSize);
		repairUpward(ERcurrentSize);
		ERcurrentSize++;
	}
	else
	{
		cout << "Hospital at capacity, cannot accept more patients, refer incoming patients to nearest hospital and notify emergency services" << endl;
	}
}
void HospitalScheduler::ERdequeue(Patient _patient)
{
	if (ERcurrentSize!=0)
	{
		Intake temp = createIntake(_patient);
		intakeList.push_back(temp);
		int patientIndex;
		for (int i=0; i<ERcurrentSize; i++)
		{
			if (ERQueue[i].name==_patient.name)
			{
				cout<<"Removing patient: "<<ERQueue[i].name<<" from the Emergency Room queue"<<endl;
				patientIndex=i;
			}
		}
		ERQueue[patientIndex]=ERQueue[ERcurrentSize-1];
		ERcurrentSize--;
		repairDownward(0);
	}
	else
	{
		cout << "Waitlist empty, no patients to remove" << endl;
	}
}
void HospitalScheduler::ERClear()
{
	bool check = ERisEmpty();
	if (check==false)
	{
		while(ERcurrentSize!=0)
		{
			ERdequeue(ERQueue[0]);
		}
		//cout<<ERcurrentSize<<endl;
		cout<<"Cleared Entire Emergency Room Queue"<<endl;
	}
	else if (check==true)
	{
		cout<<"Emergency Room Queue Already Empty"<<endl;
	}
}
void HospitalScheduler::DocAdd(std::string _name, int _timeUntilAvailable, int _docType)
{
	if (docCurrentSize<docCapacity)
	{
		Doctor *temp=new Doctor();
		temp->name=_name;
		temp->timeUntilAvailable=_timeUntilAvailable;
		temp->docType=_docType;
		time_t result;
	  result = time(NULL);
	  struct tm* brokentime = new tm();
	  localtime_r(&result, brokentime);
		int hours, minutes;
		hours=(_timeUntilAvailable%60);
		minutes=(_timeUntilAvailable)-(hours*60);
		brokentime->tm_hour=brokentime->tm_hour+hours;
		brokentime->tm_min=brokentime->tm_min+minutes;
		mktime(brokentime);
		temp->availableTime=asctime_r(brokentime,temp->mybuf);
		cout<<"Adding: "<<temp->name<<" to the list of available doctors"<<endl;
		//cout<<temp->availableTime<<endl;
		docList.push_back(*temp);
		docCurrentSize++;
		//cout<<docCurrentSize<<endl;
		for (int i=0; i<ERcurrentSize; i++)
		{
			assignPtoD(i);
		}
	}
	else
	{
		cout << "Too many doctors, cannot add" << endl;
	}
}
void HospitalScheduler::DocRemove(string _name)
{
	bool found=false;
	int index;
	for (int i=0; i<docCurrentSize; i++)
	{
		if (docList[i].name==_name)
		{
			found=true;
			index=i;
		}
	}
	if (found==false)
	{
		cout<<"Doctor with name: "<<_name<<" is not currently working"<<endl;
	}
	else if (found==true)
	{
		cout<<"Are you sure you would like to remove Dr."<<docList[index].name<<"?"<<endl;
		cout<<"Dr."<<docList[index].name<<" currently has: "<<endl;
		cout<<docList[index].numCurrentPatients<<" current patients and "<<docList[index].numAssignedPatients<<" assigned patients"<<endl;
		string input;
		while(true)
		{
			cout<<"Enter 'Y' if you would like to proceed, or 'N' if you would not like to remove Dr."<<docList[index].name<<endl;
			getline(cin, input);
			if (input=="Y")
			{
				cout<<"Removing Dr."<<docList[index].name<<" from the list of working doctors"<<endl;
				docList.erase(docList.begin()+index);
				docCurrentSize--;
				for (int i=0; i<docList.size(); i++)
				{
					assignPtoD(i);
				}
				break;
			}
			if (input=="N")
			{
				cout<<"Dr."<<docList[index].name<<" was not removed"<<endl;
				break;
			}
			else
			{
				cout<<"Incorrect input, please try again."<<endl;
			}
		}
	}
}
Patient HospitalScheduler::peek()
{
	if (ERcurrentSize!=0)
	{
		return ERQueue[0];
	}
	else
	{
		cout<<"Waitlist empty, no patients to view"<<endl;
	}
}
bool HospitalScheduler::ERisFull()
{
	if (ERcurrentSize==ERcapacity)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool HospitalScheduler::ERisEmpty()
{
	if (ERcurrentSize==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool HospitalScheduler::DocisFull()
{
	if (docCurrentSize==docCapacity)
	{
		return true;
	}
	else
	{
		return false;
	}
}
Doctor HospitalScheduler::docIsAvailable()
{
	Doctor select=docList[0];
	for (int i=0; i<docList.size(); i++)
	{
		if (docList[i].timeUntilAvailable<select.timeUntilAvailable)
		{
			select = docList[i];
		}
	}
	return select;
}
float HospitalScheduler::createPriorityValue(Patient _patient)
{
	float temp;
	temp = (_patient.severity*100)+_patient.waittime-_patient.treatmenttime;
	return (1/temp);
}
void HospitalScheduler::assignPtoD(int index)
{
	Doctor doc=docTypeFinder(ERQueue[index].docType);
	for (int i=0; i<docList.size(); i++)
	{
		if (docList[i].name==doc.name)
		{
			docList[i].numAssignedPatients+=1;
		}
	}
	ERQueue[index].assignedDoc=doc;
}
void HospitalScheduler::createNumPatientsMax(Doctor _doctor)
{
	int index;
	for (int i=0; i<docList.size(); i++)
	{
		if (docList[i].name==_doctor.name)
		{
			index=i;
		}
	}
	if((_doctor.totalSeverity>=8)&&(_doctor.numCurrentPatients==1))
	{
		docList[index].numPatientsMax=2;
	}
	else if ((_doctor.totalSeverity>14)&&(_doctor.numCurrentPatients==2))
	{
		docList[index].numPatientsMax=2;
	}
	else if ((_doctor.totalSeverity<=14)&&(_doctor.numCurrentPatients>=2))
	{
		docList[index].numPatientsMax=3;
	}
	else if (_doctor.totalSeverity<8)
	{
		docList[index].numPatientsMax=3;
	}
	else
	{
		docList[index].numPatientsMax=3;
	}
}
void HospitalScheduler::adjustWaitTime()
{
	for (int i=0; i<docList.size(); i++)
	{
		Doctor *current = &docList[i];
		int sum = current->timeUntilAvailable;
		int _tt1=current->tt1;
		int _tt2=current->tt2;
		int _tt3=current->tt3;
		int _tt4=0;
		for (int i=0; i<ERcurrentSize; i++)
		{
			if (ERQueue[i].assignedDoc.name==current->name)
			{
				ERQueue[i].waittime=sum;
				if (_tt4==0)
				{
					_tt4=ERQueue[i].treatmenttime;
					sum+=_tt4;
				}
				else if ((_tt1<=_tt2)&&(_tt1<=_tt3)&&(_tt1<=_tt4))
				{
					sum+=_tt1;
					_tt1=ERQueue[i].treatmenttime;
				}
				else if ((_tt2<=_tt1)&&(_tt2<=_tt3)&&(_tt2<=_tt4))
				{
					sum+=_tt2;
					_tt2=ERQueue[i].treatmenttime;
				}
				else if ((_tt3<=_tt1)&&(_tt3<=_tt2)&&(_tt3<=_tt4))
				{
					sum+=_tt3;
					_tt3=ERQueue[i].treatmenttime;
				}
				else
				{
					sum+=_tt4;
					_tt4=ERQueue[i].treatmenttime;
				}
			}
		}
	}
}
void HospitalScheduler::docLunch(string _name)
{
	int index;
	for(int i=0; i<docList.size(); i++)
	{
		if (docList[i].name==_name)
		{
			index=i;
		}
	}
	docList[index].timeUntilAvailable+=60;
	time_t result;
  result = time(NULL);
  struct tm* brokentime = new tm();
  localtime_r(&result, brokentime);
	int addtime=docList[index].timeUntilAvailable;
	int hours, minutes;
	hours=addtime%60;
	minutes=addtime-(hours*60);
	brokentime->tm_hour=brokentime->tm_hour+hours;
	brokentime->tm_min=brokentime->tm_min+minutes;
	mktime(brokentime);
	docList[index].availableTime=asctime_r(brokentime, docList[index].mybuf);
	cout<<"Now, Dr."<<docList[index].name<<" will not be available to take patients until: "<<docList[index].availableTime<<endl;
}
Doctor HospitalScheduler::docTypeFinder(int _docType)
{
	Doctor select=docList[0];
	for (int i=0; i<docList.size(); i++)
	{
		if (select.docType!=_docType)
		{
			if (docList[i].docType==_docType)
			{
				select=docList[i];
			}
		}
		else if ((select.docType==_docType)&&(docList[i].docType==_docType))
		{
			if (docList[i].timeUntilAvailable<select.timeUntilAvailable)
			{
				select=docList[i];
			}
			else if (docList[i].numAssignedPatients<select.numAssignedPatients)
			{
				select=docList[i];
			}
		}
	}
	return select;
}
void HospitalScheduler::removeIntake()
{
	for (int i=0; i<intakeList.size(); i++)
	{
		int hour1=(int)intakeList[i].mybufD[11]-48;
		int hour2=(int)intakeList[i].mybufD[12]-48;
		int min1=(int)intakeList[i].mybufD[14]-48;
		int min2=(int)intakeList[i].mybufD[15]-48;
		int sec1=(int)intakeList[i].mybufD[17]-48;
		int sec2=(int)intakeList[i].mybufD[18]-48;
		int hour=(hour1*10)+(hour2);
		int min=(min1*10)+(min2);
		int sec=(sec1*10)+(sec2);
		long disTime=(hour*360)+(min*60)+sec;
		time_t result;
		result = time(NULL);
		struct tm* brokentime = new tm();
		localtime_r(&result, brokentime);
		char mybuf[50];
		asctime_r(brokentime,mybuf);
		int _hour1=(int)mybuf[11]-48;
		int _hour2=(int)mybuf[12]-48;
		int _min1=(int)mybuf[14]-48;
		int _min2=(int)mybuf[15]-48;
		int _sec1=(int)mybuf[17]-48;
		int _sec2=(int)mybuf[18]-48;
		int _hour=(_hour1*10)+(_hour2);
		int _min=(_min1*10)+(_min2);
		int _sec=(_sec1*10)+(_sec2);
		long curTime=(_hour*360)+(_min*60)+_sec;
		cout<<disTime<<endl;
		cout<<curTime<<endl;
		if (disTime==curTime)
		{
			cout<<endl;
			cout<<"============ ATTENTION ============"<<endl;
			cout<<"Patient: "<<intakeList[i].patient.name<<" is currently being discharged."<<endl;
			cout<<endl;
			Doctor doc = intakeList[i].patient.assignedDoc;
			int index;
			for (int j=0; j<docList.size(); j++)
			{
				if (docList[j].name==doc.name)
				{
					index=j;
				}
			}
			docList[index].numCurrentPatients-=1;
			if (docList[index].tt1==intakeList[i].patient.treatmenttime)
			{
				docList[index].tt1=0;
			}
			else if (docList[index].tt2==intakeList[i].patient.treatmenttime)
			{
				docList[index].tt2=0;
			}
			else if (docList[index].tt3==intakeList[i].patient.treatmenttime)
			{
				docList[index].tt3=0;
			}
			docList[index].totalSeverity-=intakeList[i].patient.severity;
			docList[index].numCurrentPatients-=1;
			createNumPatientsMax(docList[index]);
			if (docList[index].numCurrentPatients<docList[index].numPatientsMax)
			{
				docList[index].timeUntilAvailable=0;
				time_t result;
			  result = time(NULL);
			  struct tm* brokentime = new tm();
			  localtime_r(&result, brokentime);
				docList[index].availableTime=asctime_r(brokentime,docList[index].mybuf);
			}
			intakeList.erase(intakeList.begin()+i);
		}
	}
}
Intake HospitalScheduler::createIntake(Patient _patient)
{
	Intake temp;
	temp.patient=_patient;
	time_t result;
	result = time(NULL);
	struct tm* brokentime = new tm();
	localtime_r(&result, brokentime);
	temp.intakeTime=asctime_r(brokentime, temp.mybufI);
	time_t result1;
	result1 = time(NULL);
	struct tm* brokentime1 = new tm();
	localtime_r(&result1, brokentime1);
	int addtime=_patient.treatmenttime;
	int hours, minutes;
	hours=(addtime%60);
	minutes=(addtime)-(hours*60);
	brokentime1->tm_hour=brokentime1->tm_hour+hours;
	brokentime1->tm_min=brokentime1->tm_min+minutes;
	mktime(brokentime1);
	temp.dischargeTime=asctime_r(brokentime1,temp.mybufD);
	intakeList.push_back(temp);
	cout<<addtime<<endl;
	cout<<asctime_r(brokentime, temp.mybufI)<<endl;
	cout<<asctime_r(brokentime1,temp.mybufD)<<endl;
	return temp;
}
void HospitalScheduler::intakePatient()
{
	for (int i=0; i<docList.size(); i++)
	{
		//cout<<docList[i].availableTime<<endl;
		time_t result;
		result = time(NULL);
		struct tm* brokentime = new tm();
		localtime_r(&result, brokentime);
		char mybuf[50];
		asctime_r(brokentime,mybuf);
		int hour1=(int)docList[i].mybuf[11]-48;
		int hour2=(int)docList[i].mybuf[12]-48;
		int min1=(int)docList[i].mybuf[14]-48;
		int min2=(int)docList[i].mybuf[15]-48;
		int sec1=(int)docList[i].mybuf[17]-48;
		int sec2=(int)docList[i].mybuf[18]-48;
		int hour=(hour1*10)+(hour2);
		int min=(min1*10)+(min2);
		int sec=(sec1*10)+(sec2);
		long docTime=(hour*360)+(min*60)+sec;
		int _hour1=(int)mybuf[11]-48;
		int _hour2=(int)mybuf[12]-48;
		int _min1=(int)mybuf[14]-48;
		int _min2=(int)mybuf[15]-48;
		int _sec1=(int)mybuf[17]-48;
		int _sec2=(int)mybuf[18]-48;
		int _hour=(_hour1*10)+(_hour2);
		int _min=(_min1*10)+(_min2);
		int _sec=(_sec1*10)+(_sec2);
		long curTime=(_hour*360)+(_min*60)+_sec;
		//cout<<"1"<<endl;
		//cout<<docTime<<endl;
		//cout<<curTime<<endl;
		if ((docList[i].numPatientsMax>docList[i].numCurrentPatients)&&(docTime<=curTime))
		{
			//cout<<"2"<<endl;
			for (int j=0; j<ERcurrentSize; j++)
			{
				//cout<<"3"<<endl;
				if (ERQueue[j].assignedDoc.name==docList[i].name)
				{
					//cout<<"4"<<endl;
					if (docList[i].numCurrentPatients==0)
					{
						//cout<<"5"<<endl;
						docList[i].tt1=ERQueue[j].treatmenttime;
					}
					else if (docList[i].numCurrentPatients==1)
					{
						//cout<<"6"<<endl;
						docList[i].tt2=ERQueue[j].treatmenttime;
					}
					else if (docList[i].numCurrentPatients==2)
					{
						//cout<<"7"<<endl;
						docList[i].tt3=ERQueue[j].treatmenttime;
					}
					if ((docList[i].timeUntilAvailable>ERQueue[j].treatmenttime)&&(docList[i].numCurrentPatients==docList[i].numPatientsMax))
					{
						//cout<<"8"<<endl;
						time_t result;
						result = time(NULL);
						struct tm* brokentime = new tm();
						localtime_r(&result, brokentime);
						docList[i].timeUntilAvailable=ERQueue[j].treatmenttime;
						int treatmenttime=ERQueue[j].treatmenttime;
						int hours, minutes;
						hours=(treatmenttime%60);
						minutes=(treatmenttime)-(hours*60);
						brokentime->tm_hour=brokentime->tm_hour+hours;
						brokentime->tm_min=brokentime->tm_min+minutes;
						mktime(brokentime);
						docList[i].availableTime=asctime_r(brokentime,docList[i].mybuf);
					}
					if (docList[i].timeUntilAvailable==0)
					{
						//cout<<"9"<<endl;
						if (docList[i].numPatientsMax==docList[i].numCurrentPatients)
						{
							//cout<<"10"<<endl;
							if ((docList[i].tt1<=docList[i].tt2)&&(docList[i].tt1<=docList[i].tt3))
							{
								//cout<<"11"<<endl;
								docList[i].timeUntilAvailable+=docList[i].tt1;
							}
							else if ((docList[i].tt2<=docList[i].tt1)&&(docList[i].tt2<=docList[i].tt3))
							{
								//cout<<"12"<<endl;
								docList[i].timeUntilAvailable+=docList[i].tt2;
							}
							else if ((docList[i].tt3<=docList[i].tt1)&&(docList[i].tt3<=docList[i].tt2))
							{
								//cout<<"13"<<endl;
								docList[i].timeUntilAvailable+=docList[i].tt3;
							}
						}
					}
					//cout<<"14"<<endl;
					time_t result;
				  result = time(NULL);
				  struct tm* brokentime = new tm();
				  localtime_r(&result, brokentime);
					int hours, minutes;
					hours=(docList[i].timeUntilAvailable%60);
					minutes=(docList[i].timeUntilAvailable)-(hours*60);
					brokentime->tm_hour=brokentime->tm_hour+hours;
					brokentime->tm_min=brokentime->tm_min+minutes;
					mktime(brokentime);
					docList[i].availableTime=asctime_r(brokentime,docList[i].mybuf);
					docList[i].totalSeverity+=ERQueue[j].severity;
					docList[i].numCurrentPatients+=1;
					docList[i].numAssignedPatients-=1;
					createNumPatientsMax(docList[i]);
					cout<<endl;
					cout<<"============ ATTENTION ============"<<endl;
					cout<<"Dr."<<docList[i].name<<" is ready to see: "<<ERQueue[j].name<<endl;
					cout<<"Please bring "<<ERQueue[j].name<<" to see Dr."<<docList[i].name<<endl;
					cout<<endl;
					ERdequeue(ERQueue[j]);
				}
			}
		}
	}
}
void HospitalScheduler::printERQueue()
{
	for (int i=0; i<ERcurrentSize; i++)
	{
		Patient current=ERQueue[i];
		cout<<"Patient Name: "<<current.name<<endl;
		cout<<"Patient Severity: "<<current.severity<<endl;
		//cout<<"Patient Priority Value: "<<current.priorityValue<<endl;
		//cout<<"Type of Doctor: "<<current.docType<<"."<<endl;
		cout<<"Patient Current Wait Time: "<<current.waittime<<endl;
		cout<<"Patient Treatment Time: "<<current.treatmenttime<<endl;
		cout<<"Patient Current Assigned Doctor: "<<current.assignedDoc.name<<endl;
	}
}
void HospitalScheduler::printDocList()
{
	for (int i=0; i<docList.size(); i++)
	{
		Doctor current = docList[i];
		cout<<"Doctor Name: "<<current.name<<endl;
		cout<<"Current Number of Patients Being Seen by Dr."<<current.name<<": "<<current.numCurrentPatients<<endl;
		cout<<"Current Number of Patients Waiting for Dr."<<current.name<<": "<<current.numAssignedPatients<<endl;
		//cout<<"Type of Doctor: "<<current.docType<<"."<<endl;
		cout<<"Time when available for next patient: "<<current.availableTime<<endl;
	}
}
void HospitalScheduler::printIntakeList()
{
	for (int i=0; i<intakeList.size(); i++)
	{
		Intake current = intakeList[i];
		cout<<"Patient Name: "<<current.patient.name<<endl;
	}
}
void HospitalScheduler::printNext5Patients()
{
	if (ERisEmpty()==true)
	{
		cout<<"Emergency Room Queue is empty"<<endl;
	}
	else
	{
		for (int i=0; i<5; i++)
		{
			Patient current=ERQueue[i];
			cout<<"Patient Name: "<<current.name<<endl;
			cout<<"Patient Severity: "<<current.severity<<endl;
			cout<<"Patient Current Wait Time: "<<current.waittime<<endl;
			cout<<"Patient Treatment Time: "<<current.treatmenttime<<endl;
			cout<<"Patient Current Assigned Doctor: "<<current.assignedDoc.name<<endl;
		}
	}
}
void HospitalScheduler::repairUpward(int nodeIndex)
{
	for (int i=0; i<ERcurrentSize; i++)
	{
		ERQueue[i].priorityValue=createPriorityValue(ERQueue[i]);
	}
	for (int j=nodeIndex; j>0; j--)
	{
		if (ERQueue[j].priorityValue<ERQueue[j-1].priorityValue)
		{
			Patient temp = ERQueue[j-1];
			ERQueue[j-1]=ERQueue[j];
			ERQueue[j]=temp;
		}
	}
}
void HospitalScheduler::repairDownward(int nodeIndex)
{
	for (int i=0; i<ERcurrentSize; i++)
	{
		ERQueue[i].priorityValue=createPriorityValue(ERQueue[i]);
	}
	for (int j=nodeIndex; j<ERcurrentSize-1; j++)
	{
		if (ERQueue[j].priorityValue>ERQueue[j+1].priorityValue)
		{
			Patient temp = ERQueue[j+1];
			ERQueue[j+1]=ERQueue[j];
			ERQueue[j]=temp;
		}
	}
}
