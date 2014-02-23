
// Anagram.cpp
// written by M. Hoskins "Hashish Innocently Amok" (no1453@gmail.com)
// last revision: 2013/12/29

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct wlelm {  // world list element
	std::string word;
	wlelm *next;
	wlelm *prev;
};

struct wlist {  // word list
	wlelm *head;
	wlelm *tail;
};

int numberOf=-1, numberAnagram=0;

int myfind(std::string &searchst, char letter) {  // find position of char 'letter' in string 'searchst'
	unsigned int i;

	for (i=0; i<searchst.length(); i++)
		if ( searchst[i]==letter )
			return i;
	return -1;
}

char tolower(char c) { // if  char 'c' is uppercase, change it to lowercase
	if (c>='A' && c<='Z')
		return (char) c-'A'+'a';
	else
		return c;
}

void downcase(std::string &str) // converrt string 'str' to all lowercase
{
	const int length = str.length();
	for(int i=0; i < length; ++i)
	{
		str[i] = tolower(str[i]);
	}
}

// The following six functions are for working with word lists
void wlappend(wlist &list, std::string x) {
	if (list.head==NULL) {
		list.head=list.tail=new wlelm;
		list.head->prev=NULL;
		list.head->next=NULL;
	}
	else {
		list.tail->next=new wlelm;
		list.tail->next->prev=list.tail;
		list.tail->next->next=NULL;
		list.tail=list.tail->next;
	}
	list.tail->word=x;
}

void wlerase(wlist &l) {
	wlelm *temp;

	while(l.tail!=NULL) {
		temp=l.tail;
		l.tail=l.tail->prev;
		delete temp;
	}
	l.head=NULL;
}

bool wlempty(wlist l) { return l.head==NULL; }

void wlrecsort(wlist &l, wlelm *current) {
	wlelm *sorting,*find,*nextsort=NULL;
	bool curtail, changelist=false;

	if (current==NULL)
		return;
	if (l.head==l.tail)
		return;
	if (current==l.head)
		current=current->next;
	curtail=(current==l.tail);

	if (current->word.length()>current->prev->word.length()) {
		changelist=true;
		nextsort=current->next;
		sorting=current;
		sorting->prev->next=sorting->next;
		if (!curtail)
			sorting->next->prev=sorting->prev;
		else
			l.tail=sorting->prev;

		find=sorting->prev;
		while( ((find==l.head) ? false : find->prev->word.length()<sorting->word.length()))
			find=find->prev;

		if (find==l.head) {
			l.head->prev=sorting;
			sorting->next=l.head;
			l.head=sorting;
		}
		else {
			find->prev->next=sorting;
			sorting->prev=find->prev;
			sorting->next=find;
			find->prev=sorting;
		}
		if (curtail)
			return;
	}
	if (changelist)
		current=nextsort;
	else
		current=current->next;
	wlrecsort(l, current);
}

void wlsort (wlist &l) {
	wlelm *current;

	if (wlempty(l))
		return;
	current=l.head->next;
	wlrecsort(l, current);
}

void wlprint(wlist l) {
	wlelm *current;
	int num=0;

	current=l.head;
	cout << "BEGIN wl---";
	cout << "Word List--------" ;
	while(current!=NULL) {
		cout << current->word << " ";
		num++;
		current=current->next;
	}
	cout << num << "---END wl" << endl;
}

void remove(std::string &x, int startpos, int count) {
	std::string n="";
	int i,l;

	l=x.length();
	if (startpos>=0 && startpos+count<=l) {
		for(i=0;i<startpos;i++)
			n+=x[i];
		for(i=startpos+count; i<l; i++)
			n+=x[i];
		x=n;
	}
}

bool issubstr(std::string word, std::string phrase, std::string &left) {  // boolean result of "Is 'word'  found in 'phrase'?"
	int pos;
	unsigned int lenw;
	std::string temp,letter,nleft;
	char let;

	lenw=word.length();
	if (lenw>phrase.length())
		return false;

	temp=word;
	left=phrase;
	while (lenw--) {
		let=temp[0];
		pos=myfind(left, let);
		if (pos<0)
			return false;
		remove(temp,0,1);
		remove(left,pos,1);
	}
	return true;
}

void allchar(std::string &x) { // remove anything but lowercase letters in string 'x'
	std::string n="";
	int lenw=x.length(),i;

	for (i=0; i<lenw; i++)
		if (x[i]>='a' && x[i]<='z')
			n+=x[i];
	x=n;
}

void anagram(std::string bstr, wlist list, std::string anstr) { // main recursive workhorse function
	wlelm *curword, *ncword;
	std::string newanstr, left;
	wlist nlist;

	nlist.head=NULL;
	nlist.tail=NULL;
	curword=list.head;
	while(curword!=NULL) {
		issubstr(curword->word,anstr,newanstr);
		if (newanstr=="") {
			cout << bstr << curword->word << endl;
			if (++numberAnagram==numberOf)
				exit(0);
		}
		else {
			if (issubstr(curword->word, newanstr, left))
				ncword=curword;
			else
				ncword=curword->next;

			while(ncword!=NULL) {
				if (issubstr(ncword->word, newanstr, left)) {
					if (left=="") {
						cout << bstr+curword->word+" " << ncword->word << endl;
						if (++numberAnagram==numberOf)
							exit(0);
					}
					else
						wlappend(nlist, ncword->word);
				}
				ncword=ncword->next;
			}
			if (nlist.head!=NULL) {
				anagram(bstr+curword->word+" ",nlist, newanstr);
				wlerase(nlist);
			}
		}
		curword=curword->next;
	}
}

int main(int argc, char *argv[]) {
	ifstream dict;

	std::string dictword, anstr, temp, left, required, throwaway;
	std::string exename, comarg1, comarg2, comarg3;
	wlist list;
	char * pEnd;

	exename=argv[0];

	if (argc>1) {
		comarg1=argv[1];
		anstr=comarg1;
	}
	else {
		cerr << exename << ": Must supply string to be anagrammed.  Exiting..." << endl;
		return -1;
	}
	if (argc>2) {
		comarg2=argv[2];
		required=comarg2;
		if (required=="-")
			required="";
	}
	else
		required="";
	if (argc>3) {
		comarg3=argv[3];
		numberOf=strtol(argv[3], &pEnd, 10);
		if (numberOf<1) {
			cerr << exename << ": error, third argument must be a positive integer.  Exiting..." << endl;
			return -1;
		}
	}

	list.head=NULL;
	list.tail=NULL;

	dict.open("/usr/dict/anagramwords"); // A list of english words, one per line
	if (dict.fail()) {
		cerr << exename << ": error: cant open /usr/dict/anagramwords.  Exiting..." << endl;
		exit(-1);
	}

	downcase(anstr);
	downcase(required);
	allchar(anstr);
	allchar(required);

	if (required.length()>0) {
		if (issubstr(required,anstr,temp))
			anstr=temp;
		else {
			cerr << exename << ": error: '" << required << "' isn't in the string to be anagrammed.  Exiting...." << endl;
			exit(-1);
		}
	}

	dict >> dictword;
	allchar(dictword);
	while (!dict.eof()) {
		downcase(dictword);
		if (issubstr(dictword, anstr, left))
			wlappend(list, dictword);
		dict >> dictword;
		allchar(dictword);
	}
	dict.close();

	wlsort(list);
	//  wlprint(list);
	if (required!="")
		anagram(required+" ", list, anstr);
	else
		anagram("", list, anstr);
	exit(0);
}



