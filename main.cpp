#include <fstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <deque>
#include <filesystem>

enum Screen {
    Menu,
    Game,
    Options,
    Ranking
};

struct Word {
    sf::Text text;
    double speed;
};

std::string getProjectDirectory() {
    return std::filesystem::current_path().string();
}
//pobieranie ścieżki pliku
//https://stackoverflow.com/questions/45401822/how-to-convert-filesystem-path-to-string


auto loadWordsFromFile(const std::string& fileName) -> std::vector<std::string> {
    std::vector<std::string> words;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return words;
    }
    std::string line;
    while (file >> line) {
        words.push_back(line);
    }
    file.close();
    return words;
}

auto main() -> int {
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "Monkey Typer");
    window.setFramerateLimit(60);

    std::string currentInput;
    sf::Text inputText;
    sf::Font font;
    sf::Clock spawnClock;
    auto spawnInterval = 2.0;
    Screen currentScreen = Menu;
    auto score = 0;
    auto lives = 3;
    sf::Clock gameClock;
    auto blockTextInput = false;

    //Ustawienia czcionki
    sf::Text timerText, menuText, optionsText, rankingText, scorText, livesText;
    std::vector<std::string> fonts = {
        getProjectDirectory() + "../../fonts/arial.ttf",
        getProjectDirectory() + "../../fonts/historyofwawa.ttf",
        getProjectDirectory() + "../../fonts/sunflower.ttf"
    };

    int currentFontIndex = 0;
    int fontSize = 40;


    timerText.setFont(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(700, 5);
    timerText.setString("Time: 0");

    menuText.setFont(font);
    menuText.setCharacterSize(40);
    menuText.setColor(sf::Color::White);
    menuText.setString("1.StartGame \n2.Options \n3.Ranking \nESC.Exit");
    menuText.setPosition(700, 450);

    optionsText.setFont(font);
    optionsText.setCharacterSize(40);
    optionsText.setColor(sf::Color::White);
    optionsText.setString("Options:\n1. Change Font\n2. Increase Font Size\n3. Decrease Font Size\nESC. Return");
    optionsText.setPosition(100, 100);

    rankingText.setFont(font);
    rankingText.setCharacterSize(40);
    rankingText.setColor(sf::Color::White);
    rankingText.setString("Ranking\nPress Escape to return");
    rankingText.setPosition(100, 100);

    scorText.setFont(font);
    scorText.setCharacterSize(40);
    scorText.setColor(sf::Color::White);
    scorText.setString("Score:" + std::to_string(score));
    scorText.setPosition(1200, 5);

    livesText.setFont(font);
    livesText.setCharacterSize(40);
    livesText.setColor(sf::Color::White);
    livesText.setString("Lives:" + std::to_string(lives));
    livesText.setPosition(50, 5);



    // Wczytaj słowa z pliku
    std::vector<std::string> wordList = loadWordsFromFile(getProjectDirectory() + "../../wordListEn");
    if (wordList.empty()) {
        std::cerr << "Word list is empty or file not found!" << std::endl;
        return -1;
    }

    std::deque<Word> activeWords;



    if (!font.loadFromFile(fonts[currentFontIndex])) {
        std::cerr << "Nie można załadować czcionki!" << std::endl;
        return -1;
    }
    inputText.setFont(font);
    inputText.setCharacterSize(70);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(10, 1100);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if(currentScreen == Menu) {
                if(event.type == sf::Event::KeyPressed) {
                    if(event.key.code == sf::Keyboard::Num1) {
                        currentScreen = Game;
                        currentInput.clear();
                        activeWords.clear();
                        lives = 3;
                        score = 0;
                        scorText.setString("Score: " + std::to_string(score));
                        gameClock.restart();
                        blockTextInput = true;

                    }
                    if(event.key.code == sf::Keyboard::Num2) {
                        currentScreen = Options;
                    }
                    if(event.key.code == sf::Keyboard::Num3) {
                        currentScreen = Ranking;
                    }
                    if(event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
            }


            if (blockTextInput && event.type == sf::Event::TextEntered) {
                blockTextInput = false;
                continue;
            }

            if (currentScreen == Options) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        currentFontIndex = (currentFontIndex + 1) % fonts.size();
                        if (!font.loadFromFile(fonts[currentFontIndex])) {
                            std::cerr << "Nie można załadować czcionki!" << std::endl;
                        } else {
                            menuText.setFont(font);
                            optionsText.setFont(font);
                            rankingText.setFont(font);
                            scorText.setFont(font);
                            livesText.setFont(font);
                            inputText.setFont(font);
                        }
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        fontSize += 5;
                        menuText.setCharacterSize(fontSize);
                        optionsText.setCharacterSize(fontSize);
                        rankingText.setCharacterSize(fontSize);
                        scorText.setCharacterSize(fontSize);
                        livesText.setCharacterSize(fontSize);
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        if (fontSize > 10) {
                            fontSize -= 5;
                            menuText.setCharacterSize(fontSize);
                            optionsText.setCharacterSize(fontSize);
                            rankingText.setCharacterSize(fontSize);
                            scorText.setCharacterSize(fontSize);
                            livesText.setCharacterSize(fontSize);
                        }
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentScreen = Menu;
                    }
                }
            }

            if (currentScreen == Game) {
                if (event.type == sf::Event::TextEntered && currentScreen == Game) {
                    if (event.text.unicode == '\b' && !currentInput.empty()) {
                        currentInput.pop_back();
                    } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        currentInput += event.text.unicode;
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentScreen = Screen::Menu;
                }

            }

            if (currentScreen == Screen::Options) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentScreen = Screen::Menu;

                    }
                }
            }

            if (currentScreen == Screen::Ranking) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        currentScreen = Screen::Menu;
                    }
                }
            }

        }
        // Renderowanie
        window.clear(sf::Color::Black);

        if (currentScreen == Menu) {
            window.draw(menuText);
        }

        if (currentScreen == Game) {
            int elapsedSeconds = (gameClock.getElapsedTime().asSeconds());
            timerText.setString("Time: " + std::to_string(elapsedSeconds));

            // Spawnowanie nowych słów
            if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
                spawnClock.restart();
                std::string randomWord = wordList[rand() % wordList.size()];
                Word newWord;
                newWord.text.setFont(font);
                newWord.text.setString(randomWord);
                newWord.text.setCharacterSize(45);
                newWord.text.setFillColor(sf::Color::Red);
                newWord.text.setPosition(1600, (rand() % 900) + 100);
                newWord.speed = ( 100 + rand() % 100 ) + gameClock.getElapsedTime().asSeconds();
                activeWords.push_back(newWord);
            }


            // Ruch słów w lewo
            for (auto& word : activeWords) {
                word.text.move(-word.speed * 0.012, 0);
            }

            // Usuwanie słów, które opuściły ekran
            for (auto it = activeWords.begin(); it != activeWords.end();) {
                it->text.move(-it->speed * 0.016, 0);

                if (it->text.getPosition().x + it->text.getGlobalBounds().width < 0) {
                    it = activeWords.erase(it);
                    lives--;
                    livesText.setString("Lives: " + std::to_string(lives));
                } else {
                    ++it;
                }
            }
            if (lives <= 0) {
                currentScreen = Menu;
                lives = 3;
                livesText.setString("Lives: " + std::to_string(lives));
            }

            for (auto it = activeWords.begin(); it != activeWords.end();) {
                if (currentInput == it->text.getString()) {
                    score += 10 * currentInput.size();
                    it = activeWords.erase(it);
                    currentInput.clear();
                    scorText.setString("Score: " + std::to_string(score));
                } else {
                    ++it;
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                currentInput.clear();
            }


            inputText.setString(currentInput);
            window.draw(inputText);
            window.draw(timerText);
            window.draw(scorText);
            window.draw(livesText);

            for (const auto& word : activeWords) {
                window.draw(word.text);
            }
            window.draw(inputText);
        } else if (currentScreen == Options) {

            window.draw(optionsText);
        } else if (currentScreen == Ranking) {
            window.draw(rankingText);
        }
        window.display();
    }
    return 0;
}
