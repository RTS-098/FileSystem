 #include <iostream>
#include <string>
#include<vector>
using namespace std;

struct Metdata
{
	int number;
	double size;
	string type;
	string dateaccessed;
	string datemodified;
	string datelastchange;
	string data;
	int links;
	Metdata()
	{
		number = 0;
		size = 0;
		type = "";// folder or file
		dateaccessed = "";
		datemodified = "";
		datelastchange = "";
		links = 0;
		data = "empty";
	}
	Metdata(const int num, const double siz, const string typ, const string datea, const double datem, const double datel, const int link, const string _data)
	{
		number = num;
		size = siz;
		type = typ;
		dateaccessed = datea;
		datemodified = datem;
		datelastchange = datel;
		links = link;
		data = _data;
	}
};

string extractstring(string input, int index) //задава наклонена черта преди команда
{
	string newstr = "";
	size_t size = input.size();
	int i = index;
	while (input[i] == ' ' || input[i] == '/')

		//while (input[i] == '/')
	{
		i++;
	}
	while (input[i] != '/' && input[i] != ' ' && i < size)
	{
		newstr = newstr + input[i];
		i++;

	}
	return newstr;
}

class File  //В клас файл създаваме дърво
 {
private:
	string name;
	Metdata data;
	vector<File*> child; //създаваме векторът child, който представлява клоните на дървото
	File* parent;
public:

	File()
	{
		name = "";
		child = vector<File*>(0);
		parent = NULL;
	}
	File(const string nam, Metdata dat, vector<File*> children, File* prt = NULL)
	{
		name = nam;
		data = dat;
		child = children;
		parent = prt;

	}
	File& operator=(File const& files)
	{
		if (this != &files)
		{
			name = files.name;
			data = files.data;
			child = files.child;

		}
		return *this;
	}
	~File()
	{
		delete[] this;
	}

	int biggestnum(int max, File* fsys, string type) //тази ф-ия намира родителската папка и проверява колко файла/папки има вътре
	{
		if (fsys == NULL)
		{
			return max;
		}
		if (max < fsys->data.number && fsys->data.type == type)
		{
			max = fsys->data.number;
		}
		int temp = 0;

		for (int i = 0; i < fsys->child.size(); i++)
		{

			temp = biggestnum(max, fsys->child[i], type);
			if (temp > max)
			{
				max = temp;
			}
		}



		return max;
	}

	File* getParent() //указателел към файл, който ни връща една позиция назад
	{
		return this->parent;
	}

	string getData()
	{
		return this->data.data;
	}


	void setNumber(int _data) {
		this->data.number = _data;
	}

	string getType()
	{
		return this->data.type;
	}

	void setType(string _type)
	{
		this->data.type = _type;
	}

	void setChild(string name, int index, string type) // проверява дали всички метаданни са променени правилно
	{
		File* root = new File;
		root = this;
		while (root->parent != NULL)
		{
			root = root->parent;
		}
		if (this->child.size() <= index)
			this->child.resize(index + 1);
		File* temp = new File;
		temp->name = name;
		temp->data.number = biggestnum(0, root, type) + 1;
		temp->parent = this;
		this->child[index] = temp;
		this->child[index]->parent = this;
		this->child[index]->data.links = 1;
		if (this->child[index]->parent != NULL)
		{
			this->child[index]->parent->data.links++;
		}
		this->child[index]->data.type = type;
	}

	File* getChild(int index)
	{
		return this->child[index];
	}

	void setName(string name)
	{
		this->name = name;
	}

	string getName()
	{
		return this->name;
	}

	int getChildSize()
	{
		return this->child.size();
	}


	void printChildren()
	{
		for (int i = 0; i < this->child.size(); i++)
		{
			cout << this->child[i]->getName() << ' ';
		}

	}

	void removeChild(int index)
	{
		int sizee = this->child.size();

		this->child[index] = NULL;
		this->child.resize(sizee - 1);
	}

	void printMetData()
	{
		cout << "Poreden nomer: " << this->data.number << endl;
		cout << "Razmer: " << this->data.size << endl;
		cout << "Tip: " << this->data.type << endl;
		cout << "Posleden dostyp: " << this->data.dateaccessed << endl;
		cout << "Poslenda promqna na sydyrjanieto:" << this->data.datemodified << endl;
		cout << "Poslenda promqna na metadannite: " << this->data.datelastchange << endl;
		cout << "Tvyrdi vryzki: " << this->data.links << endl;
		cout << "Sydyrjanie " << this->data.data << endl;
	}

	bool checkforchildren() //ф-ия с която проверяваме дали има листа
	{
		if (this->child[0] == NULL)
			return true;
		else
			return false;

	}

	void childreplace(int index1, int index2)
	{
		this->child[index1] = this->child[index2];

	}

	void resizechildren(int sizze)
	{
		this->child.resize(sizze);
	}

	void setData(string _data)
	{
		this->data.data = _data;
	}
};

File* searchforfile(string name, File* fsys)//корен на директорията за абсолютния път, случайна позиция за текущия път
{

	if (fsys->getName() == name)
	{
		return fsys;
	}
	File* temp = new File;
	for (int i = 0; i < fsys->getChildSize(); i++)  //използваме рекурсия за постоянното обхождане за намиране на файла
	{

		temp = searchforfile(name, fsys->getChild(i));
		if (temp->getName() == name)
		{
			break;
		}

	}
	return temp;

}

string reversal(string normal)
{
	int index = 0;
	string reverse;
	while (index < normal.size())
	{
		reverse = '/' + extractstring(normal, index) + reverse;
		index++;
		while (normal[index] != '/' && index < normal.size())
		{
			index++;
		}
	}
	return reverse;
}

void removeFolder(File* system) //проверяваме дали има деца и ако има с помощта на рекурсия обхождаме всяко едно от тях и го трием
{
	if (system->checkforchildren())
	{
		File* parents = new File;
		parents = system->getParent();
		int sizee = parents->getChildSize();
		for (int i = 0; i < sizee; i++)
		{
			if (parents->getChild(i) == system)
			{
				for (int t = i; t < sizee - 1; t++)
				{

					parents->childreplace(t, t + 1);

				}
				parents->removeChild(sizee - 1);
				sizee = parents->getChildSize();
			}
		}
	}

}

void setNumberinFS(File* root, File* search, string type)//обхождаме всяка папка и променяме номерациите им така че да са верни
{
	if (root == NULL)
	{
		return;
	}
	int max = root->biggestnum(0, root, type);
	for (int i = 0; i < search->getChildSize(); i++)
	{
		if (search->getChild(i)->getType() == type)
		{
			search->getChild(i)->setNumber(max);
			max = search->biggestnum(0, root, type);

		}
		setNumberinFS(root, search->getChild(i), type);

	}

}

void removeFile(File* files)
{
	File* parents = new File;
	parents = files->getParent();
	int sizee = parents->getChildSize();
	for (int i = 0; i < sizee; i++)
	{
		if (parents->getChild(i) == files)
		{
			for (int t = i; t < sizee - 1; t++)
			{

				parents->childreplace(t, t + 1);

			}
			parents->removeChild(sizee - 1);
			sizee = parents->getChildSize();

		}
	}

}

void copyfiles(vector<File*> copy, File* directory) //взима вектора на една папка и го присвоява на друга
{
	int sizee = directory->getChildSize();
	directory->resizechildren(sizee + copy.size());
	//int i = 0;
	for (int i = 0; i < copy.size(); i++)
	{
		directory->setChild(copy[i]->getName(), sizee + i, "file");
	}

}


int main()
{
	string input;
	File* FileSystem = new File;
	File* curr = new File;
	curr = FileSystem;
	FileSystem->setName("/");
	FileSystem->setChild("folder", 0, "folder");
	FileSystem = FileSystem->getChild(0);
	FileSystem->setChild("folder1", 0, "folder");
	FileSystem->setChild("folder2", 1, "folder");
	FileSystem->setChild("file1", 2, "file");
	FileSystem->getChild(0)->setChild("file2", 0, "file");
	FileSystem->getChild(0)->setChild("file3", 1, "file");
	FileSystem->getChild(1)->setChild("file4", 0, "file");
	FileSystem->getChild(1)->setChild("folder3", 1, "folder");
	FileSystem->getChild(1)->getChild(1)->setChild("file 5", 0, "file");

	setNumberinFS(FileSystem, FileSystem, "file");


	while (true)
	{
		cout << "$ ";
		getline(cin, input, '\n');
		int index = 0;
		while (input[index] != '\0')
		{
			string temp = extractstring(input, index);
			int i = 0;
			while (temp[i] != '\0')
			{
				i++;
				index++;
			}


			//ls
			if (temp == "ls")
			{

				if (input[index + 1] == '/')
				{
					while (input[index] != '\0' && index <= input.size())
					{

						temp = extractstring(input, index);
						int i = 0;
						while (input[index] == ' ' || input[index] == '/')
						{
							index++;
						}
						while (temp[i] != '\0')
						{
							i++;
							index++;
						}
						curr = searchforfile(temp, FileSystem);

					}
					curr->printChildren();
				}
				else
				{

					while (input[index] != '\0' && index <= input.size())
					{
						temp = extractstring(input, index);
						i = 0;
						while (input[index] == ' ' || input[index] == '/')
						{
							index++;
						}
						while (temp[i] != '\0')
						{
							i++;
							index++;
						}
						curr = searchforfile(temp, curr);
					}
					curr->printChildren();
				}

			}

			//append
			if (temp == "append")
			{
				bool check = 0;
				string saveText = "";
				while (input[index] != '\0' && index <= input.size())
				{
					temp = extractstring(input, index);

					int i = 0;
					while (input[index] == ' ' || input[index] == '/')
					{
						index++;
					}
					while (temp[i] != '\0')
					{
						i++;
						index++;
					}
					if (temp == ">")
					{
						check = 1;
					}

					else
					{
						saveText = saveText + searchforfile(temp, FileSystem)->getData() + " ";
					}


					if (check == 1)
					{
						temp = extractstring(input, index);
						i = 0;
						while (input[index] == ' ' || input[index] == '/')
						{
							index++;
						}
						while (temp[i] != '\0')
						{
							i++;
							index++;
						}

						curr = searchforfile(temp, FileSystem);
						curr->setData(curr->getData() + " " + saveText);
					}
					if (check == 1 && saveText == "")
					{
						getline(cin, saveText, '\n');
						curr = searchforfile(temp, FileSystem);
						curr->setData(saveText);
						//cout << "This text goes to "<< temp << endl;
					}
					if (check == 0 && index == input.size())
					{
						cout << saveText << endl;
					}


				}
			}

			//cp
			if (temp == "cp")
			{
				File* copy = new File;
				File* folder = new File;
				vector<File*> copyvec; //създаваме нов вектор,който присвоява вектора на папката от която трябва да копираме
				int k = 0;
				while (input[index] != '\0' && index <= input.size())
				{
					temp = extractstring(input, index);
					int i = 0;
					while (input[index] == ' ' || input[index] == '/')
					{
						index++;
					}
					while (temp[i] != '\0')
					{
						i++;
						index++;
					}
					if (searchforfile(temp, FileSystem)->getType() == "file")
					{
						copyvec.resize(k + 1);
						copyvec[k] = searchforfile(temp, FileSystem);
					}
					else folder = searchforfile(temp, FileSystem);

					k++;
				}
				copyfiles(copyvec, folder);
			}

			//rm
			if (temp == "rm")
			{
				while (input[index] != '\0' && index <= input.size())
				{
					temp = extractstring(input, index);
					int i = 0;
					while (input[index] == ' ' || input[index] == '/')
					{
						index++;
					}
					while (temp[i] != '\0')
					{
						i++;
						index++;
					}
					removeFile(searchforfile(temp, FileSystem));

				}
				setNumberinFS(FileSystem, 0, "file");
			}

			//mkdir
			if (temp == "mkdir")
			{
				while (input[index] != '\0' && index <= input.size())
				{
					temp = extractstring(input, index);
					int i = 0;
					while (input[index] == ' ' || input[index] == '/')
					{
						index++;
					}
					while (temp[i] != '\0')
					{
						i++;
						index++;
					}
					int t = curr->getChildSize();
					curr->resizechildren(t + 1);
					curr->setChild(temp, t, "folder");

				}
				setNumberinFS(FileSystem, FileSystem, "folder");
			}

			//rmdir
			if (temp == "rmdir")
			{
				while (input[index] != '\0' && index <= input.size())
				{
					temp = extractstring(input, index);
					int i = 0;
					while (input[index] == ' ' || input[index] == '/')
					{
						index++;
					}
					while (temp[i] != '\0')
					{
						i++;
						index++;
					}
					removeFolder(searchforfile(temp, FileSystem));
				}
				setNumberinFS(FileSystem, FileSystem, "folder");
			}



			//append
			if (temp == "append")
			{
				temp = extractstring(input, index);
				curr = searchforfile(temp, curr);
				curr->printMetData();
			}

		}
		cout << endl;

	}

	//cout << extractstring("cd folder/folder1/file2", 10);

	//cout << removespace(" space lol     lmao") << endl;
	//cout << reversal("folder/folder 2/folder 3");

	return 0;


}
