#include "HospitalScheduler.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <time.h>

using namespace std;

void printMenu();

int main(int argc, char* argv[])
{

	int _ERSize=stoi(argv[1]);
	int _DocSize=stoi(argv[2]);
	cout << endl;
	//Instructions: doctors must be input first, then patients, then other functionality is available
  cout << "+================= Instructions =================+" << endl;
  cout << " First, at least one doctor must be added " << endl;
  cout << " Then incoming patients can be added " << endl;
	cout << " Once both doctors and patients have been inputted, " << endl;
	cout << " then other functionality is available " << endl;
  cout << " At least one doctor must always be in the system " << endl;
  cout << "+------------------------------------------------+" << endl;
	HospitalScheduler x(_ERSize,_DocSize);		//instantiate class object
	int option = 0;
	while (option!=16)
	{
		x.intakePatient();	//always runs in the background, when a doctor is available their next patient is admitted
		x.removeIntake();		//always runs in the background, when a patient's treatment time is reached they are discharged
		printMenu();
		string o;
		getline(cin, o);
		cout<<endl;
		option=stoi(o);
		if (option==1)			//create docList from file
		{
			cout<<"Enter filename: "<<endl;
			string filename;
			getline(cin, filename);
			cout<<endl;
			ifstream infile;
			infile.open(filename);
			string line;
			string name;
			string _docType;
			int docType;
			while(getline(infile, line))			//loop through full file
			{
				if (x.DocisFull()==true)
				{
					cout << "Too many doctors, cannot add" << endl;
					break;
				}
				stringstream ss(line);
				getline(ss, name, ',');
				//cout<<name<<endl;
				getline(ss, _docType);
				//cout<<_docType<<endl;
				docType=stoi(_docType);
				//cout<<name<<endl;
				//cout<<docType<<endl;
				x.DocAdd(name, 0, docType);		//add doctor to docList
			}
		}
		else if (option==2)				//create ERQueue from file
		{
			cout<<"Enter filename: "<<endl;
			string filename;
			getline(cin, filename);
			cout<<endl;
			ifstream infile;
			infile.open(filename);
			string line;
			string name;
			string _severity;
			string _waittime;
			string _treatmenttime;
			string _docType;
			int severity;
			int waittime;
			int treatmenttime;
			int docType;
			while(getline(infile, line))			//loop through file
			{
				if (x.ERisFull()==true)
				{
					cout << "Hospital at capacity, cannot accept more patients, refer incoming patients to nearest hospital and notify emergency services" << endl;
					break;
				}
				stringstream ss(line);
				getline(ss, name, ',');
				//cout<<name<<endl;
				getline(ss, _severity, ',');
				//cout<<_docType<<endl;
				getline(ss, _waittime, ',');
				getline(ss, _treatmenttime, ',');
				getline(ss, _docType);
				severity=stoi(_severity);
				waittime=stoi(_waittime);
				treatmenttime=stoi(_treatmenttime);
				docType=stoi(_docType);
				x.ERenqueue(name, severity, waittime, treatmenttime, docType);		//add patient to queue
			}
			x.adjustWaitTime();			//adjust wait time for patients after changes have been made
		}
		else if (option==3)         //add a single patient
		{
			string name;
			string _severity;
			string _treatmenttime;
			string _docType;
			int severity;
			int treatmenttime;
			int docType;
			cout<<"Enter patient's name: "<<endl;
			getline(cin, name);
			cout<<endl;
			cout<<"Enter patient's severity: "<<endl;
			getline(cin, _severity);
			cout<<endl;
			cout<<"Enter patient's approximate treatment time (in minutes): "<<endl;
			getline(cin, _treatmenttime);
			cout<<endl;
			cout<<"Enter patient's necessary type of doctor: "<<endl;
			cout<<"1=nurse, 2=emergency medicine physician, 3=EMT, 4=trauma surgeon, 5=orthopedics, 6=other"<<endl;
			getline(cin, _docType);
			cout<<endl;
			severity=stoi(_severity);
			treatmenttime=stoi(_treatmenttime);
			docType=stoi(_docType);
			x.ERenqueue(name, severity, 0, treatmenttime, docType);
			x.adjustWaitTime();
		}
		else if (option==4)			//add single doctor
		{
			string name;
			string _docType;
			int docType;
			cout<<"Enter doctor's name: "<<endl;
			getline(cin, name);
			cout<<endl;
			cout<<"Enter type of doctor: "<<endl;
			cout<<"1=nurse, 2=emergency medicine physician, 3=EMT, 4=trauma surgeon, 5=orthopedics, 6=other"<<endl;
			getline(cin, _docType);
			cout<<endl;
			docType=stoi(_docType);
			x.DocAdd(name, 0, docType);			//add doctor to queue
			x.adjustWaitTime();							//adjust wait time for patients after changes
		}
		else if (option==5)			//remove doctor from queue
		{
			string _name;
			cout<<"Enter doctor's name you would like to remove: "<<endl;
			getline(cin, _name);
			cout<<endl;
			x.DocRemove(_name);
		}
		else if (option==6)			//view top priority patient, highest priorityValue
		{
			if (x.ERisEmpty()==false)
			{
				Patient important = x.peek();
				cout<<"============ Top Priority Patient ============"<<endl;
				cout<<"Patient Name: "<<important.name<<endl;
				cout<<"Patient Severity: "<<important.severity<<endl;
				//cout<<"Patient Priority Value: "<<current.priorityValue<<endl;
				//cout<<"Type of Doctor: "<<current.docType<<"."<<endl;
				cout<<"Patient Current Wait Time: "<<important.waittime<<endl;
				cout<<"Patient Treatment Time: "<<important.treatmenttime<<endl;
				cout<<"Patient Current Assigned Doctor: "<<important.assignedDoc.name<<endl;
				cout <<"+--------------------------------------------+" << endl;
			}
			else if (x.ERisEmpty()==true)
			{
				cout<<"Emergency Room Queue is empty, no patients to view"<<endl;
			}
		}
		else if (option==7)			//clear ER queue
		{
			x.ERClear();
		}
		else if (option==8)		//adjust wait time for doctor when taking hour lunch break
		{
			string _name;
			cout<<"Please enter the name of the doctor who will be taking a lunch break: "<<endl;
			getline(cin, _name);
			cout<<endl;
			x.docLunch(_name);
		}
		else if (option==9)		//prints information for the next doctor available
		{
			Doctor next = x.docIsAvailable();
			cout<<"==================== Next Available Doctor ===================="<<endl;
			cout<<"Doctor Name: "<<next.name<<endl;
			cout<<"Current Number of Patients Being Seen by Dr."<<next.name<<": "<<next.numCurrentPatients<<endl;
			cout<<"Current Number of Patients Waiting for Dr."<<next.name<<": "<<next.numAssignedPatients<<endl;
			cout << "+-------------------------------------------------------------+" << endl;
		}
		else if (option==10)		//print entire ERQueue & information
		{
			x.printERQueue();
		}
		else if (option==11)		//print entire docList & information
		{
			x.printDocList();
		}
		else if (option==12)		//print entire intakeList & information
		{
			x.printIntakeList();
		}
		else if (option==13)		//display next 5 patients in queue
		{
			x.printNext5Patients();
		}
		else if (option==14)		//manually discharge patient if finished early
		{
			string _name;
			cout<<"Please enter the name of the patient who is being discharged: "<<endl;
			getline(cin, _name);
			cout<<endl;
			//cout<<_name;
			x.earlyDischargeIntake(_name);
		}
		else if (option==15)	//end program
		{
			break;
		}
		else
		{
			cout<<"Invalid input, please try again"<<endl;
		}
	}
	cout<<"Quitting program..."<<endl;
	x.~HospitalScheduler();	//clean up memory
	return 0;
}
void printMenu()
{
	cout << endl;
  cout << "Select a numerical option:" << endl;
  cout << "+===================== Main Menu =====================+" << endl;
  cout << " 1. Create List of Available Doctors from File " << endl;
  cout << " 2. Create Emergency Room Patient Queue from File " << endl;
  cout << " 3. Add Patient to Queue " << endl;
  cout << " 4. Add Doctor to List of Available Doctors " << endl;
  cout << " 5. Remove Doctor from List of Available Doctors " << endl;
  cout << " 6. See Top Priority Patient " << endl;
  cout << " 7. Clear Emergency Room Patient Queue " << endl;
  cout << " 8. Give Doctor Lunch Break " << endl;
	cout << " 9. View Next Available Doctor " << endl;
	cout << " 10. View Entire Queue " << endl;
	cout << " 11. View Entire Doctor List " << endl;
	cout << " 12. View Entire Intake List " << endl;
	cout << " 13. View Upcoming Patients & Wait Times " << endl;
	cout << " 14. Discharge Patients "<<endl;
	cout << " 15. Quit " << endl;
  cout << "+-----------------------------------------------------+" << endl;
  cout << "#> ";
}
