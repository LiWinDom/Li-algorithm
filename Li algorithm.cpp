#define _ver "1.2"
#include <iostream>
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>

//visible options
const uint8_t height = 20; // height of game field
const uint8_t width = 20; // width of game field
const uint8_t size = 20; //size of each sell
const uint64_t wallCellColor = 0x808080FF; //color of cell of border in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t wayCellColor = 0x00C0C0FF; //color of cell of way in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t startCellColor = 0xC080C0FF; //color of cell of satrt and finish in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t backgroundColor = 0xFFFFFFFF; //color of background in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t textColor = 0x000000FF; //color of text in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t cellTextColor = 0xFFFFFFFF; //color of text in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t bordersColor = 0xFFFFFFFF; //color of borders in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint64_t fieldBorderColor = 0x000000FF; //color of field border in HEX format (0x11223344: 11 - red color, 22 - green color, 33 - blue color, 44 - alpha channel)
const uint8_t borderThinkness = 2; //border thinkness, set to 0, if you don't want to see borders


void clearLi(std::vector<std::vector<int>>& field) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i][j] > 0 || field[i][j] == -20) {
                field[i][j] = 0;
            }
        }
    }
}

void createCell(std::vector<std::vector<int>>& field, const uint8_t& x, const uint8_t& y, const uint8_t curMode = 0) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i][j] == -20) {
                field[i][j] = 0;
            }
            else if (field[i][j] == -curMode && (curMode == 21 || curMode == 22)) {
                field[i][j] = 0;
            }
        }
    }
    field[y][x] = -curMode;

    return;
}

void clearField(std::vector<std::vector<int>>& field) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) createCell(field, i, j, 0);
    }
    return;
}

uint8_t checkField(std::vector<std::vector<int>>& field) {
    bool isStart = false, isFinish = false;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i][j] == -21) {
                isStart = true;
            }
            if (field[i][j] == -22) {
                isFinish = true;
            }
        }
    }

    if (!isStart) {
        return 21;
    }
    if (!isFinish) {
        return 22;
    }
    return 0;
}

uint16_t findWay(std::vector<std::vector<int>>& field, int x = -1, int y = -1) {
    if (x == -1 && y == -1) {
        for (x = 0; x < height; ++x) {
            bool isBreak = false;

            for (y = 0; y < width; ++y) {
                if (field[x][y] == -22) {
                    isBreak = true;
                    break;
                }
            }
            if (isBreak) break;
        }

        bool isWay = false;

        if (x > 0) {
            if (field[x - 1][y] > 0 || field[x - 1][y] == -21) {
                isWay = true;
            }
        }
        if (x < height - 1) {
            if (field[x + 1][y] > 0 || field[x + 1][y] == -21) {
                isWay = true;
            }
        }
        if (y > 0) {
            if (field[x][y - 1] > 0 || field[x][y - 1] == -21) {
                isWay = true;
            }
        }
        if (y < height - 1) {
            if (field[x][y + 1] > 0 || field[x][y + 1] == -21) {
                isWay = true;
            }
        }
        if (!isWay) {
            return -1;
        }
    }
    
    if (field[x][y] == -21) {
        return 1;
    }

    int32_t steps = 10e9;
    int minLoc;
    if (x > 0) {
        if (field[x - 1][y] < steps && field[x - 1][y] != -22 && field[x - 1][y] != -10 && field[x - 1][y] != 0) {
            minLoc = 10;
            steps = field[x - 1][y];
        }
    }
    if (x < height - 1) {
        if (field[x + 1][y] < steps && field[x + 1][y] != -22 && field[x + 1][y] != -10 && field[x + 1][y] != 0) {
            minLoc = 11;
            steps = field[x + 1][y];
        }
    }
    if (y > 0) {
        if (field[x][y - 1] < steps && field[x][y - 1] != -22 && field[x][y - 1] != -10 && field[x][y - 1] != 0) {
            minLoc = 20;
            steps = field[x][y - 1];
        }
    }
    if (y < height - 1) {
        if (field[x][y + 1] < steps && field[x][y + 1] != -22 && field[x][y + 1] != -10 && field[x][y + 1] != 0) {
            minLoc = 21;
            steps = field[x][y + 1];
        }
    }
    if (minLoc == 10) {
        findWay(field, x - 1, y);
        if (field[x][y] != -22) {
            field[x][y] = -20;
        }
    }
    if (minLoc == 11) {
        findWay(field, x + 1, y);
        if (field[x][y] != -22) {
            field[x][y] = -20;
        }
    }
    if (minLoc == 20) {
        findWay(field, x, y - 1);
        if (field[x][y] != -22) {
            field[x][y] = -20;
        }
    }
    if (minLoc == 21) {
        findWay(field, x, y + 1);
        if (field[x][y] != -22) {
            field[x][y] = -20;
        }
    }

    if (steps == -21) {
        return 1;
    }
    return steps + 1;
}

void liAlg(std::vector<std::vector<int>>& field, int x = -1, int y = -1, uint16_t counter = 0) {
    if (x == -1 && y == -1) {
        for (x = 0; x < height; ++x) {
            bool isBreak = false;

            for (y = 0; y < width; ++y) {
                if (field[x][y] == -21) {
                    isBreak = true;
                    break;
                }
            }
            if (isBreak) break;
        }
    }
    else {
        field[x][y] = counter;
    }

    if (x > 0) {
        if (field[x - 1][y] != -10 && field[x - 1][y] != -21 && field[x - 1][y] != -22) {
            if (field[x - 1][y] > counter + 1 || field[x - 1][y] == 0) {
                liAlg(field, x - 1, y, counter + 1);
            }
        }
    }
    if (x < height - 1) {
        if (field[x + 1][y] != -10 && field[x + 1][y] != -21 && field[x + 1][y] != -22) {
            if (field[x + 1][y] > counter + 1 || field[x + 1][y] == 0) {
                liAlg(field, x + 1, y, counter + 1);
            }
        }
    }
    if (y > 0) {
        if (field[x][y - 1] != -10 && field[x][y - 1] != -21 && field[x][y - 1] != -22) {
            if (field[x][y - 1] > counter + 1 || field[x][y - 1] == 0) {
                liAlg(field, x, y - 1, counter + 1);
            }
        }
    }
    if (y < width - 1) {
        if (field[x][y + 1] != -10 && field[x][y + 1] != -21 && field[x][y + 1] != -22) {
            if (field[x][y + 1] > counter + 1 || field[x][y + 1] == 0) {
                liAlg(field, x, y + 1, counter + 1);
            }
        }
    }

    return;
}

void drawBorders(sf::RenderWindow& window) {
    for (int i = 0; i <= height; ++i) {
        sf::RectangleShape rect;
        rect.setOutlineThickness(0);
        if (i == 0 || i == height) {
            rect.setSize(sf::Vector2f((width * size) + (width + 1) * borderThinkness, borderThinkness));
            rect.setPosition(0, size * i + i * borderThinkness);
            rect.setFillColor(sf::Color(fieldBorderColor));
        }
        else {
            rect.setSize(sf::Vector2f((width * size) + (width - 1) * borderThinkness, borderThinkness));
            rect.setPosition(borderThinkness, size * i + i * borderThinkness);
            rect.setFillColor(sf::Color(bordersColor));
        }
        window.draw(rect);
    }
    for (int i = 0; i <= width; ++i) {
        sf::RectangleShape rect;
        rect.setOutlineThickness(0);
        if (i == 0 || i == width) {
            rect.setSize(sf::Vector2f(borderThinkness, (height * size) + (height + 1) * borderThinkness));
            rect.setPosition(size * i + i * borderThinkness, 0);
            rect.setFillColor(sf::Color(fieldBorderColor));
        }
        else {
            rect.setSize(sf::Vector2f(borderThinkness, (height * size) + (height - 1) * borderThinkness));
            rect.setPosition(size * i + i * borderThinkness, borderThinkness);
            rect.setFillColor(sf::Color(bordersColor));
        }
        window.draw(rect);
    }
    return;
}

void showField(sf::RenderWindow& window, const std::vector<std::vector<int>>& field) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int color;
            std::string text = "";

            if (field[i][j] == -10) {
                color = wallCellColor;
            }
            else if (field[i][j] == -20) {
                color = wayCellColor;
            }
            else if (field[i][j] == -21) {
                color = startCellColor;
                text = "S";
            }
            else if (field[i][j] == -22) {
                color = startCellColor;
                text = "F";
            }
            else continue;


            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(size, size));
            rect.setOutlineThickness(0);
            rect.setPosition(j * size + (j + 1) * borderThinkness, i * size + (i + 1) * borderThinkness);
            rect.setFillColor(sf::Color(color));
            window.draw(rect);
            if (text != "") {
                sf::Font font;
                font.loadFromFile("resourses/Chava.ttf");

                sf::Text cellText;
                cellText.setFont(font);
                cellText.setCharacterSize(size);
                cellText.setFillColor(sf::Color(cellTextColor));
                cellText.setString(text);
                cellText.setPosition(j * size + (j + 1) * borderThinkness + size * 0.1, i * size + (i + 1) * borderThinkness - size * 0.1);

                window.draw(cellText);
            }
        }
    }
    return;
}

void showText(sf::RenderWindow& window, const std::string& text) {
    sf::Font font;
    
    font.loadFromFile("resourses/Chava.ttf");

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(size);
    infoText.setFillColor(sf::Color(textColor));
    infoText.setString(text);

    infoText.setPosition(size * 0.25, (height * size) + (height + 1) * borderThinkness + size * 0.25);
    window.draw(infoText);

    return;
}

void gameEventProcessing(sf::RenderWindow& window, sf::Event& event, std::vector<std::vector<int>>& field, uint8_t& curMode, int16_t& steps) {
    static bool isMouseReleased = true;

    if (event.type == sf::Event::Closed) window.close();
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            steps = 0;
            curMode = 10;
        }
        else if (event.key.code == sf::Keyboard::S) {
            steps = 0;
            curMode = 21;
        }
        else if (event.key.code == sf::Keyboard::F) {
            steps = 0;
            curMode = 22;
        }
        else if (event.key.code == sf::Keyboard::BackSpace) {
            clearField(field);
            steps = 0;
            curMode = 10;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            clearLi(field);
            uint8_t check = checkField(field);

            if (check == 21) {
                steps = -21;
            }
            else if (check == 22) {
                steps = -22;
            }
            else {
                liAlg(field);
                steps = findWay(field);
            }
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        if (pos.x > 0 && pos.y > 0) {
            if (pos.x < (width * size) + (width + 1) * borderThinkness && pos.y < (height * size) + (height + 1) * borderThinkness) {
                createCell(field, pos.x / (size + borderThinkness), pos.y / (size + borderThinkness), curMode);
            }
        }

        steps = -10;
        isMouseReleased = false;
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        if (pos.x > 0 && pos.y > 0) {
            if (pos.x < (width * size) + (width + 1) * borderThinkness && pos.y < (height * size) + (height + 1) * borderThinkness) {
                createCell(field, pos.x / (size + borderThinkness), pos.y / (size + borderThinkness), 0);
            }
        }

        steps = -10;
        isMouseReleased = false;
    }
    else if (event.type == sf::Event::MouseButtonReleased) isMouseReleased = true;
    return;
}

int main() {
    std::vector<std::vector<int>> field(height, std::vector<int>(width, 0));
    uint8_t curMode = 10;
    int16_t steps = -10;
    std::string title = "Li argorithm - version ";
    title += _ver;

    sf::RenderWindow gameWindow(sf::VideoMode((width * size) + (width + 1) * borderThinkness, (height * size) + (height + 1) * borderThinkness + (size * 1.75)), title, sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("resourses/grafics/icon.png");
    gameWindow.setIcon(48, 48, icon.getPixelsPtr());
    gameWindow.setVerticalSyncEnabled(true);

    while (gameWindow.isOpen()) {
        std::string text;

        gameWindow.clear(sf::Color(backgroundColor));
        sf::Event gameEvent;
        while (gameWindow.pollEvent(gameEvent)) gameEventProcessing(gameWindow, gameEvent, field, curMode, steps);

        drawBorders(gameWindow);
        showField(gameWindow, field);

        if (steps > 0) {
            text = "Founded way with " + std::to_string(steps) + " step(s)";
        }
        else if (steps == -1) {
            text = "No way founded!";
        }
        else if (steps == -21) {
            text = "You need to set start pos";
        }
        else if (steps == -22) {
            text = "You need to set finish pos";
        }
        else if (curMode == 10) {
            text = "You are drawing walls";
        }
        else if (curMode == 21) {
            text = "You are drawing start pos";
        }
        else if (curMode == 22) {
            text = "You are drawing finish pos";
        }
        showText(gameWindow, text);
        gameWindow.display();
    }
    return 0;
}