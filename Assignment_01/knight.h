#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "main.h"

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue);
int Check(int &HP);
void Solve_line_3(string line_3, string &file_1, string &file_2, string &file_3);
void Fibonacci(int *Fibo);
void Check_Condition(int &HP, int HP_MAX, int &level, int &phoenixdown, int &remedy, int &maidenkiss, int &condition, int tmp_level, int &rescue, int index, int event);
void Check_Phoenixdown(int &HP, int HP_MAX, int &phoenixdown);
void Even_1(int &rescue, int event);
void Even_2(int &HP, int &level, int index, int event);
void Even_3(int &HP, int HP_MAX, int &level, int index, int &count, int &remedy, int event, int &condition);
void Even_4(int &level, int &maidenkiss, int &count, int &tmp_level, int index, int event, int &condition);
void Even_5(int &HP, int HP_MAX, int &level, int &phoenixdown, int event);
void Even_6(int &HP, int event);
int NAM_LOAI_1(int n, int *n2);
int NAM_LOAI_2(int n, int *n2);
int NAM_LOAI_3(int n, int *n2);
int NAM_LOAI_4(int n, int *n2);
void Even_7(int &HP, int HP_max, int &phoenixdown, int event, string file_1);
void Even_9(string &file_2, int &remedy, int &maidenkiss, int &phoenixdown);
bool Check_Prime(int n);
void Even_12(int HP, int Hiepsi, int level, int &rescue, int remedy, int maidenkiss, int phoenixdown);
void Even_13(string &file_3, int &HP, int HP_max, int &count);
void adventureToKoopa(string file_input, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue);
#endif // __KNIGHT_H__