#ifndef HospitalScheduler_HPP
#define HospitalScheduler_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>

struct Doctor
{
	std::string name;						//name of doctor
	int numPatientsMax=3;				//number of patients a doctor can see at a time
	int numCurrentPatients=0;		//number of patients the doctor is currently seeing
	int numAssignedPatients=0;	//number of patients waiting in Queue to see doctor
	int totalSeverity=0;				//sum of the severity of patients
	int timeUntilAvailable;		//in minutes
	char * availableTime;			//time when available
	int docType;							//1=nurse, 2=emergency medicine physician, 3=EMT, 4=trauma surgeon, 5=orthopedics, 6=other
	int tt1=0;									//treatmenttime of patient1
	int tt2=0;									//treatment time of patient 2;
	int tt3=0;									//treatment time of patient 3;
	char mybuf[50];							//char string for availableTime
};
struct Patient
{
	std::string name;
	int severity;	//scale 1-10
	int waittime=0;	//in minutes
	int treatmenttime;	//in minutes
	int docType;	//1=nurse, 2=emergency medicine physician, 3=EMT, 4=trauma surgeon, 5=orthopedics, 6=other
	float priorityValue; //taking into account all factors, what is the priority of this patient from 1 to current capacity (1 being most important)
	Doctor assignedDoc;	//doctor assigned to patient
};
struct Intake
{
	Patient patient;
	char * intakeTime;	//when the patient sees a doctor
	char * dischargeTime;	//when the patient leaves the hospital
	char mybufI[50];			//char string for intakeTime
	char mybufD[50];			//char string for dischargeTime
};

class HospitalScheduler
{
	public:
		//Instantiate class object
		HospitalScheduler(int ERsize, int docSize);
		//destructor
		~HospitalScheduler();
		//add new patient to waitlist
		void ERenqueue(std::string _name, int _severity, int _waittime, int _treatmenttime, int _docType);
		//remove patient once linked doctor becomes available & add to intakeList
		void ERdequeue(Patient _patient);
		//clear the entire ERQueue;
		void ERClear();
		//adds a doctor to the list of current working doctors
		void DocAdd(std::string _name, int _timeUntilAvailable, int _docType);
		//removes a specific doctor from the list of current working doctors
		void DocRemove(std::string _name);
		//return Patient struct for patient at front of waitlist
		Patient peek();
		//returns true if the ER is at capacity
		bool ERisFull();
		//returns true if the waitlist is empty
		bool ERisEmpty();
		//returns true if the list of doctors is full
		bool DocisFull();
		//returns Doctor struct for the next available doctor
		Doctor docIsAvailable();
		//calculate the priority of a given patient & add to struct
		float createPriorityValue(Patient _patient);
		//defaults to 3, however if a patient is of a high severity this number will decrease
		void createNumPatientsMax(Doctor _doctor);
		//adjust wait time for all patients based on incoming patients & changes
		void adjustWaitTime();
		//Add 60 minute wait time to a specific doctor
		void docLunch(std::string _name);
		//returns doctor struct for next available certain type of doctor
		Doctor docTypeFinder(int _docType);
		//Assign a patient to a doctor, update structs accordingly, to be done when patient enters ER, not when they reach intake
		void assignPtoD(int index);
		//discharge a patient from hospital when their treatment time has elapsed
		void removeIntake();
		//Manually discharge patient if finished early
		void earlyDischargeIntake(std::string _name);
		//create Intake struct
		Intake createIntake(Patient patient);
		//continuously run to intake patients when doctor becomes available
		void intakePatient();
		//print entire ER queue
		void printERQueue();
		//print entire list of doctors
		void printDocList();
		//print entire list of current patients being seen
		void printIntakeList();
		//print upcoming patients & wait times
		void printNext5Patients();


	private:
		//maintain correct order by swapping node with parent if necessary
		void repairUpward(int nodeIndex);
		//maintain correct order by swapping node with child if necessary
		void repairDownward(int nodeIndex);

		Patient *ERQueue;
		std::vector<Intake> intakeList;
		std::vector<Doctor> docList;
		int ERcurrentSize;
		int ERcapacity;
		int docCurrentSize;
		int docCapacity;
		int intakeCurrentSize;
		int intakeCapacity;
};

#endif
