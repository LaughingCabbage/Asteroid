#include "Menu.h"
#include "hsManager.h"

MenuButton::MenuButton(sf::Text& text, sf::RectangleShape& box){
    this->buttonText = text;
    this->buttonBox = box;
    buttonBox.setOrigin(buttonBox.getLocalBounds().width/2, buttonBox.getLocalBounds().height/2);

}

void MenuButton::setPosition(sf::Vector2f position){
    buttonBox.setPosition(position);
    buttonText.setPosition(position);

}

void MenuButton::drawButton(sf::RenderWindow* window){
    window->draw(buttonBox);
    window->draw(buttonText);

}

void MenuButton::select(){
    buttonText.setColor(sf::Color::Yellow);
    buttonBox.setFillColor(sf::Color::Green);

}

void MenuButton::clearSelect(){
    buttonText.setColor(sf::Color::Black);
    buttonBox.setFillColor(sf::Color::Yellow);

}


Menu::Menu(sf::RenderWindow* window){
    this->menuWindow = window;
    this->menuOptions = play;
    setup();
}
Menu::~Menu(){}

void Menu::setup(){
    if(!menuFont.loadFromFile("pixelmix.ttf")){
        std::cout << "font load error\n";
    }
    HsManager.setWindow(menuWindow);
    HsManager.setup();
    addOption("Play", sf::Vector2f(400,150));
    addOption("Score", sf::Vector2f(400, 300));
    addOption("Options", sf::Vector2f(400, 450));
    inputRate = 0.2;
    menuButtons.at(0).select();

}

void Menu::addOption(std::string option, sf::Vector2f position){
    sf::RectangleShape box(sf::Vector2f(200, 100));
    box.setFillColor(sf::Color::Yellow);
    sf::Text text(option, menuFont);
    //center text
    text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    text.setColor(sf::Color::Black);
    MenuButton button(text, box);
    button.setPosition(position);
    menuButtons.push_back(button);

}

void Menu::renderMenu(){
    menuWindow->clear();
    parseMenuInput();
    if(!menuButtons.empty()){
        for(std::size_t i = 0; i < menuButtons.size(); ++i){
            menuButtons[i].drawButton(menuWindow);
        }
    }
    menuWindow->display();
}

void Menu::selectUp(){

    menuButtons.at(menuOptions).clearSelect();
    if(menuOptions == play){
        menuOptions = options;
    }else if(menuOptions == score){
        menuOptions = play;
    }else{
        menuOptions = score;
    }
    menuButtons.at(menuOptions).select();

}

void Menu::selectDown(){
    menuButtons.at(menuOptions).clearSelect();
    if(menuOptions == options){
        menuOptions = play;
    }else if(menuOptions == score){
        menuOptions = options;
    }else{
        menuOptions = score;
    }
    menuButtons.at(menuOptions).select();
}

void Menu::playGame(){
    int player_score;
    std::string player_name = "LaughingCabbage";
    Game Asteroid(menuWindow);
    Asteroid.setup();
    Asteroid.gameLoop(player_score, player_name);
    if(player_name.size() >= 2){
        std::cout << "name okay\n";
        HsManager.writeScore(player_score, player_name);
    }
}

void Menu::showHighScore(){
    HsManager.loadScores();
    HsManager.display();

}

void Menu::selectOption(){
    switch(menuOptions){
        case play:
            playGame();
        break;
        case score:
            showHighScore();
        break;
        case options:
            //options
        break;

    }
}

void Menu::parseMenuInput(){
    sf::Event evt;
    //Loop through all window events
    while(menuWindow->pollEvent(evt)){
        //request closing window
        if(evt.type == sf::Event::Closed){
            menuWindow->close();
        }
    }
    //restrict key selections to 0.5 per second
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && (inputClock.getElapsedTime().asSeconds() > inputRate)){
        selectUp();
        inputClock.restart();
    }
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && (inputClock.getElapsedTime().asSeconds() > inputRate)){
        selectDown();
        inputClock.restart();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
        selectOption();
    }

}
