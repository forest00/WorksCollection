#include <stdio.h>
#include <conio.h>
#include <vector>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "conio_system.h"

template <typename ... Types>
std::string Format(const std::string& fmt, Types ...args)
{
	int len = std::snprintf(nullptr, 0, fmt.c_str(), args...);
	std::vector<char> buf(static_cast<size_t>(len + 1));
	std::snprintf(&buf.front(), buf.size(), fmt.c_str(), args...);
	return std::string(&buf.front(), &buf.back() + 1);
}

void Wait(int millisec)
{
	int wait_clock = millisec / 1000 * CLOCKS_PER_SEC;
	int point = clock();
	while (clock() - point < wait_clock);
}

constexpr int ToMilliSec(double sec)
{
	return static_cast<int>(sec * 1000.0);
}

constexpr double ToSec(int millisec)
{
	return static_cast<double>(millisec) / 1000.0;
}

double ClocksToSec(int clocks)
{
	return static_cast<double>(clocks) / static_cast<double>(CLOCKS_PER_SEC);
}

DWORD FillScreen(HANDLE std_handle, CONSOLE_SCREEN_BUFFER_INFO *screen_info, char fill_character)
{
	COORD zero = { 0,0 };
	DWORD written = 0;
	FillConsoleOutputCharacter(std_handle, ' ', screen_info->srWindow.Right * screen_info->srWindow.Bottom, zero, &written);
	return written;
}

COORD COORD_Literal(int x, int y)
{
	COORD a;
	a.X = x;
	a.Y = y;
	return a;
}

COORD Add(COORD a, COORD b)
{
	COORD c;
	c.X = a.X + b.X;
	c.Y = a.Y + b.Y;
	return c;
}

//格付け
std::string Rating(int score)
{
	int star_limit = 8;
	std::vector<char> buf(star_limit * 2 + 1);
	int star_num = score / 20;
	bool half_star = (score % 20) >= 10;
	int true_star_num = star_num + half_star;
	for (int i = 0; i < star_num; i++)
	{
		buf[i * 2 + 0] = "★"[0];
		buf[i * 2 + 1] = "★"[1];
	}
	if (half_star)
	{
		buf[star_num * 2 + 0] = "☆"[0];
		buf[star_num * 2 + 1] = "☆"[1];
	}
	buf[true_star_num * 2 + 1] = '\0';
	return std::string(&buf.front(), &buf.back() + 1);
}

//スコアのみ出力する
void OutputScore(int score)
{
	std::string str = Format("%d %s", score, Rating(score).c_str());
	printf_s(str.c_str());
}

//ランキング出力する
void OutputRank(int rank, int score)
{
	std::string str = Format("rank %d : %d %s", rank, score, Rating(score).c_str());
	printf_s(str.c_str());
}

int Game(ConIOSystem &con)
{
	const char *ranking_filename = "ranking.txt";
	constexpr int record_max = 0x100;
	constexpr int record_display_max = 15;
	int record[record_max];
	auto std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_info;
	GetConsoleScreenBufferInfo(std_handle, &screen_info);
	COORD zero = COORD_Literal(0, 0);
	COORD center = COORD_Literal(screen_info.srWindow.Right / 2, screen_info.srWindow.Bottom / 2);
	DWORD fill_coc = 0;
	constexpr double wait_sec = 1.0 / 60.0;
	constexpr int wait_millisec = ToMilliSec(wait_sec);
	constexpr double game_sec = 5.0;
	constexpr double ranking_blink_all_sec = 0.3;
	constexpr double ranking_blink_draw_sec = 0.2;
	constexpr int judge_num = 2;
	int judge_keys[judge_num];
	judge_keys[0] = 'd';
	judge_keys[1] = 'k';
	bool game_continue = true;
	constexpr bool do_init_ranking = false;
	int count = 0;
	int rank = -1;
	//ランキング初期化
	if (do_init_ranking)
	{
		for (auto &i : record)
		{
			i = 0;
		}
		std::ofstream out(ranking_filename, std::ofstream::binary);
		if (out.fail())
		{
			return -1;
		}
		out.write(reinterpret_cast<char*>(record), sizeof(record));
		out.close();
	}
	//ゲームループ
	while (game_continue)
	{
		system("cls");
		int judge_now = 0;
		//ランキング読み込み
		{
			for (auto &i : record)
			{
				i = 0;
			}
			std::ifstream in(ranking_filename, std::ifstream::binary);
			if (in.fail() == false)
			{
				in.read(reinterpret_cast<char*>(record), sizeof(record));
				in.close();
			}
		}
		//連打ループ
		bool cancel_flag = false;
		while (true)
		{
			con.Update();
			bool break_flag = false;
			for (int i = 0; i < judge_num; i++)
			{
				if (con.Now() == judge_keys[i])
				{
					count = 1;
					judge_now = i;
					judge_now++;
					judge_now %= judge_num;
					break_flag = true;
					break;
				}
			}
			if (con.Cancel())
			{
				cancel_flag = true;
				break_flag = true;
			}
			if (break_flag)
			{
				break;
			}
			SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 0)));
			OutputScore(0);
			SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 1)));
			printf_s("%lf / %lf\n", 0.0, game_sec);
			SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 2)));
			printf_s("press %c or % c\n", toupper(judge_keys[0]), toupper(judge_keys[1]));
			SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 3)));
			printf_s("cancel : %s key", con.CancelString().c_str());
			Wait(wait_millisec);
		}
		int start_clock = clock();
		double elapsed_sec = 0.0;
		FillScreen(std_handle, &screen_info, ' ');
		if (cancel_flag == false)
		{
			while (elapsed_sec < game_sec)
			{
				elapsed_sec = ClocksToSec(clock() - start_clock);
				con.Update();
				auto judge_key = judge_keys[judge_now];
				if (con.Now() == judge_key)
				{
					count++;
					judge_now++;
					judge_now %= judge_num;
				}
				SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 0)));
				OutputScore(count);
				SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 1)));
				printf_s("%lf / %lf\n", elapsed_sec, game_sec);
				SetConsoleCursorPosition(std_handle, Add(center, COORD_Literal(-8, 2)));
				printf_s("press %c\n", toupper(judge_key));
				Wait(wait_millisec);
			}
			//ランキング登録
			{
				for (int i = 0; i < record_max; i++)
				{
					if (record[i] <= count)
					{
						for (int k = record_max - 1; k > i; k--)
						{
							record[k] = record[k - 1];
						}
						record[i] = count;
						rank = i;
						break;
					}
				}
			}
			//ランキング出力
			{
				std::ofstream out(ranking_filename, std::ofstream::binary);
				if (out.fail())
				{
					return -1;
				}
				out.write(reinterpret_cast<char*>(record), sizeof(record));
				out.close();
			}
		}
		//メニューループ
		system("cls");
		bool view_ranking = false;
		while (true)
		{
			elapsed_sec = ClocksToSec(clock() - start_clock);
			con.Update();
			if (view_ranking)
			{
				if (con.Back())
				{
					view_ranking = false;
					system("cls");
					FillScreen(std_handle, &screen_info, ' ');
					continue;
				}
			}
			else
			{
				if (con.Entry())
				{
					game_continue = true;
					break;
				}
				if (con.Exit())
				{
					game_continue = false;
					break;
				}
				if (con.View())
				{
					view_ranking = true;
					system("cls");
					printf("\tback : %s key\n", con.BackString().c_str());
					for (int i = 0; i < record_max / 2; i++)
					{
						int i1 = i;
						int i2 = i + record_max / 2;
						SetConsoleCursorPosition(std_handle, COORD_Literal(8, i + 2));
						OutputRank(i1 + 1, record[i1]);
						if (i1 == rank)
						{
							printf_s(" <- YOU");
						}
						SetConsoleCursorPosition(std_handle, COORD_Literal(center.X + 8, i + 2));
						OutputRank(i2 + 1, record[i2]);
						if (i2 == rank)
						{
							printf_s(" <- YOU");
						}
					}
					printf_s("\n\n");
					printf("\tback : %s key\n", con.BackString().c_str());
					continue;
				}
				bool is_draw = fmod(elapsed_sec, ranking_blink_all_sec) <= ranking_blink_draw_sec;
				SetConsoleCursorPosition(std_handle, zero);
				puts("");
				if (count == 0)
				{
					printf_s("\trecord : -\n");
				}
				else
				{
					printf_s("\trecord : %d\n", count);
				}
				puts("");
				int rank_0 = max(rank - record_display_max / 2, 0);
				for (int i = 0; i < record_display_max; i++)
				{
					int i1 = i;
					int i2 = i + rank_0;
					SetConsoleCursorPosition(std_handle, COORD_Literal(8, i + 3));
					if (!(i1 == rank && is_draw == false))
					{
						OutputRank(i1 + 1, record[i1]);
					}
					else
					{
						printf_s("                              ");
					}
					SetConsoleCursorPosition(std_handle, COORD_Literal(center.X + 8, i + 3));
					if (!(i2 == rank && is_draw == false))
					{
						OutputRank(i2 + 1, record[i2]);
					}
					else
					{
						printf_s("                              ");
					}
				}
				printf_s("\n\n");
				printf_s("\tretry : %s key\n", con.EntryString().c_str());
				printf_s("\tview rankings: %s key\n", con.ViewString().c_str());
				printf_s("\texit : %s key\n", con.ExitString().c_str());
			}
			Wait(wait_millisec);
		}
	}
	return 0;
}

int main()
{
	ConIOSystemWrap con;
	Game(con.GetRef());
	return 0;
}
