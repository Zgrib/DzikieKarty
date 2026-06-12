#include <algorithm> // Wymagane dla std::remove_if
#include "GameManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow& window)
    : window_(window), board_(nullptr), selectedCard(nullptr) { // upewnij się, że inicjalizujesz selectedCard

    float boardX = window_.getSize().x * 0.35f;
    float boardY = 0.f;
    float boardWidth = window_.getSize().x * 0.30f;
    float boardHeight = window_.getSize().y * 0.70f;

    sf::FloatRect boardBounds(boardX, boardY, boardWidth, boardHeight);

    // Twoja siatka ma 2 rzędy i 4 kolumny
    board_ = new GameBoard(boardBounds, 2, 4);
    battleEngine_.setGM(this);
    player = battleEngine_.player;
    eai = battleEngine_.eai;
}

void GameManager::initVisualSlots(const sf::Texture& slotTex) {
    visualSlots_.clear();
    if (board_ == nullptr) return;


    int rows = 2;
    int cols = 4;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            VisualSlot vs;
            vs.row = r;
            vs.col = c;
            vs.sprite.setTexture(slotTex);

            // Pobieramy wyliczoną pozycję slotu z GameBoard
            sf::Vector2f pos = board_->getSlotPosition(r, c);

            // Centrujemy teksturę slotu wewnątrz komórki siatki
            float slotW = board_->getSlotWidth();
            float slotH = board_->getSlotHeight();
            float texW = slotTex.getSize().x;
            float texH = slotTex.getSize().y;

            // Opcjonalne skalowanie, jeśli phSlot.png jest za duże/za małe:
            // vs.sprite.setScale(slotW / texW, slotH / texH);

            float paddingX = (slotW - (texW * vs.sprite.getScale().x)) / 2.f;
            float paddingY = (slotH - (texH * vs.sprite.getScale().y)) / 2.f;
            if(r==1) paddingY -=80;
            else paddingY +=80;

            vs.sprite.setPosition(pos.x + paddingX, pos.y + paddingY);
            visualSlots_.push_back(vs);
        }
    }
}

GameManager::~GameManager() {
    delete board_;
}


Player& GameManager::getPlayer(){
    return *player;
}

EnemyAI& GameManager::getAI(){
    return *eai;
}


void GameManager::cleanupDeadCards() {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 2; ++r) {
            Card* cardOnBoard = battleEngine_.board[c][r];
            if (cardOnBoard != nullptr && cardOnBoard->getHealth() <= 0) {
                battleEngine_.board[c][r] = nullptr; // Pole jest teraz bezpiecznie puste
            }
        }
    }

    // 2. Dopiero teraz bezpiecznie usuwamy obiekty z pamięci RAM i z wektora deployedCards_
    for (Card* card : deployedCards_) {
        if (card != nullptr && card->getHealth() <= 0) {
            delete card; // Fizyczne zwolnienie pamięci
        }
    }

    deployedCards_.erase(
        std::remove_if(deployedCards_.begin(), deployedCards_.end(),
                       [](Card* card) {
                           return card == nullptr || card->getHealth() <= 0;
                       }
                       ),
        deployedCards_.end()
        );
}

BattleEngine& GameManager::getBattleEngine(){
    return battleEngine_;
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
    battleEngine_.setCardInSlot(card, row, col);
    // 3. Zapisujemy w wektorze obiektów do wyrenderowania
    deployedCards_.push_back(card);

    return true;
}

void GameManager::removeDeployedCard(Card* card) {
    if (card == nullptr) return;
    deployedCards_.erase(
        std::remove_if(deployedCards_.begin(), deployedCards_.end(),
                       [card](Card* c) { return c == card; }),
        deployedCards_.end()
        );
}


bool GameManager::tryPlayCard(Player& player, Card* card, int targetRow, int targetCol) {
    if (card == nullptr || board_ == nullptr) return false;

    // Sprawdzenie, czy slot docelowy jest wolny
    if (!battleEngine_.isSlotEmpty(targetRow, targetCol)) {
        std::cout << "Ten slot jest juz zajety!\n";
        return false;
    }

    // Ponieważ karty zostały usunięte natychmiast po kliknięciu w Context,
    // tutaj po prostu pozwalamy na zagranie nowej karty!
    placeCard(card, targetRow, targetCol);

    // Usunięcie z ręki gracza
    player.hand.erase(std::remove(player.hand.begin(), player.hand.end(), card), player.hand.end());
    player.updateHandPositions();

    return true;
}

bool GameManager::handleBoardClick(sf::Vector2f mousePos) {
    if (selectedCard == nullptr) return false;

    for (const auto& vs : visualSlots_) {
        if (vs.sprite.getGlobalBounds().contains(mousePos)) {

            if (vs.row != 1) {
                std::cout << "Nie mozesz zagrac karty na strone przeciwnika!\n";
                return false;
            }

            if (!battleEngine_.isSlotEmpty(vs.row, vs.col)) {
                std::cout << "Ten slot jest zajety przez inna karte!\n";
                return false;
            }

            Player* player = battleEngine_.player;

            // Wywołujemy odchudzone tryPlayCard bez przekazywania wektora par
            bool success = tryPlayCard(*player, selectedCard, vs.row, vs.col);

            if (success) {
                std::cout << "Zagrano karte na slot [" << vs.row << ", " << vs.col << "]\n";
                selectedCard->setSelected(false);
                selectedCard = nullptr;
                return true;
            }
        }
    }
    return false;
}

void GameManager::drawBoardElements() {
    for (const auto& vs : visualSlots_) {
        if (battleEngine_.isSlotEmpty(vs.row, vs.col)) {
            window_.draw(vs.sprite);
        }
    }


    for (Card* card : deployedCards_) {
        if (card != nullptr) {
            card->Draw();
        }
    }
    if (battleEngine_.player != nullptr) {
        for (Card* card : battleEngine_.player->hand) {
            if (card != nullptr) {
                card->Draw();
            }
        }
    }
}


Card* GameManager::BuildCard(int _damage, int _health,std::string _name, int _cost, CostType _ct,
                const sf::Texture& _texture, const sf::Texture& _background,
                const sf::Font& _font, sf::RenderWindow &window, int z=0)
{
    Card* c = new Card(_health, _damage, _name, _cost, _ct, _texture, window, z);
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

    CustomTextDrawable* name = new CustomTextDrawable();
    name->text->setString(_name);
    name->setRelativePosition(10, 0);
    name->text->setCharacterSize(24);
    name->text->setFillColor(sf::Color::Black);
    name->text->setFont(_font);
    name->text->setOutlineThickness(1);
    c->addChild(name);

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
    // Zamiast polegać na wskaźnikach wyciąganych z surowej karty, które mogą być uszkodzone,
    // wyciągamy referencję do okna oraz sprawdzamy tekstury.
    sf::RenderWindow& windowRef = const_cast<sf::RenderWindow&>(c->getWindow());

    // Upewniamy się, że czcionka istnieje. Jeśli c->font jest nullem, używamy bezpiecznego fallbacku.
    if (c->font == nullptr) {
        std::cerr << "Blad: Karta poddawana klonowaniu nie posiada przypisanej czcionki!\n";
    }

    // Tworzymy czysty, świeży klon karty za pomocą sprawdzonych struktur BuildCard
    Card* newClone = BuildCard(
        c->getDamage(),
        c->getHealth(),
        c->name,
        c->getCost(),
        c->getCostType(),
        c->spriteTexture,  // Oryginalna tekstura stwora (np. wilk, wąż)
        c->background,     // Oryginalne tło karty
        *(c->font),        // Czcionka
        windowRef,
        0
        );

    return newClone;
}


GameBoard* GameManager::getBoard() const {
    return board_;
}