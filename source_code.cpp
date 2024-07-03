#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include <ctype.h>


#define titleLength 50
#define descriptionLength 1000
#define maxElements 1000
#define animWait 0.9

int local_year, local_month, local_day, local_hour, local_minute;

// General
void notExistAlert();
void doneAlert();
void programSymbol();
void greetingAnimation();
int generate_random_id();
void remove_newline(char *line);
void displayMenu();
void clearScreen();
void flaseValueAlert();
void pause();
bool isLeapYear();
float daysInDateTime(int y, int m, int d, int h, int mm);
float remainingDays(struct dateTime deadLine, int y, int m, int d, int h, int mm);

// Menu
void addTask();
void displayTasks();
void modifyTask();
void removeTask();
void searchTask();
void statisticsTasks();
void quit();

// Sorting and Filtering
void sortTasksByAlphabetical(struct task myTasks[], int size);
void sortTasksByDeadLine(struct task myTasks[], int size);
void filterTasksByDeadLine3d(struct task myTasks[], int size);

// For date and time
struct dateTime {
	int y;
	int m;
	int d;
	int h;
	int mm;
};

// For task
struct task {
	int id;
	char title[titleLength];
	char description[descriptionLength];
	struct dateTime deadLine;
	int status;
	char createdIn[16];
};

// For status
enum statusType {
	todo = 1,
	inProgress = 2,
	done = 3
};




char choice;
int status, tasksCount = 0;
struct task tasks[maxElements], filteredTasks[maxElements];

int main() {
	

	// Get system date and time
	time_t now = time(&now);
	struct tm* dt = localtime(&now);
	local_year = dt->tm_year + 1900;
	local_month = dt->tm_mon + 1;
	local_day = dt->tm_mday;
	local_hour = dt->tm_hour;
	local_minute = dt->tm_min;
	
	
	//
	while(true) {
		
		// Store tasks in tasks.txt file
		FILE *fp;
		fp = fopen("tasks.txt", "a");
			
			
		begin:
		// Colors switching so fast -> cool switch animation
		greetingAnimation();
		// Clear screen then show menu and get user choice
		clearScreen();
		displayMenu();
		printf("\tEntrer votre choix: ");
		scanf("%c", &choice);
		
		//More spaces
		printf("\n\n");
		
		// Work with user choice
		switch(choice) {
		
			// Add task
			case '1': {
					
				addTask();
				
				// Finish the work and return to the principal menu
				break;
			}
		
			// Add Multi Tasks
			case '2': {
						
				while(true) {
				
					// Add new task
					addTask();
				
					// Ask user to add more task
					bool walk;
					printf("\tAppuyez sur 1 pour continuer ou sur 0 pour arreter l\'ajout: ");
					scanf("%d", &walk);
					
					if(!walk) {
						break;
					}
				}
			
				// Finish the work and return to the principal menu
				break;
			}
		
			// Display All
			case '3': {
							
				displayTasks();
				
				// Finish the work and return to the principal menu
				break;
			}
		
			// Modification
			case '4': {
				
				modifyTask();
				
				// Finish the work and return to the principal menu
				break;
			}
		
			// Delete
			case '5': {
			
				removeTask();
				
				// Finish the work and return to the principal menu
				break;
			}

			// Search
			case '6': {
			
				searchTask();
				
				// Finish the work and return to the principal menu
				break;
			}

			// Statistics
			case '7': {
				
				statisticsTasks();
				
				// Finish the work and return to the principal menu
				break;
			}
			
			// Quit
			case '8': {
				
				// Store tasks in the file and finish the program
				for(int i = 0; i < tasksCount; i++) {
					fprintf(fp, "#%d [%s] [%s] [%s] -> [%.4d-%.2d-%.2d %.2d:%.2d] statut[%d].\n", tasks[i].id, tasks[i].title, tasks[i].description, tasks[i].createdIn, tasks[i].deadLine.y, tasks[i].deadLine.m, tasks[i].deadLine.d, tasks[i].deadLine.h, tasks[i].deadLine.mm, tasks[i].status);
				}
				fclose(fp);
				goto finish;
				
			}
			
		
		}
	
	}

	finish:
		return 0;
}

// Program symbol
void programSymbol() {
	printf("\n\n");
    printf("\t\t  _____    ___     ___     ___   \n");
    printf("\t\t |_   _|  / _ \\   |   \\   / _ \\  \n");
    printf("\t\t   | |   | (_) |  | |) | | (_) | \n");
    printf("\t\t  _|_|_   \\___/   |___/   \\___/  \n");
    printf("\t\t_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"| \n");
    printf("\t\t`-0-0-'` -0-0-'` -0-0-'` -0-0-' \n");
	printf("\n\n");
}

// Principal menu
void displayMenu() {
	// Change console text color
	system("color 6");
	
	// Dosplay the draw
	programSymbol();
	
	// Display the menu
	printf("\tMenu:\n");
	printf("\t#1 Ajouter une nouvelle tache.\n");
	printf("\t#2 Ajouter plusieurs nouvelles taches.\n");
	printf("\t#3 Afficher la liste de toutes les taches.\n");
	printf("\t#4 Modifier une tache.\n");
	printf("\t#5 Supprimer une tache par identifiant.\n");
	printf("\t#6 Rechercher les Taches.\n");
	printf("\t#7 Statistiques.\n");
	printf("\t#8 Quiter.\n");
	
	printf("\n\n");

}

// Clear the console screen
void clearScreen() {
	system("cls");
}

// Show an input error message
void flaseValueAlert() {
	printf("\tVous avez entre une fausse valeur!\n\t");
	system("pause");
}

// Pause the program
void pause() {
	printf("\t");
	system("pause");
}

// Check if the year is leap year(année bissextile) or not
bool isLeapYear(int year) {
	
	bool result;
	if(year % 400 == 0) {
		result = true;
	} else if(year % 100 == 0) {
		result = false;
	} else if(year % 4 == 0) {
		result = true;
	} else {
		result = true;
	}
	
	return result;
	
}

// Returns the days in the month
int daysInMonth(int m, bool isLeapYear) {
	
	switch(m) {
		
		case 1: case 3: case 5: case 7: case 8: case 10: case 12: {
    		return 31;
			break;
		}
		case 2: {
			if(isLeapYear) {
				return 29;
			} else {
				return 28;
			}
			break;
		}
		
		case 4: case 6: case 9: case 11: {
    		return 30;
			break;
		}
	}
}

// Count the days in a specific date and time
float daysInDateTime(int y, int m, int d, int h, int mm) {
		
		float days = 0;
		
		// Days in yaer from 1 to our_year-1
		for(int i = 1; i < y; i++) {
			// Check if it's a leap year
			if(isLeapYear(y)) {
				days += 366;
			} else {
				days += 365;
			}
		}
		
		
		// From first month to our_month-1
		for(int i = 1; i < m; i++) {
			// Count days in month and returns it
			days += daysInMonth(m, isLeapYear(y));
		}
		
		// Count days
		days += d;
		
		// Hours to days
		days += (float) h/24;
		
	return days;
}


// Days remaining untill the deadLine
float remainingDays(struct dateTime deadLine, int y, int m, int d, int h, int mm) {
	
	float deadLineDateTime = daysInDateTime(deadLine.y, deadLine.m, deadLine.d, deadLine.h, deadLine.mm);
	float localDayeTime = daysInDateTime(y, m, d, h, mm);;
	
	return deadLineDateTime - localDayeTime;
}

// Remove the new line that given by fgets
void remove_newline(char *line)
{
    int new_line = strlen(line) -1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
}

// Generate a random id
int generate_random_id() {
	
  srand(time(NULL));
  int random_number = rand();
  return random_number;

}

// Colors animation for switching
void greetingAnimation() {
	
	system("color 10");
	sleep(animWait);
	
	system("color 20");
	sleep(animWait);

	system("color 30");
	sleep(animWait);

	system("color 40");
	sleep(animWait);

	system("color 50");
	sleep(animWait);
	
	system("color 60");
	sleep(animWait);
	system("cls");
	
	system("color 70");
	sleep(animWait);
	
	system("color 80");
	sleep(animWait);
	
	system("color 50");
	sleep(animWait);
	
	system("color 90");
	sleep(animWait);

}

// Alert messages
void notExistAlert() {
	printf("\n\tPas trouve!\n");
}

void doneAlert() {
	printf("\n\tOperation accomplie avec succes!\n");
}

// Add task
void addTask() {
	
	printf("\tEntrer les donnes de tache.\n");
	
	// Put the random id in the task id
	tasks[tasksCount].id = generate_random_id();
	
	
	//Put the local date and time in the task createdIn	
    sprintf(tasks[tasksCount].createdIn, "%04d-%02d-%02d %02d:%02d", local_year, local_month, local_day, local_hour, local_minute);

	// Ask for the title
	printf("\tTitre: ");
	//The first time the fgets not waiting -> we have put this code to solve that
	getchar();
	fgets(tasks[tasksCount].title, titleLength, stdin);
	remove_newline(tasks[tasksCount].title);
		
	// Ask for the description	
	printf("\tDescription: ");
	fgets(tasks[tasksCount].description, descriptionLength, stdin);
	remove_newline(tasks[tasksCount].description);
				
	// Two booleans for check if date and time of the deadLine is correct
	bool taskDateTimeNotCorrectWithSysTime, taskDateTimeNotCorrectWithLogicTime;
				
	do {
		// First make taskDateTimeCorrect false before checking 
		taskDateTimeNotCorrectWithSysTime = false;
		
		// Ask for deadLine
		char input[16];	
		printf("\tDeadline[YYYY-MM-DD HH:MM]: ");
		scanf("%d-%d-%d %d:%d", &tasks[tasksCount].deadLine.y, &tasks[tasksCount].deadLine.m, &tasks[tasksCount].deadLine.d, &tasks[tasksCount].deadLine.h, &tasks[tasksCount].deadLine.mm);
		
		
		// IF date_time in deadLine less than date_time in system
		if(daysInDateTime(tasks[tasksCount].deadLine.y, tasks[tasksCount].deadLine.m, tasks[tasksCount].deadLine.d, tasks[tasksCount].deadLine.h, tasks[tasksCount].deadLine.mm) - daysInDateTime(local_year, local_month, local_day, local_hour, local_minute) < 0)
		{
			taskDateTimeNotCorrectWithSysTime = true;
		}
					
		// 2000 <= year <= 3000 && 1 <= month <= 12 && 1 <= day <= 31 && 0 <= hour <= 24 && 0 <= minute <= 59
		taskDateTimeNotCorrectWithLogicTime = tasks[tasksCount].deadLine.y < 2000 || tasks[tasksCount].deadLine.y > 3000 || tasks[tasksCount].deadLine.m < 1 || tasks[tasksCount].deadLine.m > 12 || tasks[tasksCount].deadLine.d < 1 || tasks[tasksCount].deadLine.d > 31 || tasks[tasksCount].deadLine.h < 0 || tasks[tasksCount].deadLine.h > 23 || tasks[tasksCount].deadLine.mm < 0 || tasks[tasksCount].deadLine.mm > 59;
				
	} while(taskDateTimeNotCorrectWithSysTime || taskDateTimeNotCorrectWithLogicTime);
	
	// Ask for status
	do {
		char input[2];
		printf("\tStatut[1, 2, 3]:\n");
		printf("\t#1 A realiser.\n");
		printf("\t#2 En cours de realisation.\n");
		printf("\t#3 Finalisee.\n\t: ");
		scanf("%d", &tasks[tasksCount].status);
		//fgets(input, sizeof(input), stdin);
		//sscanf(input, "%d", &tasks[tasksCount].status);
	} while(tasks[tasksCount].status < 1 || tasks[tasksCount].status > 3);
				
	tasksCount++;
			
}

// Display the task informations
void displayTasks() {
	
	// If tasks list is empty don't continue
	if(tasksCount == 0) {
		printf("\tVide!\n");
		goto function_finish;
	}
	
	// Get from user how tasks will be shown by sort and filtering
	int sortType;
	printf("\t#1 Trier les taches par ordre alphabetique.\n");
	printf("\t#2 Trier les taches par deadline.\n");
	printf("\t#3 Afficher les taches dont le deadline est dans 3 jours ou moins.\n\n");
	
	do{
		printf("\tEntre votre choix: ");
		scanf("%d", &sortType);
	} while(sortType < 1 || sortType > 3);
		
	
	// Filter then display the tasks
	printf("\n\tLa liste des taches:\n");
	
	if(sortType == 1) {
		sortTasksByAlphabetical(tasks, tasksCount);
	} else if(sortType == 2) {
				
		sortTasksByDeadLine(tasks, tasksCount);
			
	} else {
		filterTasksByDeadLine3d(tasks, tasksCount);
	}
	
	function_finish:
		pause();
}


void modifyTask() {
	
	int modificationPart;
			
	do {
		printf("\t#1 Modifier la description d\'une tache.\n");
		printf("\t#2 Modifier le statut d\'une tache.\n");
		printf("\t#3 Modifier le deadline d\'une tache.\n\n");
					
		printf("\tEntrer votre choix: ");
		scanf("%d", &modificationPart);
					
	} while(modificationPart < 1 || modificationPart > 3);
	
	// First get the id of the task from the user		
	int id;
	do{
		printf("\n\tEntrer le id de tache: ");
		scanf("%d", &id);
	} while(id < 1);
				
			
	if(modificationPart == 1) {
		
		// Update description
		char newDescription[descriptionLength];
		printf("\n\tEntrer le nouveaux description: ");
		//The first time the fgets not waiting -> we have to put this code to solve that
		getchar();
		fgets(newDescription, descriptionLength, stdin);
		remove_newline(newDescription);
		
		// If the id is found -> edit the task description
		bool isFound = false;
			
		// Find the task then makes the edit
		for(int i = 0; i < tasksCount; i++) {
			if(tasks[i].id == id) {
				strcpy(tasks[i].description, newDescription);
				isFound = true;
				break;
			}
		}
		
		// Show the alert
		if(!isFound) {
			notExistAlert();
		} else {
			doneAlert();
		}
		
		pause();
				
					
	} else if(modificationPart == 2) {
		
		// Update status
		int newStatus;
		do {
			printf("\tEntrer le nouveaux statut[1, 2, 3]:\n");
			printf("\t#1 A realiser.\n");
			printf("\t#2 En cours de realisation.\n");
			printf("\t#3 Finalisee.\n\t: ");
			scanf("%d", &newStatus);	
		} while(newStatus < 1 || newStatus > 3);
		
			
		// Find the task then make the edit
		bool isFound = false;
		for(int i = 0; i < tasksCount; i++) {
			if(tasks[i].id == id) {
				tasks[i].status = newStatus;
				isFound = true;
				break;
			}
		}
		
		// Show the alert
		if(!isFound) {
			notExistAlert();
		} else {
			doneAlert();
		}
		
		pause();
					
	} else {
		// Choice = 3 ->		
		// Update deadLine
		struct dateTime newDeadLine;
					
		// Declare two unpass tickets variables
		bool taskDateTimeNotCorrectWithSysTime, taskDateTimeNotCorrectWithLogicTime;
				
		do {
			// First make taskDateTimeCorrect false before checking 
			taskDateTimeNotCorrectWithSysTime = false;
					
			printf("\tDeadLine[YYYY-MM-DD HH:MM]: ");
			scanf("%d-%d-%d %d:%d", &newDeadLine.y, &newDeadLine.m, &newDeadLine.d, &newDeadLine.h, &newDeadLine.mm);
					
			// IF date_time in deadLine less date_time in system
			if(daysInDateTime(newDeadLine.y, newDeadLine.m, newDeadLine.d, newDeadLine.h, newDeadLine.mm) - daysInDateTime(local_year, local_month, local_day, local_hour, local_minute) < 0) {
				taskDateTimeNotCorrectWithSysTime = true;
			}
					
			// 2000 <= year <= 3000 && 1 <= month <= 12 && 1 <= day <= 31 && 0 <= hour <= 24 && 0 <= minute <= 59
			taskDateTimeNotCorrectWithLogicTime = newDeadLine.y < 2000 || newDeadLine.y > 3000 || newDeadLine.m < 1 || newDeadLine.m > 12 || newDeadLine.d < 1 || newDeadLine.d > 31 || newDeadLine.h < 0 || newDeadLine.h > 23 || newDeadLine.mm < 0 || newDeadLine.mm > 59;
				
		} while(taskDateTimeNotCorrectWithSysTime || taskDateTimeNotCorrectWithLogicTime);
				
			// Find the task then makes the edit
			bool isFound = false;
			
			for(int i = 0; i < tasksCount; i++) {
				if(tasks[i].id == id) {
					tasks[i].deadLine.y = newDeadLine.y;
					tasks[i].deadLine.m = newDeadLine.m;
					tasks[i].deadLine.d = newDeadLine.d;
					tasks[i].deadLine.h = newDeadLine.h;
					tasks[i].deadLine.mm = newDeadLine.mm;
					
					isFound = true;
					break;
				}
			}
			
			// Show the alert
			if(!isFound) {
				notExistAlert();
			} else {
				doneAlert();
			}
			
			pause();
				
		}
	
}

// Remove the task
void removeTask() {
	
	// Get the id of the task from the user
	int id;
				
	do{
		printf("\tEntrer le id de tache: ");
		scanf("%d", &id);
	} while(id < 1);
	
	// Check if the task is found		
	bool isFound = false;
	for(int i = 0; i < tasksCount; i++) {
		
		// If the task is the last one -> Just encrease the array
		// If the task is not the last one -> Move the i+1 task to the i task
		
		// If the taks is exist
		if(tasks[i].id == id) {
			isFound = true;
		}
		
		// If the task isFound and not the last one
		if(isFound && i != tasksCount-1) {
			tasks[i] = tasks[i+1];
						
		} else if(isFound) {
			// If the task isFound and it's the last one
			// Size - 1
			tasksCount--;
			break;
		}
		
	}
				
	// Show alert message if task doesn't exist
	if(!isFound) {
		notExistAlert();
	} else {
		doneAlert();
	}
				
	pause();
	
}

// Search for the task
void searchTask() {
	
	int searchBy;
			
	printf("\t#1 Rechercher une tache par son Identifiant.\n");
	printf("\t#2 Rechercher une tache par son Titre.\n");
				
	do {
		printf("\tEntrer votre choix: ");
		scanf("%d", &searchBy);	
	} while(searchBy < 1 || searchBy > 2);
				
				
	bool isFound = false;
	
	if(searchBy == 1) {
		
		// Get the id of the task from the user
		int id;
		printf("`\n\tEntrer le id de tache: ");
		scanf("%d", &id);
			
		for(int i = 0; i < tasksCount; i++) {
			
			if(tasks[i].id == id) {
				
				// The task is found
				isFound = true;
				// Show it
				printf("\n\t#id: %d\n", tasks[i].id);
				printf("\tTitre: %s\n", tasks[i].title);
				printf("\tDescription: %s\n", tasks[i].description);
				printf("\tDate de creation: %s\n", tasks[i].createdIn);
				printf("\tDeadLine: %.4d-%.2d-%.2d %.2d:%.2d\n", tasks[i].deadLine.y, tasks[i].deadLine.m, tasks[i].deadLine.d, tasks[i].deadLine.h, tasks[i].deadLine.mm);
				printf("\tStatut: ");
				
				if(tasks[i].status == todo) {
					printf("A realiser.\n");
				} else if(tasks[i].status == inProgress) {
					printf("En cours de realisation.\n");
				} else if(tasks[i].status == done) {
					printf("Finalisee.\n");
				}	
							
				// You found it, no need to keep looping
				break;

			}
			
		}
		
					
	} else {
		
		// Ask for the new title
		char title[titleLength];
		printf("\n\tEntrer le titre de la tache: ");
		//The first time the fgets not waiting -> we have put this code to solve that
		getchar();
		fgets(title, titleLength, stdin);
		remove_newline(title);
						
										
		for(int i = 0; i < tasksCount; i++) {
						
			int isTitlesNotEquals = strcmp(tasks[i].title, title);
			
			if(isTitlesNotEquals == 0) {
				// Yeah I found it -> show the result
				isFound = true;
				printf("\n\t#id: %d\n", tasks[i].id);
				printf("\tTitre: %s\n", tasks[i].title);
				printf("\tDescription: %s\n", tasks[i].description);
				printf("\tDate de creation: %s\n", tasks[i].createdIn);
				printf("\tDeadLine: %.4d-%.2d-%.2d %.2d:%.2d\n", tasks[i].deadLine.y, tasks[i].deadLine.m, tasks[i].deadLine.d, tasks[i].deadLine.h, tasks[i].deadLine.mm);
				printf("\tStatut: ");
				
				if(tasks[i].status == todo) {
					printf("A realiser.\n");
				} else if(tasks[i].status == inProgress) {
					printf("En cours de realisation.\n");
				} else if(tasks[i].status == done) {
					printf("Finalisee.\n");
				}
				
				// You found it, no need to keep looping
				break;
			}
		}
		
	}
	
	// Not found -> show the alert message
	if(!isFound) {
		notExistAlert();
	}
	pause();
	
}

void statisticsTasks() {
	
	printf("\n\t#1 Afficher le nombre total des taches.\n");
	printf("\t#2 Afficher le nombre de taches completes et incompletes.\n");
	printf("\t#3 Afficher le nombre de jours restants jusqu'au delai de chaque tache.\n\n");
	
			
	int statisticsType;
	do {
		printf("\tEntrer votre choix: ");
		scanf("%d", &statisticsType);
	} while(statisticsType < 1 || statisticsType > 3);
				
	if(statisticsType == 1) {
		printf("\n\tLe nombre total des taches est %d.\n", tasksCount);
		pause();
	} else if(statisticsType == 2) {
					
		int tasksCompletedCount = 0, tasksUnCompletedCount;
		
		// If status == done -> tasksCompletedCount++	
		for(int i = 0; i < tasksCount; i++) {
			if(tasks[i].status == done) {
				tasksCompletedCount++;	
			}
		}
		
		// NotCompleted_Tasks = All_Tasks - Compledted_Tasks
		tasksUnCompletedCount = tasksCount - tasksCompletedCount;
				
		printf("\n\tLe nombre des taches completes est %d et incompletes est %d.\n", tasksCompletedCount, tasksUnCompletedCount);
		pause();
		
	} else {
		
		// Show the number of days remaining until each task's deadline.
		int i;
		for(i = 0; i < tasksCount; i++) {
			
			// Count the remaining days on the task
			float rd = remainingDays(tasks[i].deadLine, local_year, local_month, local_day, local_hour, local_minute);
			// And show it	
			printf("\t#%d Le nombre de jours restants jusqu\'au delai est %.2f jours.\n", tasks[i].id, rd);
		}
		
		// If the list is empty
		if(i == tasksCount) {
		
			printf("\n\tVide!\n");
			
		}
		pause();
	}
	
}


// Sorting and Filtering
void sortTasksByAlphabetical(struct task myTasks[], int size) {
	
	// Move the tasks to the sortedTasks array
	struct task sortedTasks[maxElements];
	
	for(int i = 0; i < size; i++) {
		sortedTasks[i] = myTasks[i];
	}
	
	// Sorting by Bubble sort
	struct task swap;
	for(int i = 0; i < size-1; i++) {
		for(int j = 0; j < size-i-1; j++) {
			// strcmp for compare
			// strcmp returns 1 if str1 > str2 -> "bd" > "ba"

			if(strcmp(sortedTasks[j].title, sortedTasks[j + 1].title) > 0) { // If "a", "b" -> -1 | "b", "a" -> 1 | "a", "a" -> 0
				swap = sortedTasks[j];
				sortedTasks[j] = sortedTasks[j+1];
				sortedTasks[j+1] = swap;
			}
		}
	}
	
	// Show the sorted tasks
	for(int i = 0; i < size; i++) {
		printf("\n\t#id: %d\n", sortedTasks[i].id);
		printf("\tTitre: %s\n", sortedTasks[i].title);
		printf("\tDescription: %s\n", sortedTasks[i].description);
		printf("\tDate de creation: %s\n", sortedTasks[i].createdIn);
		printf("\tDeadLine: %.4d-%.2d-%.2d %.2d:%.2d\n", sortedTasks[i].deadLine.y, myTasks[i].deadLine.m, myTasks[i].deadLine.d, myTasks[i].deadLine.h, myTasks[i].deadLine.mm);
		printf("\tStatut: ");
		if(sortedTasks[i].status == todo) {
			printf("A realiser.\n");
		} else if(sortedTasks[i].status == inProgress) {
			printf("En cours de realisation.\n");
		} else if(sortedTasks[i].status == done) {
			printf("Finalisee.\n");
		}	
	}
					
}


void sortTasksByDeadLine(struct task myTasks[], int size) {					
	
	// Move the tasks to the sortedTasks array
	struct task sortedTasks[maxElements];
	
	for(int i = 0; i < size; i++) {
		sortedTasks[i] = myTasks[i];
	}
			
	// Sorting by Bubble sort
	// Arrange in ascending order -> Disposer par ordre croissant
	struct task swap;
	for(int i = 0; i < size-1; i++) {
		for(int j = 0; j < size-i-1; j++) {
			float daysFirst = daysInDateTime(sortedTasks[j].deadLine.y, sortedTasks[j].deadLine.m, sortedTasks[j].deadLine.d, sortedTasks[j].deadLine.h, sortedTasks[j].deadLine.mm);
			float daysSecond = daysInDateTime(sortedTasks[j+1].deadLine.y, sortedTasks[j+1].deadLine.m, sortedTasks[j+1].deadLine.d, sortedTasks[j+1].deadLine.h, sortedTasks[j+1].deadLine.mm);
			if(daysSecond < daysFirst) {
				swap = sortedTasks[j];
				sortedTasks[j] = sortedTasks[j+1];
				sortedTasks[j+1] = swap;
			}
		}
	}
	
	// Show the sorted tasks
	for(int i = 0; i < size; i++) {
		
		printf("\n\t#id: %d\n", sortedTasks[i].id);
		printf("\tTitre: %s\n", sortedTasks[i].title);
		printf("\tDescription: %s\n", sortedTasks[i].description);
		printf("\tDate de creation: %s\n", sortedTasks[i].createdIn);
		printf("\tDeadLine: %.4d-%.2d-%.2d %.2d:%.2d\n", sortedTasks[i].deadLine.y, sortedTasks[i].deadLine.m, sortedTasks[i].deadLine.d, sortedTasks[i].deadLine.h, sortedTasks[i].deadLine.mm);
		printf("\tStatut: ");
		if(sortedTasks[i].status == todo) {
			printf("A realiser.\n");
		} else if(sortedTasks[i].status == inProgress) {
			printf("En cours de realisation.\n");
		} else if(sortedTasks[i].status == done) {
			printf("Finalisee.\n");
		}
		
	}
					
}



void filterTasksByDeadLine3d(struct task myTasks[], int size) {					
	
	// Filtering by deadLine 3y or less
	for(int i = 0; i < size; i++) {
		
		// If daysIn(task.deadLine) - daysIn(local_date_time) <= 3 days
		if(daysInDateTime(myTasks[i].deadLine.y, myTasks[i].deadLine.m, myTasks[i].deadLine.d, myTasks[i].deadLine.h, myTasks[i].deadLine.mm) - daysInDateTime(local_year, local_month, local_day, local_hour, local_minute) <= 3) {
			
			// Show the task
			printf("\n\t#id: %d\n", myTasks[i].id);
			printf("\tTitre: %s\n", myTasks[i].title);
			printf("\tDescription: %s\n", myTasks[i].description);
			printf("\tDate de creation: %s\n", myTasks[i].createdIn);
			printf("\tDeadLine: %.4d-%.2d-%.2d %.2d:%.2d\n", myTasks[i].deadLine.y, myTasks[i].deadLine.m, myTasks[i].deadLine.d, myTasks[i].deadLine.h, myTasks[i].deadLine.mm);
			printf("\tStatut: ");
			if(myTasks[i].status == todo) {
				printf("A realiser.\n");
			} else if(myTasks[i].status == inProgress) {
				printf("En cours de realisation.\n");
			} else if(myTasks[i].status == done) {
				printf("Finalisee.\n");
			}
		
		}
		
	}
					
					
}

