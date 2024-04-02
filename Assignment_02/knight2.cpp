#include "knight2.h"

/*7.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseItem----------------
--------------------------------------------------------------------*/
bool Antidote::canUse(BaseKnight *knight)
{
    return true;
}
void Antidote::use(BaseKnight *knight)
{
    return;
}

bool Phoenix::canUse(BaseKnight *knight)
{
    int hp = knight->getHP();
    int maxHp = knight->getMaxHP();

    switch (phoenix_kind)
    {
    case 1:
        return hp <= 0;
    case 2:
        return hp < (maxHp / 4);
    case 3:
        return hp < (maxHp / 3);
    case 4:
        return hp < (maxHp / 2);
    }
    return false;
}
void Phoenix::use(BaseKnight *knight)
{
    int hp = knight->getHP();
    int maxHp = knight->getMaxHP();

    switch (phoenix_kind)
    {
    case 1:
    case 2:
        knight->setHP(maxHp);
        break;
    case 3:
        if (hp <= 0)
            knight->setHP((maxHp / 3));
        else
            knight->setHP((maxHp / 4) + hp);
        break;
    case 4:
        if (hp <= 0)
            knight->setHP((maxHp / 2));
        else
            knight->setHP((maxHp / 5) + hp);
        break;
    }
}
/*7.*-----------------------------------------------------------------
-----------------END implementation of class BaseItem-----------------
--------------------------------------------------------------------*/

/*6.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseBag----------------
--------------------------------------------------------------------*/
string BaseBag::toString() const
{
    string out = "Bag[count=";
    out = out + to_string(count) + ';';

    Bag_List *tmp = head;
    while (tmp != nullptr)
    {
        string name = tmp->item->toString();
        out = out + name + ',';
        tmp = tmp->next;
    }
    if (out[out.length() - 1] == ',')
        out[out.length() - 1] = ']';
    else
        out += ']';

    return out;
}
bool BaseBag::insertItem(BaseItem *item)
{
    if (maxSlot != -99 && count >= maxSlot)
    {
        return false;
    }

    Bag_List *tmp = new Bag_List(item, head);
    head = tmp;
    ++count;

    return true;
}
BaseItem *BaseBag::get(ItemType itemType)
{
    Bag_List *tmp = head;
    if (tmp->item->getType() == itemType)
    {
        BaseItem *tmpItem = tmp->item;
        return tmpItem;
    }
    tmp = tmp->next;

    return nullptr;
}

bool BaseBag::check(BaseKnight *knight)
{
    Bag_List *tmp = head;
    while (tmp != nullptr)
    {
        if (tmp->item->canUse(knight))
            return true;
    }
    return false;
}

bool BaseBag::use(ItemType itemType, BaseKnight *knight)
{
    if (check(knight))
    {
        BaseItem *item = get(itemType);
        if (item != nullptr)
        {
            item->use(knight);
            --count;
            Bag_List *tmp = head;
            head = head->next;
            delete tmp;

            return true;
        }
    }

    return false;
}
void BaseBag::dropItem()
{
    if (count > 0)
    {
        --count;
        Bag_List *tmp = head;
        head = head->next;
        delete tmp;
    }
}
/*6.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseBag----------------
--------------------------------------------------------------------*/

/*4.*-----------------------------------------------------------------
-----------------BEGIN implementation of class BaseKnight-------------
--------------------------------------------------------------------*/
bool Prime(int n)
{
    if (n < 2)
        return false;
    for (int i = 2; i < sqrt(n); i++)
        if (n % i == 0)
            return false;

    return true;
}
bool Pythagoras(int n)
{
    if (n < 100 || n > 999)
        return false;
    int a = n % 10;
    int b = (n / 10) % 10;
    int c = n / 100;

    if (a * b * c == 0)
        return false;
    if ((a * a + b * b == c * c) || (b * b + c * c == a * a) || (c * c + a * a == b * b))
        return true;

    return false;
}
BaseKnight *BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
{
    if (Prime(maxhp) == true)
        return new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if (maxhp == 888)
        return new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if (maxhp > 99 && maxhp <= 999 && Pythagoras(maxhp) == true)
        return new DragonKnight(id, maxhp, level, gil, 0, phoenixdownI);
    else
        return new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);
}
string BaseKnight::toString() const
{
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    string s("");
    s += "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(maxhp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[knightType] + "]";
    return s;
}
bool BaseKnight::CheckHP()
{
    if (this->getHP() > 0)
        return true;
    else if (bag->use(PHOENIX, this) == true)
        return true;
    else if (this->hp <= 0 && this->gil >= 100)
    {
        this->gil -= 100;
        this->hp = this->maxhp / 2;
        return true;
    }
    return false;
}

void BaseKnight::Win_5_First_Type(BaseOpponent *opponent)
{
    int EarningGil = opponent->getEarningGil();
    this->gil += EarningGil;
}
void BaseKnight::Win_Tornbery()
{
    this->level = min(this->level + 1, 10);
}
void BaseKnight::Win_Queen()
{
    this->gil *= 2;
}

void BaseKnight::Lose_5_First_Type(BaseOpponent *opponent)
{
    int baseDame = opponent->baseDame();
    int bossLevel = opponent->getLevelBoss();
    int damage = baseDame * (bossLevel - this->level);
    this->hp -= damage;

    CheckHP();
}
void BaseKnight::Lose_Tornbery()
{
    if (antidote > 0)
    {
        bag->use(ANTIDOTE, this);
        --antidote;
    }
    else
    {
        for (int i = 3; i >= 1; i--)
            bag->dropItem();

        this->hp -= 10;
        CheckHP();
    }
}
void BaseKnight::Lose_Queen()
{
    this->gil /= 2;
}

void PaladinKnight::fight(BaseOpponent *opponent)
{
    int boss = opponent->getIdBoss();
    int bossLevel = opponent->getLevelBoss();

    if (this->level < bossLevel)
    {
        if (boss == 6)
            Lose_Tornbery();
    }
    else
    {
        if (boss <= 5)
            Win_5_First_Type(opponent);
        else if (boss == 6)
            Win_Tornbery();
        else
            Win_Queen();
    }
}
void LancelotKnight::fight(BaseOpponent *opponent)
{
    int boss = opponent->getIdBoss();
    int bossLevel = opponent->getLevelBoss();

    if (this->level < bossLevel)
    {
        if (boss == 6)
            Lose_Tornbery();
        else if (boss == 7)
            Lose_Queen();
    }
    else
    {
        if (boss <= 5)
            Win_5_First_Type(opponent);
        else if (boss == 6)
            Win_Tornbery();
        else
            Win_Queen();
    }
}
void DragonKnight::fight(BaseOpponent *opponent)
{
    int boss = opponent->getIdBoss();
    int bossLevel = opponent->getLevelBoss();

    if (this->level < bossLevel)
    {
        if (boss <= 5)
            Lose_5_First_Type(opponent);
        else if (boss == 7)
            Lose_Queen();
    }
    else
    {
        if (boss <= 5)
            Win_5_First_Type(opponent);
        else if (boss == 6)
            Win_Tornbery();
        else
            Win_Queen();
    }
}
void NormalKnight::fight(BaseOpponent *opponent)
{
    int boss = opponent->getIdBoss();
    int bossLevel = opponent->getLevelBoss();

    if (this->level < bossLevel)
    {
        if (boss <= 5)
            Lose_5_First_Type(opponent);
        else if (boss == 6)
            Lose_Tornbery();
        else if (boss == 7)
            Lose_Queen();
    }
    else
    {
        if (boss <= 5)
            Win_5_First_Type(opponent);
        else if (boss == 6)
            Win_Tornbery();
        else
            Win_Queen();
    }
}
/*4.*-----------------------------------------------------------------
----------------END implementation of class BaseKnight-------------
--------------------------------------------------------------------*/

/*3.*-----------------------------------------------------------------
----------------BEGIN implementation of class ArmyKnights-------------
--------------------------------------------------------------------*/
ArmyKnights::ArmyKnights(const string &file_armyknights)
{
    num_knight = 0;
    army = nullptr;

    ifstream file(file_armyknights);

    file >> num_knight;
    army = new BaseKnight *[num_knight];
    for (int i = 0; i < num_knight; i++)
    {
        int hp, lv, gil, antidote, phoenix;
        file >> hp >> lv >> phoenix >> gil >> antidote;
        army[i] = BaseKnight::create(i + 1, hp, lv, gil, antidote, phoenix);
    }

    file.close();
}
ArmyKnights::~ArmyKnights()
{
    for (int i = 0; i < num_knight; i++)
        if (army[i])
            delete army[i];
    delete[] army;
    army = nullptr;
    num_knight = 0;
}
int ArmyKnights::count() const
{
    return num_knight;
}
BaseKnight *ArmyKnights::lastKnight() const
{
    return army[num_knight - 1];
}
bool ArmyKnights::adventure(Events *event)
{
    bool final = true; // Bien tra ve true neu giet duoc Ultimecia, nguoc lai thi false
    int num_event = event->count();
    for (int i = 0; i < num_event; i++)
    {
        int evt = event->get(i); // Loai event
        if (num_knight <= 0)
        {
            printInfo();
            continue; // Không sử dụng break được vì nếu break thì sẽ ko xử lí những sự kiện sau khi num_knight âm.
        }
        bool tmp = throughEachEvent(i, evt);
        printInfo();
        if (!tmp)
            final = false;
    }
    return final;
}
void ArmyKnights::printInfo() const
{
    cout << "No. knights: " << this->count();
    if (this->count() > 0)
    {
        BaseKnight *lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
         << ";LancelotSpear:" << this->hasLancelotSpear()
         << ";GuinevereHair:" << this->hasGuinevereHair()
         << ";ExcaliburSword:" << this->hasExcaliburSword()
         << endl
         << string(50, '-') << endl;
}
void ArmyKnights::printResult(bool win) const
{
    cout << (win ? "WIN" : "LOSE") << endl;
}
bool ArmyKnights::fightFirstKind(int order, int event)
{
    int earningGil[5] = {100, 150, 450, 750, 800};
    int baseDame[5] = {10, 15, 45, 75, 95};
    int level_boss = (order + event) % 10 + 1;
    BaseOpponent *boss = new BaseOpponent(level_boss, event, earningGil[event - 1], baseDame[event - 1]);

    // Chien dau tu hiep si cuoi cung trong day
    for (int i = num_knight - 1; i >= 0; i--)
    {
        army[i]->fight(boss);
        if (army[i]->getHP() <= 0)
            deleteKnight(i);
        else
        {
            int gil = army[i]->getGil();
            if (gil > 999)
            {
                army[i]->setGil(999);
                passExtraGil(gil - 999, i - 1);
            }
            break; // Thang roi thi out vong loop
        }
    }

    // Chien xong xoa boss, kiem tra so luong knight
    delete boss;
    return num_knight > 0;
}
bool ArmyKnights::fightTornbery(int order)
{
    const int tornberyLevel = (order + 6) % 10 + 1;
    BaseOpponent *tornbery = new BaseOpponent(tornberyLevel, 6, 0, 0);

    for (int i = num_knight - 1; i >= 0; i--)
    {
        army[i]->fight(tornbery);
        if (army[i]->getHP() <= 0)
            deleteKnight(i);
        else
            break;
    }

    delete tornbery;
    return num_knight > 0;
}
bool ArmyKnights::fightQueenOfCards(int order)
{
    int level_boss = (order + 7) % 10 + 1;
    BaseOpponent *Queen = new BaseOpponent(level_boss, 7, 0, 0);

    BaseKnight *lastknight = this->lastKnight();
    lastknight->fight(Queen);

    int gil = lastknight->getGil();
    if (gil > 999)
    {
        lastknight->setGil(999);
        passExtraGil(gil - 999, num_knight - 1);
    }

    delete Queen;
    return true;
}
bool ArmyKnights::meetNinaDeRings()
{
    for (int i = 0; i < num_knight; i++)
    {
        int gil = army[i]->getGil();
        int hp = army[i]->getHP();
        int maxhp = army[i]->getMaxHP();

        if (army[i]->getType() == PALADIN && hp < (maxhp / 3))
            army[i]->setHP(hp + (maxhp / 5));
        else if (gil >= 50 && hp < (maxhp / 3))
        {
            army[i]->setGil(gil - 50);
            army[i]->setHP(hp + (maxhp / 5));
        }
    }

    return true;
}
bool ArmyKnights::meetDurianGarden()
{
    for (int i = 0; i < num_knight; i++)
        army[i]->setHP(army[i]->getMaxHP());

    return true;
}
bool ArmyKnights::fightOmegaWeapon()
{
    if (DefeatedOmega == true)
        return true;
    for (int i = num_knight - 1; i >= 0; i--)
    {
        if (army[i]->getType() == DRAGON || (army[i]->getHP() == army[i]->getMaxHP() && army[i]->getLevel() == 10))
        {
            DefeatedOmega = true;
            army[i]->setGil(999);
            army[i]->setLevel(10);
            return true;
        }
        else
        {
            army[i]->setHP(0);
            army[i]->CheckHP();
            if (army[i]->getHP() <= 0)
                deleteKnight(i);
            else
                return true;
        }
    }
    return num_knight > 0;
}
bool ArmyKnights::fightHades()
{
    if (DefeatedHades == true)
        return true;

    for (int i = num_knight - 1; i >= 0; i--)
    {
        if (army[i]->getLevel() == 10 || (army[i]->getType() == PALADIN && army[i]->getLevel() >= 8))
        {
            DefeatedHades = true;
            Shield = true;
            return true;
        }
        else
        {
            army[i]->setHP(0);
            army[i]->CheckHP();
            if (army[i]->getHP() <= 0)
                deleteKnight(i);
            else
                return true;
        }
    }
    return num_knight > 0;
}
bool ArmyKnights::getPhoenix(int type)
{
    BaseItem *tmp = new Phoenix(type);
    for (int i = num_knight - 1; i >= 0; i--)
    {
        if (army[i]->getBag()->insertItem(tmp))
            return true;
    }
    delete tmp;
    return true;
}
bool ArmyKnights::meetSpeacialItem(int event)
{
    switch (event)
    {
    case 95:
        Shield = true;
        break;
    case 96:
        Spear = true;
        break;
    case 97:
        Hair = true;
        break;
    case 98:
    {
        if (Shield == true && Spear == true && Hair == true)
            Sword = true;
        break;
    }
    }

    return true;
}
bool ArmyKnights::meetUltimate()
{
    if (Sword == true)
        return true;
    if (!hasPaladinShield() || !hasLancelotSpear() || !hasGuinevereHair())
        return false;

    int UltiHP = 5000; // Ultimecia's HP

    for (int i = num_knight - 1; i >= 0; i--)
    {
        int dame = army[i]->getHP() * army[i]->getLevel() * army[i]->knightBaseDamage;
        UltiHP -= dame;

        if (UltiHP <= 0)
            return true;

        deleteKnight(i);
    }
    return false;
}

void ArmyKnights::deleteKnight(int index)
{
    if (index < 0 || index >= num_knight)
        return;

    delete army[index];
    for (int i = index; i < num_knight - 1; i++)
    {
        army[i] = army[i + 1];
    }

    army[num_knight - 1] = nullptr;
    --num_knight;
}
void ArmyKnights::passExtraGil(int extraGil, int index)
{
    while (index >= 0 && extraGil > 0)
    {
        int Gil = army[index]->getGil();
        Gil += extraGil;
        extraGil = 0;

        if (Gil > 999)
        {
            extraGil = Gil - 999;
            Gil = 999;
        }
        army[index]->setGil(Gil);

        --index;
    }
}
bool ArmyKnights::throughEachEvent(int order, int event)
{
    switch (event)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return fightFirstKind(order, event);
    case 6:
        return fightTornbery(order);
    case 7:
        return fightQueenOfCards(order);
    case 8:
        return meetNinaDeRings();
    case 9:
        return meetDurianGarden();
    case 10:
        return fightOmegaWeapon();
    case 11:
        return fightHades();
    case 99:
        return meetUltimate();
    default:
        if (event >= 12 && event <= 98)
            return meetSpeacialItem(event);
        else if (event >= 110)
            return getPhoenix(event - 110);
        else
            return false;
    }
}
/*3.*-----------------------------------------------------------------
------------------END implementation of class ArmyKnights-------------
--------------------------------------------------------------------*/

/*2.*-----------------------------------------------------------------
---------------BEGIN implementation of class Events-------------------
--------------------------------------------------------------------*/
Events::Events(const string &file_events)
{
    event = nullptr;
    num_event = 0;

    ifstream file(file_events);

    file >> num_event;
    event = new int[num_event];
    for (int i = 0; i < num_event; i++)
        file >> event[i];

    file.close();
}
Events::~Events()
{
    if (this->event != nullptr)
        delete[] this->event;
    this->event = nullptr;
    num_event = 0;
}
int Events::count() const
{
    return num_event;
}
int Events::get(int i) const
{
    return this->event[i];
}
/*2.*-----------------------------------------------------------------
-----------------END implementation of class Events-------------------
--------------------------------------------------------------------*/

/*1.*-----------------------------------------------------------------
-------------BEGIN implementation of class KnightAdventure------------
--------------------------------------------------------------------*/
KnightAdventure::KnightAdventure()
{
    armyKnights = nullptr;
    events = nullptr;
}
KnightAdventure::~KnightAdventure()
{
    if (armyKnights != nullptr)
        armyKnights = nullptr;
    if (events != nullptr)
        events = nullptr;
}
void KnightAdventure::loadArmyKnights(const string &file_armyknights)
{
    armyKnights = new ArmyKnights(file_armyknights);
}
void KnightAdventure::loadEvents(const string &file_events)
{
    events = new Events(file_events);
}
void KnightAdventure::run()
{
    if (armyKnights == nullptr || events == nullptr)
        return;
    bool final = armyKnights->adventure(events); // After the final event, call the function printResult with the parameter set to true if the Knight's Army wins against Ultimecia, or false if the Army is defeated.

    armyKnights->printResult(final);
}
/*1.*-----------------------------------------------------------------
-------------END implementation of class KnightAdventure--------------
--------------------------------------------------------------------*/