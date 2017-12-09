// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Music
{
	string AlbumName;
	string Artist;
	string Rating;
	string SongName;

public:
	Music();
	Music(string Title, string Name, string AName, string num);
	void CaptureMusic();
	void ShowMusic();
	int SaveMusic(ofstream &outfile);
};

class MusicList
{
	Music **list;
	int numrecords;
	int listsize;
	int ReallocateArray();

public:
	MusicList();
	~MusicList();
	void UserInput();
	void ShowMusicList();
	int SaveMusicList(string filename);
	int ReadMusicList(string filename);
};

Music::Music()
{
	SongName = "";
	Artist = "";
	AlbumName = "";
	Rating = "";
}

Music::Music(string Title, string Name, string AName, string num)
{
	SongName = Title;
	Artist = Name;
	AlbumName = AName;
	Rating = num;
}

void Music::CaptureMusic()
{
	cout << "Enter the Song Name>";
	getline(cin, SongName);
	cout << "Enter the Artist(s) Name";
	getline(cin, Artist);
	cout << "Enter the Album Name";
	getline(cin, AlbumName);
	cout << "Enter the Rating";
	getline(cin, Rating);
	cin.ignore();
	cin.clear();
}

void Music::ShowMusic()
{
	cout << "Favorite Song: " << SongName << endl;
	cout << "Artist: " << Artist << endl;
	cout << "Album: " << AlbumName << endl;
	cout << "Rating: " << Rating << endl;
}

int Music::SaveMusic(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << SongName << "|" << Artist << "|" << AlbumName << "|" << Rating << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

MusicList::MusicList()
{
	list = new Music*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}

MusicList::~MusicList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}

int MusicList::ReallocateArray()
{
	Music **temp;
	temp = new Music*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

void MusicList::UserInput()
{
	string answer = "Y";
	cout << "Entering New Music? Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new Music();
		list[numrecords]->CaptureMusic();
		numrecords++;
		cout << "Are you entering another song? Y/N?" << endl;
		getline(cin, answer);
	}
}

void MusicList::ShowMusicList()
{
	for (int i = 0; i < numrecords; i++)
		list[i]->ShowMusic();
}

int MusicList::SaveMusicList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveMusic(output);
		}
		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}


int MusicList::ReadMusicList(string filename)
{
	string iTitle, iName, iAName, inum;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "The file could not be accessed" << endl;
		return READERROR;
	}
	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, inum, '|');
		if (!inum.empty())
		{
			getline(infile, iTitle, '|');
			getline(infile, iName, '|');
			getline(infile, iAName, '|');
			getline(infile, inum);
			list[numrecords] = new Music(iTitle, iName, iAName, inum);
			numrecords++;
		}
	}
	infile.close();
	return 0;
}

int main()
{
	string filename;
	cout << "Enter the full filepath name" << endl;
	getline(cin, filename);

	MusicList myMusic;
	string answer;
	int error;
	error = myMusic.ReadMusicList(filename);
	if (error)
	{
		cout << "Cannot read - contnue with creating a new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	myMusic.UserInput();
	myMusic.SaveMusicList(filename);
	myMusic.ShowMusicList();

	return 0;

}