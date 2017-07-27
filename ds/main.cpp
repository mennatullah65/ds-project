#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<windows.h>
#include<stdlib.h>
using namespace std;

void gotoxy(int x, int y){
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
BOOL SetConsoleFontSize(COORD dwFontSize){
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	if (!GetCurrentConsoleFontEx(output, false, &info))
		return false;
	info.dwFontSize = dwFontSize;
	return SetCurrentConsoleFontEx(output, false, &info);
}
/*
GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, console_info);
console_info.dwFontSize.X = 9;
console_info.dwFontSize.Y = 9;
SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, console_info);
*/
/*
HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_FONT_INFOEX font;
GetCurrentConsoleFontEx(outcon, false, &font);
font.dwFontSize.X = 12;
font.dwFontSize.Y = 16;
SetCurrentConsoleFontEx(outcon, false, &font);
*/
typedef struct console_info {
			ULONG cbSize;
			DWORD nFont;
			COORD dwFontSize;
			UINT  FontFamily;
			UINT  FontWeight;
			WCHAR FaceName[LF_FACESIZE];
		} console_info;


class Dictionary{
private:
	fstream dic;  
	int max_space;
	map<string, string>verb;
	map<string, string>noun;
	map<string, string>adjective;
	map<string, string>adverb;
	long long num_of_words;
	long long num_of_nouns;
	long long num_of_verbs;
	long long num_of_adjectives;
	long long num_of_adverbs;
public:
	Dictionary(){
		num_of_words = num_of_nouns = num_of_verbs = num_of_adjectives = max_space= 0;
	}
	//Read
	bool Read_from_file(){
		dic.open("dic.txt");
		if (dic.is_open()){
			string word; 
			while (getline(dic, word)){
				word[0] = toupper(word[0]);
				for (int i = 0; i < word.size(); i++){
					if (word[i] == ' '){
						if (word[i + 1] == 'n' ){
							noun[word.substr(0, i)] = word.substr(i + 3, word.size() - 1);
							num_of_nouns++;
						}
						else if (word[i + 1] == 'v' ){
							verb[word.substr(0, i)] = word.substr(i + 3, word.size() - 1);
							num_of_verbs++;
						}
						else if (word[i + 1] == 'a' &&word[i + 2] == 'd'&&word[i + 3] == 'j'){
							adjective[word.substr(0, i)] = word.substr(i + 5, word.size() - 1);
							num_of_adjectives++;
						}
						else if (word[i + 1] == 'a' &&word[i + 2] == 'd'&&word[i + 3] == 'v'){
							adverb[word.substr(0, i)] = word.substr(i + 5, word.size() - 1);
							num_of_adverbs++;
						}
						if (i > max_space){ max_space = i; }
						break;
					}
				}
				num_of_words = num_of_nouns + num_of_verbs + num_of_adjectives + num_of_adverbs;
			}
			dic.close();
			return true;
		}
		cout << "Can't open the file !" << endl;
		return false;
	}
	//View
	void View_dic(){
		cout << "The nouns : " << endl;
		for (map< string, string>::iterator i = noun.begin(); i != noun.end(); i++){
			cout << (*i).first; for (int space = (*i).first.size(); space < max_space; space++)cout << " "; cout << ":   " << (*i).second << endl;
			Sleep(5000);
		}
		cout << "The verbs : " << endl;
		for (map< string, string>::iterator i = verb.begin(); i != verb.end(); i++){
			cout << (*i).first << "   :   " << (*i).second << endl;
			Sleep(5000);
		}
		cout << "The adjectives : " << endl;
		for (map< string, string>::iterator i = adjective.begin(); i != adjective.end(); i++){
			cout << (*i).first << "   :   " << (*i).second << endl;
			Sleep(5000);
		}
	}
	//Search
	void search(map< string, string>m, bool&word_excist, string word,string type){
		if (!m[word].empty()){
			cout << word << " is " << type << " and its def. is:  " << m[word] << endl;
			word_excist = true;
		}
	}
	void searchFile()
	{
		string word;
		cout << "Enter the word you want to search for: "; cin >> word;
		bool word_excist = false;
		int begin, end; 
		for (begin=0; begin < word.size() && !isalpha(word[begin]); begin++); 
		for (end = word.size()-1; end >=0 && !isalpha(word[end]); end--);
		word = word.substr(begin, end + 1);    word[0] = toupper(word[0]);
		//Search in nouns
		search(noun, word_excist, word,"a noun");
		if (word_excist == false){
		//Search in verbs
			search(verb, word_excist, word, "a verb");
			if (word_excist == false){
		//Search in adjectives
				search(adjective, word_excist, word, "an adjective");
				if (word_excist == false){
	    //Search in adverbs
					search(adverb, word_excist, word, "an adverb");
                    if (word_excist == false){ cout << "This definition does not exist." << endl; }
				}
			}
		}
	}
	//Edit
	void Edit(){
		int type; cout << "Choose the type of the word you want to edit \n ( 1: Noun | 2: Verb | 3: Adjective | 4: Adverb  ): \n "; cin >> type;
		string word; cout << "Enter the word you want to edit : "; cin >> word;
		int begin, end;
		for (begin = 0; begin < word.size() && !isalpha(word[begin]); begin++);
		for (end = word.size() - 1; end >= 0 && !isalpha(word[end]); end--);
		word = word.substr(begin, end + 1);
		string definition; cout << "Enter the definition : "; cin >> definition;
		dic.open("dic.txt", ios::out | ios::app);
		string line; bool word_excist=false;
		while (getline(dic, line)){
			for (int i = 0; i < line.size(); i++){
				if (line[i] == ' '){
					cout << line.substr(0, i);
					if (word == line.substr(0, i)){
						word_excist = true;
						

						dic << line.substr(0, i);
						if (type == 1){ dic << " n. "; }else if (type == 2){ dic << " v. "; }else if (type == 3){ dic << " adj. "; }else if (type == 4){ dic << " adv. "; }
						dic << definition << endl;
					}
				}
			}
		}

		if (word_excist == false){cout << "The word doesn't excist !" << endl;}
	}
	void Add(){
		int type; cout << "Choose the type of the word you want to edit \n ( 1: Noun | 2: Verb | 3: Adjective | 4: Adverb  ): \n "; cin >> type;
		string word; cout << "Enter the word you want to edit : "; cin >> word;
		int begin, end;
		for (begin = 0; begin < word.size() && !isalpha(word[begin]); begin++);
		for (end = word.size() - 1; end >= 0 && !isalpha(word[end]); end--);
		word = word.substr(begin, end + 1);
		string definition; cout << "Enter the definition : "; cin >> definition;

		
		
	}
};
void Project(){
	system("mode 270,250");
	Dictionary d;
	d.Read_from_file();
	d.searchFile();
	d.View_dic();
	
}

int main(){

	Project();

return 0;}
