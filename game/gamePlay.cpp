#include "gamePlay.h"
#include "gameState.h"

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
            if (isValid(nx, ny) && !visited[ny][nx] && tempMap[ny][nx].type != mapObjType::WALL) {
                visited[ny][nx] = true;
                q.push({ nx, ny });
                ++reachable;
            }
        }
    }

    int passable = 0;
    for (int y = 0; y < 10; ++y)
        for (int x = 0; x < 10; ++x)
            if (tempMap[y][x].type != mapObjType::WALL)
                ++passable;

    return reachable == passable;
}


void generateMap(int enemyCount, int healCount, int difficulty, const std::map<std::string, Pokemon>& allPokes) {
    std::random_device rd;
    std::mt19937 rng(rd());
    if (allPokes.size() < 3) {
        std::cout << "FATAL: Недостаточно покемонов!\n";
        abort(); // ← вот он
    }
    
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

        for (int y = 0; y < 10; ++y)
            for (int x = 0; x < 12; ++x)
                map[y][x] = { mapObjType::AIR, Texture2D{} };

        // Установка игрока в центр
        map[5][5].type = mapObjType::PLAYER;
        map[5][5].texture = player.skins[1];

        // Сначала случайно расставим стены (зависит от сложности)
        std::uniform_int_distribution<> wallDist(0, 99);
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 12; ++x) {
                if (x == 5 && y == 5) continue;

                if (wallDist(rng) < 15 + difficulty * 3)
                {
                    map[y][x].type = mapObjType::WALL;
                    map[y][x].texture = Texture2D{GetTexture("wall")};
                }
                    
            }
        }

        // Расставим врагов
        int placedEnemies = 0;
        for (int attempts = 0; placedEnemies < enemyCount && attempts < 1000; ++attempts) {
            int x = rng() % 10;
            int y = rng() % 10;
            if ((x != 5 || y != 5) && map[y][x].type == mapObjType::AIR) {
                map[y][x].type = mapObjType::ENEMY;
                map[y][x].texture = Texture2D{ GetTexture("person_f_0") };
                std::cout << "[DEBUG] Попытка создать бота: allPokes.size() = " << allPokes.size() << std::endl;
                placedEnemies++;
            }
        }

        // Расставим зелья
        int placedHeals = 0;
        for (int attempts = 0; placedHeals < healCount && attempts < 1000; ++attempts) {
            int x = rng() % 10;
            int y = rng() % 10;
            if ((x != 5 || y != 5) && map[y][x].type == mapObjType::AIR) {
                map[y][x].type = mapObjType::HEAL;
                map[y][x].texture = Texture2D{GetTexture("up")};
                placedHeals++;
            }
        }

        // Проверка достижимости
        if (isReachable(map, 5, 5)) break;

        // Очистка и повтор генерации
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
            case mapObjType::WALL:   c = '#'; break;
            case mapObjType::ENEMY:  c = 'E'; break;
            case mapObjType::PLAYER: c = 'P'; break;
            case mapObjType::HEAL:   c = '+'; break;
            case mapObjType::AIR:    c = '.'; break;
            }
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
}

void DrawGameplay() {
    DrawTexture(GetTexture("menu_ingame"), 0, 0, WHITE);


    bool test = true;
    if (map == nullptr) return;

    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 12; ++j)
            if (map[i][j].type != mapObjType::PLAYER)
                DrawTextureEx(map[i][j].texture, { static_cast<float>(40.7 + j * 75+j*0.82), static_cast <float>(87.0 + i * 74 + i*0.85) }, 0, 0.1f, WHITE);
    float baseX = 40.7f + player.x * 75.0f + player.x * 0.82f;
    float baseY = 87.0f + player.y * 74.0f + player.y * 0.85f;

    DrawTextureEx(map[5][5].texture, {baseX + 15.36f, baseY + 15.36f}, 0, 0.07f, WHITE);
    if (IsKeyPressed(KEY_D))
    {
        map[5][5].texture = player.skins[3];
        player.walk(1, 0);
    }
    if (IsKeyPressed(KEY_A))
    {
        map[5][5].texture = player.skins[4];
        player.walk(-1, 0);
    }
    if (IsKeyPressed(KEY_W))
    {
        map[5][5].texture = player.skins[2];
        player.walk(0, -1);
    }
    if (IsKeyPressed(KEY_S))
    {
        map[5][5].texture = player.skins[1];
        player.walk(0, 1);
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showSettingsWindow = !showSettingsWindow;

        printMapToConsole();
    }

    if (!showSettingsWindow) {

    }
    else {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 100 });
        DrawSettings(855, 200);
    }
}

void Player::walk(int step_x, int step_y)
{
    if (map == nullptr) return;

    if (step_x == 1 && x < 11 && map[y + step_y][x + step_x].type != mapObjType::WALL)
    {
        x += step_x;

    }
    else if (step_y == 1 && y < 9 && map[y + step_y][x + step_x].type != mapObjType::WALL)
    {
        y += step_y;
    }
    else if (step_y == -1 && y > 0 && map[y + step_y][x + step_x].type != mapObjType::WALL)
    {
        y += step_y;
    }
    else if (step_x == -1 && x > 0 && map[y + step_y][x + step_x].type != mapObjType::WALL)
    {
        x += step_x;
    }
    gainHP();
    std::cout << x << " " << y << " " << std::endl;
}

void Player::gainHP()
{
    if (map == nullptr) return;

    if (map[y][x].type == mapObjType::HEAL)
    {
        team[0].Heal();
        team[1].Heal();
        team[2].Heal();
        map[y][x].type = mapObjType::AIR;
        map[y][x].texture = {};
    }
}