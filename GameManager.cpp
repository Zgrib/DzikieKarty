#include "GameManager.h"

GameManager::GameManager(sf::RenderWindow& window)
    : window_(window), board_(nullptr) {

    // Obliczamy obszar zajmowany przez tło gry z Context.cpp:
    // x od 25% do 75% szerokości ekranu, y zajmuje 70% wysokości ekranu
    float boardX = window_.getSize().x * 0.35f;
    float boardY = 0.f;
    float boardWidth = window_.getSize().x * 0.30f;
    float boardHeight = window_.getSize().y * 0.70f;

    sf::FloatRect boardBounds(boardX, boardY, boardWidth, boardHeight);

    board_ = new GameBoard(boardBounds, 2, 4);
}

GameManager::~GameManager() {
    delete board_;
}

bool GameManager::placeCard(Card* card, int row, int col) {


    if (board_ == nullptr || card == nullptr) return false;

    if (!battleEngine_.isSlotEmpty(row, col))
        return false;

    // 2. Pobranie dopasowanej pozycji z GameBoard
    sf::Vector2f targetPos = board_->getSlotPosition(row, col);

    // 2. Skalowanie szerokości i wysokości karty
    float cardWidth = card->background.getSize().x * card->getScale().x;
    float cardHeight = card->background.getSize().y * card->getScale().y;

    // 3. Centrowanie w poziomie (oś X) pozostaje bez zmian
    float paddingX = (board_->getSlotWidth() - cardWidth) / 2.f;

    // 4. Pozycjonowanie w pionie (oś Y) z zachowaniem 20px odstępu
    float finalY = 0.f;
    if (row == 0) {
        // Górny rząd: pozycjonujemy przyklejony do dolnej krawędzi swojego slotu
        finalY = targetPos.y + board_->getSlotHeight() - cardHeight - 10.f;
    } else {
        // Dolny rząd: pozycjonujemy przyklejony do górnej krawędzi swojego slotu
        finalY = targetPos.y + 10.f;
    }

    card->setPosition(targetPos.x + paddingX, finalY);

    // 3. Zapisujemy w wektorze obiektów do wyrenderowania
    deployedCards_.push_back(card);

    return true;
}

void GameManager::drawBoardElements() {
    for (Card* card : deployedCards_) {
        if (card != nullptr) {
            card->Draw();
        }
    }
}



Card* GameManager::BuildCard(int _damage, int _health, int _cost, CostType _ct,
                const sf::Texture& _texture, const sf::Texture& _background,
                const sf::Font& _font, sf::RenderWindow &window, int z=0)
{
    Card* c = new Card(_health, _damage, _cost, _ct, _texture, window, z);
    c->background = _background;
    c->setTexture(c->background);
    c->font = &_font;
    c->window = &window;

    c->spriteTexture = _texture;
    CustomDrawable* cardSprite = new CustomDrawable();
    cardSprite->setTexture(c->spriteTexture);
    cardSprite->setRelativePosition(9, 36);
    cardSprite->setScale(0.89,0.89);
    c->addChild(cardSprite);


    // std::cout << "spriteTexture size: " << c->spriteTexture.getSize().x << "x" << c->spriteTexture.getSize().y << "\n";
    // std::cout << "cardSprite texture ptr: " << cardSprite->getTexture() << "\n";
    // std::cout << "cardSprite textureRect: " << cardSprite->getTextureRect().width << "x" << cardSprite->getTextureRect().height << "\n";




    CustomTextDrawable* health = new CustomTextDrawable(&(c->health));
    health->setRelativePosition(90, 140);
    health->text->setCharacterSize(24);
    health->text->setFillColor(sf::Color::Black);
    health->text->setFont(_font);
    health->text->setOutlineThickness(1);
    c->addChild(health);

    CustomTextDrawable* damage = new CustomTextDrawable(&(c->damage));
    damage->setRelativePosition(14, 140);
    damage->text->setCharacterSize(24);
    damage->text->setFillColor(sf::Color::Black);
    damage->text->setFont(_font);
    damage->text->setOutlineThickness(1);
    c->addChild(damage);

    CustomTextDrawable* cost = new CustomTextDrawable(&(c->cost));
    cost->setRelativePosition(60, 30);
    cost->text->setCharacterSize(12);
    cost->text->setFillColor(sf::Color::Black);
    cost->text->setFont(_font);
    cost->text->setOutlineThickness(1);
    c->addChild(cost);

    if(_ct == CostType::BLOOD)
        cost->after = " Blood";
    else
        cost->after = " Bones";

    return c;

}



Card* GameManager::cloneCard(const Card* c){
    sf::RenderWindow& windowRef = const_cast<sf::RenderWindow&>(c->getWindow());
    const sf::Texture* currentSpriteTex = c->sprite.getTexture();
    return BuildCard(
        c->getDamage(),
        c->getHealth(),
        c->getCost(),
        c->getCostType(),
        currentSpriteTex ? *currentSpriteTex : c->spriteTexture,
        c->background,
        *(c->font),
        windowRef
        );




    //return BuildCard(c->health, c->damage, c->cost, c->costType_, *(c->sprite.getTexture(),c->getTexture(), c->getWindow()));
}



GameBoard* GameManager::getBoard() const {
    return board_;
}