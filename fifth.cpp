/*

c++ code to automatically invite players to a game of league of legends						written by Patrick Stewart

before running this code, you should already have a .txt file containing a list
of valid league of legends summoner names, separated by newlines. This can be obtained
by running the spiderPull python script, which will automatically generate a properly 
formatted list of valid summoner names. 

*/

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <io.h>

using namespace std;

void LeftClick();
void MouseMove(int x, int y);
void keypress(char c);
void typeWord(string s, int e_flag = 1);
void invitePlayer(string summonerName);
void deleteWord(int n);
int endsWith(string s, string suffix);
void LeftClick(POINT p);
void accept_inputs();
POINT get_mouse_pos();
void emptyInvList();
void getFilePath();

//x,y locations on screen
POINT invite1, //the button to add a player to the invite list
	  invite2, //the button to empty the invite list and invite all players
	  textbox, //the textbox used to type player names into the client
	  errorbox;//the button to close the error box

string currentName = "", line = "";
char tempChar;
int playersInList = 0;
string filepath = "";
ifstream myfile;

int main()

{
	getFilePath();
	accept_inputs();
	_chmod(filepath.c_str(), _S_IREAD | _S_IWRITE);  //files created by the spiderPull python script 													
												     //need their permissions modified to be read.

	while (getline(myfile, currentName) && GetKeyState(VK_TAB) == 0)
	{
		Sleep(1500);
		invitePlayer(currentName);
		++playersInList;
			
		//a call to invitePlayer adds a player to the invite list. Once the invite list fills up, calling LeftClick(invite2)
		//will empty the invite list and invite all players on it to the game. When this button is clicked it can cause
		//several stacking error boxes to appear in the location of errorbox. It can take multiple calls to LeftClick(errorbox)
		//to get rid of all of the error messages. All of this is handled by emptyInvList
		if (playersInList >= 13)					
		{
			emptyInvList();
		}
	
	}

	myfile.close();
	
	return 0;
}

void getFilePath() //sets global variables filepath and myfile, a string and an ifstream respectively
{
	do
	{
		cout << "Enter name or path of file containing summoner names\n(Enter 'SampleNames' to use default name list) -> ";
		getline(cin, filepath);
		if (!endsWith(filepath, ".txt"))
		{
			cout << "file must be of type '.txt', assuming " << filepath << ".txt" << endl;
			filepath.append(".txt");
		}
		cout << "file path: " << filepath << endl;
		myfile.open(filepath, ios_base::in);
		if (myfile)
		{
			cout << "\nsuccessfully opened " << filepath << " for reading." << endl;
		}
		else 
		{
			cout << "\nfailed to open " << filepath << " for reading." << endl;
	    }
	} while (!myfile); //file validation
	return;
}
void emptyInvList()
{
	LeftClick(invite2);
	playersInList = 0;
	Sleep(100);
	LeftClick(errorbox);
	LeftClick(errorbox);
	LeftClick(errorbox);
}
void accept_inputs()
{
	cout<< "Open your game client to a ranked game lobby and wait for the tone.\n"
		<< "Please click once each time you hear the tone:\n"
		<< "1. click the invite players button\n"
		<< "2. click the right-most quarter of the text box\n"
		<< "3. click the 'OK' button in the error window\n"
		<< "4. click the right-most quarter of the text box\n"
		<< "5. click the invite players button\n\n"
		<< "The autoInviter will then begin to invite players from the input file. Press TAB at any time to exit." << endl;


	system("pause");//give the user time to open client

	Sleep(1500); //Sleeps make the input process much smoother
	Beep(300, 500);
	invite1 = get_mouse_pos();

	Sleep(500);
	Beep(300, 500);
	textbox = get_mouse_pos();

	typeWord("legacyVG"); //intentionally invalid summoner name, makes client display error box

	Sleep(500);
	Beep(300, 500);//so that user can input where the button to close it is
	errorbox = get_mouse_pos();

	Sleep(500);

	LeftClick(textbox);
	deleteWord(8);
	typeWord("ricky");	  //valid summoner name, adding a name to the invite list allows the second invite button to be pressed.

	Beep(300, 500);
	invite2 = get_mouse_pos();

	Sleep(200);
}

int endsWith(string s, string suffix) //random function to check if the provided file path ends with ".txt"
{
	int i = s.length() - suffix.length(); //jump to where beginning of suffix should be in s.
	int j = 0;
	while (i < s.length())
	{
		if (s[i] != suffix[j])
		{
			return 0;
		}
		++i;
		++j;
	}

	return 1;
}

void LeftClick() //send input to simulate a left click
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

//simulate moving the mouse to an x,y position on the screen
void MouseMove(int x, int y)
{
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = x*(65535.0f / fScreenWidth);
	double fy = y*(65535.0f / fScreenHeight);
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}

void LeftClick(POINT p) //LeftClick() overloaded to accept a location on the screen to click
{
	MouseMove(p.x, p.y);
	LeftClick();
}

POINT get_mouse_pos()
{
	POINT p;

	cout << "Please enter input now" << endl;

	while ((GetKeyState(VK_LBUTTON) & 0x100) == 0)
	{
	}
	GetCursorPos(&p);

	return p;
}

WORD keyMap(char c)
{
	WORD keys[] = { VK_RETURN, VK_SPACE, VK_BACK, 0X30, 0X31, 0X32, 0X33, 0X34, 0X35, 0X36, 0X37, 0X38, 0X39, 0x41, 0x42, 0x43, 0x44, 0x45,
		0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A };
	//keys is an array of key code WORDS for every key [0-9] and [A-Z], with keys[0] corresponding to "Enter" key. See link at end of function.

	WORD vkey = 0x30;

	if ((int)c >= 48 && (int)c <= 57) //char is [0-9]
	{
		vkey = keys[((int)c) - 45];
	}
	else if (c == ' ')
	{
		vkey = keys[1];
	}
	else if (c == '<')
	{
		vkey = keys[2]; // < is the signal for backspace
	}
	else if (c == '!')
	{
		vkey = keys[0]; // ! is the signal for enter
	}
	else
	{
		vkey = keys[((int)c) - 52];
	}

	return vkey;

	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx list of keycodes

}

void keypress(char c)
{
	WORD vkey = keyMap(c); //map char to corresponding key code
	INPUT input;

	input.type = INPUT_KEYBOARD;
	input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = vkey;
	input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));

}

void typeWord(string s, int e_flag)
{

	for (int i = 0; i < s.length(); i++)
	{
		keypress(toupper(s[i]));
	}

	if (e_flag == 1) //e flag is should we press enter afterwards, yes by default
	{
		keypress('!');
	}

	//cout << e_flag;

}

//invitePlayer function does a lot of the work, the steps are:
// 1. click on the invite button
// 2. click on the text box where a name is to be entered
// 3. type in the name of the summoner to be invited, typeWord includes an 'enter' keypress by default.
// 4. there is always a chance of a summoner name being invalid (e.g. if the players account has been deleted).
// When you try to invite an invalid name, an error box pops up. It is a simple fix to click on the
// button to close the error box every time a player is invited.
// 5. after inviting the player, their name still remains typed into the text box. a call to deleteWord clears the text box for the
// next name.
void invitePlayer(string summonerName)
{
	LeftClick(invite1);			      //step 1
	LeftClick(textbox);               //step 2
	typeWord(summonerName);           //step 3
	LeftClick(errorbox);              //step 4

	LeftClick(textbox);	              //step 5	  
	deleteWord(summonerName.length());
}

void deleteWord(int n) //deletes a word of length n from a text box, used to clear player names after inviting them
{
	for (int i = 0; i < n; i++)
	{
		keypress('<');
	}
}
