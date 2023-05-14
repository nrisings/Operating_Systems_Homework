# Operating_Systems_Homework
                        OPERATING SYSTEMS (Spring Course)


HOMEWORK 1 DESCRIPTION :

This assignment was locked Apr 16 at 11:59pm.
Spring is also knocking on the door of the "Drink My Friend" vineyard. As we all know, grapes love care and work, but most of all they love labour, which is usually rewarded with a good harvest. The vineyard needs a lot of helpers, so an application is being prepared for those who want to apply for a job.

Applications are invited from those who can work during the weeks of the spring season. When applying, they should give their name and which days of the week they can go to work. The days are given as follows: Monday Wednesday Thursday. Candidates will separate the days with a space. The farm knows how many workers it needs on which days (Mondays, Tuesdays, etc.). If a day is already full, the application should not accept applicants for that day.

The data of the applicants will be stored in a file, and in addition to the data entry, we should have the possibility to modify, delete and create a list of applicants by area or a complete list of applicants.

Create a C (C++) language program to perform this task, the solution should run either on opsys.inf.elte.hu or on a Linux system similar to it. The solution must be presented to the tutor during the week following the deadline.

Upload the source files in ZIP format here.

Deadline: 10 April 2023.

 
HOMEWORK 2 DESCRIPTION :
Spring is also knocking at the door of the "Drink my friend" vineyard
As we all know, grapes love care and work, but most of all they love to work, 
which is usually rewarded with a good harvest. The vineyard needs a lot of helpers, 
so an application is being prepared for those who apply for the job.


Applications are invited from those who can work during the weeks of the spring season.
The data is available in a file (see first homework) and any changes can be made.

"Drink my friend" vineyard (parent process) each morning will review the applications 
and needs for the day and start the worker process(es) for the day (child process(es)) 
Only a small bus is available that can carry a maximum of 5 people and there are two buses 
only. (The vineyard is a small company, so no more than 10 workers per day are needed.) 
The vineyard starts the minibus(es), who, when ready to go, send a signal back to the parent, and then receive the name list of workers as a reply via pipe from the vineyard. 
This information is also written on the screen by the child process (bus). As soon as the workers have been brought in, a summary message via message queue, is sent back to the vineyard, indicating how many of the requested workers have been brought in. This is written to the screen by the vineyard and then they terminate. The parent waits for the end of the child process and is ready to organise the next day's worker shuttle. (They can add or modify another applicant or start another day's bus run.)

Create a C program to do this, the solution should run either on opsys.inf.elte.hu server or on a Linux system similar to it. The solution must be presented to the tutor in the week following the deadline.

Upload the source files in .c or zip format here.

Deadline for submission: 14 May 2023.

