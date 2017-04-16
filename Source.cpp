#include <stdlib.h>
#include<iostream>
#include <time.h>
#include<vector>
using namespace std;
#pragma warning( disable : 4996 )
FILE*f;
int player_num = 0; 

	double win = 0;
	double  draw = 0;
	double  lose = 0;
	int AI_Burst = 0;
	int Player_Burst = 0;
	int AI_Player_1_Burst = 0;
	int AI_Player_2_Burst = 0;
	int Playercardsbigger = 0;
	int AI_cards_bigger = 0;
	double AI_ave = 0;
	double Player_ave = 0;
	int A_num=0; int P_num=0;
	double PC = 0; double AC = 0; int Five = 0;
bool yes_no() 
{
	char ans;
	int i = 0;
	while (i == 0) {
		cout << "\nYes or No?( input:y\\n)";
		cin >> ans;
		if (ans == 'y' || ans == 'Y') return true;
		if (ans == 'n' || ans == 'N') return false;
		cout << "error input." << endl;
	}
}

class suit_list
{
public:
	int current_order;//發到第幾張牌
	vector<int>member;
	int nums;
	void set(int num)
	{
		nums = num;

		for (int i = 0; i <num; i++)	member.push_back(i);
	}

	void radom() {
		for (int i = 0; i<1000; i++) {
			int x = rand() % nums;
			int y = rand() % nums;
			int z;

			z = member[x];
			member[x] = member[y];
			member[y] = z;
		}
	}

	void display() {
		for (int i = 0; i < nums; i++)	cout << member[i] % 13 << " ";
	}
};
suit_list list;
class acknowledge
{
public:
	int player_id;

	vector<int>card;

};
class player {
public:
	void player_set(int i)//建構函數 將總玩家數加一
	{
		player_id = i;

		num = 0;
	}
	int player_id;
	int card[5];
	int num;//player當前手牌數

	int score() {
		int points = 0;
		int ace = 0;
		for (int i = 0; i < num; i++) {
			switch (card[i] % 13) {
			case 1:
				ace++;
				points += 11;
				break;
			case 0:
			case 11:
			case 12:
				points += 10;
				break;
			default:
				points += card[i] % 13;
				break;
			}
		}
		while (points>21 && ace != 0) {
			points -= 10;
			ace--;
		}
		if (points > 21)points = -1;

		return points;
	}
	vector <acknowledge> rival;
	void display()
	{

		cout << "\nplayer "<<player_id<< " currently holding: ";
		for (int i = 0; i < num; i++)
			cout <<card[i]%13<<"  ";

		cout<<endl;

	}

};
player *P;
void add_record()
{
	srand(time(NULL));
	char NAME[100] = "win_rate_data";
	int a;
	char c[1000];
	sprintf(c, "_%d", rand() % 1000);

	
	strcat(NAME, c);
	strcat(NAME, ".txt");
	f=fopen(NAME, "w");
}
//n is for the number of players and a is cards'order 
void JoinGame(int &playnum, suit_list &a)
{


	srand(time(NULL));
	int temp;

	cout << "Game Start!" << endl;
	//cout << "How many player?" << endl;
	//cin >> temp;
	temp = 4;
	playnum = temp;


	//shuffle
	a.set(52 * 4);
	a.radom();
	//a.display();

}
void dealing(player &player)
{

	player.card[player.num] = list.member[0];
	
	list.member.erase(list.member.begin());
	player.num++;

}
void broadcast_info(player &player_info, bool Public)
{
	cout << endl;
	if (Public)
	{
		cout << "[ FOR ALL ] :" << endl;
		cout << "player " << player_info.player_id << " draw " << player_info.card[player_info.num - 1] % 13 << endl;
		cout << endl;
	}
	else
	{
		cout << "[ FOR player " << player_info.player_id << " ] :" << endl;
		cout << "player " << player_info.player_id << " draw " << player_info.card[player_info.num - 1] % 13 << endl;
		cout << endl;
	}
}
//-1輸局 0平局 1贏局

int win_check(player &b, player &a)
{
	PC += a.num;
	AC += b.num;
	if (a.score() == -1)Player_Burst++;
	else
	{
		Player_ave += a.score();
		P_num++;
	}
	if (b.score() == -1)
		AI_Burst++;
	else
	{
		AI_ave += b.score();
		A_num++;
	}
	if (a.score() == -1 || a.score()<b.score())
	{

		cout << "player " << a.player_id << " lose the match" << endl;
		lose++;
		if (a.score() != -1)
			AI_cards_bigger++;
		return -1;
	}
	if (a.num == 5)
	{
		cout << "player " << a.player_id << " win the match" << endl;
		win++;
		Five++;
		return 1;
	}
	if (a.score()>b.score())
	{
		cout << "player " << a.player_id << " win the match" << endl;
		win++;
		Playercardsbigger++;
		return 1;
	}
	if (a.score() ==b.score())
	{
		cout << "game draw" << endl;
		draw++;
		return 0;
	}
}
void set_record(player a,player b,int c)
{
	
	fprintf( f,"set %d:\nplayer %d currently holding: ",win+lose+draw,b.player_id);
	for (int i = 0; i <b.num; i++)
		fprintf(f,"%d ",b.card[i] % 13 );
	fprintf(f, "\nbanker %d currently holding: ", a.player_id);
	for (int i = 0; i <a.num; i++)
		fprintf(f, "%d ", a.card[i] % 13);
	

}
void under17(player P)
{
	while (P.score() != -1)
	{
		if (P.score() <= 17)
		{
			dealing(P);
			broadcast_info(P, true);
		}
		else
			break;
	}

}
void start_up(player *P)
{
	for (int i = 1; i < player_num; i++)
	{
		dealing(P[i]);
		//broadcast_info(P[i], 0);

	}
	broadcast_info(P[3], 0);
	dealing(P[0]);
	broadcast_info(P[0], true);
	for (int i = 1; i < player_num; i++)
	{
		dealing(P[i]);
		broadcast_info(P[i], 1);

	}
	dealing(P[0]);

}
void AI_progress(player &P)
{
	while (P.score() != -1)
	{
		if (P.score() <= 17)
		{
			dealing(P);
			broadcast_info(P, true);
		}
		else
			break;
	}
	
	if (P.score() == -1)
	{
		cout << "player " << P.player_id << " burst " << endl;
		P.display();
	}
}
void basic_progress(player* &P)
{
	
	AI_progress(P[1]);
	if (P[1].score() == -1)
		AI_Player_1_Burst++;
	AI_progress(P[2]);
	if (P[2].score() == -1)
		AI_Player_2_Burst++;
	P[3].display();
	cout << "player current point :" << P[3].score() << endl;
	while (P[3].score() != -1)
	{
		cout << "\ndraw card?" << endl;
		if (yes_no())
		{
			dealing(P[3]);

			broadcast_info(P[3], true);
			P[3].display();
			cout << "player current point :" << P[3].score() << endl;

		}
		else
			break;
	}
	AI_progress(P[0]);

}

int main3()
{
	srand(time(NULL));
	//set_card(card1,1);


	list.set(222);
	list.radom();
	list.display();
	cout << endl;
	cout << endl;
	player one;
	dealing(one);
	broadcast_info(one, true);
	dealing(one);
	broadcast_info(one, false);

	cout << one.score();
	system("pause");
	return 0;
}

int main()
{
	bool end = true;
	add_record();
	JoinGame(player_num, list);
cout << "you are player 3~~~" << endl;

	while (end)
	{
		//list.display();
		cout << endl;
		cout << endl;
		player	*P = new player[player_num];
		for (int i = 0; i < player_num; i++)
			P[i].player_set(i);

	
		start_up(P);
		basic_progress(P);

		//result
		{
			cout << endl;
			P[0].display();
			P[1].display();
			P[2].display();
			P[3].display();
			cout << endl;
			cout << "banker final point :" << P[0].score() << endl;
			
			cout << "player final point :" << P[3].score() << endl;
			cout << endl;
			//set_record(P[0], P[1], 

			win_check(P[0], P[3]);
		}

		cout << "win: " << win << endl;
		cout << "lose: " << lose << endl;
		cout << "draw " << draw << endl;
		cout <<"cards remain: " <<list.member.size() << endl;
		cout << "\nend game?" << endl;
		if (list.member.size() <= 104)
		{
			list.member.clear();
			JoinGame(player_num, list);
		}
		if (yes_no())
		{
			double wr;
			wr = win / (win + draw + lose)*100.0;

			fprintf(f, "result:\n\ngame set: %.0f \nwin: %.0f\nlose: %.0f\ndraw: %.0f\n\n", (win + draw + lose), win, lose, draw);
			fprintf(f, "win rate: %.3f%%\n", wr);
			fprintf(f, "AI's point bigger: %d \nPlayer's point bigger: %d\n", AI_cards_bigger, Playercardsbigger);
			fprintf(f, "AI burst: %d\nAI Player 1 burst: %d\nAI Player 2 burst: %d\nPlayer burst: %d\nFive cards: %d\n\n", AI_Burst,AI_Player_1_Burst,AI_Player_2_Burst ,Player_Burst, Five);
			fprintf(f, "AI's average score (without burst): %.3f \n", AI_ave / A_num);
			fprintf(f, "AI's average card hold: %.3f \n", AC / (win + draw + lose));
			fprintf(f, "Plyer's average score (without burst): %.3f \n", Player_ave / P_num);
			fprintf(f, "Player's average card hold: %.3f \n", PC / (win + draw + lose));
			return 0;

		}
		

	}
}

