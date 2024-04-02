#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
class BaseItem;
class Phoenix;
class Antidote;
class BaseKnight;
class Events;
class ArmyKnights;
class KnightAdventure;

/*7.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseItem----------------
--------------------------------------------------------------------*/
enum ItemType
{
    ANTIDOTE = 0,
    PHOENIX
};

class BaseItem
{
protected:
    ItemType type;

public:
    virtual ~BaseItem(){};
    virtual ItemType getType() { return type; }
    virtual string toString() = 0;
    virtual bool canUse(BaseKnight *knight) = 0;
    virtual void use(BaseKnight *knight) = 0;
};
class Antidote : public BaseItem
{
public:
    Antidote()
    {
        this->type = ANTIDOTE;
    }
    ~Antidote() {}

    string toString() { return "Antidote"; }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};

class Phoenix : public BaseItem
{
protected:
    int phoenix_kind;

public:
    Phoenix(int num)
    {
        this->type = PHOENIX;
        phoenix_kind = num;
    }
    ~Phoenix() {}

    string toString()
    {
        string arr[5] = {"O", "I", "II", "III", "IV"};
        return "Phoenix" + arr[phoenix_kind];
    }

    bool canUse(BaseKnight *knight);

    void use(BaseKnight *knight);
};
/*7.*-----------------------------------------------------------------
-----------------END implementation of class BaseItem-----------------
--------------------------------------------------------------------*/

/*6.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseBag----------------
--------------------------------------------------------------------*/
class BaseBag
{
private:
    struct Bag_List
    {
        BaseItem *item;
        struct Bag_List *next;

        Bag_List(BaseItem *tmpitem, Bag_List *nextptr)
        {
            item = tmpitem;
            next = nextptr;
        }
        ~Bag_List()
        {
            if (item != nullptr)
                delete item;
        }
    };
    int count;
    int maxSlot;

public:
    Bag_List *head = NULL;

    BaseBag(int phoenixdown, int antidote, int max = -99)
    {
        maxSlot = max;
        count = 0;

        for (int i = 0; i < phoenixdown; i++)
        {
            BaseItem *tmp = new Phoenix(1);
            this->insertItem(tmp);
        }
        for (int i = 0; i < antidote; i++)
        {
            BaseItem *tmp = new Antidote();
            this->insertItem(tmp);
        }
    }

    ~BaseBag()
    {
        if (head != nullptr)
            head = nullptr;
        count = 0;
    }
    virtual string toString() const;

    virtual bool insertItem(BaseItem *item);
    virtual bool check(BaseKnight *knight);
    virtual BaseItem *get(ItemType itemType);

    virtual bool use(ItemType itemType, BaseKnight *knight);
    virtual void dropItem();
};
/*6.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseBag----------------
--------------------------------------------------------------------*/

/*5.*-----------------------------------------------------------------
----------------BEGIN implementation of class BaseOpponent------------
--------------------------------------------------------------------*/
class BaseOpponent
{
protected:
    int level;       // Boss's Level
    int id_boss;     // Kind of Boss
    int earning_gil; // Earned Gil when winning Boss
    int baseDamage;  // Damage of Boss effect to knights
public:
    BaseOpponent(int lv, int id, int gil, int dame)
    {
        level = lv;
        id_boss = id;
        earning_gil = gil;
        baseDamage = dame;
    }
    int getLevelBoss() { return level; }
    int getIdBoss() { return id_boss; }
    int getEarningGil() { return earning_gil; }
    int baseDame() { return baseDamage; }
};
/*5.*-----------------------------------------------------------------
-----------------END implementation of class BaseOpponent-------------
--------------------------------------------------------------------*/

/*4.*-----------------------------------------------------------------
-----------------END implementation of class BaseKnight---------------
--------------------------------------------------------------------*/
enum KnightType
{
    PALADIN = 0,
    LANCELOT,
    DRAGON,
    NORMAL
};
class BaseKnight
{
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag *bag;
    KnightType knightType;

public:
    static BaseKnight *create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    virtual ~BaseKnight()
    {
        if (bag)
            delete bag;
        bag = nullptr;
    }
    string toString() const;

    int getHP() { return this->hp; }
    int getMaxHP() { return this->maxhp; }
    int getLevel() { return this->level; }
    int getGil() { return this->gil; }
    BaseBag *getBag() { return this->bag; }
    KnightType getType() const { return knightType; }

    void setHP(int n) { this->hp = n; }
    void setLevel(int n) { this->level = n; }
    void setGil(int n) { this->gil = n; }

    double knightBaseDamage; // Damage each knight effect to Ultimate.
    virtual void fight(BaseOpponent *opponent) = 0;
    virtual bool CheckHP();

protected:
    virtual void Win_5_First_Type(BaseOpponent *opponent);
    virtual void Win_Tornbery();
    virtual void Win_Queen();

    virtual void Lose_5_First_Type(BaseOpponent *opponent);
    virtual void Lose_Tornbery();
    virtual void Lose_Queen();
};
class PaladinKnight : public BaseKnight
{
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->hp = maxhp;
        this->maxhp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixdownI, antidote, -99); // Túi đồ của Paladin được Merlin phù phép với phép thuật không gian nên không bị giới hạn số lượng vật phẩm
        this->knightType = PALADIN;
        knightBaseDamage = 0.06;
    }
    ~PaladinKnight()
    {
        if (bag != nullptr)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};
class LancelotKnight : public BaseKnight
{
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->hp = maxhp;
        this->maxhp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixdownI, antidote, -99); // Túi đồ của Paladin được Merlin phù phép với phép thuật không gian nên không bị giới hạn số lượng vật phẩm
        this->knightType = LANCELOT;
        knightBaseDamage = 0.05;
    }
    ~LancelotKnight()
    {
        if (bag != nullptr)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};
class DragonKnight : public BaseKnight
{
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->hp = maxhp;
        this->maxhp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixdownI, antidote, -99); // Túi đồ của Paladin được Merlin phù phép với phép thuật không gian nên không bị giới hạn số lượng vật phẩm
        this->knightType = DRAGON;
        knightBaseDamage = 0.075;
    }
    ~DragonKnight()
    {
        if (bag != nullptr)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};
class NormalKnight : public BaseKnight
{
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    {
        this->id = id;
        this->hp = maxhp;
        this->maxhp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixdownI, antidote, -99); // Túi đồ của Paladin được Merlin phù phép với phép thuật không gian nên không bị giới hạn số lượng vật phẩm
        this->knightType = NORMAL;
        knightBaseDamage = 0;
    }
    ~NormalKnight()
    {
        if (bag != nullptr)
            delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent *opponent);
};
/*4.*-----------------------------------------------------------------
----------------END implementation of class BaseKnight-------------
--------------------------------------------------------------------*/

/*3.*-----------------------------------------------------------------
----------------BEGIN implementation of class ArmyKnights-------------
--------------------------------------------------------------------*/
class ArmyKnights
{
private:
    BaseKnight **army; // Representation of the Knight's army, with each knight being a pointer.
    int num_knight;    // Number of knights

public:
    bool Shield = false;
    bool Spear = false;
    bool Hair = false;
    bool Sword = false;
    bool DefeatedOmega = false;
    bool DefeatedHades = false;

    ArmyKnights(const string &file_armyknights);
    ~ArmyKnights();
    bool adventure(Events *event);
    int count() const;
    BaseKnight *lastKnight() const;

    bool hasPaladinShield() const { return Shield; }
    bool hasLancelotSpear() const { return Spear; }
    bool hasGuinevereHair() const { return Hair; }
    bool hasExcaliburSword() const { return Sword; }

    void printInfo() const;
    void printResult(bool win) const;

private:
    bool throughEachEvent(int index, int event);

    // Indentify each event
    bool fightFirstKind(int order, int event);
    bool fightTornbery(int order);
    bool fightQueenOfCards(int order);
    bool meetNinaDeRings();
    bool meetDurianGarden();
    bool fightOmegaWeapon();
    bool fightHades();
    bool getPhoenix(int n);
    bool meetSpeacialItem(int event);
    bool meetUltimate();

    void deleteKnight(int index);
    void passExtraGil(int extraGil, int index);
};
/*3.*-----------------------------------------------------------------
------------------END implementation of class ArmyKnights-------------
--------------------------------------------------------------------*/

/*2.*-----------------------------------------------------------------
---------------BEGIN implementation of class Events-------------------
--------------------------------------------------------------------*/
class Events
{
    int *event;    // Representation of the ID of each event.
    int num_event; // Number of events.

public:
    Events(const string &file_events);
    ~Events();
    int count() const;
    int get(int i) const;
};
/*2.*-----------------------------------------------------------------
-----------------END implementation of class Events-------------------
--------------------------------------------------------------------*/

/*1.*-----------------------------------------------------------------
-------------BEGIN implementation of class KnightAdventure------------
--------------------------------------------------------------------*/
class KnightAdventure
{
private:
    ArmyKnights *armyKnights; // Pointer pointing to the class ArmyKnights.
    Events *events;           // Pointer pointing to the class Events.

public:
    KnightAdventure();
    ~KnightAdventure();

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};
/*1.*-----------------------------------------------------------------
-------------END implementation of class KnightAdventure--------------
--------------------------------------------------------------------*/
#endif // __KNIGHT2_H__