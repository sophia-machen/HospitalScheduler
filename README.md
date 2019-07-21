# HospitalScheduler
Backend files for a ER schedule queue software which gives patients a unique priority value, pairs doctors and patients, and runs autonomously using the time.h library

## Structs
Three structs are used to define the three different states a person could be in: Doctor (Doctor providing medical care), Patient (person waiting to be treated), and Intake (person who is currently being treated)  
These structs are defined as follows:
### Doctor
Doctors are defined by their name, patients, and time values. A doctor can be seeing up to three patients (intakes) at a time. They also have a list of patients currently assigned to them, who are waiting in the ER Queue. A doctor's totalSeverity is defined by the sum of the severities of each patient (intake) currently being seen by the doctor. This number becomes important when a doctor cannot see any more patients or their are other applicable doctors with more availability. A doctor's timeUntilAvailable is the time (in minutes) when a doctor is expected to be able to see another patient (intake), e.g. when one of their patients (intakes) is treated & discharged, or when they get off break. A doctor's availableTime is the time the doctor will be available in the standard time format. Doctors are also defined by their type or specialty, allowing for patients to be matched to the type of doctor best suited for their needs. 
### Patient
Patients are defined by their name, severity of injury, current wait time (beginning with 0 minutes), estimated treatment time (in minutes), the type of doctor needed to treat their injuries, their priority value (which takes into account all factors and numbers them on a scale from 1-most urgent, to currentQueueSize-least urgent), and the doctor that has been assigned to them
### Intake
Intakes are patients who have been admitted into the ER once their assigned doctor has become available. Intakes are defined by their patient (struct), the time they see a doctor, and the time the patient (intake) is scheduled to be discharged based on their treatment time

## Application of the Time.h Library
Time functions leave "their result in an internal static object and return a pointer to that object. Subsequent calls to these function will modify the same object." - https://www.codecogs.com/library/computing/c/time.h/ctime.php?alias=asctime_r  
To bypass this bug and make the time library usable in this application, a buffer, individual to each struct is created, where the time data is stored

## Enqueue Function
As long at the ER is not at capacity, a patient is added to the wait list by calculating their priority value, and placing them in the queue with a patient whose priority value is higher is before them, and a patient whose priority value is lower is after them

## Dequeue Function
As long as the ER is not empty, a patient is removed from the wait list, but then added to the vector list containing all intakes (patients currently being treated)

## Clear Function
Function to delete all records of patients currently in the queue

## DocAdd Function
As long as the hospital does not have too many ER doctors, a doctor is added to the vector list of available doctors. With the addition of a new doctor, all patients' assigned doctors are restructured

## DocRemove Function
As long as the doctor exists and is working, the doctor is removed from the vector list of available doctors. With the subtraction of a doctor, all patient's assigned doctors are restructured

## Peek Function
As long as the ER Queue is not empty, returns the patient to be seen next

## ERisFull Function
Checks to see if the current size of the ER queue is equal to the capacity of the queue

## ERisEmpty Function
Checks to see if the ER queue is empty

## DocisFull Function
Checks to see if the current number of working doctors is equal to the ER's capacity for working doctors

## docIsAvailable Function
Finds the next available doctor based on timeUntilAvailable (when the doctor will be available to see another patient (intake)

## createPriorityValue Function
A patient's individual priority value is calculated using the relative severity of their injury (0-10), the current amount of time they have been waiting, as well as their estimated total treatment time

## assignPtoD Function
Uses the docTypeFinder function to assign a doctor to a patient, and appends the doctor's number of assigned patients

## createNumPatientsMax Function
Adjusts the maximum number of patients (intakes) a doctor can be seeing at a single time based on the total severity of all of their patients

## adjustWaitTime Function
Adjusts the wait time of all patients in the ER Queue based on when their assigned doctor is available and how many patients and intakes are in front of them

## docLunch Function
Gives the corresponding doctor a 60 minute break and adjusts their availableTime

## docTypeFinder Function
Finds the next available doctor given the input docType. Compares doctors of the same type using the number of patients assigned to them. If no doctor of that type is available, the next available doctor, no matter the type, is returned

## removeIntake Function
Runs autonomously in the background, when an intake is finished being seen by a doctor (based on treatment time), the patient (intake) is discharged, the system prints out a notification, and the assigned doctor's members are adjusted

## earlyDischargeIntake Function
Manually discharge a patient (intake) if finished faster than treatment time

## createIntake Function
Changes a patient to an intake and adjusts members accordingly

## intakePatient Function
Runs autonomously in the background, when a doctor is available, their next patient is admitted. The system prints out a notification to take the waiting patient to see their corresponding doctor, they are dequeued, added to the intake vector list, and members of both the doctor and patient (turned intake) are adjusted

## printERQueue Function
Prints the entire patient list of the ER Queue, with their name, severity, current wait time, treatment time, and the doctor currently assigned to them

## printDocList Function
Prints the entire doctor list, with their name, current number of patients (intakes) being seen, current number of patients waiting to see the doctor, and the time the doctor will be available to see the next patient

## printIntakeList Function
Prints the name of all patients (intakes) currently being seen by doctors

## printNext5Patients Function
Prints the name, severity, wait time, treatment time, and current assigned doctor for the next 5 patients waiting in the ER Queue

## Main & Functionality
The program takes the capacity of the ER and the capacity of the working doctors as command line arguments  
First, at least one doctor must be added. Then, incoming patients can be added. Once at least one doctor and at least one patient have been added, all other functionality is available  
The intakePatient and removeIntake functions always run in the background and is able to print notifications on the screen  
Users have 15 functionality options:  

#### 1. Create List of Available Doctors from File
Uses fileio to build the docList vector until capacity is reached, using DocAdd()

#### 2. Create Emergency Room Patient Queue from File
Uses fileio to build the ERQueue until capacity is reached, using ERenqueue() and adjustWaitTime()

#### 3. Add Patient to Queue
Manually add a patient, using ERenqueue() and adjustWaitTime()

#### 4. Add Doctor to List of Available Doctors
Manually add a doctor, using DocAdd() and adjustWaitTime()

#### 5. Remove Doctor from List of Available Doctors
Remove a doctor using their name, using DocRemove()

#### 6. See Top Priority Patient
View the patient with the highest priorityValue, using peek()

#### 7. Clear Emergency Room Patient Queue
Remove all patients from the ER Queue, using ERClear()

#### 8. Give Doctor Lunch Break
Adjusts the wait time of a doctor given user input of name, using docLunch()

#### 9. View Next Available Doctor
Prints information for the next doctor available, using docIsAvailable()

#### 10. View Entire Queue
Prints entire ER Queue & member information, using printERQueue()

#### 11. View Entire Doctor List
Prints entire docList & member information, using printDocList()

#### 12. View Entire Intake List
Prints entire intakeList & member information, using printIntakeList()

#### 13. View Upcoming Patients & Wait Times
Displays next 5 patients in ER Queue & member information, using printNext5Patients()

#### 14. Discharge Patients
Manually discharge patient (intake) if finished early, using earlyDischargeIntake()

#### 15. Quit
Quit program
