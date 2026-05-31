#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <limits>

using namespace std;

// ================= ANSI COLOR =================

string colorRank(string rank) {
    if(rank == "U") return "\033[31m";
    if(rank == "S+") return "\033[38;5;201m";
    if(rank == "SSR") return "\033[33m";
    if(rank == "SR") return "\033[35m";
    if(rank == "R") return "\033[34m";
    return "\033[37m";
}

string resetColor() {
    return "\033[0m";
}

// ================= HERO CLASS =================

class Hero {
public:
    string rank, name;
    int maxHp, hp;
    int damage, skillDamage;
    int level;
    int evolution;
    int price, gemPrice;
    string card;
    bool hasCard;
    bool owned;

    Hero() {}

    Hero(string r, string n, int h, int d, int sd,
         int p, int gp, bool o = false) {

        rank = r;
        name = n;

        maxHp = h;
        hp = h;

        damage = d;
        skillDamage = sd;

        price = p;
        gemPrice = gp;

        owned = o;
        level = 1;
        evolution = 0;
        card = "None";
        hasCard = false;

        if(rank == "SR") {
            maxHp += 30;
            damage += 5;
            skillDamage += 10;
        }
        else if(rank == "SSR") {
            maxHp += 70;
            damage += 15;
            skillDamage += 25;
        }
        else if(rank == "U") {
            maxHp += 150;
            damage += 35;
            skillDamage += 60;
        }

        hp = maxHp;
    }

    void resetHP() {
        hp = maxHp;
    }
};

// ================= GLOBAL =================

int gold = 500;
int soulGems = 0;
int stage = 1;
int dailyGemShop = 10;
int cash = 0;
int vipLevel = 0;
int shardR = 0;
int shardSR = 0;
int shardSPlus = 0;
int shardSSR = 0;
int shardU = 0;
bool code_13092013_used = false;
bool code_namphong_used = false;
bool code_05082013_used = false;
bool code_keocon_used = false;
// ================= DATA =================

vector<Hero> heroes = {
    // R
    Hero("R", "Tanjiro", 100, 15, 25, 100, 3),
    Hero("R", "Inosuke", 130, 20, 25, 110, 3),
    Hero("R", "Zenitsu", 120, 10, 25, 120, 4),

    // SR
    Hero("SR", "Naruto", 120, 20, 35, 250, 8, true),
    Hero("SR", "Luffy", 140, 18, 40, 300, 10),
    Hero("SR", "Rukia", 150, 22, 40, 350, 12),
    Hero("SR", "Renji", 170, 25, 45, 400, 15),
    Hero("SR", "Chad", 180, 30, 50, 450, 18),
    Hero("SR", "Orihime", 200, 15, 60, 500, 20),

    // SSR
    Hero("SSR", "Ichigo", 170, 25, 50, 500, 20),
    Hero("SSR", "Goku", 250, 40, 80, 1000, 50),
    Hero("SSR", "Usato", 260, 70, 120, 1200, 60),
    Hero("SSR", "Alucard", 270, 80, 150, 2000, 100),
    Hero("SSR", "Cosmic Garou", 300, 120, 200, 1500, 80),

    Hero("SSR", "Uryu", 220, 35, 70, 800, 35),
    Hero("SSR", "Toshiro", 240, 45, 90, 1100, 55),
    Hero("SSR", "Grimmjow", 280, 75, 140, 2200, 110),
    Hero("SSR", "Ulquiorra", 320, 100, 180, 3000, 140),

    // S+
    Hero("S+", "Trunks", 350, 200, 250, 2000, 110),
    Hero("S+", "Black Goku", 450, 250, 300, 2500, 140),

    Hero("S+", "Byakuya", 500, 280, 350, 3200, 180),
    Hero("S+", "Kenpachi", 650, 350, 450, 4500, 250),
    Hero("S+", "Yoruichi", 550, 320, 400, 3800, 220),
    Hero("S+", "Urahara", 700, 420, 550, 6000, 300),
    Hero("S+", "Shunsui", 850, 500, 650, 8000, 350),

    // U
    Hero("U", "Saitama", 999, 999, 999, 9999, 200),
    Hero("U", "Sung Jin Woo", 1000, 1000, 1000, 10000, 200),

    Hero("U", "Aizen", 1500, 1600, 2200, 15000, 350),
    Hero("U", "Yamamoto", 2000, 2500, 3200, 18000, 450),
    Hero("U", "Ichibe", 2800, 3500, 4500, 22000, 650),
    Hero("U", "Yhwach", 5000, 7000, 10000, 50000, 1200),

    Hero("U", "Atomic", 220, 35, 70, 0, 0, false),
    Hero("U", "Rimuru", 1200, 1200, 1200, 10500, 250),
    Hero("U", "Gilgamesh", 2000, 2500, 3500, 11000, 300),
    Hero("U", "Dio", 3500, 5000, 7000, 25000, 800)
};
vector<Hero> enemies = {
    Hero("R", "Inosuke", 90, 12, 20, 0, 0),
    Hero("SR", "Sasuke", 120, 18, 35, 0, 0),
    Hero("SR", "Zoro", 140, 20, 40, 0, 0),
    Hero("SSR", "Aizen", 180, 28, 55, 0, 0),
    Hero("SSR", "Vegeta", 260, 42, 85, 0, 0),
    Hero("U", "Madara", 350, 55, 110, 0, 0)
};

// ================= UI =================

void line() {
    cout << "\n====================================\n";
}

// ================= SAVE =================

void saveGame() {

    ofstream f("save.txt");

    if(!f) {
        cout << "Save failed!\n";
        return;
    }

    f << gold << "\n";
    f << soulGems << "\n";
    f << stage << "\n";
    f << dailyGemShop << "\n";
    f << cash << "\n";
    f << vipLevel << "\n";

    f << shardR << "\n";
    f << shardSR << "\n";
    f << shardSPlus << "\n";
    f << shardSSR << "\n";
    f << shardU << "\n";
    f << code_13092013_used << "\n";
    f << code_namphong_used << "\n";
    f << code_05082013_used << "\n";
    f << code_keocon_used << "\n";
    f << heroes.size() << "\n";

    for(auto &h : heroes) {

        f << h.rank << "\n";
        f << h.name << "\n";

        f << h.maxHp << "\n";
        f << h.damage << "\n";
        f << h.skillDamage << "\n";

        f << h.price << "\n";
        f << h.gemPrice << "\n";

        f << h.level << "\n";
        f << h.evolution << "\n";
        f << h.owned << "\n";

        f << h.card << "\n";
        f << h.hasCard << "\n";
    }

    cout << "Game Saved!\n";
}

// ================= LOAD =================

void loadGame() {

    ifstream f("save.txt");

    if(!f) return;

    f >> gold;
    f >> soulGems;
    f >> stage;
    f >> dailyGemShop;
    f >> cash;
    f >> vipLevel;

    f >> shardR;
    f >> shardSR;
    f >> shardSPlus;
    f >> shardSSR;
    f >> shardU;
    f >> code_13092013_used;
    f >> code_namphong_used;
    f >> code_05082013_used;
    f >> code_keocon_used;
    int n;
    f >> n;

    f.ignore(numeric_limits<streamsize>::max(), '\n');

    for(int i = 0; i < n; i++) {

        string rank, name, card;

        int maxHp;
        int damage;
        int skillDamage;

        int price;
        int gemPrice;

        int level;
        int evolution;
        bool owned;
        bool hasCard;

        getline(f, rank);
        getline(f, name);

        f >> maxHp;
        f >> damage;
        f >> skillDamage;

        f >> price;
        f >> gemPrice;

        f >> level;
        f >> evolution;
        f >> owned;

        f.ignore();

        getline(f, card);

        f >> hasCard;

        f.ignore(numeric_limits<streamsize>::max(), '\n');

        for(int j = 0; j < heroes.size(); j++) {

            if(heroes[j].name == name &&
               heroes[j].rank == rank) {

                heroes[j].maxHp = maxHp;
                heroes[j].damage = damage;
                heroes[j].skillDamage = skillDamage;
                heroes[j].level = level;
                heroes[j].evolution = evolution;
                heroes[j].owned = owned;

                heroes[j].card = card;
                heroes[j].hasCard = hasCard;

                heroes[j].hp = heroes[j].maxHp;

                break;
            }
        }
    }

    cout << "Loaded!\n";
}

// ================= REDEEM CODE =================

void redeemCode() {

    line();

    cout << "REDEEM CODE\n";

    line();

    cout << "Enter code: ";

    string code;
    cin >> code;

    if(code == "13092013") {

        if(code_13092013_used) {
            cout << "Code already used!\n";
            return;
        }

        bool alreadyHaveAtomic = false;

        for(auto &h : heroes) {
            if(h.name == "Atomic" && h.owned)
                alreadyHaveAtomic = true;
        }

        if(!alreadyHaveAtomic) {
            for(auto &h : heroes) {
                if(h.name == "Atomic") {
                    h.owned = true;
                }
            }
        }

        code_13092013_used = true;

        cout << "\n====================\n";

        cout << "SECRET HERO UNLOCKED!\n";

        cout << colorRank("U")
             << "[U] "
             << resetColor()
             << "Atomic\n";

        cout << "====================\n";
    }
    else if(code == "NAMPHONGDEPTRAI") {

        if(code_namphong_used) {
            cout << "Code already used!\n";
            return;
        }

        soulGems += 300;

        code_namphong_used = true;

        cout << "You received 300 Soul Gems!\n";
    }
    else if(code == "KEOCON") {

    if(code_keocon_used) {

        cout << "Code already used!\n";
        return;
    }

    shardU += 20;

    code_keocon_used = true;

    cout << "\n====================\n";

    cout << "SPECIAL REWARD!\n";

    cout << "+20 U SHARDS\n";

    cout << "Current U Shards: "
         << shardU << "\n";

    cout << "====================\n";
}
    else if(code == "05082013")
{
    if(code_05082013_used)
    {
        cout << "Code already used!\n";
        return;
    }

    gold += 100000;
    soulGems += 500;

    code_05082013_used = true;
    cout << "================\n";
    cout << " SPECIAL REWARDS!\n";
    cout << "   100000 golds!\n";
    cout << "   300 gems!\n";
    cout << "================\n";
    }
    else
    {
        cout << "Invalid code!\n";
    }
}

// ================= SHOP =================
vector<Hero> defaultHeroes = heroes;
void shop() {

    int choice;

    while(true) {

        vector<int> shopIndex;

        line();

        cout << "HERO SHOP\n";

        line();

        cout << "Gold: " << gold
             << "\nSoul Gems: " << soulGems << "\n";

        for(int i = 0; i < heroes.size(); i++) {

            if(heroes[i].name == "Atomic")
                continue;

            shopIndex.push_back(i);

            cout << shopIndex.size() << ". "
                 << colorRank(heroes[i].rank)
                 << "[" << heroes[i].rank << "] "
                 << resetColor()
                 << heroes[i].name
                 << " | GOLD: " << heroes[i].price
                 << " | GEMS: " << heroes[i].gemPrice;

            if(heroes[i].owned)
                cout << " [OWNED]";

            cout << "\n";
        }

        cout << "\n0 Exit\n";
        cout << "Choose: ";

        cin >> choice;

        if(choice == 0)
            return;

        if(choice < 1 || choice > shopIndex.size())
            continue;

        int id = shopIndex[choice - 1];

        if(heroes[id].owned) {
            cout << "Already owned!\n";
            continue;
        }

        cout << "1 Gold\n2 Gems\nChoose: ";

        int t;
        cin >> t;

        if(t == 1) {

            if(gold >= heroes[id].price) {

                gold -= heroes[id].price;

                heroes[id].owned = true;

                cout << "\n====================\n";

                cout << "NEW HERO!\n";

                cout << colorRank(heroes[id].rank)
                     << "[" << heroes[id].rank << "] "
                     << resetColor()
                     << heroes[id].name
                     << "\n";

                cout << "====================\n";
            }
            else {
                cout << "Not enough gold!\n";
            }
        }
        else if(t == 2) {

            if(soulGems >= heroes[id].gemPrice) {

                soulGems -= heroes[id].gemPrice;

                heroes[id].owned = true;

                cout << "Purchased!\n";
            }
            else {
                cout << "Not enough gems!\n";
            }
        }
    }
}

// ================= GEM SHOP =================

void soulGemShop() {

    line();

    cout << "SOUL GEM SHOP\n";

    line();

    cout << "Gold: " << gold
         << " | Gems: " << soulGems << "\n";

    cout << "Daily Remaining: "
         << dailyGemShop << "\n";

    cout << "1 Gem = 500 Gold\n";

    cout << "How many gems: ";

    int a;
    cin >> a;

    if(a <= 0)
        return;

    if(a > dailyGemShop) {

        cout << "Daily limit exceeded!\n";

        return;
    }

    int cost = a * 500;

    if(gold >= cost) {

        gold -= cost;

        soulGems += a;

        dailyGemShop -= a;

        cout << "Purchased!\n";
    }
    else {
        cout << "Not enough gold!\n";
    }
}

// ================= UPGRADE =================

void upgradeHero() {

    vector<int> list;

    for(int i = 0; i < heroes.size(); i++) {

        if(heroes[i].owned)
            list.push_back(i);
    }

    if(list.empty()) {

        cout << "No heroes owned!\n";

        return;
    }

    line();

    cout << "UPGRADE\n";

    line();

    for(int i = 0; i < list.size(); i++) {

        int id = list[i];

        int cost = heroes[id].level * 200;

        cout << i << ". "
     << colorRank(heroes[id].rank)
     << "[" << heroes[id].rank << "] "
     << resetColor()
     << heroes[id].name
        << " +" << heroes[id].evolution
        << " | LV "
             << heroes[id].level
             << " | COST "
             << cost
             << "\n";
    }

    int c;
    cin >> c;

    if(c < 0 || c >= list.size())
        return;

    int id = list[c];

    int cost = heroes[id].level * 200;

    if(gold < cost) {

        cout << "Not enough gold!\n";

        return;
    }

    gold -= cost;

    heroes[id].level++;

    heroes[id].maxHp += 20;
    heroes[id].damage += 5;
    heroes[id].skillDamage += 10;

    heroes[id].hp = heroes[id].maxHp;

    cout << "Upgrade successful!\n";
}

// ================= FIGHT =================

void fight(Hero &player, Hero enemy) {

    player.resetHP();
    enemy.resetHP();

    line();

    cout << player.name
         << " VS "
         << enemy.name
         << "\n";

    while(player.hp > 0 && enemy.hp > 0) {

        line();

        cout << player.name
             << " HP: "
             << player.hp
             << "/"
             << player.maxHp
             << "\n";

        cout << enemy.name
             << " HP: "
             << enemy.hp
             << "/"
             << enemy.maxHp
             << "\n";

        cout << "\n1 Attack\n2 Skill\n";

        int a;
        cin >> a;

        if(a == 1) {

            enemy.hp -= player.damage;

            cout << player.name
                 << " used Attack!\n";
        }
        else {

            enemy.hp -= player.skillDamage;

            cout << player.name
                 << " used Skill!\n";
        }

        if(enemy.hp <= 0)
            break;

        if(rand() % 2) {

            player.hp -= enemy.damage;

            cout << enemy.name
                 << " attacked!\n";
        }
        else {

            player.hp -= enemy.skillDamage;

            cout << enemy.name
                 << " used Skill!\n";
        }

        if(player.hp < 0)
            player.hp = 0;

        if(enemy.hp < 0)
            enemy.hp = 0;
    }

    line();

    if(player.hp > 0) {

        int reward = 100 + rand() % 300;

        cout << "YOU WIN!\n";

        cout << "Reward: "
             << reward
             << " Gold\n";

        gold += reward;

        stage++;
    }
    else {

        cout << "YOU LOSE!\n";
    }
}

// ================= STORY =================

void storyMode() {

    vector<int> list;

    for(int i = 0; i < heroes.size(); i++) {

        if(heroes[i].owned)
            list.push_back(i);
    }

    if(list.empty()) {

        cout << "You don't own any heroes!\n";

        return;
    }

    line();

    cout << "SELECT HERO\n";

    line();

    for(int i = 0; i < list.size(); i++) {

        cout << i << ". "
     << colorRank(heroes[list[i]].rank)
     << "[" << heroes[list[i]].rank << "] "
     << resetColor()
     << heroes[list[i]].name
        << " +" << heroes[list[i]].evolution
        << " | LV "
                     << heroes[list[i]].level
                     << "\n";
    }

    int c;
    cin >> c;

    if(c < 0 || c >= list.size())
        return;

    Hero &p = heroes[list[c]];

    Hero e = enemies[rand() % enemies.size()];

    fight(p, e);
}
void resetGame()
{
    heroes = defaultHeroes;

    gold = 500;
    soulGems = 0;
    stage = 1;
    dailyGemShop = 10;
    cash = 0;
    vipLevel = 0;

    shardR = 0;
    shardSR = 0;
    shardSPlus = 0;
    shardSSR = 0;
    shardU = 0;

    code_13092013_used = false;
    code_namphong_used = false;
    code_05082013_used = false;
    code_keocon_used = false;

    for(auto &h : heroes)
    {
        h.level = 1;
        h.evolution = 0;   // thêm dòng này

        h.card = "None";
        h.hasCard = false;

        if(h.name == "Naruto")
            h.owned = true;
        else
            h.owned = false;

        h.hp = h.maxHp;
    }

    remove("save.txt");

    cout << "\n====================\n";
    cout << "GAME RESET COMPLETE!\n";
    cout << "====================\n";
}
void topUpSystem() {

    line();

    cout << "TOP UP SYSTEM\n";

    line();

    cout << "Current Cash: "
         << cash
         << "\n";

    cout << "VIP LEVEL: "
         << vipLevel
         << "\n";

    cout << "\n1. 10K VND";
    cout << "\n2. 20K VND";
    cout << "\n3. 50K VND";
    cout << "\n4. 100K VND";
    cout << "\n0. Exit";

    cout << "\n\nChoose: ";

    int c;
    cin >> c;

    if(c == 0)
        return;

    if(c == 1) {

        cash += 10000;

        soulGems += 100;

        gold += 5000;

        cout << "\n+100 Gems";
        cout << "\n+5000 Gold\n";
    }
    else if(c == 2) {

        cash += 20000;

        soulGems += 250;

        gold += 12000;

        cout << "\n+250 Gems";
        cout << "\n+12000 Gold\n";
    }
    else if(c == 3) {

        cash += 50000;

        soulGems += 700;

        gold += 30000;

        cout << "\n+700 Gems";
        cout << "\n+30000 Gold\n";
    }
    else if(c == 4) {

        cash += 100000;

        soulGems += 1500;

        gold += 80000;

        cout << "\n+1500 Gems";
        cout << "\n+80000 Gold\n";
    }

    if(cash >= 100000)
        vipLevel = 5;
    else if(cash >= 50000)
        vipLevel = 4;
    else if(cash >= 20000)
        vipLevel = 3;
    else if(cash >= 10000)
        vipLevel = 2;
    else
        vipLevel = 1;

    cout << "\n====================\n";

    cout << "TOP UP SUCCESS!\n";

    cout << "VIP LEVEL: "
         << vipLevel
         << "\n";

    cout << "====================\n";
}
void openChest() {

    line();
    cout << "HERO CHEST\n";
    line();

    cout << "Cost: 30 Soul Gems\n";

    if(soulGems < 30) {

        cout << "Not enough Soul Gems!\n";

        return;
    }

    soulGems -= 30;

    cout << "-30 Soul Gems\n\n";
    line();

    int goldReward = 1500 + rand() % 201;
    int gemReward = 5 + rand() % 6;

    gold += goldReward;
    soulGems += gemReward;

    cout << "Gold +" << goldReward << "\n";
    cout << "Soul Gems +" << gemReward << "\n";

    double roll = (rand() % 10000) / 100.0;

    if(roll < 0.5) {

        shardU++;

        cout << "Obtained [U] Hero Shard x1\n";
    }
    else if(roll < 13.0) {

        shardSSR++;

        cout << "Obtained [SSR] Hero Shard x1\n";
    }
    else if(roll < 20.0) {

        shardSPlus++;

        cout << "Obtained [S+] Hero Shard x1\n";
    }
    else if(roll < 50.0) {

        shardSR++;

        cout << "Obtained [SR] Hero Shard x1\n";
    }
    else {

        shardR++;

        cout << "Obtained [R] Hero Shard x1\n";
    }

    cout << "\n=== SHARDS ===\n";

    cout << "R   : " << shardR << "\n";
    cout << "SR  : " << shardSR << "\n";
    cout << "S+  : " << shardSPlus << "\n";
    cout << "SSR : " << shardSSR << "\n";
    cout << "U   : " << shardU << "\n";
}
void exchangeShards() {

    line();
    cout << "SHARD EXCHANGE\n";
    line();

    cout << "1. R Hero    (100 R Shards)\n";
    cout << "2. SR Hero   (80 SR Shards)\n";
    cout << "3. S+ Hero   (50 S+ Shards)\n";
    cout << "4. SSR Hero  (30 SSR Shards)\n";
    cout << "5. U Hero    (10 U Shards)\n";
    cout << "0. Exit\n";

    int c;
    cin >> c;

    string rank;
    int *shard = nullptr;
    int cost = 0;

    if(c == 1) {
        rank = "R";
        shard = &shardR;
        cost = 100;
    }
    else if(c == 2) {
        rank = "SR";
        shard = &shardSR;
        cost = 80;
    }
    else if(c == 3) {
        rank = "S+";
        shard = &shardSPlus;
        cost = 50;
    }
    else if(c == 4) {
        rank = "SSR";
        shard = &shardSSR;
        cost = 30;
    }
    else if(c == 5) {
        rank = "U";
        shard = &shardU;
        cost = 10;
    }
    else {
        return;
    }

    if(*shard < cost) {
        cout << "Not enough shards!\n";
        return;
    }

    *shard -= cost;

    vector<int> list;

    for(int i = 0; i < heroes.size(); i++) {
        if(heroes[i].rank == rank)
            list.push_back(i);
    }

    int id = list[rand() % list.size()];

    heroes[id].owned = true;

    cout << "\n====================\n";
    cout << "NEW HERO UNLOCKED!\n";

    cout << colorRank(heroes[id].rank)
         << "[" << heroes[id].rank << "] "
         << resetColor()
         << heroes[id].name
         << "\n";

    cout << "====================\n";
}
void evolutionHero() {

    vector<int> list;

    for(int i = 0; i < heroes.size(); i++)
        if(heroes[i].owned)
            list.push_back(i);

    if(list.empty()) {
        cout << "No heroes owned!\n";
        return;
    }

    line();
    cout << "EVOLUTION\n";
    line();

    for(int i = 0; i < list.size(); i++) {

        int id = list[i];

        cout << i << ". "
             << colorRank(heroes[id].rank)
             << "[" << heroes[id].rank << "] "
             << resetColor()
             << heroes[id].name
             << " | LV " << heroes[id].level
             << " | EVO +" << heroes[id].evolution
             << "\n";
    }

    int c;
    cin >> c;

    if(c < 0 || c >= list.size())
        return;

    int id = list[c];

    int needLevel;
    int needGems;

    if(heroes[id].evolution == 0) {
        needLevel = 10;
        needGems = 100;
    }
    else if(heroes[id].evolution == 1) {
        needLevel = 25;
        needGems = 500;
    }
    else if(heroes[id].evolution == 2) {
        needLevel = 50;
        needGems = 2000;
    }
    else if(heroes[id].evolution == 3) {
        needLevel = 100;
        needGems = 10000;
    }
    else {
        cout << "MAX EVOLUTION!\n";
        return;
    }

    if(heroes[id].level < needLevel) {
        cout << "Need Level "
             << needLevel << "!\n";
        return;
    }

    if(soulGems < needGems) {
        cout << "Need "
             << needGems
             << " Soul Gems!\n";
        return;
    }

    soulGems -= needGems;

    heroes[id].evolution++;

    heroes[id].maxHp += heroes[id].maxHp / 2;
    heroes[id].damage += heroes[id].damage / 2;
    heroes[id].skillDamage += heroes[id].skillDamage / 2;

    heroes[id].hp = heroes[id].maxHp;

    cout << "\n====================\n";
    cout << heroes[id].name
         << " EVOLVED TO +"
         << heroes[id].evolution
         << "!\n";
    cout << "====================\n";
}
// ================= MAIN =================
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
int main() {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);
    srand(time(0));

    loadGame();

    atexit(saveGame);

    while(true) {

        line();

        cout << "ANIME RNG GAME\n";

        line();

        cout << "Gold: " << gold
             << " | Gems: " << soulGems
             << " | Stage: " << stage
             << "\n";
        cout << "\n=== SHARDS ===\n";

        cout << "R   : " << shardR << "\n";
        cout << "SR  : " << shardSR << "\n";
        cout << "S+  : " << shardSPlus << "\n";
        cout << "SSR : " << shardSSR << "\n";
        cout << "U   : " << shardU << "\n";
        cout << "\n1 Story";
        cout << "\n2 Hero Shop";
        cout << "\n3 Gem Shop";
        cout << "\n4 Upgrade";
        cout << "\n5 Save";
        cout << "\n6 Redeem";
        cout << "\n7 Top Up";
        cout << "\n8 Reset Game";
        cout << "\n9 Hero Chest";
        cout << "\n10 Shard Exchange";
        cout << "\n11 Evolution";
        cout << "\n0 Quit";
        cout << "\n\nChoose: ";

        int c;
        cin >> c;

        if(c == 1)
            storyMode();

        else if(c == 2)
            shop();

        else if(c == 3)
            soulGemShop();

        else if(c == 4)
            upgradeHero();

        else if(c == 5)
            saveGame();

        else if(c == 6)
            redeemCode();

        else if(c == 7)
            topUpSystem();

        else if(c == 8)
            resetGame();
        else if(c == 9)
            openChest();

        else if(c == 10)
            exchangeShards();

        else if(c == 11)
            evolutionHero();

        else if(c == 0)
            break;
    }

    return 0;
}
