#include "gamePlay.h"
#include "drawMenus.h"

Player player;

mapObj** map = nullptr;
int selectedPoke = 0;
int botSelectedPoke = 0;
int currBot = 0;
std::vector<Bot> bots;

double waitStartTime = GetTime();
bool waiting = true, waiting1 = true;
bool task = 1;
bool choice = 1;

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

std::vector<Bot> GenerateBotArray(int count, const std::map<std::string, Pokemon>& allPokes) {
    std::vector<Bot> result;

    if (allPokes.size() < 3) {
        std::cerr << "FATAL: Недостаточно покемонов для генерации ботов (size = "
            << allPokes.size() << ")\n";
        return result;
    }

    for (int i = 0; i < count; ++i) {
        result.emplace_back(allPokes); // вызывает Bot(const map<...>&)
    }

    return result;
}

void generateMap(int enemyCount, int healCount, int difficulty, const std::map<std::string, Pokemon>& allPokes) {
    std::random_device rd;
    std::mt19937 rng(rd());
    if (allPokes.size() < 3) {
        std::cout << "FATAL: Недостаточно покемонов!\n";
        abort(); // ← вот он
    }

    bots = GenerateBotArray(enemyCount, allPokes);
    
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
void DrawPokemonInfo(const Pokemon& p, Vector2 pos, bool isEnemy) {
    std::string typeStr = TypeToString(p.getType());
    std::string levelStr = "Lv. " + std::to_string(p.getHp() > 0 ? p.getHp() / 10 : 1); // примерная формула
    std::string hpStr = "HP: " + std::to_string(p.getHp()) + " / " + std::to_string(p.getMaxHp());
    std::string defStr = "DEF: " + std::to_string(p.getHp() > 0 ? p.getHp() / 2 : 10); // фейковый def

    int fontSize = 20;
    Color color = isEnemy ? MAROON : DARKBLUE;

    DrawText(typeStr.c_str(), pos.x, pos.y, fontSize, color);
    DrawText(levelStr.c_str(), pos.x, pos.y + 22, fontSize, color);
    DrawText(hpStr.c_str(), pos.x, pos.y + 44, fontSize, color);
    DrawText(defStr.c_str(), pos.x, pos.y + 66, fontSize, color);
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

    if (map[y][x].type == mapObjType::ENEMY)
    {
        map[y][x].type = mapObjType::AIR;
        StartTransition(BATTLE);
        currBot++;
    }
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

void Player::battle()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 2);

    DrawTexture(GetTexture("battle"), 0, 0, WHITE);

    DrawTextureEx(team[0].getSkin(), { 78, 786 }, 0, 2.0f, WHITE);
    DrawTextureEx(team[1].getSkin(), { 352, 786 }, 0, 2.0f, WHITE);
    DrawTextureEx(team[2].getSkin(), { 635, 786 }, 0, 2.0f, WHITE);
    DrawPokemonInfo(team[0], { 89, 640 }, false);
    DrawPokemonInfo(team[1], { 369, 640 }, false);
    DrawPokemonInfo(team[2], { 646, 640 }, false);

    const Bot& enemy = bots[currBot];
    DrawPokemonInfo(enemy.team[0], { 742, 285 }, true);
    DrawPokemonInfo(enemy.team[1], { 1025, 285 }, true);
    DrawPokemonInfo(enemy.team[2], { 1302, 285 }, true);

    DrawTextureEx(bots[currBot].team[0].getSkin(), { 726, 67 }, 0, 2.0f, WHITE);
    DrawTextureEx(bots[currBot].team[1].getSkin(), { 1008, 67 }, 0, 2.0f, WHITE);
    DrawTextureEx(bots[currBot].team[2].getSkin(), { 1300, 67 }, 0, 2.0f, WHITE);

    Rectangle pokeRects[3] = {
        {  89, 799, 133, 155 },
        { 369, 799, 135, 155 },
        { 646, 799, 141, 155 }
    };

    Rectangle botRects[3] = {
    { 742, 74, 133, 152 },
    { 1025, 74, 127, 152 },
    { 1302, 74, 140, 152 }
    };

    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < 3; ++i) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, pokeRects[i])) {
            selectedPoke = i;
        }

        if (i == selectedPoke) {
            DrawRectangleRec(pokeRects[i], Fade(BLUE, 0.3f));
        }
    }

    // Подсветка выбранного покемона бота
    for (int i = 0; i < 3; ++i) {
        if (i == botSelectedPoke) {
            DrawRectangleRec(botRects[i], Fade(RED, 0.3f));              // прозрачная заливка
            DrawRectangleLinesEx(botRects[i], 3, MAROON);                // рамка
        }
    }
    DrawTextureEx(team[selectedPoke].getSkin(), { 211, 278 }, 0, 2.5f, WHITE);

    DrawTextureEx(bots[currBot].team[botSelectedPoke].getSkin(), { 1188, 410 }, 0, 2.5f, WHITE);
    if (bots[currBot].teamIsAlive() && teamIsAlive())
    {
        if (task)
        {
            if (GuiButton({ 937, 819, 263, 107 }, "") && team[selectedPoke].getHp() > 0)
            {
                bots[currBot].team[botSelectedPoke].TakeDamage(team[selectedPoke].getMoves()[0].power);
                player.exp++;
                task = 0;
                waiting = 1, waiting1 = 1, choice = 1;
            }
            if (GuiButton({ 1245, 822, 229, 104 }, "") && exp > 2 && team[selectedPoke].getHp() > 0)
            {
                bots[currBot].team[botSelectedPoke].TakeDamage(team[selectedPoke].getMoves()[1].power);
                player.exp = 0;
                task = 0;
                waiting = 1, waiting1 = 1, choice = 1;
            }
        }
        else
        {
            if (waiting) {
                if (GetTime() - waitStartTime >= 5.0) {
                    waiting = false;
                }
            }
            else if (choice) {
                botSelectedPoke = dist(rng);
                if (bots[currBot].team[botSelectedPoke].getHp() > 0)
                    choice = 0;
            }

            if (waiting1) {
                if (GetTime() - waitStartTime >= 5.0) {
                    waiting1 = false;
                }
            }
            else {
                if (bots[currBot].exp == 3 && bots[currBot].team[botSelectedPoke].getHp() > 0)
                {
                    team[selectedPoke].TakeDamage(bots[currBot].team[botSelectedPoke].getMoves()[1].power);
                    task = 1;
                }
                else if (bots[currBot].team[botSelectedPoke].getHp() > 0)
                {
                    team[selectedPoke].TakeDamage(bots[currBot].team[botSelectedPoke].getMoves()[0].power);
                    task = 1;
                }
            }
        }
    }
    else if (!teamIsAlive())
        currentScreen = SCREEN_MENU;
    else if (!bots[currBot].teamIsAlive())
        currentScreen = SCREEN_GAMEPLAY;
}