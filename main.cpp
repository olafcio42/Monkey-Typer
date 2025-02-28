#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iostream>

struct Word {
    std::string text;
    sf::Text sfText;
    float speed;
};

std::vector<std::string> loadWordsFromFile(const std::string &filePath) {
    std::vector<std::string> words;
    std::ifstream file(filePath);
    std::string word;

    if (file.is_open()) {
        while (std::getline(file, word)) {
            words.push_back(word);
        }
    }
    return words;
}

int main() {
    std::vector<std::string> englishWords = loadWordsFromFile("C:\\Users\\olafo\\CLionProjects\\Projekt_PJC\\assets\\english.txt");
    std::vector<std::string> polishWords = loadWordsFromFile("C:\\Users\\olafo\\CLionProjects\\Projekt_PJC\\assets\\polish.txt");
    std::vector<std::string> germanWords = loadWordsFromFile("C:\\Users\\olafo\\CLionProjects\\Projekt_PJC\\assets\\germany.txt");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Monkey Typer");
    window.setFramerateLimit(60);
    sf::Font font, font2;

    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf") || !font2.loadFromFile("C:\\Windows\\Fonts\\times.ttf")) {
        std::cerr << "Error loading fonts!" << std::endl;
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Word> activeWords;

    sf::Clock wordClock;
    float wordInterval = 2.0f;
    sf::Clock deltaClock;
    std::string currentInput;

    int missedWords = 0;
    const int maxMissed = 10;

    int score = 0;
    int lives = 3;

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Green);
    inputText.setPosition(10, 550);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(650, 10);
    scoreText.setString("Score: 0");

    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(10, 10);
    livesText.setString("Lives: 3");

    sf::Clock speedUpClock;
    float speedUpInterval = 10.0f;
    bool gameOver = false;

    sf::RectangleShape restartButton(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(350, 350);

    sf::Text restartButtonText;
    restartButtonText.setFont(font);
    restartButtonText.setString("Start New Game");
    restartButtonText.setCharacterSize(24);
    restartButtonText.setFillColor(sf::Color::White);
    restartButtonText.setPosition(370, 365);

    sf::Text menuText;
    menuText.setFont(font);
    menuText.setString("Press Enter to Start");
    menuText.setCharacterSize(36);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(250, 250);

    sf::Text difficultyMenuText;
    difficultyMenuText.setFont(font);
    difficultyMenuText.setString("Select Difficulty: 1 - Easy, 2 - Medium, 3 - Hard");
    difficultyMenuText.setCharacterSize(24);
    difficultyMenuText.setFillColor(sf::Color::White);
    difficultyMenuText.setPosition(150, 300);

    sf::Text languageMenuText;
    languageMenuText.setFont(font);
    languageMenuText.setString("Select Language: 1 - English, 2 - Polish, 3 - German");
    languageMenuText.setCharacterSize(24);
    languageMenuText.setFillColor(sf::Color::White);
    languageMenuText.setPosition(150, 300);

    sf::Text colorMenuText;
    colorMenuText.setFont(font);
    colorMenuText.setString("Select Word Color: 1 - Red, 2 - Green, 3 - Blue");
    colorMenuText.setCharacterSize(24);
    colorMenuText.setFillColor(sf::Color::White);
    colorMenuText.setPosition(150, 300);

    sf::Text changeDifficultyText;
    changeDifficultyText.setFont(font);
    changeDifficultyText.setString("Press P to change difficulty");
    changeDifficultyText.setCharacterSize(24);
    changeDifficultyText.setFillColor(sf::Color::White);
    changeDifficultyText.setPosition(250, 390);

    sf::Text changelanguageMenuText;
    changelanguageMenuText.setFont(font);
    changelanguageMenuText.setString("Press L to change language");
    changelanguageMenuText.setCharacterSize(24);
    changelanguageMenuText.setFillColor(sf::Color::White);
    changelanguageMenuText.setPosition(250, 420); //ziolko 420 heheheheheheh XDDDD jaranko

    sf::Text changeFontText;
    changeFontText.setFont(font);
    changeFontText.setString("Press F to change font");
    changeFontText.setCharacterSize(24);
    changeFontText.setFillColor(sf::Color::White);
    changeFontText.setPosition(250, 450);

    sf::Text changeColorText;
    changeColorText.setFont(font);
    changeColorText.setString("Press C to change color");
    changeColorText.setCharacterSize(24);
    changeColorText.setFillColor(sf::Color::White);
    changeColorText.setPosition(250, 480);

    auto restartGame = [&]() {
        activeWords.clear();
        currentInput.clear();
        score = 0;
        lives = 3;
        missedWords = 0;
        gameOver = false;
        wordClock.restart();
        speedUpClock.restart();
    };

    bool inMenu = true;
    bool inDifficultyMenu = false;
    bool inLanguageMenu = false;
    bool inColorMenu = false;
    int difficultyLevel = 1;
    std::vector<std::string> currentWordList = englishWords;

    sf::Font* currentFont = &font;
    sf::Color wordColor = sf::Color::Green;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameOver && event.type == sf::Event::MouseButtonPressed) {
                if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    restartGame();
                }
            }

            if (inDifficultyMenu && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    difficultyLevel = 1;
                    wordInterval = 3.0f;
                    speedUpInterval = 10.0f;
                    inDifficultyMenu = false;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    difficultyLevel = 2;
                    wordInterval = 2.0f;
                    speedUpInterval = 8.0f;
                    inDifficultyMenu = false;
                } else if (event.key.code == sf::Keyboard::Num3) {
                    difficultyLevel = 3;
                    wordInterval = 0.5f;
                    speedUpInterval = 5.0f;
                    inDifficultyMenu = false;
                }
            }

            if (inLanguageMenu && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    currentWordList = englishWords;
                    inLanguageMenu = false;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    currentWordList = polishWords;
                    inLanguageMenu = false;
                } else if (event.key.code == sf::Keyboard::Num3) {
                    currentWordList = germanWords;
                    inLanguageMenu = false;
                }
            }

            if (inColorMenu && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    wordColor = sf::Color::Red;
                    inColorMenu = false;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    wordColor = sf::Color::Green;
                    inColorMenu = false;
                } else if (event.key.code == sf::Keyboard::Num3) {
                    wordColor = sf::Color::Blue;
                    inColorMenu = false;
                }
            }
                if (!gameOver && !inMenu && !inDifficultyMenu && !inLanguageMenu && !inColorMenu && event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8) {
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                        }
                    } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
                        currentInput += static_cast<char>(event.text.unicode);
                    }

                    inputText.setString(currentInput);

                    auto it = std::find_if(activeWords.begin(), activeWords.end(),
                        [&](const Word &w) { return w.text == currentInput; });

                    if (it != activeWords.end()) {
                        activeWords.erase(it);
                        currentInput.clear();
                        inputText.setString(currentInput);
                        if (it->text.size() > 5) {
                            score += 20;
                        } else {
                            score += 10;
                        }
                        scoreText.setString("Score: " + std::to_string(score));
                    }
                }

            if (inMenu && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                inMenu = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                if (!gameOver && inMenu) {
                    inDifficultyMenu = true;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) {
                if (!gameOver && inMenu) {
                    inLanguageMenu = true;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
                if (!gameOver && inMenu) {
                    inColorMenu = true;
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
                if (gameOver || inMenu || inDifficultyMenu || inLanguageMenu || inColorMenu) {
                    if (currentFont == &font) {
                        currentFont = &font2;
                    } else {
                        currentFont = &font;
                    }
                    inputText.setFont(*currentFont);
                    scoreText.setFont(*currentFont);
                    livesText.setFont(*currentFont);
                    restartButtonText.setFont(*currentFont);
                    menuText.setFont(*currentFont);
                    difficultyMenuText.setFont(*currentFont);
                    languageMenuText.setFont(*currentFont);
                    changeDifficultyText.setFont(*currentFont);
                    changelanguageMenuText.setFont(*currentFont);
                    changeFontText.setFont(*currentFont);
                    colorMenuText.setFont(*currentFont);
                    changeColorText.setFont(*currentFont);
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (!gameOver) {
                    inMenu = true;
                    restartGame();
                }
            }
        }
        if (!gameOver && !inMenu && !inDifficultyMenu && !inLanguageMenu && !inColorMenu && wordClock.getElapsedTime().asSeconds() >= wordInterval) {
            Word newWord;
            std::uniform_int_distribution<int> dist(0, currentWordList.size() - 1);
            newWord.text = currentWordList[dist(gen)];
            newWord.sfText.setFont(*currentFont);
            newWord.sfText.setString(newWord.text);
            newWord.sfText.setCharacterSize(24);
            newWord.sfText.setFillColor(wordColor);
            std::uniform_int_distribution<int> yDist(0, 570);
            newWord.sfText.setPosition(800, static_cast<float>(yDist(gen)));
            std::uniform_int_distribution<int> speedDist(100, 200);
            newWord.speed = static_cast<float>(speedDist(gen));
            activeWords.push_back(newWord);
            wordClock.restart();
        }

        if (!gameOver && !inMenu && !inDifficultyMenu && !inLanguageMenu && speedUpClock.getElapsedTime().asSeconds() >= speedUpInterval) {
            for (auto &w : activeWords) {
                w.speed += 20.0f;
            }
            speedUpClock.restart();
        }

        float deltaTime = deltaClock.restart().asSeconds();
        for (auto &w : activeWords) {
            w.sfText.move(-w.speed * deltaTime, 0);
        }

        for (auto it = activeWords.begin(); it != activeWords.end(); ) {
            if (it->sfText.getPosition().x + it->sfText.getLocalBounds().width < 0) {
                it = activeWords.erase(it);
                missedWords++;
                if (missedWords >= 1) {
                    lives--;
                    missedWords = 0;
                    if (lives <= 0) {
                        gameOver = true;
                    }
                }
            } else {
                ++it;
            }
        }
        livesText.setString("Lives: " + std::to_string(lives));

        window.clear();

        if (inDifficultyMenu) {
            window.draw(difficultyMenuText);
        }
        else if (inLanguageMenu) {
            window.draw(languageMenuText);
        }
        else if (inColorMenu) {
            window.draw(colorMenuText);
        }
        else if (inMenu) {
            window.draw(menuText);
            window.draw(changeDifficultyText);
            window.draw(changelanguageMenuText);
            window.draw(changeFontText);
            window.draw(changeDifficultyText);
            window.draw(changeColorText);
        } else {
            for (const auto &w : activeWords) {
                window.draw(w.sfText);
            }
            window.draw(inputText);
            window.draw(scoreText);
            window.draw(livesText);

            if (gameOver) {
                sf::Text gameOverText;
                gameOverText.setFont(*currentFont);
                gameOverText.setCharacterSize(36);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setString("Game Over\nYour Score: " + std::to_string(score));
                gameOverText.setPosition(250, 250);
                window.draw(gameOverText);
                window.draw(restartButton);
                window.draw(restartButtonText);
            }
        }
        window.display();
    }
    return 0;
}
