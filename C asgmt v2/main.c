//------ DEFAULT ADMIN ACCOUNT -------
// ID   : S0001
// PSWD : 12345678@
// 
// 
//------ DEFAULT MEMBER ACCOUNT ------
// ID   : M0001
// PSWD : 12345678@

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // Used for SYSTEMTIME

#pragma warning (disable:4996)
#pragma warning (disable:6031)

#include "intro.h" // Intro Animation 

// Constants variables
#define HEADER "Powered by A-Plus Sdn Bhd"
#define MEMBERS_INFORMATION_FILE "membersInfo.bin"
#define STAFFS_INFORMATION_FILE "staffsInfo.bin"
#define TRAIN_SCHEDULE_FILE "trainSchedule.txt"
#define BOOKED_FILE "booked.txt"
#define FEEDBACK_FILE "feedback.bin"
#define READ_ME_FILE "readme.txt"
#define DB_MAX 1000 // maximum of database size
#define PAUSE 1500 // milliseconds for pausing = 1.5 second
#define PRICE_PER_KM 0.2 // RM 0.20 per km
#define FUEL_PRICE_PER_KM 0.05 
#define VIP_PRICE 1.5 // Normal price * 1.5

//Structs
// [Note] We decide to let Typedef struct is not for used as nested struct
struct Date {
	int day, month, year;
};
struct Time {
	int hours, minutes;
};
struct User {
	char id[6];
	char name[30];
	char password[19];
	char gender;
	char ic[15];
	char email[30];
	struct Date birthday;
	struct Date dateReg;
};
typedef struct {
	struct User userDetails;
	float balance;
	//int vipRole;
}Member;
typedef struct {
	struct User userDetails;
	char roles[15];
	float salary;
}Staff;
typedef struct {
	char trainId[6];
	char from[20], to[20];
	struct Date date;
	struct Time departTime;
	struct Time arriveTime;
	char status[20];
	float km;
}Train;
typedef struct {
	char ticketId[8];
	char trainId[6];
	char seat[3];
	char memberId[6];
	float price;
	struct Date dateBooked;
	struct Time timeBooked;
	int outDated;
}Ticket;
typedef struct {
	char memberId[6];
	char message[300];
	SYSTEMTIME timeSent;
	int repliedStatus; // 1 = replied, 0 = not yet
	char replyMsg[300];
	SYSTEMTIME timeReply;
	char staffId[6];
}Feedback;


//Database
Member membersDb[DB_MAX];
Staff staffsDb[DB_MAX];
Train trainScheDb[DB_MAX];
Ticket bookedDb[DB_MAX];
Feedback feedbackDb[DB_MAX];
Train outDatedTrain[DB_MAX];

//Global variables=
SYSTEMTIME globalCurrentTime;
Member loggedMember;
Staff LoggedStaff;
int memDbSize = 0;
int staffDbSize = 0;
int trainScheDbSize = 0;
int bookedDbSize = 0;
int feedbackDbSize = 0;
int outDatedTrainSize = 0;

//Function Prototype
//--- Global Module ----
void clear(); //Clear the screen and show the header
void color(char colour[10]); //Change text color 
void loadDatabase(); //Get all data from file and put in database structs
void pressEnter(); //Pause until user press enter
void invalidInput(); //Prompt user invalid input
void warnZeroToReturn();
void saveDatabase(); //Save all data inside database structs into the files
void mainMenu(); 
void help();
void drawTable(int type, int headerFooter); // [1 = Train Schedule, 2 = Member]  [1 = table header , 2 = table footer]
int strCheckIsZero(char input[100]);
struct Date getCurrentDate();
struct Time getCurrentTime();
int validatePswd(char pswd[19]); //valid will return 1
int validateGender(char gender);
int validateEmail(char email[30]);
void stringUpper(char* str);
//int validateName();
//--- Member Module ---
void memberLogIn(); //Member login page
void regMember(); //register member page
void memberMenu(); //Menu page of member
void topUp(); //Top up user balance
void showAllMem();
void updateMember(Member memberToUpdate);
void editMember(Member memberToUpdate, int whoEdit); // 1 = member, 2 = staff
void deleteMember(Member memberToDelete);
void makeFeedback();
void inbox();
void viewMemProfile(Member memberToView);
//--- Staff Module ---
void staffLogIn();
void staffMenu(); // menu for all staff's function
void regStaff();
void viewFeedback();
void replyFeedback(int feedbackIndex);
void generateReport();
void editStaffInfo();
void filterStaff();
void showAllStaff();
void updateSalaryRoles(int staffIndex);
void staffProfile(Staff staff);
void deleteStaff(Staff staff);
//--- Train Schedule Module ---
void trainMenu();
void showAllTrain(int memOrStaff);// 1 is member , 2 is staff
void showAvailableTrain();
void addNewTrain();
void editTrain();
void deleteTrain();
void viewTrainSche();
void showTrainWithIndex(int trainIndex);
void trainTable(Train train);
void refreshTrainSche();
//--- Booking Module ---
void buyTicket();
void chooseSeat(char trainId[6], int trainIndex);
void showTrainSeat(char bookedSeat[26][3], int type, char ticketSeat[3]); // type 1 = show all booked seat,  type 2 = change seat , ticketSeat is used for only when change seat
int validateSeatInput(char seat[3]);
int validateSeatBooked(char bookedTicket[27][3], char seat[3]);
void buySeat(Ticket buyTicket, char bookedTicket[27][3]);
void viewTicket(Member memTicketToView, int whoView); // 1 is member view itself,  2 is staff view the member booked ticket
void showAllTicket();
void editTicket(int ticketIndex);
void deleteTicket(int ticketIndex);
void refundTicket(Member ticketOwner, int ticketIndex);
void changeSeat(Member ticketOwner, int ticketIndex);
void refreshTicket();   //when member is deleted or train is passed

//Main Function
void main() {
	
	intro();	
	loadDatabase();
	pressEnter();
	mainMenu();
	saveDatabase();

	clear();
	printf("[Thank You For Using Our System]\n\n");
	color("red");
	printf("[Exited]\n");
	color("white");
}

//Function Defination
void mainMenu() {

	int select;
	
	do {

		clear();

		printf("Please select :\n\n");
		printf("[0] EXIT\n");
		printf("[1] Staff Log In\n");
		printf("[2] Member Log In\n");
		printf("[3] Member Register\n");
		printf("[4] Help\n\n");
		printf(">> ");

		select = -1; // avoid non-digit input and it fetch back the previous input
		//-- rewind dosen't work when it contains previous input from last function calling --
		scanf("%d", &select);
		rewind(stdin);

		switch (select) {
		case 0:
			return;
		case 1:
			staffLogIn();
			break;
		case 2:
			memberLogIn();
			break;
		case 3:
			regMember();
			break;
		case 4:
			help();
			break;
		default:
			invalidInput();
		}
	} while (1);
}
void clear() {
	system("cls");
	printf("\033[47;30m"); //Header special color
	printf("[");
	printf("%s", HEADER);
	printf("]\n\n");
	color("white");
}
void color(char colour[10]) {
	if ((strcmp(colour, "red") == 0))
		printf("\033[1;31m");
	else if ((strcmp(colour, "yellow") == 0))
		printf("\033[1;33m");
	else if ((strcmp(colour, "green") == 0))
		printf("\033[0;32m");
	else if ((strcmp(colour, "blue") == 0))
		printf("\033[0;34m");
	else if ((strcmp(colour, "purple") == 0))
		printf("\033[0;35m");
	else if ((strcmp(colour, "cyan") == 0))
		printf("\033[0;36m");
	else if ((strcmp(colour, "white") == 0))
		printf("\033[0;37m");
	//printf("\033[0;30m");
	else
		printf("\033[0;37m"); //default white
}
void loadDatabase() {

	FILE* fptrMembers, * fptrStaffs, * fptrTrainSche, * fptrBooked, * fptrFeedback;

	Member memRead;
	Staff staffRead;
	Train trainScheRead;
	Ticket bookedRead;
	Feedback feedbackRead;

	fptrMembers = fopen(MEMBERS_INFORMATION_FILE, "rb"); // Binary file
	fptrStaffs = fopen(STAFFS_INFORMATION_FILE, "rb");   // Binary file
	fptrTrainSche = fopen(TRAIN_SCHEDULE_FILE, "r");
	fptrBooked = fopen(BOOKED_FILE, "r");
	fptrFeedback = fopen(FEEDBACK_FILE, "rb"); // Binary file

	//--- Check Does Files Exists ---
	if (fptrMembers == NULL)
		exit(-1);
	if (fptrStaffs == NULL)
		exit(-1);
	if (fptrTrainSche == NULL)
		exit(-1);
	if (fptrBooked == NULL)
		exit(-1);
	if(fptrFeedback == NULL)
		exit(-1);

	while (fread(&memRead, sizeof(memRead), 1, fptrMembers) == 1) {
		membersDb[memDbSize] = memRead;
		memDbSize++;
	}
	while (fread(&staffRead, sizeof(staffRead), 1, fptrStaffs) == 1) {
		staffsDb[staffDbSize] = staffRead;
		staffDbSize++;
	}
	while (fscanf(fptrTrainSche, "%[^|]|%[^|]|%[^|]|%d/%d/%d|%d:%d|%d:%d|%[^|]|%f\n",
		&trainScheRead.trainId, &trainScheRead.from, &trainScheRead.to,
		&trainScheRead.date.day, &trainScheRead.date.month, &trainScheRead.date.year,
		&trainScheRead.departTime.hours, &trainScheRead.departTime.minutes,
		&trainScheRead.arriveTime.hours, &trainScheRead.arriveTime.minutes,
		&trainScheRead.status, &trainScheRead.km) != EOF)
	{
		trainScheDb[trainScheDbSize] = trainScheRead;
		trainScheDbSize++;
	}
	while (fscanf(fptrBooked, "%[^|]|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%d:%d|%d\n",
		&bookedRead.ticketId, &bookedRead.trainId, &bookedRead.seat, &bookedRead.memberId, &bookedRead.price,
		&bookedRead.dateBooked.day, &bookedRead.dateBooked.month, &bookedRead.dateBooked.year,
		&bookedRead.timeBooked.hours, &bookedRead.timeBooked.minutes, &bookedRead.outDated) != EOF)
	{
		bookedDb[bookedDbSize] = bookedRead;
		bookedDbSize++;
	}
	while (fread(&feedbackRead, sizeof(feedbackRead), 1, fptrFeedback) == 1) {
		feedbackDb[feedbackDbSize] = feedbackRead;
		feedbackDbSize++;
	}

	fclose(fptrMembers);
	fclose(fptrStaffs);
	fclose(fptrTrainSche);
	fclose(fptrBooked);
	fclose(fptrFeedback);

}
void pressEnter() {

	color("white");
	printf("\n\n[Press ");
	color("green");
	printf("ENTER ");
	color("white");
	printf("to return]");
	getchar();
	rewind(stdin);
}
void help() {

	FILE* fptrReadMe;
	char string[1024];
	fptrReadMe = fopen(READ_ME_FILE, "r");

	if (fptrReadMe == NULL)
		exit(-1);

	clear();
	printf("[HELP]\n\n");
	color("purple");
	printf("[Staff Account]\n");
	color("white");
	printf("ID       : %s\n", staffsDb[0].userDetails.id);
	printf("Password : %s\n\n", staffsDb[0].userDetails.password);

	color("purple");
	printf("[Member Account]\n");
	color("white");
	printf("ID       : %s\n", membersDb[0].userDetails.id);
	printf("Password : %s\n\n", membersDb[0].userDetails.password);

	while (fscanf(fptrReadMe, "%[^\n]\n", &string) != EOF) {
		printf("%s\n", string);
	}
	pressEnter();
}
struct Date getCurrentDate() {

	struct Date currentDate;

	GetLocalTime(&globalCurrentTime);
	currentDate.day = globalCurrentTime.wDay;
	currentDate.month = globalCurrentTime.wMonth;
	currentDate.year = globalCurrentTime.wYear;

	return currentDate;
}
struct Time getCurrentTime() {

	struct Time currentTime;

	GetLocalTime(&globalCurrentTime);
	currentTime.hours = globalCurrentTime.wHour;
	currentTime.minutes = globalCurrentTime.wMinute;

	return currentTime;
}
int validatePswd(char pswd[19]) {
	if (strlen(pswd) >= 8 && strlen(pswd) <= 18) {
		for (int i = 0; i < strlen(pswd); i++) {
			// Check for special characters
			if ((pswd[i] < 65 || pswd[i] > 90) &&  // Not uppercase letters
				(pswd[i] < 97 || pswd[i] > 122) && // Not lowercase letters
				(pswd[i] < 48 || pswd[i] > 57)) { // Not digits
				return 1;
			}
		}
	}
	return 0;
}
int validateGender(char gender) {
	if (gender == 'M' || gender == 'F')
		return 1;
	else
		return 0;
}
int validateIC(char ic[15]) {
	int dash = 0;
	if (strlen(ic) == 14) {
		for (int i = 0; i < strlen(ic); i++) {
			if (ic[i] == '-' && ((i == 6) || (i == 9)))
				dash++;
		}
	}
	return (dash == 2) ? 1 : 0;
}
int validateEmail(char email[30]) {
	int specialChar = 0;
	for (int i = 0; i < strlen(email); i++) {
		if (email[i] == '@' || email[i] == '.')
			specialChar++;
	}
	return (specialChar >= 2) ? 1 : 0;
}
void stringUpper(char* str) {
	while (*str != '\0') {
		*str = toupper(*str);
		str++;
	}
}
void viewMemProfile(Member memberToView) {

	printf(" __________________________________________\n");
	printf("|                                          |\n");
	printf("|            Member Information            |\n");
	printf("|__________________________________________|\n");
	printf("| ID       | %-30s|\n", memberToView.userDetails.id);
	printf("|----------|-------------------------------|\n");
	printf("| Name     | %-30s|\n", memberToView.userDetails.name);
	printf("|----------|-------------------------------|\n");
	printf("| Password | %-30s|\n", memberToView.userDetails.password);
	printf("|----------|-------------------------------|\n");
	printf("| Gender   | %-30c|\n", memberToView.userDetails.gender);
	printf("|----------|-------------------------------|\n");
	printf("| IC       | %-30s|\n", memberToView.userDetails.ic);
	printf("|----------|-------------------------------|\n");
	printf("| Birthday | %02d/%02d/%04d                    |\n", memberToView.userDetails.birthday.day, memberToView.userDetails.birthday.month, memberToView.userDetails.birthday.year);
	printf("|----------|-------------------------------|\n");
	printf("| Email    | %-30s|\n", memberToView.userDetails.email);
	printf("|----------|-------------------------------|\n");
	printf("| Balance  | RM %-27.2f|\n", memberToView.balance);
	printf("|__________|_______________________________|\n\n");
}
void invalidInput() {
	color("white");
	printf("[");
	color("red");
	printf("Invalid Input");
	color("white");
	printf("]\n\n");
	color("white");
	_sleep(PAUSE);
}
void warnZeroToReturn() {
	color("red");
	printf("[WARN] ");
	color("white");
	printf("Press [");
	color("red");
	printf("0");
	color("white");
	printf("] to return!\n");
}
void saveDatabase() {

	FILE* fptrMembers, * fptrStaffs, * fptrTrainSche, * fptrBooked, *fptrFeedback;

	fptrMembers = fopen(MEMBERS_INFORMATION_FILE, "wb"); // Binary file
	fptrStaffs = fopen(STAFFS_INFORMATION_FILE, "wb");   // Binary file
	fptrTrainSche = fopen(TRAIN_SCHEDULE_FILE, "w");
	fptrBooked = fopen(BOOKED_FILE, "w");
	fptrFeedback = fopen(FEEDBACK_FILE, "wb");

	for (int i = 0; i < memDbSize; i++) {
		fwrite(&membersDb[i], sizeof(membersDb[i]), 1, fptrMembers);
	}
	for (int i = 0; i < staffDbSize; i++) {
		fwrite(&staffsDb[i], sizeof(staffsDb[i]), 1, fptrStaffs);
	}
	for (int i = 0; i < trainScheDbSize; i++) {
		fprintf(fptrTrainSche, "%s|%s|%s|%d/%d/%d|%d:%d|%d:%d|%s|%.2f\n",
			trainScheDb[i].trainId,
			trainScheDb[i].from,
			trainScheDb[i].to,
			trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
			trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
			trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
			trainScheDb[i].status,
			trainScheDb[i].km);
	}

	for (int i = 0; i < bookedDbSize; i++) {
		fprintf(fptrBooked, "%s|%s|%s|%s|%.2f|%d/%d/%d|%d:%d|%d\n",
			bookedDb[i].ticketId, bookedDb[i].trainId, bookedDb[i].seat, bookedDb[i].memberId, bookedDb[i].price,
			bookedDb[i].dateBooked.day, bookedDb[i].dateBooked.month, bookedDb[i].dateBooked.year,
			bookedDb[i].timeBooked.hours, bookedDb[i].timeBooked.minutes, bookedDb[i].outDated);
	}
	for (int i = 0; i < feedbackDbSize; i++) {
		fwrite(&feedbackDb[i], sizeof(feedbackDb[i]), 1, fptrFeedback);
	}
	fclose(fptrMembers);
	fclose(fptrStaffs);
	fclose(fptrTrainSche);
	fclose(fptrBooked);
	fclose(fptrFeedback);
}
void regMember() {

	Member member = { {"id", "-", "-", '-', "-", "-" ,0,0,0,0,0,0}, 0 };
	char confirm;

	if (memDbSize != 0) {
		memDbSize++;
		sprintf(member.userDetails.id, "%c%04d", 'M', memDbSize);// format the char and int into string for [Staff ID]	
	}
	else {
		memDbSize++;
		strcpy(member.userDetails.id, "M0001");
	}

	for (int i = 0; i < 6; i++) {
		clear();
		printf("[Register New Member]\n\n");
		warnZeroToReturn();
		viewMemProfile(member);

		color("cyan"); printf("[+] "); color("white");
		switch (i) {
		case 0:
			printf("Please input your name : ");
			gets(member.userDetails.name);
			rewind(stdin);
			stringUpper(&member.userDetails.name);
			if (strCheckIsZero(member.userDetails.name)) { memDbSize--; return; }
			break;
		case 1:
			printf("[Min 8 character, Max 18 character, 1 special character]\n"); color("cyan"); printf("[+] "); color("white");
			printf("Please input your password : ");
			gets(member.userDetails.password);
			rewind(stdin);
			if (strCheckIsZero(member.userDetails.password)) { memDbSize--; return; }

			//validate pswd
			if (!validatePswd(member.userDetails.password)) {
				strcpy(member.userDetails.password, "-");
				color("red");
				printf("\n[Invalid Password Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 2:
			printf("Please input your gender [M/F] : ");
			scanf("%c", &member.userDetails.gender);
			rewind(stdin);
			member.userDetails.gender = toupper(member.userDetails.gender);
			if (member.userDetails.gender == '0') { memDbSize--; return; }
			//validate gender
			if (!validateGender(member.userDetails.gender)) {
				member.userDetails.gender = '-';
				color("red");
				printf("\n[Invalid Gender]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 3:
			printf("Please input your IC [xxxxxx-xx-xxxx] : ");
			gets(member.userDetails.ic);
			rewind(stdin);
			if (strCheckIsZero(member.userDetails.ic)) { memDbSize--; return; }
			//validate ic
			if (!validateIC(member.userDetails.ic)) {
				strcpy(member.userDetails.ic, "-");
				color("red");
				printf("\n[Invalid IC Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 4:
			printf("Please enter your Birthday [1/1/2024] : ");
			scanf("%d/%d/%d", &member.userDetails.birthday.day,
				&member.userDetails.birthday.month,
				&member.userDetails.birthday.year);
			rewind(stdin);
			break;
		case 5:
			printf("Please input your email [xxx@xxx.xxx] : ");
			gets(member.userDetails.email);
			rewind(stdin);
			if (strCheckIsZero(member.userDetails.email)) { memDbSize--; return; }
			//validate email
			if (!validateEmail(member.userDetails.email)) {
				strcpy(member.userDetails.email, "-");
				color("red");
				printf("\n[Invalid Email Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;			
		default:
			printf("System Error");
			exit(-1);
		}
	}

	clear();
	printf("[Create New Member Confirmation]\n\n");
	viewMemProfile(member);

	//--- Ask user to confirm ---
	printf("Create existing member account [Y/N] >> ");
	scanf("%c", &confirm);
	rewind(stdin);
	confirm = toupper(confirm);

	if (confirm == 'Y') {

	    ////////////////////////////////
	   ////// CREATED NEW MEMBER //////
	  ////////////////////////////////

		member.userDetails.dateReg = getCurrentDate();
		membersDb[memDbSize - 1] = member;
		saveDatabase();

		color("green");	printf("\n[New Member Account Created]\n");
	}
	else {

		////////////////////////////
	   ////////// FAILED //////////
	  ////////////////////////////

		memDbSize -= 1;

		color("red"); printf("\n[Account FAILED to create!]\n");
	}

	pressEnter();
}
void memberLogIn() {

	char memIdInput[10], pswdInput[18];
	int validMemId = 0;
	int loop = 1;

	do {
		clear();
		printf("[Log In Member Account]\n\n");
		warnZeroToReturn();

		color("purple"); printf("[NOTE] ");
		color("white");  printf("You can only log into a member account that is registered!\n\n");

		color("cyan"); printf("[+] "); color("white");
		printf("Member ID  [Eg. M0001] : ");
		gets(memIdInput);
		rewind(stdin);
		if (strCheckIsZero(memIdInput)) { return; }

		color("cyan"); printf("[+] "); color("white");
		printf("Password               : ");
		gets(pswdInput);
		rewind(stdin);
		if (strCheckIsZero(pswdInput)) { return; }

		//-----check is User ID and Password correct----
		memIdInput[0] = toupper(memIdInput[0]);
		for (int i = 0; i < memDbSize; i++) {
			if (strcmp(membersDb[i].userDetails.id, memIdInput) == 0) {
				validMemId = 1;
				if (strcmp(membersDb[i].userDetails.password, pswdInput) == 0) {
					//----successfully Logged In----
					loggedMember = membersDb[i];
					memberMenu();
					loop = 0;
					break;
				}
				else {
					color("red");
					printf("\n[Wrong Password]");
					color("white");
					_sleep(PAUSE);
				}
			}
		}
		if (validMemId != 1) {
			color("red");
			printf("\n[Member ID doesn't exist]");
			color("white");

			_sleep(PAUSE);
		}
	} while (loop == 1);
}
void memberMenu() {

	int select;

	clear();

	color("green");
	printf("[You Successfully Logged Into Your Account]\n\n");
	color("white");

	do {
		select = -1; // reset the value to invalid, prevent it run the previous function when user input non-integer value
		refreshTrainSche();

		printf("Member ID : %s\n\n", loggedMember.userDetails.id);

		printf("Please select : \n\n");
		printf("[0] Log Out\n");
		printf("[1] Top Up Balance\n");
		printf("[2] View Train Schedule\n");
		printf("[3] Buy Ticket\n");
		printf("[4] View My Ticket\n");
		printf("[5] Edit Personal Information\n");
		printf("[6] Feedback\n");
		printf("[7] Inbox\n\n");
		printf(">> ");

		scanf("%d", &select);
		rewind(stdin);

		switch (select)
		{
		case 0:
			color("red");
			printf("\n[Logging Out...]");
			color("white");
			_sleep(PAUSE);
			break;
		case 1:
			topUp();
			break;
		case 2:
			viewTrainSche();
			break;
		case 3:
			buyTicket();
			break;
		case 4:
			viewTicket(loggedMember,1);
			break;
		case 5:
			editMember(loggedMember,1); 
			break;
		case 6:
			makeFeedback();
			break;
		case 7:
			inbox();
			break;
		default:
			invalidInput();
		}
		clear();
	} while (select != 0);
}
void topUp() {

	float amount;
	char yesNo;

	for (int i = 0; i < 3; i++) {
		clear();

		printf("[Top Up Balance]\n\n");
		warnZeroToReturn();
		printf("\n");
		color("cyan"); printf("[+] "); color("white");
		printf("Member ID       : %s\n", loggedMember.userDetails.id);
		color("cyan"); printf("[+] "); color("white");
		printf("Account Balance : RM%.2f\n\n", loggedMember.balance);

		if (i == 0) {
			printf("Amount to top up [RM] >> ");
			scanf("%f", &amount);
			rewind(stdin);

			// i want make money validation only number can input, no negative unless -1
			if (amount <= 0)
				break;
		}
		else if (i == 1) {
			printf("Are you sure you want to top up RM%.2f into your member account ? [Y/N] >> ", amount);
			scanf("%c", &yesNo);
			rewind(stdin);

			if (yesNo == 'Y' || yesNo == 'y') {
				loggedMember.balance += amount;
				updateMember(loggedMember);
			}
			else {
				printf("\n[");
				color("red");
				printf("Failed To Top Up");
				color("white");
				printf("]");
				pressEnter();
				i = -1;
			}
		}
		else {
			printf("[");
			color("green");
			printf("You Had Successfully Topped Up Your Balance");
			color("white");
			printf("]");
			pressEnter();
		}
	}
}
void inbox() {

	int inboxes = 0;

	clear();
	printf("[Inbox]\n\n");
	printf("------------------------------------\n");
	for (int i = 0; i < feedbackDbSize; i++) {
		if (strcmp(feedbackDb[i].memberId, loggedMember.userDetails.id) == 0) {
			if (feedbackDb[i].repliedStatus) {
				inboxes++;
				color("green");
				printf("[%02d/%02d/%04d] ", feedbackDb[i].timeReply.wDay, feedbackDb[i].timeReply.wMonth, feedbackDb[i].timeReply.wYear);
				color("white");
				printf("[ You ] %s\n", feedbackDb[i].message);
				color("green");
				printf("[%02d/%02d/%04d] ", feedbackDb[i].timeReply.wDay, feedbackDb[i].timeReply.wMonth, feedbackDb[i].timeReply.wYear);
				color("white");
				printf("[Admin] %s\n\n", feedbackDb[i].replyMsg);
			}
		}
	}
	if (inboxes == 0) {
		color("red");
		printf("No inboxes found\n");
		color("white");
	}
	printf("------------------------------------\n");
	printf("(%d) inboxes\n\n", inboxes);

	pressEnter();
}
void updateMember(Member memberToUpdate) {

	for (int i = 0; i < memDbSize; i++) {
		if (strcmp(membersDb[i].userDetails.id, memberToUpdate.userDetails.id) == 0) {
			membersDb[i] = memberToUpdate;
			break;
		}
	}
	saveDatabase();
}
void editMember(Member memberToUpdate, int whoEdit) {

	Member updateDetails;
	int select;
	char yesNo;

	do {
		clear();
		(whoEdit == 1) ? printf("[Personal Information]\n\n") : printf("[Existing Member Information]\n\n");
		viewMemProfile(memberToUpdate);
		printf("\n[Edit]\n\n");
		printf("Please select : \n\n");
		printf("[0] Back \n");
		printf("[1] Name \n");
		printf("[2] Password \n");
		printf("[3] Gender \n");
		printf("[4] IC \n");
		printf("[5] Email \n");
		if (whoEdit != 1) {
			printf("[6] "); color("red"); printf("Delete Existing Member\n"); color("white");
		}
		printf("\n");
		printf(">> ");
		scanf("%d", &select);
		rewind(stdin);

		switch (select) {
		case 0:
			break;
		case 1:
			printf("\nChange Name To : ");
			gets(updateDetails.userDetails.name);
			rewind(stdin);
			strcpy(memberToUpdate.userDetails.name, updateDetails.userDetails.name);
			break;
		case 2:
			printf("\nChange Password To : ");
			gets(updateDetails.userDetails.password);
			rewind(stdin);
			if (validatePswd(updateDetails.userDetails.password)) {
				strcpy(memberToUpdate.userDetails.password, updateDetails.userDetails.password);
			}
			else {
				color("red");
				printf("[Please follow the password format]");
				_sleep(PAUSE);
			}
			break;
		case 3:
			printf("\nChange Gender To : ");
			scanf("%c", &updateDetails.userDetails.gender);
			rewind(stdin);
			updateDetails.userDetails.gender = toupper(updateDetails.userDetails.gender);
			if (validateGender(updateDetails.userDetails.gender)) {
				memberToUpdate.userDetails.gender = updateDetails.userDetails.gender;
			}
			else {
				color("red");
				printf("[Invalid gender]");
				_sleep(PAUSE);
			}
			break;
		case 4:
			printf("\nChange IC To : ");
			gets(updateDetails.userDetails.ic);
			rewind(stdin);
			if(validateIC(updateDetails.userDetails.ic)){
				strcpy(memberToUpdate.userDetails.ic, updateDetails.userDetails.ic);
			}
			else {
				color("red");
				printf("[Invalid IC format]");
				_sleep(PAUSE);
			}
			break;
		case 5:
			printf("\nChange Email To : ");
			gets(updateDetails.userDetails.email);
			rewind(stdin);
			if (validateEmail(updateDetails.userDetails.email)) {
				strcpy(memberToUpdate.userDetails.email, updateDetails.userDetails.email);
			}
			else {
				color("red");
				printf("[Invalid Email]");
				_sleep(PAUSE);
			}
			break;
		case 6:
			if (whoEdit == 1) {
				invalidInput();
			}
			else {
				printf("\nAre you sure you want to delete existing member [Y/N] ?\n");
				printf(">> ");
				scanf("%c", &yesNo);
				rewind(stdin);
				if (toupper(yesNo) == 'Y') {
					deleteMember(memberToUpdate);
					color("green");
					printf("[Existing Member Successfully Deleted]");
					pressEnter();
					select = 0;
				}
				else {
					color("red");
					printf("[Existing Member Failed To Delete]");
					pressEnter();
				}
			}
			break;
		default:
			invalidInput();
			break;
		}
		  ////////////////////////////////////////////////////////////////////
		 ///Update the logged member if it is updated by the member itself///
		////////////////////////////////////////////////////////////////////
		if (whoEdit == 1)
			loggedMember = memberToUpdate;

		updateMember(memberToUpdate);

	} while (select != 0);

}
void deleteMember(Member memberToDelete) {

	for (int i = 0; i < memDbSize; i++) {
		if (strcmp(memberToDelete.userDetails.id, membersDb[i].userDetails.id) == 0) {
			for (int x = i; x < memDbSize; x++) {
				membersDb[x] = membersDb[x + 1];
			}
			memDbSize--;
			break;
		}
	}
	//--- Also delete the ticket made from the member ---
	for (int i = 0; i < bookedDbSize; i++) {
		if (strcmp(memberToDelete.userDetails.id, bookedDb[i].memberId) == 0) {
			deleteTicket(i);
		}
	}
	
	//--- Also delete feedback made from the member ---
	for (int i = 0; i < feedbackDbSize; i++) {
		if (strcmp(memberToDelete.userDetails.id, feedbackDb[i].memberId) == 0) {
			for (int x = i; x < feedbackDbSize; x++) {
				feedbackDb[x] = feedbackDb[x + 1];
			}
			feedbackDbSize--;
		}
	}
	saveDatabase();
}

void makeFeedback() {

	Feedback makeFeedback;
	char yesNo;

	clear();
	printf("[Feedback]\n\n");
	warnZeroToReturn();

	color("purple"); printf("[Note] ");
	color("white");  printf("Message you sent will NOT be anonymously, please think before you sent.\n");
	color("purple"); printf("[Note] ");
	color("white");  printf("Once ENTER pressed the message will be sent.\n\n");

	printf("Please input the message that you wanted to feedback : ");
	gets(makeFeedback.message);
	rewind(stdin);
	if (strCheckIsZero(makeFeedback.message)) { return; }

	printf("\nAre you sure you wanted to sent the message [Y/N] ? \n");
	printf(">> ");
	scanf("%c", &yesNo);
	rewind(stdin);
	yesNo = toupper(yesNo);

	if (yesNo == 'Y') {
		GetLocalTime(&globalCurrentTime);
		strcpy(makeFeedback.memberId, loggedMember.userDetails.id);
		makeFeedback.timeSent = globalCurrentTime;
		makeFeedback.repliedStatus = 0;
		feedbackDb[feedbackDbSize] = makeFeedback;
		feedbackDbSize++;
		saveDatabase();

		color("green");
		printf("[You successfully sent the feedback]\n");
		pressEnter();
	}
	else {
		color("red");
		printf("[Failed to sent the feedback]\n");
		pressEnter();
	}
}
void drawTable(int type, int headerFooter) {

	// Type 1 = Train
	// Type 2 = Member
	// Type 3 = Ticket
	// Type 4 = Feedback
	// Type 5 = Report
	// Type 6 = Staff

	if (type == 1) {
		if (headerFooter == 1) {
			printf(" ______________________________________________________________________________________________________\n");
			printf("|          |              |              |            |             |             |           |        |\n");
			printf("| Train ID |     From     |      To      |    Date    | Depart Time | Arrive Time |  Status   |   Km   |\n");
			printf("|__________|______________|______________|____________|_____________|_____________|___________|________|\n");
		}
		else {
			printf("|__________|______________|______________|____________|_____________|_____________|___________|________|\n");
		}
	}
	else if (type == 2) {
		if (headerFooter == 1) {
			printf(" ___________________________________________________________________________________________________________________________________________________________\n");
			printf("|             |                              |                        |        |                  |                               |             |           |\n");
			printf("|  Member ID  |             Name             |        Password        | Gender |        IC        |             Email             |  Birth Day  |  Balance  |\n");
			printf("|_____________|______________________________|________________________|________|__________________|_______________________________|_____________|___________|\n");
		}
		else {
			printf("|_____________|______________________________|________________________|________|__________________|_______________________________|_____________|___________|\n");
		}
	}
	else if (type == 3) {

		if (headerFooter == 1) {
			printf(" ___________________________________________________________________________________________\n");
			printf("|             |            |        |             |         |               |               |\n");
			printf("|  Ticket ID  |  Train ID  |  Seat  |  Member ID  |  Price  |  Date Bought  |  Time Bought  |\n");
			printf("|_____________|____________|________|_____________|_________|_______________|_______________|\n");
		}
		else {
			printf("|_____________|____________|________|_____________|_________|_______________|_______________|\n\n");
		}
	}
	else if (type == 4) {
		if (headerFooter == 1) {
			printf(" ______________________________________\n");
			printf("|       |           |                  |\n");
			printf("| Index | Member ID | Feedback Message |\n");
			printf("|_______|___________|__________________|\n");
		}
		else {
			printf("|_______|___________|__________________|\n\n");
		}
	}
	else if (type == 5) {
		if (headerFooter == 1) {
			printf(" __________________________________________________\n");
			printf("|                                  |               |\n");
			printf("|           Description            |   Price (RM)  |\n");
			printf("|__________________________________|_______________|\n");
		}
		else {
			printf("|__________________________________|_______________|\n");
		}
	}
	else if (type == 6) {
		if (headerFooter == 1) {
			printf(" __________________________________________________________________________\n");
			printf("|          |                                |                   |          |\n");
			printf("| Staff ID |              Name              |       Roles       |  Salary  |\n");
			printf("|__________|________________________________|___________________|__________|\n");
		}
		else {
			printf("|__________|________________________________|___________________|__________|\n\n");
		}
	}
}
int strCheckIsZero(char input[100]) {
	if (strcmp(input,"0") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}
void staffLogIn() {
	char staffIdInput[10], staffPswdInput[20];
	int validStaffId = 0;
	int loop = 1;
	do {
		clear();
		printf("[Log In Staff Account]\n\n");
		warnZeroToReturn();
		color("purple"); printf("[Note] ");
		color("white");  printf("You can only create a new staff account after you logged to an staff account.\n");
		color("purple"); printf("[Note] ");
		color("white");  printf("Please contact your supervisor if you did not have a staff account yet.\n\n");

		printf("Staff ID [Eg. S0001] : ");
		gets(staffIdInput);
		rewind(stdin);
		staffIdInput[0] = toupper(staffIdInput[0]);
		if (strCheckIsZero(staffIdInput)) { return; }

		printf("Password             : ");
		gets(staffPswdInput);
		rewind(stdin);
		if (strCheckIsZero(staffPswdInput)) { return; }

		for (int i = 0; i < staffDbSize; i++) {
			if (strcmp(staffsDb[i].userDetails.id, staffIdInput) == 0) {
				validStaffId = 1;
				if (strcmp(staffsDb[i].userDetails.password, staffPswdInput) == 0) {
					//--- successfully Logged In ---
					LoggedStaff = staffsDb[i];
					staffMenu();
					loop = 0;
					break;
				}
				else {
					color("red");
					printf("\n[Wrong Password]");
					color("white");
					_sleep(PAUSE);
				}
			}
		}
		if (validStaffId != 1) {
			color("red");
			printf("\n[Staff ID doesn't exist]");
			color("white");

			_sleep(PAUSE);
		}
	} while (loop == 1);
}
void staffProfile(Staff staff) {

	printf(" __________________________________________\n");
	printf("|                                          |\n");
	printf("|             Staff Information            |\n");
	printf("|__________________________________________|\n");
	printf("| ID       | %-30s|\n", staff.userDetails.id);
	printf("|----------|-------------------------------|\n");
	printf("| Name     | %-30s|\n", staff.userDetails.name);
	printf("|----------|-------------------------------|\n");
	printf("| Password | %-30s|\n", staff.userDetails.password);
	printf("|----------|-------------------------------|\n");
	printf("| Gender   | %-30c|\n", staff.userDetails.gender);
	printf("|----------|-------------------------------|\n");
	printf("| IC       | %-30s|\n", staff.userDetails.ic);
	printf("|----------|-------------------------------|\n");
	printf("| Birthday | %02d/%02d/%04d                    |\n", staff.userDetails.birthday.day, staff.userDetails.birthday.month, staff.userDetails.birthday.year);
	printf("|----------|-------------------------------|\n");
	printf("| Email    | %-30s|\n", staff.userDetails.email);
	printf("|----------|-------------------------------|\n");
	printf("| Roles    | %-30s|\n", staff.roles);
	printf("|----------|-------------------------------|\n");
	printf("| Salary   | RM %-27.2f|\n", staff.salary);
	printf("|__________|_______________________________|\n\n");

}
void staffMenu() {

	int select;

	clear();

	color("green");
	printf("[You Successfully Logged Into Your Account]\n\n");
	color("white");

	do {
		select = -1; // reset the value to invalid, prevent it run the previous function when user input non-integer value
		refreshTrainSche();

		printf("Staff ID : %s\n\n", LoggedStaff.userDetails.id);

		printf("Please select : \n\n");
		printf("[0] Log Out\n");
		printf("[1] Create New Staff Account\n");
		printf("[2] View / Edit Member Details\n");
		printf("[3] Train Schedule\n");
		printf("[4] Edit Ticket\n");
		printf("[5] View Feedback\n");
		printf("[6] Generate Report\n");
		printf("[7] Edit Personal Information\n");
		printf("[8] Update Staff Salary & Roles [Only for CEO & Manager] \n\n");
		printf(">> ");

		scanf("%d", &select);
		rewind(stdin);

		switch (select)
		{
		case 0:
			color("red");
			printf("\n[Logging Out...]");
			color("white");
			_sleep(PAUSE);
			break;
		case 1:
			regStaff();
			break;
		case 2:
			showAllMem();
			break;
		case 3:
			trainMenu();
			break;
		case 4:
			showAllTicket();
			break;
		case 5:
			viewFeedback();
			break;
		case 6:
			generateReport();
			break;
		case 7:
			editStaffInfo();
			break;
		case 8:
			if (strcmp(LoggedStaff.roles, "CEO") == 0 || strcmp(LoggedStaff.roles, "MANAGER") == 0) {
				filterStaff();
			}
			else {
				color("red");
				printf("[Sorry, this feature only for CEO & Manager]");
				_sleep(PAUSE);
			}
			break;
		default:
			invalidInput();
		}
		clear();
	} while (select != 0);
}
void regStaff() {

	Staff staff = { {"id", "-", "-", '-', "-", "-" ,0,0,0,0,0,0}, "-", 0 };
	char confirm;
	int roleSelect;

	if (staffDbSize != 0) {
		staffDbSize++;
		sprintf(staff.userDetails.id, "%c%04d", 'S', staffDbSize);// format the char and int into string for [Staff ID]	
	}
	else {
		staffDbSize++;
		strcpy(staff.userDetails.id, "S0001");
	}
	for (int i = 0; i < 8; i++) {
		clear();
		printf("[Create New Staff]\n\n");
		warnZeroToReturn();
		staffProfile(staff);

		color("cyan"); printf("[+] "); color("white");
		switch (i) {
		case 0:
			printf("Please input staff name : ");
			gets(staff.userDetails.name);
			rewind(stdin);
			stringUpper(staff.userDetails.name);
			if (strCheckIsZero(staff.userDetails.name)) { staffDbSize--; return; }
			break;
		case 1:
			printf("[Min 8 character, Max 18 character, 1 special character]\n"); color("cyan"); printf("[+] "); color("white");
			printf("Please input staff password : ");
			gets(staff.userDetails.password);
			rewind(stdin);
			if (strCheckIsZero(staff.userDetails.password)) { staffDbSize--; return; }

			//validate pswd
			if(!validatePswd(staff.userDetails.password)) {
				strcpy(staff.userDetails.password, "-");
				color("red");
				printf("\n[Invalid Password Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 2:
			printf("Please input staff gender [M/F] : ");
			scanf("%c", &staff.userDetails.gender);
			rewind(stdin);
			staff.userDetails.gender = toupper(staff.userDetails.gender);
			if (staff.userDetails.gender == '0') { staffDbSize--; return; }
			//validate gender
			if (!validateGender(staff.userDetails.gender)) {
				staff.userDetails.gender = '-';
				color("red");
				printf("\n[Invalid Gender]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 3:
			printf("Please input staff IC [xxxxxx-xx-xxxx] : ");
			gets(staff.userDetails.ic);
			rewind(stdin);
			if (strCheckIsZero(staff.userDetails.ic)) { staffDbSize--; return; }
			//validate ic
			if (!validateIC(staff.userDetails.ic)) {
				strcpy(staff.userDetails.ic,"-");
				color("red");
				printf("\n[Invalid IC Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 4:
			printf("Please enter staff Birthday [DD/MM/YYYY] : ");
			scanf("%d/%d/%d", &staff.userDetails.birthday.day,
				&staff.userDetails.birthday.month,
				&staff.userDetails.birthday.year);
			rewind(stdin);
			break;
		case 5:
			printf("Please input staff email [xxx@xxx.xxx] : ");
			gets(staff.userDetails.email);
			rewind(stdin);
			if (strCheckIsZero(staff.userDetails.email)) { staffDbSize--; return; }
			//validate email
			if (!validateEmail(staff.userDetails.email)) {
				strcpy(staff.userDetails.email,"-");
				color("red");
				printf("\n[Invalid Email Format]");
				color("white");
				_sleep(PAUSE);
				i--;
			}
			break;
		case 6:
			printf("Please select role with number : \n\n");
			printf("[1] = CEO\n");
			printf("[2] = Manager\n");
			printf("[3] = Normal Staff\n\n");
			printf(">> ");
			scanf("%d", &roleSelect);
			rewind(stdin);

			//validate roles
			switch (roleSelect) {
			case 1:
				strcpy(staff.roles, "CEO");
				break;
			case 2:
				strcpy(staff.roles, "MANAGER");
				break;
			case 3:
				strcpy(staff.roles, "NORMAL STAFF");
				break;
			default:
				invalidInput();
				i--;
			}
			
			break;
		case 7:
			printf("Please input staff salary : RM ");
			scanf("%f", &staff.salary);
			rewind(stdin);

			//validate salary
			if (staff.salary < 1500) {
				staff.salary = 0;
				color("red");
				printf("[Monthly salary must more than RM 1500.00]");
				_sleep(PAUSE);
				i--;
			}
			break;
		default:
			printf("System Error");
			exit(-1);
		}
	}

	clear();
	printf("[Create New Staff Confirmation]\n\n");
	staffProfile(staff);

	//--- Ask user to confirm ---
	printf("Create existing staff account [Y/N] >> ");
	scanf("%c", &confirm);
	rewind(stdin);
	confirm = toupper(confirm);

	if (confirm == 'Y') {

	    ///////////////////////////////
	   ////// CREATED NEW STAFF //////
	  ///////////////////////////////
		staff.userDetails.dateReg = getCurrentDate();
		staffsDb[staffDbSize - 1] = staff;
		saveDatabase();

		color("green");	printf("\n[Staff Account Created]\n");
		color("red");   printf("[WARNING] ");
		color("white");	printf("Please inform the staff his/her staff ID and default password you set to Log In his/her account!\n");
		color("red");	printf("[WARNING] ");
		color("white");	printf("Please inform the staff his/her to reset the password and make confirmation on his/her personal details!\n\n");
	}
	else {

	    ////////////////////////////
	   ////////// FAILED //////////
	  ////////////////////////////

		staffDbSize -= 1;

		color("red"); printf("\n[Account FAILED to create!]\n");
	}

	pressEnter();
}
void showAllStaff(){
	
	drawTable(6, 1);
	for (int i = 0; i < staffDbSize; i++) {
		printf("|  %s   | %-30s | %-17s |RM%-8.2lf|\n",
			staffsDb[i].userDetails.id,
			staffsDb[i].userDetails.name,
			staffsDb[i].roles,
			staffsDb[i].salary);
	}
	drawTable(6, 2);

}
void updateSalaryRoles(int staffIndex) {
	int select;
	int roleSelect;
	char yesNo;
	do {
		select = -1;
		clear();
		printf("[Update existing staff Salary & Roles]\n\n");
		drawTable(6, 1);
		printf("|  %s   | %-30s | %-17s |RM%-8.2lf|\n",
			staffsDb[staffIndex].userDetails.id,
			staffsDb[staffIndex].userDetails.name,
			staffsDb[staffIndex].roles,
			staffsDb[staffIndex].salary);
		drawTable(6, 2);

		printf("Please select : \n\n");
		printf("[0] Back\n");
		printf("[1] Salary\n");
		printf("[2] Role\n");
		printf("[3] Delete Staff\n\n");
		printf(">> ");
		scanf("%d", &select);
		rewind(stdin);

		printf("\n===============================================\n\n");
		switch (select) {
		case 0:
			break;
		case 1:
			printf("\nUpdate Monthly Salary To : ");
			scanf("%f", &staffsDb[staffIndex].salary);
			rewind(stdin);
			if (staffsDb[staffIndex].salary < 1500) {
				color("red");    printf("[WARNING] ");
				color("white");  printf("The salary you set for the staff is lower than the label law!");
				_sleep(PAUSE);
			}
			break;
		case 2:
			printf("\nPlease select role with number : \n\n");
			printf("[1] = CEO\n");
			printf("[2] = Manager\n");
			printf("[3] = Normal Staff\n\n");
			printf(">> ");
			scanf("%d", &roleSelect);
			rewind(stdin);
			switch (roleSelect) {
			case 1:
				strcpy(staffsDb[staffIndex].roles, "CEO");
				break;
			case 2:
				strcpy(staffsDb[staffIndex].roles, "MANAGER");
				break;
			case 3:
				strcpy(staffsDb[staffIndex].roles, "NORMAL STAFF");
				break;
			default:
				invalidInput();
			}
			break;
		case 3:
			printf("Are you sure you want to delete existing staff [Y/N] : ");
			scanf("%c", &yesNo);
			rewind(stdin);
			if (yesNo == 'y' || yesNo == 'Y') {
				deleteStaff(staffsDb[staffIndex]);
				printf("[Successfully deleted existing staff]");
				pressEnter();
				return;
			}
			else {
				color("red");
				printf("[Failed to delete existing staff]");
				_sleep(PAUSE);
			}
			break;
		default:
			invalidInput();
		}
		saveDatabase();

		//--- Update logged staff ---
		if (strcmp(LoggedStaff.userDetails.id, staffsDb[staffIndex].userDetails.id )== 0) {
			LoggedStaff = staffsDb[staffIndex];
		}
	} while (select != 0);
}
void deleteStaff(Staff staff) {
	for (int i = 0; i < staffDbSize; i++) {
		if (strcmp(staff.userDetails.id, staffsDb[i].userDetails.id) == 0) {
			for (int x = i; x < memDbSize; x++) {
				staffsDb[x] = staffsDb[x + 1];
			}
			staffDbSize--;
			break;
		}
	}
	saveDatabase();
}
void filterStaff() {

	char filterInput[30];
	int staffsIndexArray[DB_MAX];
	int staffIndexSize = 0;
	
	do {
		clear();
		printf("[All Staffs]\n\n");
		warnZeroToReturn();
		showAllStaff();

		printf("Filter with [Staff ID / Staff Roles] : ");
		gets(filterInput);
		rewind(stdin);
		stringUpper(filterInput);
		if (strCheckIsZero(filterInput)) { return; }

		staffIndexSize = 0; //reset for the next loop
		for (int i = 0; i < staffDbSize; i++) {
			if (strcmp(filterInput, staffsDb[i].roles) == 0) {
				staffsIndexArray[staffIndexSize] = i;
				staffIndexSize++;
			}
			else if (strcmp(filterInput, staffsDb[i].userDetails.id)==0) {
				staffsIndexArray[staffIndexSize] = i;
				staffIndexSize++;
				break;
			}
		}

		if (staffIndexSize > 0) {
			if (staffIndexSize == 1) {
				//Filtered with Staff ID
				updateSalaryRoles(staffsIndexArray[0]);
			}
			else {
				//Filtered with Roles
				clear();
				printf("[Filtered Staffs]\n\n");
				warnZeroToReturn();
				drawTable(6, 1);
				for (int i = 0; i < staffIndexSize; i++) {
					printf("|  %s   | %-30s | %-17s |RM%-8.2lf|\n",
						staffsDb[staffsIndexArray[i]].userDetails.id,
						staffsDb[staffsIndexArray[i]].userDetails.name,
						staffsDb[staffsIndexArray[i]].roles,
						staffsDb[staffsIndexArray[i]].salary);
				}
				drawTable(6, 2);
				printf("Please input specific Staff ID : ");
				gets(filterInput);
				rewind(stdin);
				stringUpper(filterInput);

				for (int i = 0; i < staffDbSize; i++) {
					if (strcmp(staffsDb[i].userDetails.id, filterInput) == 0) {
						updateSalaryRoles(i);
						break;
					}
				}
			}
		}
		else {
			invalidInput();
		}
	} while (1); // When user press "0" only it break the loop
}
void generateReport() {

	float totalStaffSalary = 0;
	float totalKm = 0;
	float profit = 0;
	float ticket = 0;
	int newStaff = 0;
	int newMem = 0;

	GetLocalTime(&globalCurrentTime);

	for (int i = 0; i < staffDbSize; i++) {
		totalStaffSalary += staffsDb[i].salary;
		if (staffsDb[i].userDetails.dateReg.month == globalCurrentTime.wMonth &&
			staffsDb[i].userDetails.dateReg.year == globalCurrentTime.wYear) {
			newStaff++;
		}
	}
	for (int i = 0; i < memDbSize; i++) {
		if (membersDb[i].userDetails.dateReg.month == globalCurrentTime.wMonth &&
			membersDb[i].userDetails.dateReg.year == globalCurrentTime.wYear) {
			newMem++;
		}
	}
	for (int i = 0; i < trainScheDbSize; i++) {
		if((trainScheDb[i].date.year == globalCurrentTime.wYear) && (trainScheDb[i].date.month == globalCurrentTime.wMonth)){
			totalKm += trainScheDb[i].km;
		}
	}
	for (int i = 0; i < bookedDbSize; i++) {
		if ((bookedDb[i].dateBooked.year == globalCurrentTime.wYear) && (bookedDb[i].dateBooked.month == globalCurrentTime.wMonth)) {
			ticket += bookedDb[i].price;
		}
	}
	profit = ticket - (totalKm * FUEL_PRICE_PER_KM) - totalStaffSalary;

	clear();
	printf("[Monthly Report]\n\n");
	printf("Date : %d/%d/%d\n\n", globalCurrentTime.wDay, globalCurrentTime.wMonth, globalCurrentTime.wYear);
	printf("New Staff Added In This Month  : %d\n", newStaff);
	printf("New Member Added In This Month : %d\n\n", newMem);
	drawTable(5, 1);
	printf("| Total Staffs Salary              | -RM%-10.2f |\n", totalStaffSalary);
	printf("| Total Fuel Expenses              | -RM%-10.2f |\n", totalKm * FUEL_PRICE_PER_KM);
	printf("| Total Ticket Sold                | +RM%-10.2f |\n", ticket);
	printf("|__________________________________|_______________|\n");
	printf("|                                  |               |\n");
	printf("|                                  | "); (profit > 0) ? color("green") : color("red"); printf("RM%-11.2f ", profit); color("white"); printf("| \n");
	drawTable(5, 2);

	pressEnter();
}
void showAllMem() {
	char memberIdToEdit[10];
	int validMemId;
	int memberIndex;
	do {
		clear();
		printf("[Member Details]\n\n");
		warnZeroToReturn();
		color("red");   printf("[WARNING] ");
		color("white");	printf("Please maximize the windows to prevent glitched view!\n\n");
		
		drawTable(2, 1);
		for (int i = 0; i < memDbSize; i++) {
			printf("|%-13s|%-30s|%-24s|%-8c|%-18s|%-31s|%02d/%02d/%04d   |%-11.2f|\n",
				membersDb[i].userDetails.id,
				membersDb[i].userDetails.name,
				membersDb[i].userDetails.password,
				membersDb[i].userDetails.gender,
				membersDb[i].userDetails.ic,
				membersDb[i].userDetails.email,
				membersDb[i].userDetails.birthday.day, membersDb[i].userDetails.birthday.month, membersDb[i].userDetails.birthday.year,
				membersDb[i].balance);
		}
		drawTable(2, 2);

		printf("\n[Edit]\n\n");
		printf("Input the Member ID that you wanted to edit : ");
		gets(memberIdToEdit);
		rewind(stdin);
		memberIdToEdit[0] = toupper(memberIdToEdit[0]);
		if (strCheckIsZero(memberIdToEdit)) { break; }

		validMemId = 0;
		for (int i = 0; i < memDbSize; i++) {
			if (strcmp(membersDb[i].userDetails.id, memberIdToEdit) == 0) {
				validMemId = 1;
				memberIndex = i;
				break;
			}
		}

		if (validMemId == 1) {
			editMember(membersDb[memberIndex],2);
		}
		else {
			color("red");
			printf("\n[Member ID doesn't exits]");
			_sleep(PAUSE);
		}

	} while (1);
}
void trainMenu() {
	int select;
	do {
		clear();
		printf("[Train]\n\n");
		drawTable(1, 1);
		for (int i = 0; i < trainScheDbSize; i++) {
			printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
				trainScheDb[i].trainId,
				trainScheDb[i].from,
				trainScheDb[i].to,
				trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
				trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
				trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
				trainScheDb[i].status,
				trainScheDb[i].km);
		}
		drawTable(1, 2);
		printf("\nPlease select : \n\n");
		printf("[0] Back\n");
		printf("[1] Add New Train\n");
		printf("[2] Edit Train\n");
		printf("[3] Delete Train\n\n");
		printf(">> ");
		scanf("%d", &select);
		rewind(stdin);

		switch (select) {
		case 0:
			break;
		case 1:
			addNewTrain();
			break;
		case 2:
			editTrain();
			break;
		case 3:
			deleteTrain();
			break;
		default:
			invalidInput();
		}
	} while (select != 0);
}
void showAllTrain(int memOrStaff) {
	drawTable(1, 1);
	for (int i = 0; i < trainScheDbSize; i++) {
		if (memOrStaff == 1) {
			if (strcmp(trainScheDb[i].status, "Available") == 0 || strcmp(trainScheDb[i].status, "Full") == 0) {
				printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
					trainScheDb[i].trainId,
					trainScheDb[i].from,
					trainScheDb[i].to,
					trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
					trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
					trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
					trainScheDb[i].status,
					trainScheDb[i].km);
			}
		}
		else {
			printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
				trainScheDb[i].trainId,
				trainScheDb[i].from,
				trainScheDb[i].to,
				trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
				trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
				trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
				trainScheDb[i].status,
				trainScheDb[i].km);
		}
	}
	drawTable(1, 2);
}
void showAvailableTrain() {
	drawTable(1, 1);
	for (int i = 0; i < trainScheDbSize; i++) {
		if (strcmp(trainScheDb[i].status, "Available") == 0) {
			printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
				trainScheDb[i].trainId,
				trainScheDb[i].from,
				trainScheDb[i].to,
				trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
				trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
				trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
				trainScheDb[i].status,
				trainScheDb[i].km);
		}
	}
	drawTable(1, 2);
}
void trainTable(Train train) {

	printf(" _________________________________\n");
	printf("|              Train              |\n");
	printf("|                                 |\n");
	printf("|---------------------------------|\n");
	printf("| Train ID    | %s             |\n", train.trainId);
	printf("|-------------|-------------------|\n");
	printf("| From        | %-18s|\n", train.from);
	printf("|-------------|-------------------|\n");
	printf("| To          | %-18s|\n", train.to);
	printf("|-------------|-------------------|\n");
	printf("| Date        | %02d/%02d/%04d        |\n", train.date.day, train.date.month, train.date.year);
	printf("|-------------|-------------------|\n");
	printf("| Depart Time | %02d:%02d             |\n", train.departTime.hours, train.departTime.minutes);
	printf("|-------------|-------------------|\n");
	printf("| Arrive Time | %02d:%02d             |\n", train.arriveTime.hours, train.arriveTime.minutes);
	printf("|-------------|-------------------|\n");
	printf("| KM          | %-18.2f|\n", train.km);
	printf("|-------------|-------------------|\n");
	printf("| Status      | %-18s|\n", train.status);
	printf("|_____________|___________________|\n\n");
}
void addNewTrain() {

	Train newTrain = { " ", " ", " ", {0,0,0}, {0,0}, {0,0},"Available" , 0};
	char yesNo;
	int latestTrainId;
	int select;

	if (trainScheDbSize != 0) {
		sscanf(trainScheDb[trainScheDbSize - 1].trainId, "T%d", &latestTrainId);
		sprintf(newTrain.trainId, "%c%04d", 'T', latestTrainId + 1);
	}
	else {
		strcpy(newTrain.trainId, "T0001");
	}

	for (int i = 0; i < 7; i++) {
		clear();

		printf("[Add New Train]\n\n");
		warnZeroToReturn();
		trainTable(newTrain);
		printf("\n===================================================================\n\n");
		switch (i){
		case 0:
			printf("[1] PENANG\t");
			printf("[2] KEDAH\t");
			printf("[3] MELAKA\t");
			printf("[4] PERLIS\t");
			printf("[5] JOHOR\n\n");
			printf("From : ");
			scanf("%d", &select);
			rewind(stdin);
			switch (select)	{
			case 0:
				return;
			case 1:
				strcpy(newTrain.from,"PENANG");
				break;
			case 2:
				strcpy(newTrain.from, "KEDAH");
				break;
			case 3:
				strcpy(newTrain.from, "MELAKA");
				break;
			case 4:
				strcpy(newTrain.from, "PERLIS");
				break;
			case 5:
				strcpy(newTrain.from, "JOHOR");
				break;
			default:
				invalidInput();
				i--;
			}
			break;
		case 1:
			printf("[1] PENANG\t");
			printf("[2] KEDAH\t");
			printf("[3] MELAKA\t");
			printf("[4] PERLIS\t");
			printf("[5] JOHOR\n\n");
			printf("From : ");
			scanf("%d", &select);
			rewind(stdin);
			switch (select) {
			case 0:
				return;
			case 1:
				strcpy(newTrain.to, "PENANG");
				break;
			case 2:
				strcpy(newTrain.to, "KEDAH");
				break;
			case 3:
				strcpy(newTrain.to, "MELAKA");
				break;
			case 4:
				strcpy(newTrain.to, "PERLIS");
				break;
			case 5:
				strcpy(newTrain.to, "JOHOR");
				break;
			default:
				invalidInput();
				i--;
			}
			break;
		case 2:
			printf("Date [Eg. 1/1/2024] : ");
			scanf("%d/%d/%d", &newTrain.date.day, &newTrain.date.month, &newTrain.date.year);
			rewind(stdin);
			break;
		case 3:
			printf("Depart Time [H:M] : ");
			scanf("%d:%d", &newTrain.departTime.hours, &newTrain.departTime.minutes);
			break;
		case 4:
			printf("Depart Time [H:M] : ");
			scanf("%d:%d", &newTrain.arriveTime.hours, &newTrain.arriveTime.minutes);
			break;
		case 5:
			printf("KM  : ");
			scanf("%f", &newTrain.km);
			rewind(stdin);
			break;
		case 6:
			printf("\nConfirm adding existing Train ? [Y/N]\n");
			printf(">> ");
			scanf("%c", &yesNo);
			rewind(stdin);
			yesNo = toupper(yesNo);

			if (yesNo == 'Y') {
				trainScheDb[trainScheDbSize] = newTrain;
				trainScheDbSize++;
				saveDatabase();

				color("green");
				printf("\n[New Train Added]");
				color("white");
				pressEnter();
			}
			else {
				color("red");
				printf("\n[Failed To Add New Train]");
				color("white");
				pressEnter();
			}
			break;
		default:
			color("red");
			printf("[System Error]");
			_sleep(PAUSE);
			break;
		}
	}
}
void editStaffInfo() {

	Staff updateDetails;
	int select;
	char yesNo;

	do {
		clear();
		printf("[Personal Information]\n\n");
		staffProfile(LoggedStaff);
		printf("\n[Edit]\n\n");
		printf("Please select : \n\n");
		printf("[0] Back \n");
		printf("[1] Name \n");
		printf("[2] Password \n");
		printf("[3] Gender \n");
		printf("[4] IC \n");
		printf("[5] Email \n\n");
		printf(">> ");
		scanf("%d", &select);
		rewind(stdin);

		switch (select) {
		case 0:
			break;
		case 1:
			printf("\nChange Name To : ");
			gets(updateDetails.userDetails.name);
			rewind(stdin);
			strcpy(LoggedStaff.userDetails.name, updateDetails.userDetails.name);
			break;
		case 2:
			printf("\nChange Password To : ");
			gets(updateDetails.userDetails.password);
			rewind(stdin);
			if (validatePswd(updateDetails.userDetails.password)) {
				strcpy(LoggedStaff.userDetails.password, updateDetails.userDetails.password);
			}
			else {
				color("red");
				printf("[Invalid Password Format]");
				_sleep(PAUSE);
			}
			break;
		case 3:
			printf("\nChange Gender To : ");
			scanf("%c", &updateDetails.userDetails.gender);
			rewind(stdin);
			updateDetails.userDetails.gender = toupper(updateDetails.userDetails.gender);
			if (validateGender(updateDetails.userDetails.gender)) {
				LoggedStaff.userDetails.gender = updateDetails.userDetails.gender;
			}
			else {
				color("red");
				printf("[Invalid Gender]");
				_sleep(PAUSE);
			}
			break;
		case 4:
			printf("\nChange IC To : ");
			gets(updateDetails.userDetails.ic);
			rewind(stdin);
			strcpy(LoggedStaff.userDetails.ic, updateDetails.userDetails.ic);
			break;
		case 5:
			printf("\nChange Email To : ");
			gets(updateDetails.userDetails.email);
			rewind(stdin);
			strcpy(LoggedStaff.userDetails.email, updateDetails.userDetails.email);
			break;
		default:
			invalidInput();
			break;
		}

		for (int i = 0; i < staffDbSize; i++) {
			if (strcmp(LoggedStaff.userDetails.id, staffsDb[i].userDetails.id) == 0) {
				staffsDb[i] = LoggedStaff;
				break;
			}
		}
		saveDatabase();

	} while (select != 0);
}
void editTrain() {

	Train editTrain;
	int trainIndex = -1; //Used for the train index and also validate
	int select;

	clear();

	printf("[Edit Train]\n\n");
	warnZeroToReturn();
	showAllTrain(2);

	printf("\nPlease input the train ID that you want to edit [Eg. T0001] : ");
	gets(editTrain.trainId);
	rewind(stdin);
	editTrain.trainId[0] = toupper(editTrain.trainId[0]);
	if (strCheckIsZero(editTrain.trainId)) { return; }

	for (int i = 0; i < trainScheDbSize; i++) {
		if (strcmp(editTrain.trainId, trainScheDb[i].trainId) == 0) {
			trainIndex = i;
			break;
		}
	}
	
	// Found train
	if (trainIndex != -1) {
		editTrain = trainScheDb[trainIndex];
		select = -1;
		while (select != 0) {
			clear();

			printf("[Edit Train]\n\n");
			warnZeroToReturn();
			trainTable(editTrain);
			printf("[0] Back\n");
			printf("[1] From\n");
			printf("[2] To\n");
			printf("[3] Date\n");
			printf("[4] Depart Hour\n");
			printf("[5] Arrive Hour\n");
			printf("[6] Km\n\n");

			printf(">> ");
			scanf("%d", &select);
			rewind(stdin);

			printf("\n===================================================================\n\n");
			switch (select) {
			case 0:
				return;
			case 1:
				printf("[1] PENANG\t");
				printf("[2] KEDAH\t");
				printf("[3] MELAKA\t");
				printf("[4] PERLIS\t");
				printf("[5] JOHOR\n\n");
				printf("From : ");
				scanf("%d", &select);
				rewind(stdin);
				switch (select) {
				case 0:
					break;
				case 1:
					strcpy(editTrain.from, "PENANG");
					break;
				case 2:
					strcpy(editTrain.from, "KEDAH");
					break;
				case 3:
					strcpy(editTrain.from, "MELAKA");
					break;
				case 4:
					strcpy(editTrain.from, "PERLIS");
					break;
				case 5:
					strcpy(editTrain.from, "JOHOR");
					break;
				default:
					invalidInput();
				}
				break;
			case 2:
				printf("[1] PENANG\t");
				printf("[2] KEDAH\t");
				printf("[3] MELAKA\t");
				printf("[4] PERLIS\t");
				printf("[5] JOHOR\n\n");
				printf("From : ");
				scanf("%d", &select);
				rewind(stdin);
				switch (select) {
				case 1:
					strcpy(editTrain.to, "PENANG");
					break;
				case 2:
					strcpy(editTrain.to, "KEDAH");
					break;
				case 3:
					strcpy(editTrain.to, "MELAKA");
					break;
				case 4:
					strcpy(editTrain.to, "PERLIS");
					break;
				case 5:
					strcpy(editTrain.to, "JOHOR");
					break;
				default:
					invalidInput();
				}
				break;
			case 3:
				printf("Date [Eg. 1/1/2024] : ");
				scanf("%d/%d/%d", &editTrain.date.day, &editTrain.date.month, &editTrain.date.year);
				rewind(stdin);
				break;
			case 4:
				printf("Depart Time [H:M] : ");
				scanf("%d:%d", &editTrain.departTime.hours, &editTrain.departTime.minutes);
				break;
			case 5:
				printf("Depart Time [H:M] : ");
				scanf("%d:%d", &editTrain.arriveTime.hours, &editTrain.arriveTime.minutes);
				break;
			case 6:
				printf("KM  : ");
				scanf("%f", &editTrain.km);
				rewind(stdin);
				break;
			default:
				invalidInput();
				break;
			}

			trainScheDb[trainIndex] = editTrain;
			saveDatabase();
		}
	
	}
	else {
		//Not Found
		invalidInput();
	}
}
void deleteTrain() {
	Train deleteTrain;
	int trainIndex = -1;
	char yesNo;

	clear();

	printf("[Delete Train]\n\n");
	warnZeroToReturn();
	showAllTrain(2);

	printf("\nPlease input the train ID that you want to delete [Eg. T0001] : ");
	gets(deleteTrain.trainId);
	rewind(stdin);
	deleteTrain.trainId[0] = toupper(deleteTrain.trainId[0]);
	if (strCheckIsZero(deleteTrain.trainId)) { return; }

	for (int i = 0; i < trainScheDbSize; i++) {
		if (strcmp(deleteTrain.trainId, trainScheDb[i].trainId) == 0) {
			trainIndex = i;
			break;
		}
	}

	if (trainIndex != -1) {
		clear();

		printf("[Delete existing Train]\n\n");
		drawTable(1, 1);

		printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
			trainScheDb[trainIndex].trainId,
			trainScheDb[trainIndex].from,
			trainScheDb[trainIndex].to,
			trainScheDb[trainIndex].date.day, trainScheDb[trainIndex].date.month, trainScheDb[trainIndex].date.year,
			trainScheDb[trainIndex].arriveTime.hours, trainScheDb[trainIndex].arriveTime.minutes,
			trainScheDb[trainIndex].departTime.hours, trainScheDb[trainIndex].departTime.minutes,
			trainScheDb[trainIndex].status,
			trainScheDb[trainIndex].km);

		drawTable(1, 2);

		printf("\nAre you sure you wanted to delete existing Train [Y/N] ?\n");
		printf(">> ");
		scanf("%c", &yesNo);
		rewind(stdin);
		yesNo = toupper(yesNo);

		if (yesNo == 'Y') {
			for (int i = 0; i < bookedDbSize; i++) {
				if (strcmp(bookedDb[i].trainId, trainScheDb[trainIndex].trainId) == 0) {
					for (int x = i; x < bookedDbSize - 1; x++) {
						bookedDb[x] = bookedDb[x + 1];
					}
					bookedDbSize--;
				}
			}
			for (int i = trainIndex; i < trainScheDbSize-1; i++) {
				trainScheDb[i] = trainScheDb[i + 1];
			}
			trainScheDbSize --;
			saveDatabase();
			color("green");
			printf("\n[Existing Train Successfully Deleted]");
			color("white");
			pressEnter();
		}
		else {
			color("red");
			printf("[Existing Train Failed To Delete]");
			color("white");
			pressEnter();
		}
	}
	else {
		invalidInput();
	}

}
void viewTrainSche() {
	char input[100];

	do {
		clear();
		
		printf("[Train Schedule]\n\n");
		warnZeroToReturn();
		showAllTrain(1);

		printf("\nFilter with [Train ID / From / To] : ");
		gets(input);
		rewind(stdin);
		stringUpper(input);
		if (strCheckIsZero(input)) { return; }

		clear();
		printf("[Train Schedule]\n\n");
		drawTable(1, 1);
		for (int i = 0; i < trainScheDbSize; i++) {
			if (strcmp(trainScheDb[i].status, "Unavailable") == 0)
				continue;
			if (strcmp(input, trainScheDb[i].trainId) == 0) {
				printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
					trainScheDb[i].trainId,
					trainScheDb[i].from,
					trainScheDb[i].to,
					trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
					trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
					trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
					trainScheDb[i].status,
					trainScheDb[i].km);
			}
			else if (strcmp(input, trainScheDb[i].from) == 0) {
				printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
					trainScheDb[i].trainId,
					trainScheDb[i].from,
					trainScheDb[i].to,
					trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
					trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
					trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
					trainScheDb[i].status,
					trainScheDb[i].km);
			}
			else if (strcmp(input, trainScheDb[i].to) == 0) {
				printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
					trainScheDb[i].trainId,
					trainScheDb[i].from,
					trainScheDb[i].to,
					trainScheDb[i].date.day, trainScheDb[i].date.month, trainScheDb[i].date.year,
					trainScheDb[i].arriveTime.hours, trainScheDb[i].arriveTime.minutes,
					trainScheDb[i].departTime.hours, trainScheDb[i].departTime.minutes,
					trainScheDb[i].status,
					trainScheDb[i].km);
			}
		}
		drawTable(1, 2);
		pressEnter();

	} while (1); // Infinity loop until user press [0] to return
}
void showTrainWithIndex(int trainIndex) {
	drawTable(1, 1);
	printf("|%-10s|%-14s|%-14s|%02d/%02d/%04d  |%02d:%02d        |%02d:%02d        |%-11s|%-8.1f|\n",
		trainScheDb[trainIndex].trainId,
		trainScheDb[trainIndex].from,
		trainScheDb[trainIndex].to,
		trainScheDb[trainIndex].date.day, trainScheDb[trainIndex].date.month, trainScheDb[trainIndex].date.year,
		trainScheDb[trainIndex].arriveTime.hours, trainScheDb[trainIndex].arriveTime.minutes,
		trainScheDb[trainIndex].departTime.hours, trainScheDb[trainIndex].departTime.minutes,
		trainScheDb[trainIndex].status,
		trainScheDb[trainIndex].km);
	drawTable(1, 2);
}
void viewFeedback() {

	int feedbackIndex;
	
	do {
		clear();
		printf("[Feedback from members]\n\n");
		warnZeroToReturn();

		color("purple"); printf("\n[Note] ");
		color("white");  printf("Green color means replied\n\n");

		drawTable(4, 1);
		for (int i = 0; i < feedbackDbSize; i++) {
			if (feedbackDb[i].repliedStatus)
				color("green");
			else
				color("white");
			printf("| %5d | %9s | %.13s... |\n", i + 1, feedbackDb[i].memberId, feedbackDb[i].message);
		}
		color("white");
		drawTable(4, 2);

		printf("Please input the index to choose : ");
		scanf("%d", &feedbackIndex);
		rewind(stdin);

		if (feedbackIndex != 0) {
			replyFeedback(feedbackIndex-1);
		}

	} while (feedbackIndex!=0);
}
void replyFeedback(int feedbackIndex) {

	char replyMsg[300];
	char yesNo;
	
	do {
		clear();

		printf("[Reply Feedback]\n\n");
		warnZeroToReturn();

		color("purple"); printf("\n[Note] ");
		color("white");  printf("Once ENTER pressed the message will be replied.\n\n");

		printf("\nFeedback from : %s\n\n", feedbackDb[feedbackIndex]);
		printf("-------------------------------------------\n");
		printf("Dear %s,\n\n", LoggedStaff.userDetails.name);
		printf("\t%s\n\n", feedbackDb[feedbackIndex].message);
		printf("Thank you\n");
		printf("%d/%d/%d\n", feedbackDb[feedbackIndex].timeSent.wDay, feedbackDb[feedbackIndex].timeSent.wMonth, feedbackDb[feedbackIndex].timeSent.wYear);
		printf("-------------------------------------------\n\n");

		printf("Reply : ");
		gets(replyMsg);
		rewind(stdin);

		if (strCheckIsZero(replyMsg)) { return; }

		printf("\nAre you sure you wanted to reply this message [Y/N] ? \n");
		printf(">> ");
		scanf("%c", &yesNo);
		rewind(stdin);
		yesNo = toupper(yesNo);

		if (yesNo == 'Y') {
			feedbackDb[feedbackIndex].repliedStatus = 1;
			strcpy(feedbackDb[feedbackIndex].replyMsg, replyMsg);
			GetLocalTime(&globalCurrentTime);
			feedbackDb[feedbackIndex].timeReply = globalCurrentTime;
			strcpy(feedbackDb[feedbackIndex].staffId, LoggedStaff.userDetails.id);
			saveDatabase();

			color("green");
			printf("\n[You successfully replied the message]\n");
			pressEnter();
			break;
		}
		else {
			color("red");
			printf("\n[Failed to replied the message]\n");
			pressEnter();
		}
	} while (1);

}
void buyTicket() {

	char trainIdInput[6];
	int validTrainId;
	int trainIndex;

	do {
		
		clear();
		printf("[Buy Ticket]\n\n");
		warnZeroToReturn();

		//--- Only show train with status [Available]--- 
		showAvailableTrain();

		printf("\nPlease input the Train ID that you wanted to book for a seat [Eg. T0001] : ");
		gets(trainIdInput);
		rewind(stdin);
		trainIdInput[0] = toupper(trainIdInput[0]);
		if (strCheckIsZero(trainIdInput)) { return; }

		validTrainId = 0; // Initialize the Train ID inputted is not valid
		for (int i = 0; i < trainScheDbSize; i++) {
			if (strcmp(trainIdInput, trainScheDb[i].trainId) == 0) {
				validTrainId = (strcmp(trainScheDb[i].status, "Available") == 0) ? 1: -1;
				trainIndex = i;
				break;
			}
		}

		//--- Train found and it is available ---
		if (validTrainId == 1) {
			chooseSeat(trainIdInput,trainIndex);
		}
		//--- Train found but unavailable ---
		else if (validTrainId == -1) {
			color("red");
			printf("[Sorry, the train is now currently UNAVAILABLE]\n");
			_sleep(PAUSE);
		}
		//--- Train did not found ---
		else {
			color("red");
			printf("[Train ID doesn't exists]");
			_sleep(PAUSE);
		}
	} while (1); // Infinity loop until user press [0] to break
}
void showTrainSeat(char bookedSeat[27][3], int type, char ticketSeat[3]) {

	char seatNum[3];

	//--- Show the empty seat with white 'X' and booked seat with red 'X'
	// [Note] sprintf() <- combine different type variable into string

	color("cyan");
	printf("\t    1 2 3   4 5 6   7 8 9\n\n");
	color("white");
	if (type == 1) {
		for (int i = 0; i < 3; i++) {
			color("cyan");
			printf("\t%c   ", i + 65); // 65 is ascii of A
			color("white");
			for (int x = 0; x < 11; x++) {
				if (x != 3 && x != 7) {
					if (x < 3)
						sprintf(seatNum, "%c%d", i + 65, x + 1);
					else if (x < 7)
						sprintf(seatNum, "%c%d", i + 65, x);
					else
						sprintf(seatNum, "%c%d", i + 65, x - 1);
					for (int y = 0; y < 27; y++) {
						if (strcmp(ticketSeat, seatNum) == 0) {
							color("green");
							break;
						}
						else if (strcmp(bookedSeat[y], seatNum) == 0) {
							color("red");
							break;
						}
					}
					printf("X ");
					color("white");
				}
				else
					printf("  "); // Spacing between 123 456 789
			}
			printf("\n");
		}
	}
	else if (type == 2) {
		for (int i = 0; i < 3; i++) {
			color("cyan");
			printf("\t%c   ", i + 65); // 65 is ascii of A
			color("white");
			for (int x = 0; x < 11; x++) {
				if (x != 3 && x != 7) {
					if (x < 3)
						sprintf(seatNum, "%c%d", i + 65, x + 1);
					else if (x < 7)
						sprintf(seatNum, "%c%d", i + 65, x);
					else
						sprintf(seatNum, "%c%d", i + 65, x - 1);
					for (int y = 0; y < 27; y++) {
						if (strcmp(bookedSeat[y], seatNum) == 0) {
							color("red");
							break;
						}
					}
					printf("X ");
					color("white");
				}
				else
					printf("  "); // Spacing between 123 456 789
			}
			printf("\n");
		}
	}
}
int validateSeatInput(char seat[3]) {

	int invalidSeat = 0;

	switch (seat[0]) {
	case 'A':
	case 'B':
	case 'C':
		switch (seat[1]) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (seat[2] != '\0') {
				invalidSeat = 1;
			}
			break;
		default:
			invalidSeat =  1;
			break;
		}
		break;
	default:
		invalidSeat = 1;
	}

	return invalidSeat; // Valid will return 0
}
int validateSeatBooked(char bookedSeat[27][3], char seat[3]) {

	char temp[3];
	int booked = 0;

	for (int i = 65; i < 68; i++) { // 65 is ascii of A
		for (int x = 1; x < 10; x++) {
			sprintf(temp, "%c%d", i, x);
			if (strcmp(seat, temp) == 0) {
				for (int y = 0; y < 27; y++) {
					if (strcmp(bookedSeat[y], seat) == 0) {
						booked = 1;
						break;
					}
				}
				break;
			}
		}
	}
	return booked; // booked will return 1
}
void buySeat(Ticket buyTicket, char bookedTicket[27][3]) {

	int invalidSeat;
	int latestTicketId;
	char yesNo;

	//-- validating the seat number input --
	invalidSeat = validateSeatInput(buyTicket.seat); // Invalid will return 1

	if (invalidSeat == 1) {
		//Invalid input like [Eg. Z1]
		invalidInput();
	}
	else {
		invalidSeat = validateSeatBooked(bookedTicket, buyTicket.seat); // If booked return 1
		if (invalidSeat == 1) {
			//Already booked by others
			color("red");
			printf("\n[Sorry, the seat is booked]\n");
			_sleep(PAUSE);
		}
		else {
			    /////////////
			   /// Valid ///
			  /////////////
			  
			//--- Update Ticket price if VIP seat ---
			// VIP seat [A B C] [1 2 3]
			switch (buyTicket.seat[1]) {
			case '1':
			case '2':
			case '3':
				buyTicket.price *= VIP_PRICE;
				break;
			default:
				break;
			}

			//--- Check member balance ---
			// Balance enough
			
			printf("\nThe price for the seat [%s] will be [",buyTicket.seat); color("green");
			printf("RM % .2f", buyTicket.price); color("white");
			printf("]\n");
			if (loggedMember.balance >= buyTicket.price) {
				printf("Are you sure you want to book the Ticket for Train [%s] on Seat [%s] [Y/N] ?\n", buyTicket.trainId, buyTicket.seat);
				printf(">> ");
				scanf("%c", &yesNo);
				rewind(stdin);
				yesNo = toupper(yesNo);

				if (yesNo == 'Y') {
					//--- Bought the TICKET ---
					loggedMember.balance -= buyTicket.price;
					updateMember(loggedMember);
					
					if (bookedDbSize != 0) {
						sscanf(bookedDb[bookedDbSize - 1].ticketId, "TK%d", &latestTicketId);
						sprintf(buyTicket.ticketId, "%s%05d", "TK", latestTicketId + 1);
					}
					else {
						strcpy(bookedDb[bookedDbSize - 1].ticketId, "TK00001");
					}

					buyTicket.dateBooked = getCurrentDate();
					buyTicket.timeBooked = getCurrentTime();
					buyTicket.outDated = 0;

					bookedDb[bookedDbSize] = buyTicket;
					bookedDbSize++;
					saveDatabase();

					color("green");
					printf("\n[You successfully purchased the Ticket]");
					pressEnter();
				}
				else {
					color("red");
					printf("[Failed to buy the Ticket]");
					_sleep(PAUSE);
				}
			}
			// Balance Not enough
			else {
				color("red");
				printf("[Sorry, your balance is not enough for the seat. Your current balance : RM%.2f]\n", loggedMember.balance);
				_sleep(PAUSE);
			}
		}
	}
}
void chooseSeat(char trainId[6], int trainIndex) {
	 
	Ticket buyTicket;
	char bookedSeat[27][3];// Each train only have 27 seat
	int seatArray = 0;

	do {
		//--- Put in the basic Ticket information
		strcpy(buyTicket.trainId, trainScheDb[trainIndex].trainId);
		strcpy(buyTicket.memberId, loggedMember.userDetails.id);
		buyTicket.price = PRICE_PER_KM * trainScheDb[trainIndex].km;

		clear();
		printf("[Buy Ticket]\n\n");
		warnZeroToReturn();

		//--- Initialize the seat is empty ---
		for (int i = 0; i < 27; i++) {
			strcpy(bookedSeat[i], "");
		}
		seatArray = 0;
		for (int i = 0; i < bookedDbSize; i++) {
			if (strcmp(bookedDb[i].trainId, trainId) == 0) {
				strcpy(bookedSeat[seatArray], bookedDb[i].seat);
				seatArray++;
			}
		}
		if (seatArray >= 27) {
			strcpy(trainScheDb[trainIndex].status, "Full");
			saveDatabase();
			showTrainWithIndex(trainIndex);
			pressEnter();
			break;
		}
		
		showTrainWithIndex(trainIndex);

		printf("\n\n[Existing Train Seat]\n\n");
		
		showTrainSeat(bookedSeat,2,NULL);

		color("purple"); printf("\n[Note] ");
		color("white");  printf("The price for [A4 - C9] is RM%.2f", buyTicket.price);

		color("purple"); printf("\n[Note] ");
		color("white");  printf("The price for [VIP] [A1 - C3] is RM%.2f\n", buyTicket.price*VIP_PRICE);

		color("purple"); printf("\n[Note] ");
		color("white");  printf("[X] is not yet booked.\n");

		color("purple"); printf("[Note] ");
		color("white");  printf("["); color("red"); printf("X");
		color("white");  printf("] is booked.\n\n");

		printf("Please select the seat you wanted to book [Eg. A1] : ");
		gets(buyTicket.seat);
		rewind(stdin);
		buyTicket.seat[0] = toupper(buyTicket.seat[0]);
		if (strCheckIsZero(buyTicket.seat)) { return; }
	
		buySeat(buyTicket, bookedSeat);
		
	} while (1);
}
void viewTicket(Member memTicketToView, int whoView) {

	Ticket ticketToView;

	clear();

	if (whoView == 1) {
		printf("[My Ticket]\n\n");
	}
	else {
		printf("[%s's Booked Ticket]\n\n", memTicketToView.userDetails.id);
	}

	drawTable(3, 1);
	for (int i = 0; i < bookedDbSize; i++) {
		if (strcmp(bookedDb[i].memberId, memTicketToView.userDetails.id)==0){
			if(bookedDb[i].outDated != 1){
				printf("|%-13s|%-12s|%-8s|%-13s|RM%-7.2f|%02d/%02d/%04d     |%02d:%02d          |\n",
					bookedDb[i].ticketId, bookedDb[i].trainId, bookedDb[i].seat, bookedDb[i].memberId, bookedDb[i].price,
					bookedDb[i].dateBooked.day, bookedDb[i].dateBooked.month, bookedDb[i].dateBooked.year,
					bookedDb[i].timeBooked.hours, bookedDb[i].timeBooked.minutes);
			}
		}
	}
	drawTable(3, 2);

	pressEnter();
}
void showAllTicket() {

	char input[10];
	int ticketIndex;

	do {
		clear();
		printf("[All Ticket]\n\n");
		warnZeroToReturn();
		drawTable(3, 1);
		for (int i = 0; i < bookedDbSize; i++) {
			printf("|%-13s|%-12s|%-8s|%-13s|RM%-7.2f|%02d/%02d/%04d     |%02d:%02d          |\n",
				bookedDb[i].ticketId, bookedDb[i].trainId, bookedDb[i].seat, bookedDb[i].memberId, bookedDb[i].price,
				bookedDb[i].dateBooked.day, bookedDb[i].dateBooked.month, bookedDb[i].dateBooked.year,
				bookedDb[i].timeBooked.hours, bookedDb[i].timeBooked.minutes);
		}
		drawTable(3, 2);
		printf("Enter Ticket ID to edit : ");
		gets(input);
		rewind(stdin);
		if (strCheckIsZero(input)) { return; }
		//Because of ticket ID is TK
		input[0] = toupper(input[0]); //Set 't' to 'T'
		input[1] = toupper(input[1]); //Set 'k' to 'K'

		ticketIndex = -1; // Reset the input default is invalid
		for (int i = 0; i < bookedDbSize; i++) {
			if (strcmp(bookedDb[i].ticketId, input) == 0) {
				ticketIndex = i;
				break;
			}
		}

		//--- Invalid ---
		if (ticketIndex == -1) {
			invalidInput();
		}
		else {
			clear();
			editTicket(ticketIndex);
		}

	} while (1);
}
void editTicket(int ticketIndex) {

	Member ticketOwner;
	int select;

	for (int i = 0; i < memDbSize; i++) {
		if (strcmp(membersDb[i].userDetails.id, bookedDb[ticketIndex].memberId) == 0) {
			ticketOwner = membersDb[i];
			break;
		}
	}

	do {
		clear();
		printf("[Edit Ticket]\n\n");
		drawTable(3, 1);
		printf("|%-13s|%-12s|%-8s|%-13s|RM%-7.2f|%02d/%02d/%04d     |%02d:%02d          |\n",
			bookedDb[ticketIndex].ticketId, bookedDb[ticketIndex].trainId, bookedDb[ticketIndex].seat, bookedDb[ticketIndex].memberId, bookedDb[ticketIndex].price,
			bookedDb[ticketIndex].dateBooked.day, bookedDb[ticketIndex].dateBooked.month, bookedDb[ticketIndex].dateBooked.year,
			bookedDb[ticketIndex].timeBooked.hours, bookedDb[ticketIndex].timeBooked.minutes);
		drawTable(3, 2);
		printf("[Edit]\n\n");
		printf("Please select : \n\n");
		printf("[0] Back\n");
		printf("[1] Change Seat\n");
		printf("[2] Refund Ticket\n\n");
		printf(">> ");
		scanf("%d", &select);
		rewind(stdin);

		switch (select) {
		case 0:
			break;
		case 1:
			changeSeat(ticketOwner, ticketIndex);
			break;
		case 2:
			refundTicket(ticketOwner, ticketIndex);
			select = 0;
			break;
		default:
			invalidInput();
		}
	} while (select != 0);
}
void changeSeat(Member ticketOwner, int ticketIndex) {

	char changedSeatInput[3];
	char bookedSeat[27][3];// Each train only have 27 seat
	int seatArray = 0;
	int invalidSeat = 0;
	int currentSeatVip, changedSeatVip;

	do {
		clear();
		printf("[Existing Ticket]\n");
		warnZeroToReturn();
		printf("\n");
		drawTable(3, 1);
		printf("|%-13s|%-12s|%-8s|%-13s|RM%-7.2f|%02d/%02d/%04d     |%02d:%02d          |\n",
			bookedDb[ticketIndex].ticketId, bookedDb[ticketIndex].trainId, bookedDb[ticketIndex].seat, bookedDb[ticketIndex].memberId, bookedDb[ticketIndex].price,
			bookedDb[ticketIndex].dateBooked.day, bookedDb[ticketIndex].dateBooked.month, bookedDb[ticketIndex].dateBooked.year,
			bookedDb[ticketIndex].timeBooked.hours, bookedDb[ticketIndex].timeBooked.minutes);
		drawTable(3, 2);

		printf("[Change Seat]\n\n");
		//--- Initialize the seat is empty ---
		for (int i = 0; i < 27; i++) {
			strcpy(bookedSeat[i], "");
		}
		seatArray = 0;
		for (int i = 0; i < bookedDbSize; i++) {
			if (strcmp(bookedDb[i].trainId, bookedDb[ticketIndex].trainId) == 0) {
				strcpy(bookedSeat[seatArray], bookedDb[i].seat);
				seatArray++;
			}
		}
		if (seatArray >= 27) {
			color("red");
			printf("[Sorry, all seats are booked. You can't change the seat of the ticket]");
			pressEnter();
			break;
		}
		
		showTrainSeat(bookedSeat, 1, bookedDb[ticketIndex].seat);

		color("purple"); printf("\n[NOTE] ");
		color("green");  printf("X "); 
		color("white");  printf("is the current seat\n");

		color("purple"); printf("[NOTE] ");
		color("red");    printf("X ");
		color("white");  printf("is booked seat\n");

		color("purple"); printf("[NOTE] ");
		color("white");  printf("X ");
		color("white");  printf("is the seat that available to change\n\n");

		printf("\nPlease select the seat that wanted to change to [Eg. A1] : ");
		gets(changedSeatInput);
		rewind(stdin);
		changedSeatInput[0] = toupper(changedSeatInput[0]);
		if (strCheckIsZero(changedSeatInput)) { return; }

		//Invalid seat will return 1
		//Valid will return 0
		invalidSeat = validateSeatInput(changedSeatInput);

		if (invalidSeat) {
			invalidInput();
		}
		else {
			//0 is valid
			//1 is booked
			invalidSeat = validateSeatBooked(bookedSeat, changedSeatInput);
			
			if (invalidSeat) {
				color("red");
				printf("[Sorry, you can't change to a booked seat.]");
				_sleep(PAUSE);
			}
			else {
				changedSeatVip = (changedSeatInput[1] > '0' && changedSeatInput[1] < '4') ? 1 : 0;
				currentSeatVip = (bookedDb[ticketIndex].seat[1] > '0' && bookedDb[ticketIndex].seat[1] < '4') ? 1 : 0;

				if (currentSeatVip == changedSeatVip) {
					//Vip change to Vip				OR
					//Economy change to Economy
					
					   ////////////////////////////////////////
					  ///No need to change the user balance///
					 ///No performance needed             ///
					////////////////////////////////////////
				}
				else if (changedSeatVip > currentSeatVip) {
					//Current seat not vip
					//Change to vip

					//Balance enough to change to VIP
					if (ticketOwner.balance >= (bookedDb[ticketIndex].price * VIP_PRICE) - (bookedDb[ticketIndex].price)) {
						ticketOwner.balance -= (bookedDb[ticketIndex].price * VIP_PRICE) - (bookedDb[ticketIndex].price);
						printf("\n[Charged RM%.2f from %s due to changed from an economy seat to a VIP seat]\n\n",
							(bookedDb[ticketIndex].price * VIP_PRICE) - (bookedDb[ticketIndex].price), ticketOwner.userDetails.id);
						bookedDb[ticketIndex].price = bookedDb[ticketIndex].price * VIP_PRICE;
					}
					//Balance not enough
					else {
						color("red");
						printf("[Sorry, %s balance is not enough for upgrading to a VIP seat]\n", ticketOwner.userDetails.id);
						_sleep(PAUSE);
						continue;
					}
				}
				else {
					//Current seat vip
					//Change to economy
					ticketOwner.balance += (bookedDb[ticketIndex].price) - (bookedDb[ticketIndex].price / VIP_PRICE);
					printf("\n[Refunded RM%.2f to %s due to changed from a VIP seat to an economy seat]\n\n", 
						    (bookedDb[ticketIndex].price) - (bookedDb[ticketIndex].price / VIP_PRICE), ticketOwner.userDetails.id );
					bookedDb[ticketIndex].price = bookedDb[ticketIndex].price / VIP_PRICE;
				}
				updateMember(ticketOwner);

				color("green");
				printf("[You successfully changed the seat from %s to %s]", bookedDb[ticketIndex].seat, changedSeatInput);
				strcpy(bookedDb[ticketIndex].seat, changedSeatInput);
				saveDatabase();
				pressEnter();
				break;
			}
		}

	} while (1);
}
void refundTicket(Member ticketOwner, int ticketIndex) {

	char yesNo;

	printf("\nAre you sure you want to refund existing ticket [Y/N] ?\n");
	printf(">> ");
	scanf("%c", &yesNo);
	rewind(stdin);
	yesNo = toupper(yesNo);

	if (yesNo == 'Y') {
		ticketOwner.balance += bookedDb[ticketIndex].price;
		updateMember(ticketOwner);
		printf("\n[RM%.2f refunded back to %s account]\n", bookedDb[ticketIndex].price, ticketOwner.userDetails.id);
		deleteTicket(ticketIndex);
		color("green");
		printf("[Successfully refund existing ticket]");
		pressEnter();
	}
	else {
		color("red");
		printf("[Failed to refund existing ticket]");
		_sleep(PAUSE);
	}
}
void deleteTicket(int ticketIndex){

	for (int i = ticketIndex; i < bookedDbSize; i++) {
		bookedDb[i] = bookedDb[i + 1];
	}
	bookedDbSize--;
	saveDatabase();

}
void refreshTicket() {

	for (int i = 0; i < bookedDbSize; i++) {
		for (int y = 0; y < outDatedTrainSize; y++) {
			if (strcmp(bookedDb[i].trainId, outDatedTrain[y].trainId) == 0) {
				bookedDb[i].outDated = 1;
				break;
			}
		}
	}
}
void refreshTrainSche(){
	
	int refresh;

	GetLocalTime(&globalCurrentTime);
	for (int i = 0; i < trainScheDbSize; i++) {
		refresh = 0;
		if (trainScheDb[i].date.year < globalCurrentTime.wYear)
			refresh = 1;
		else if (trainScheDb[i].date.year == globalCurrentTime.wYear &&
			(trainScheDb[i].date.month < globalCurrentTime.wMonth))
			refresh = 1;
		else if (trainScheDb[i].date.year == globalCurrentTime.wYear &&
			(trainScheDb[i].date.month == globalCurrentTime.wMonth) &&
			(trainScheDb[i].date.day < globalCurrentTime.wDay))
			refresh = 1;
		else if (trainScheDb[i].date.year == globalCurrentTime.wYear &&
			(trainScheDb[i].date.month == globalCurrentTime.wMonth) &&
			(trainScheDb[i].date.day == globalCurrentTime.wDay) &&
			(trainScheDb[i].departTime.hours <= globalCurrentTime.wHour) &&
			(trainScheDb[i].departTime.minutes <= globalCurrentTime.wMinute))
			refresh = 1;

		if (refresh) {
			strcpy(trainScheDb[i].status, "Unavailable");
			outDatedTrain[outDatedTrainSize] = trainScheDb[i];
			outDatedTrainSize++;
		}
	}

	refreshTicket();
	saveDatabase();
}
