#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<fstream>
#include<algorithm>
#include <iterator>
//#include <cctype>
#include <random>
#include <chrono>
using namespace std;

std::map<string, double> score_table;

//string cipher = "itufhnufikfrfmqnhnficfqitawrqfvfrwhntaikwanyanhffwqhxspwgfinbfditknwrufdnwmtumihxwagihthxfctjfqtlhxfnahfqafhrpxwawctcprwqotpaywkhtqwadinayfqwmqpchronahqtdpkfdxniynqrlqnfadtaitknwrufdnwht";

void preprocess(string origin_cipher, string &cipher)
{
	unsigned int i = 0;
	for (i = 0; i < origin_cipher.length(); i++)
	{
		if (((origin_cipher[i] >= 'A') && (origin_cipher[i] <= 'Z')) || ((origin_cipher[i] >= 'a') && (origin_cipher[i] <= 'z')))
			cipher = cipher + origin_cipher[i];
	}
}

double caculate(map<string, double> &score_table, string s)
{

	double score = 0.0;
	unsigned int i = 0;
	string temp;
	for (i = 0; i < s.length() - 3; i++)
	{
		temp = "";
		temp = s.substr(i, 4);
		score += score_table[temp];
	}
	return score;

}

double change_and_cal(string KEY, string decipher, map<string, double> &score_table)
{
	string s = decipher;
	unsigned int i = 0;
	double score = 0.0;
	string temp;
	std::map<string, double>::iterator it;
	for (i = 0; i < s.length(); i++)
	{
		s[i] = KEY[s[i] - 'A'];
	}

	for (i = 0; i < s.length() - 3; i++)
	{
		temp = "";
		temp = s.substr(i, 4);
		it = score_table.find(temp);
		if (it != score_table.end())
			score += score_table[temp];
		else
			score += -12;
	}

	return score;


}

void random(string &KEY)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	shuffle(KEY.begin(), KEY.end(), std::default_random_engine(seed));
}


int main(void)
{
	//int number = 0;
	string text;
	cout << "Begin read score table" << endl;
	//将score读入map
	ifstream myfile("score.txt");
	//	myfile >> number;
	int i = 0;
	double score;
	while (!myfile.eof())
	{
		myfile >> text >> score;
		//cout << text<<endl;
		score_table.insert(std::pair<string, double>(text, score));
	}
	myfile.close();
	cout << "Score table reading finished" << endl;
	//cout << score_table.size()
	string origin_cipher;
	string cipher="";
	string decipher;

	cout << "Begin read cipher" << endl;
	ifstream readcipher("test_file_en.txt");
	getline(readcipher, origin_cipher);
	cout << "Cipher reading finished" << endl;

	preprocess(origin_cipher, cipher);//删去多余符号及空格

	transform(cipher.begin(), cipher.end(), cipher.begin(), toupper);
	decipher = cipher;
	string KEY = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	double MAX_score = -10000000000.0;

	//score = caculate(score_table, cipher);

	double score_max = -10000000000.0;
	string KEY_max = KEY;
	cout << "Decipher..." << endl;
	while (true)
	{
		int i = 0;

		string KEY_max_while = KEY;
		random(KEY_max_while);
		double score_max_while = change_and_cal(KEY_max_while, decipher, score_table);
		while (i < 1000)
		{
			string KEY_temp = KEY_max_while;
			int a = rand() % 26, b = rand() % 26;
			char t = KEY_temp[b];
			KEY_temp[b] = KEY_temp[a];
			KEY_temp[a] = t;
			double score_temp = change_and_cal(KEY_temp, decipher, score_table);
			if (score_max_while < score_temp)
			{
				score_max_while = score_temp;
				KEY_max_while = KEY_temp;
				i = 0;
			}
			i++;
		}
		if (score_max < score_max_while)
		{
			score_max = score_max_while;
			KEY_max = KEY_max_while;
			string temp_for_look = origin_cipher;
			for (i = 0; i < temp_for_look.length(); i++)
			{
				if (((temp_for_look[i] >= 'A') && (temp_for_look[i] <= 'Z')))
				temp_for_look[i] = KEY_max[temp_for_look[i] - 'A'];
				else if(((temp_for_look[i] >= 'a') && (temp_for_look[i] <= 'z')))
				temp_for_look[i] = KEY_max[temp_for_look[i] - 'a']+32;
				else;
			}
			cout << temp_for_look << endl;
			cout << score_max << endl;
			cout << KEY_max<<endl;
			cout << "Go on? Y or N\n";
			string answer;
			cin >> answer;
			if (answer != "Y")
				break;
		}
	}

	return 0;

}