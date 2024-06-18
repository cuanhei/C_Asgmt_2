
void intro() {
	
   char line1[] = " ___________    ____________   _____________ ",
		line2[] = "|   ______  \\   |   ________|  |_____  _____|",
		line3[] = "|  |      |  |  |   |_______        |  |     ",
		line4[] = "|  |      |  |  |    _______|       |  |     ",
		line5[] = "|  |      |  |  |   |               |  |     ",
		line6[] = "|  |______|  |  |   |               |  |     ",
		line7[] = "|___________/   |___|               |__|     ",
		line8[] = "---------------------------------------------";

   char text[] = "Welcome to A-Plus Train Ticketing System!";
   
	printf("%s\n", line7); _sleep(100); system("cls");
	printf("%s\n%s\n", line6, line7); _sleep(100); system("cls");
	printf("%s\n%s\n%s\n", line5, line6, line7); _sleep(100); system("cls");
	printf("%s\n%s\n%s\n%s\n", line4, line5, line6, line7); _sleep(100); system("cls");
	printf("%s\n%s\n%s\n%s\n%s\n", line3, line4, line5, line6, line7); _sleep(100); system("cls");
	printf("%s\n%s\n%s\n%s\n%s\n%s\n", line2, line3, line4, line5, line6, line7); _sleep(100); system("cls");
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n", line1, line2, line3, line4, line5, line6, line7); _sleep(400); system("cls");
	printf("%s\n%s\n%s\n%s\n\n%s\n%s\n%s\n", line2, line3, line4, line8, line8, line5, line6); _sleep(200); system("cls");
	printf("%s\n%s\n%s\n\n\n%s\n%s\n%s\n", line3, line4, line8, line8, line5, line6); _sleep(200); system("cls");
	printf("%s\n%s\n\n\n\n\n%s\n%s\n", line4, line8, line8, line5); _sleep(200); system("cls");
	printf("%s\n\n\n\n\n\n\n%s\n", line8, line8); _sleep(200); system("cls");

/*
	      _____   ___    ___   _  __   ___   _____          ___  __   __  ___   _____   ___  __  __
	o O O|_   _| |_ _|  / __| | |/ /  | __| |_   _|  ___   / __| \ \ / / / __| |_   _| | __||  \/  |
   o       | |    | |  | (__  | ' <   | _|    | |   |___|  \__ \  \ V /  \__ \   | |   | _| | |\/| |
  TS__[O] _|_|_  |___|  \___| |_|\_\  |___|  _|_|_  ____   |___/  _|_|_  |___/  _|_|_  |___||_|__|_|
 {======_|"""""_|"""""_|"""""_|"""""_|"""""_|"""""_|""""|_|"""""_| """ _|"""""_|"""""_|"""""_|"""""|
./o--000"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-"`-0-0-'

*/
	printf("\n\n           ___            ___    _\n    o O O /   \\   ___    | _ \\  | |   _  _    ___\n   o      | - |  |___|   |  _/  | |  | +| |  (_-<\n  TS__[O] |_|_|  _____  _|_|_  _|_|_  \\_,_|  /__/_\n {======_|\"\"\"\"\"_|     _| \"\"\" _|\"\"\"\"\"_|\"\"\"\"\"_|\"\"\"\"\"|\n./o--000\"`-0-0-\"`-0-0-\"`-0-0-\"`-0-0-\"`-0-0-\"`-0-0-'\n\n\n");

	for (int i = 0; i < strlen(text); i++) {
		printf("%c", text[i]);
		_sleep(50);
	}
}
