#include "knight.h"

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue)
{
    cout << "HP=" << HP
         << ", level=" << level
         << ", remedy=" << remedy
         << ", maidenkiss=" << maidenkiss
         << ", phoenixdown=" << phoenixdown
         << ", rescue=" << rescue << endl;
}

int Check(int &HP)
{
    HP += 1;
    if (Check_Prime(HP) == false)
        Check(HP);
    return HP;
}

void Solve_line_3(string line_3, string &file_1, string &file_2, string &file_3)
{
    string tokens[3];
    stringstream check1(line_3);
    string intermediate;
    int k = 0;
    while (getline(check1, intermediate, ','))
    {
        tokens[k] = intermediate;
        k++;
    }
    file_1 = tokens[0];
    file_2 = tokens[1];
    file_3 = tokens[2];
}

void Fibonacci(int *Fibo) // Bổ trợ hàm event 12
{
    Fibo[0] = 0;
    Fibo[1] = 1;
    for (int i = 2; i < 18; i++) // Vì số fibo thứ 17 bằng 1.597.
    {
        Fibo[i] = Fibo[i - 1] + Fibo[i - 2];
    }
}

void Check_Condition(int &HP, int HP_MAX, int &level, int &phoenixdown, int &remedy, int &maidenkiss, int &condition, int tmp_level, int &rescue, int index, int event) // Sau 1 turn, nếu máu bé hơn 0, kiểm tra còn phoenixdown hay ko, ko thì thua.
{

    if (HP < 0)
    {
        if (phoenixdown > 0)
        {
            HP = HP_MAX;
            phoenixdown--;
        }
        else
            rescue = 0;
    }
    else if (condition == 1 && remedy > 0)
    {
        condition = 0;
        remedy--;
        HP *= 5;
        HP = min(HP, HP_MAX);
    }

    else if (condition == 2 && maidenkiss > 0)
    {
        condition = 0;
        maidenkiss--;
        level = tmp_level;
    }

    if (index == event - 1 && HP > 0)
        rescue = 1;
}

void Check_Phoenixdown(int &HP, int HP_MAX, int &phoenixdown)
{
    if (HP < 0)
    {
        if (phoenixdown > 0)
        {
            HP = HP_MAX;
            phoenixdown--;
        }
    }
}

void Even_1(int &rescue, int event) // Bowser đầu hàng và trả lại công chúa
{
    if (event == 0)
        rescue = 1;
}

void Even_2(int &HP, int &level, int index, int event) // Gặp gấu MadBear, cướp Bandit, tướng cướp LordLupin, yêu tinh Elf, quỷ khổng lồ Troll
{
    int basedame; // Basedame của 5 tướng đầu.
    switch (event)
    {
    case 1:
    {
        basedame = 10;
        break;
    }
    case 2:
    {
        basedame = 15;
        break;
    }
    case 3:
    {
        basedame = 45;
        break;
    }
    case 4:
    {
        basedame = 75;
        break;
    }
    case 5:
    {
        basedame = 95;
        break;
    }
    }
    int b = index % 10;
    int level0 = index > 6 ? (b > 5 ? b : 5) : b;
    int damage = basedame * level0; // Tổng dame gây ra cho hiệp sĩ khi level thấp hơn level0 của tướng.
    if (level > level0 && level < 10)
        level = (int)min(++level, 10);
    else if (level < level0)
        HP -= damage;
}

void Even_3(int &HP, int HP_MAX, int &level, int index, int &count, int &remedy, int event, int &condition) // Gặp phù thuỷ Shaman
{
    if (event == 6)
    {
        if (count < 0) // Count <= 0 có nghĩa là đang ko bị biến thành ếch hoặc tí hon, vì khi đang là tí hon hoặc ếch thì bỏ qua phù thủy và Siren.
        {
            int level0 = index;
            if (level > level0)
                level = min(level + 2, 10);
            else if (level0 > level)
            {
                if (HP >= 5)
                    HP /= 5;
                else
                    HP = 1;
                if (remedy > 0)
                {
                    remedy--;
                    HP *= 5;
                    HP = min(HP, HP_MAX);
                }
                else
                {
                    count = 4; // Bị biến tí hon trong chính trận đó và 3 trận tiếp
                    condition = 1;
                }
            }
        }
    }
}

void Even_4(int &level, int &maidenkiss, int &count, int &tmp_level, int index, int event, int &condition) // Gặp Siren Vajsh
{
    if (event == 7)
    {
        if (count <= 0) // Tương tự even_3
        {
            int level0 = index;
            if (level > level0 && level < 10)
                level = min(level + 2, 10);
            else if (level0 > level)
            {
                if (maidenkiss > 0)
                    maidenkiss--;
                else
                {
                    condition = 3;
                    tmp_level = level;
                    level = 1;
                    count = 4; // Tương tự even_3
                }
            }
        }
    }
}

void Even_5(int &HP, int HP_MAX, int &level, int &phoenixdown, int event) // Nhặt được nấm tăng lực MushMario
{
    if (event == 11)
    {
        int n1 = ((level + phoenixdown) % 5 + 1) * 3;
        int k = 1;
        int s1 = 0;
        int tmp = 99;
        while (k <= n1)
        {
            s1 += tmp;
            tmp -= 2;
            ++k;
        } // s1 là tổng n1 số lẻ lớn nhất có 2 chữ số
        HP += s1 % 100;
        Check(HP);
        HP = min(HP, HP_MAX);
    }
}

void Even_6(int &HP, int event) // Nhặt được nấm Fibonacci MushFib
{
    int Fibo[18];
    Fibonacci(Fibo);
    if (event == 12 && HP > 1)
    {
        for (int i = 1; i < 18; i++)
        {
            if (HP < Fibo[i])
            {
                HP = Fibo[i - 1];
                break;
            }
        }
    }
}

int NAM_LOAI_1(int n, int *n2)
{
    int maxi = 0;
    int mini = 0;
    int max = n2[0];
    int min = n2[0];
    for (int i = 0; i < n; i++)
    {
        if (n2[i] >= max)
        {
            max = n2[i];
            maxi = i;
        }
        if (n2[i] <= min)
        {
            min = n2[i];
            mini = i;
        }
    }
    return maxi + mini;
}

int NAM_LOAI_2(int n, int *n2)
{
    int mtx = n2[0];
    int mti = 0;

    for (int i = 0; i < n; i++)
    {
        if (n2[i] > mtx)
        {
            mtx = n2[i];
            mti = i;
        }
    }
    for (int i = 0; i < mti; i++)
    {
        if (n2[i] >= n2[i + 1])
        {
            mtx = -2;
            mti = -3;
        }
    }
    for (int i = mti + 1; i < n - 1; i++)
    {
        if (n2[i] <= n2[i + 1])
        {
            mtx = -2;
            mti = -3;
        }
    }

    return mtx + mti;
}

int NAM_LOAI_3(int n, int n2[])
{
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = n2[i];
    }
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < 0)
            arr[i] *= -1;
        arr[i] = (17 * arr[i] + 9) % 257;
    }
    int max2 = 0;
    int min2 = 0;
    int max = arr[0];
    int min = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
            max2 = i;
        }
        if (arr[i] < min)
        {
            min = arr[i];
            min2 = i;
        }
    }
    return max2 + min2;
}

int NAM_LOAI_4(int n, int n2[])
{

    int arr[101];
    for (int i = 0; i < n; i++)
    {
        arr[i] = n2[i];
    }
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < 0)
            arr[i] *= -1;
        arr[i] = (17 * arr[i] + 9) % 257;
    }
    int max = arr[0];
    int max2_3x = -999;
    int max2_3i = 0;
    for (int i = 0; i < 3; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    for (int i = 0; i < 3; i++)
    {
        if (arr[i] < max && arr[i] > max2_3x)
        {
            max2_3x = arr[i];
            max2_3i = i;
        }
    }
    if (max == max2_3x)
    {
        max2_3x = -5;
        max2_3i = -7;
    }

    return max2_3x + max2_3i;
}

void Even_7(int &HP, int HP_max, int &phoenixdown, int event, string file_1)
{
    if (event > 100)
    {
        int n;
        ifstream file(file_1);
        file >> n;
        string line_2;
        getline(file, line_2);
        getline(file, line_2);
        for (int i = 0; i < line_2.size(); i++)
        {
            if (line_2[i] == ',')
                line_2[i] = ' ';
        }
        file.close();
        int n2[20];
        int k = 0;
        stringstream ss(line_2);
        string tmp;
        while (ss >> tmp)
        {
            n2[k] = stoi(tmp);
            k++;
        }
        // ĐỌC FILE MUSH_GHOST

        int temp = event % 10;
        event /= 10;
        int nam[101];
        nam[0] = temp;
        int m = 1;
        while (event > 100)
        {
            temp = event % 10;
            event /= 10;
            nam[m] = temp;
            m++;
        }
        // Ở ĐÂY, NAM[M] LÀ SỐ NẤM MA SẼ GẶP, NHƯNG BỊ NGƯỢC.
        // n LÀ SỐ NGUYÊN DƯƠNG N2.
        // N2[K] LÀ N2 SỐ NGUYÊN DƯƠNG.

        for (int i = m - 1; i >= 0; i--)
        {

            if (nam[i] == 1)
            {
                HP -= NAM_LOAI_1(n, n2);
                HP = min(HP, HP_max);
            }
            if (nam[i] == 2)
            {
                HP -= NAM_LOAI_2(n, n2);
                HP = min(HP, HP_max);
            }

            if (nam[i] == 3)
            {
                HP -= NAM_LOAI_3(n, n2);
                HP = min(HP, HP_max);
            }

            if (nam[i] == 4)
            {
                HP -= NAM_LOAI_4(n, n2);
                HP = min(HP, HP_max);
            }
            Check_Phoenixdown(HP, HP_max, phoenixdown);
        }
    }
}

void Even_9(string &file_2, int &remedy, int &maidenkiss, int &phoenixdown, int &count)
{
    if (count == 1)
    {
        count++;
        string line_1, line_2;
        int r1, c1;
        int arr[101][101];
        ifstream file(file_2);

        file >> r1;
        getline(file, line_1);
        getline(file, line_2);
        c1 = stoi(line_2);
        int k = 0;
        while (k < r1)
        {
            string line;
            getline(file, line);
            stringstream ss(line);
            string tmp;
            int j = 0;
            while (ss >> tmp)
            {
                arr[k][j] = stoi(tmp);
                j++;
            }
            k++;
        }
        file.close();

        for (int i = 0; i < r1; i++)
        {
            int tmp = 1;
            for (int j = 0; j < c1; j++)
            {

                if (tmp <= 3)
                {
                    if (arr[i][j] == 16)
                    {
                        remedy = min(remedy + 1, 99);
                        tmp++;
                    }
                    if (arr[i][j] == 17)
                    {
                        maidenkiss = min(maidenkiss + 1, 99);
                        tmp++;
                    }
                    if (arr[i][j] == 18)
                    {
                        phoenixdown = min(phoenixdown + 1, 99);
                        tmp++;
                    }
                }
            }
        }
    }
}

bool Check_Prime(int n)
{
    if (n == 1)
        return false;

    int i = 2;
    while (i * i <= n)
    {
        if (n % i == 0)
            return false;
        i += 1;
    }
    return true;
}

void Even_12(int HP, int Hiepsi, int level, int &rescue, int remedy, int maidenkiss, int phoenixdown)
{
    // BIẾN HIEPSI DÙNG ĐỂ PHÂN BIỆT HS BÀN TRÒN (HIEPSI = 1) VÀ HS Lancelot (HIEPSI = 2)
    if ((Hiepsi == 1 && level < 10) || (Hiepsi == 2 && level <= 7))
    {
        rescue = 0;
        display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
    }
}

void Even_13(string &file_3, int &HP, int HP_max, int &count)
{
    if (count == 1)
    {
        count++;
        int n;
        string str1 = "Merlin";
        string str2 = "merlin";
        ifstream file(file_3);
        file >> n;
        string tmp;
        getline(file, tmp);
        for (int i = 0; i < n; i++)
        {
            string line;
            getline(file, line);
            size_t found_1 = line.find(str1);
            size_t found_2 = line.find(str2);
            if (found_1 != string::npos)
                HP = min(HP + 3, HP_max);
            else if (found_2 != string::npos)
                HP = min(HP + 3, HP_max);
            else
            {
                for (int i = 0; i < line.size(); i++)
                    line[i] = tolower(line[i]);

                if (line.find('m') != string::npos && line.find('e') != string::npos && line.find('r') != string::npos && line.find('l') != string::npos && line.find('i') != string::npos && line.find('n') != string::npos)
                    HP = min(HP + 2, HP_max);
            }
        }
        file.close();
    }
}

void adventureToKoopa(string file_input, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue)
{
    ifstream file(file_input);
    file >> HP >> level >> remedy >> maidenkiss >> phoenixdown;
    int const HP_max = HP;
    string line_2, line_3;
    getline(file, line_2);
    getline(file, line_2);
    getline(file, line_3);
    file.close();
    long event[1000];
    int k = 0;
    stringstream ss(line_2);
    string tmp;
    while (ss >> tmp)
    {
        event[k] = stoi(tmp);
        k++;
    }

    string file_1, file_2, file_3;
    Solve_line_3(line_3, file_1, file_2, file_3);
    // HOÀN THÀNH PHẦN ĐỌC FILE.
    int condition = 0; // NẾU BIẾN THÀNH TÍ HON = 1 HOẶC ẾCH THÌ CONDI = 2.
    int count = -3;
    int tmp_level;
    rescue = -1;
    int even_19 = 1; // Chỉ số để gặp even 19 đúng 1 lần
    int even_18 = 1; // Chỉ số để gặp even 18 đúng 1 lần
    if (HP_max == 999)
    {
        rescue = 1;
        display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
    }
    else if (Check_Prime(HP_max))
    {
        for (int i = 0; i < k; i++)
        {
            if (HP >= 0)
            {
                if (event[i] == 1 || event[i] == 2 || event[i] == 3 || event[i] == 4 || event[i] == 5)
                    level = min(++level, 10);
                else if (event[i] == 6 || event[i] == 7)
                    level = min(level + 2, 10);
                else if (event[i] == 11)
                    Even_5(HP, HP_max, level, phoenixdown, event[i]);
                else if (event[i] == 12)
                    Even_6(HP, event[i]);
                else if (event[i] > 100)
                    Even_7(HP, HP_max, phoenixdown, event[i], file_1);
                else if (event[i] == 15)
                    remedy = min(remedy + 1, 99);
                else if (event[i] == 16)
                    maidenkiss = min(maidenkiss + 1, 99);
                else if (event[i] == 17)
                    phoenixdown = min(phoenixdown + 1, 99);
                else if (event[i] == 18)
                    Even_13(file_3, HP, HP_max, even_18);
                else if (event[i] == 19)
                    Even_9(file_2, remedy, maidenkiss, phoenixdown, even_19);
                else if (event[i] == 99)
                    Even_12(HP, 2, level, rescue, remedy, maidenkiss, phoenixdown);
                Check_Condition(HP, HP_max, level, phoenixdown, remedy, maidenkiss, condition, tmp_level, rescue, i, k);
                if (rescue == 1)
                {
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                    return;
                }
                else if (rescue == 0)
                    return;
                else
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
        }
    }
    else
    {
        for (int i = 0; i < k; i++)
        {
            if (HP >= 0)
            {

                if (event[i] == 0)
                    Even_1(rescue, event[i]);
                else if (event[i] > 0 && event[i] <= 5)
                    Even_2(HP, level, i + 1, event[i]);
                else if (event[i] == 6)
                    Even_3(HP, HP_max, level, i + 1, count, remedy, event[i], condition);
                else if (event[i] == 7)
                    Even_4(level, maidenkiss, count, tmp_level, i + 1, event[i], condition);
                else if (event[i] == 11)
                    Even_5(HP, HP_max, level, phoenixdown, event[i]);
                else if (event[i] == 12)
                    Even_6(HP, event[i]);
                else if (event[i] > 100)
                    Even_7(HP, HP_max, phoenixdown, event[i], file_1);
                else if (event[i] == 15)
                    remedy = min(remedy + 1, 99);
                else if (event[i] == 16)
                    maidenkiss = min(maidenkiss + 1, 99);
                else if (event[i] == 17)
                    phoenixdown = min(phoenixdown + 1, 99);
                else if (event[i] == 18)
                    Even_13(file_3, HP, HP_max, even_18);
                else if (event[i] == 19)
                    Even_9(file_2, remedy, maidenkiss, phoenixdown, even_19);
                else if (event[i] == 99)
                    Even_12(HP, 1, level, rescue, remedy, maidenkiss, phoenixdown);

                --count;
                if (count == 0)
                {
                    condition = 0;
                    HP = min(HP * 5, HP_max);
                    // level = tmp_level;
                }

                Check_Condition(HP, HP_max, level, phoenixdown, remedy, maidenkiss, condition, tmp_level, rescue, i, k);
                if (rescue == 0 || rescue == 1)
                {
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                    return;
                }
                else
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
        }
    }
}