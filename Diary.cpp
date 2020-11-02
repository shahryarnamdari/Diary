#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef struct Diary { //struct for each date
	string* date = NULL;
	string* text = NULL;
	bool opened = false;//to show which day we should write our diary
	Diary* next = NULL;//I used linked list
}Diary;
void mallcheck(char* mall)//check the errors when allocating memory
{
	if (mall == NULL)
	{
		cout<<"Malloc Error!\n";
		exit(EXIT_FAILURE);
	}
}
char* get_cmd()//to get eatch command line
{
	char *str = (char*)malloc(sizeof(char));//allocate memory
	int ch, i = 0;//ch is for geting characters
				  //loop for get strings
	while ((ch = getchar()) != EOF && ch != '\n')//get char untill '\n'
	{
		str = (char*)realloc(str, sizeof(char)*(i + 1));//reallocate memory to increase its size
		mallcheck(str);//check malloc errors
		str[i] = ch;//put reading chars together
		i++;
	}
	str = (char*)realloc(str, sizeof(char)*(i + 1));//reallocate memory to increase its size
	str[i] = NULL;//put NULL at the end of command line we read
	return str;//return the answer
}
void start_day(string date, Diary* Diarys_head)
{
	Diary* Diary_temp2 = Diarys_head;//make temporary head tp find true opened and make it false
	while (Diary_temp2->next != NULL)
	{
		Diary_temp2 = Diary_temp2->next;//goto next node
		if (Diary_temp2->opened == true) {
			Diary_temp2->opened = false;
			break;
		}
	}
	//search in previous nodes
	int flag = 0;//to show if this date existed before or not
	Diary* Diary_temp = Diarys_head ;//make temporary head
	while (Diary_temp->next != NULL)
	{
		Diary_temp = Diary_temp->next;//goto next node
		if (*Diary_temp->date == date)
		{
			//make opened bool true
			Diary_temp->opened = true;
			flag = 1;
		}
	}
	if (flag == 0)//create new node if it didn't exist with this date
	{
		Diary* new_node = (Diary*)malloc(sizeof(Diary));//allocating memory
		//enter the default situation with new date time
		new_node->next = NULL;
		new_node->date = new string;
		*new_node->date = date;
		new_node->text = new string;
		new_node->opened = true;
		Diary_temp->next = new_node;

	}
}
void print_summary(Diary* diary)//it gets the Diary node and it prints the summary of that
{
	string text_temp = *diary->text;//make temp
	cout << *diary->date << endl;//printing the date time
	if (text_temp.size() < 20){
		cout << *diary->text;//printing all the text
	}
	else {
		cout << text_temp.substr(0, 20) << "...\n";//printing first 20 characters
	}
}
int Find_num_of_positive_words(Diary* diary, string word)//finding number of positive words in the specific date with specific positive word
{
	string str = *diary->text; //coppying a text
	int counter = 0, match = 0; //at the end we'll return match
	int i = 0, j = 0;// i= searching in characters      j= number of matched letters
	while (str[i] != NULL)
	{
		if (word[j] == str[i])
		{
			counter++;
			j++;
		}
		else if (str[i] == word[0])
			counter = 1;
		else
		{
			counter = 0;
			j = 0;
		}
		if (counter == word.size())
		{
			if (str[i + 1] == '.' || str[i + 1] == ',' || str[i + 1] == ' ' || str[i + 1] == NULL || str[i + 1] == '\n')
			{
				if (i - (int)word.size() > -1) {
					if (str[i - word.size()] == '.' || str[i - word.size()] == ',' || str[i - word.size()] == ' ' || str[i - word.size()] == '\n')
					{
						j = 0;
						match++;
					}
					else
					{
						counter = 0;
						j = 0;
					}
				}
				else
				{
					j = 0;
					match++;
				}
			}
			else
			{
				counter = 0;
				j = 0;
			}
		}
		i++;
	}
	return match;//returning result
}

int main()
{
	Diary* Diary_head = (Diary*)malloc(sizeof(Diary));
	Diary_head->next = NULL;
	while (true) //total loop
	{
		string cmdstr = get_cmd();//get command line
		// split command line and put them in strings
		int start = 0;
		int end = cmdstr.find(" ");
		string first= cmdstr.substr(start, end - start); //we found the first word of entered line
		//////start_day
		if (first == "start_day")
		{
			start = end + 1;
			end = cmdstr.find(" ", start);
			string second = cmdstr.substr(start, end - start);//second part of cmdstr
			start_day (second, Diary_head);
			continue;
		}
		//////show_day
		if (first == "show_day")
		{
			start = end + 1;
			end = cmdstr.find(" ", start);
			string second = cmdstr.substr(start, end - start); //second part of cmdstr
			Diary* Diary_temp = Diary_head; //make temporary head tp find date and print Diarys text
			while (Diary_temp->next != NULL)
			{
				Diary_temp = Diary_temp->next;//goto next node
				if (*Diary_temp->date == second)
				{
					cout << *Diary_temp->text;
					break;
				}
			}
			continue;
		}
		///////longest_day
		if (first == "show_the_longest_day")
		{
			string temp = *Diary_head->next->text;//created to compare in size
			Diary* temp2 = Diary_head->next;//it will be the longest diary at the end
			Diary* Diary_temp = Diary_head->next;//make temporary head tp find longest diary
			while (Diary_temp->next != NULL)
			{
				Diary_temp = Diary_temp->next;//goto next node
				string text_temp = *Diary_temp->text;//created to compare in size
				if (text_temp.size() > temp.size())
				{
					temp2 = Diary_temp;
				}
			}
			print_summary(temp2);
			continue;
		}
		///////best_day
		if (first == "show_the_best_day")
		{
			int top_positive_word = 0;
			Diary* Diary_temp = Diary_head;//make temporary head to search in days
			Diary* temp = Diary_head->next;//node with most positive word
				while (Diary_temp->next != NULL)
				{
					ifstream myfile("positive-words.txt");//open text_file in myfile to read 
					int tempnum = 0;//number of matched words
					string positive_word;//the current positive word
					while (getline(myfile, positive_word))//read the text file word by word
					{
						tempnum += Find_num_of_positive_words(Diary_temp->next, positive_word);
					}
					myfile.close();

					if (top_positive_word < tempnum)
					{
						temp = Diary_temp->next;
						top_positive_word = tempnum;
					}
					Diary_temp = Diary_temp->next;//goto next node
				}
				print_summary(temp);//print the answer
			continue;
		}
		/////// writing diary
		else
		{
			Diary* Diary_temp = Diary_head;//make temporary head tp find true opened and write Diary
			while (Diary_temp->next != NULL)
			{
				Diary_temp = Diary_temp->next;//goto next node
				if (Diary_temp->opened == true)
				{
					*Diary_temp->text = *Diary_temp->text + cmdstr + "\n";
					break;
				}
			}
		}
	}
}