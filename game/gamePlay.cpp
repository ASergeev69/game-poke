#include "gamePlay.h"
#include "DrawMenus.h"

Player player;

mapObj** map = nullptr;

bool isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < 10 && y < 10;
}


bool isReachable(mapObj** tempMap, int startX, int startY) {
    bool visited[10][10]{};
    std::queue<std::pair<int, int>> q;
    q.push({ startX, startY });
    visited[startY][startX] = true;
    int reachable = 1;

    int dx[4]{ 0, 0, -1, 1 };
    int dy[4]{ -1, 1, 0, 0 };

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (isValid(nx, ny) && !visited[ny][nx] && tempMap[ny][nx].type != mapTypes::WALL) {
                visited[ny][nx] = true;
                q.push({ nx, ny });
                ++reachable;
            }
        }
    }

    int passable = 0;
    for (int y = 0; y < 10; ++y)
        for (int x = 0; x < 10; ++x)
            if (tempMap[y][x].type != mapTypes::WALL)
                ++passable;

    return reachable == passable;
}


void generateMap(int enemyCount, int healCount, int difficulty) {
    std::random_device rd;
    std::mt19937 rng(rd());


    // Очистка старой карты
    if (map != nullptr) {
        for (int i = 0; i < 10; ++i)
            delete[] map[i];
        delete[] map;
    }

    while (true) {
        // Создание новой карты
        map = new mapObj * [10];
        for (int i = 0; i < 10; ++i)
            map[i] = new mapObj[12];

        // Инициализация всех клеток
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 12; ++x) {
                map[y][x] = mapObj{ mapTypes::AIR, "air", true };
            }
        }

        // Установка игрока в центр
        map[5][5] = mapObj{ mapTypes::PLAYER, "player", true };

        // Генерация стен
        std::uniform_int_distribution<> wallDist(0, 99);
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 12; ++x) {
                if (x == 5 && y == 5) continue;

                if (wallDist(rng) < 15 + difficulty * 3) {
                    map[y][x] = mapObj{ mapTypes::WALL, "wall", false };
                }
            }
        }

        // Расставим врагов
        int placedEnemies = 0;
        for (int attempts = 0; placedEnemies < enemyCount && attempts < 1000; ++attempts) {
            int x = rng() % 12;
            int y = rng() % 10;
            if ((x != 5 || y != 5) && map[y][x].type == mapTypes::AIR) {
                map[y][x] = mapObj{ mapTypes::ENEMY, "person_f_0", false };
                placedEnemies++;
            }
        }

        // Расставим зелья
        int placedHeals = 0;
        for (int attempts = 0; placedHeals < healCount && attempts < 1000; ++attempts) {
            int x = rng() % 12;
            int y = rng() % 10;
            if ((x != 5 || y != 5) && map[y][x].type == mapTypes::AIR) {
                map[y][x] = mapObj{ mapTypes::HEAL, "up", true };
                placedHeals++;
            }
        }

        // Проверка достижимости
        if (isReachable(map, 5, 5)) break;

        // Очистка и повтор
        for (int i = 0; i < 10; ++i) delete[] map[i];
        delete[] map;
        map = nullptr;
    }
}

void printMapToConsole()
{
    std::cout << std::endl;
    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 12; ++x)
        {
            char c = '?';
            switch (map[y][x].type)
            {
            case mapTypes::WALL:   c = '#'; break;
            case mapTypes::ENEMY:  c = 'E'; break;
            case mapTypes::PLAYER: c = 'P'; break;
            case mapTypes::HEAL:   c = '+'; break;
            case mapTypes::AIR:    c = '.'; break;
            }
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
}

void DrawGameplay() {
    DrawTexture(GetTexture("menu_ingame"), 0, 0, WHITE);

}

void Player::walk(int step_x, int step_y)
{
    if (map == nullptr) return;

    if (step_x == 1 && x < 11 && map[y + step_y][x + step_x].type != mapTypes::WALL)
    {
        x += step_x;

    }
    else if (step_y == 1 && y < 9 && map[y + step_y][x + step_x].type != mapTypes::WALL)
    {
        y += step_y;
    }
    else if (step_y == -1 && y > 0 && map[y + step_y][x + step_x].type != mapTypes::WALL)
    {
        y += step_y;
    }
    else if (step_x == -1 && x > 0 && map[y + step_y][x + step_x].type != mapTypes::WALL)
    {
        x += step_x;
    }
    gainHP();
    std::cout << x << " " << y << " " << std::endl;
}

void Player::gainHP()
{
    if (map == nullptr) return;

    if (map[y][x].type == mapTypes::HEAL)
    {
        team[0].Heal();
        team[1].Heal();
        team[2].Heal();
        map[y][x].type = mapTypes::AIR;
    }
}