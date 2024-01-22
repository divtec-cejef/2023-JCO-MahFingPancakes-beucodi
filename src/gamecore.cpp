/**
  Fichier qui contient toute la logique du jeu.

  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <QThread>
#include <QPainter>

#include "gamecanvas.h"
#include "levelbuilder.h"
#include "level.h"
#include "utilities.h"
#include "resources.h"

#include "player.h"
#include "transparentplatform.h"

constexpr int SCENE_WIDTH = 1280;
constexpr int SCENE_HEIGHT = SCENE_WIDTH * 9 / 16;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas *pGameCanvas, QObject *pParent) : QObject(pParent) {
    // Mémorise l’accès au canvas (qui gère le tick et l’affichage d’une scène)
    m_pGameCanvas = pGameCanvas;

    m_pPlayer = new Player();

    connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::keyPressed);
    connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::keyReleased);
    connect(this, &GameCore::notifyMouseButtonPressed, m_pPlayer, &Player::mouseButtonPressed);
    connect(m_pPlayer, &Player::playerDied, this, &GameCore::onPlayerDied);

    m_pLevel = LevelBuilder(QPoint(0, 0)).build(this, m_pPlayer, GameFramework::NEUTRAL);
    m_pPlayer->initialize();
    m_pLevel->initialize();

    // Commence le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l’enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu’au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le débogueur est démarré.
    m_pGameCanvas->startTick();
}

//! Change le niveau actuel.
//! \param targetLevel  Position du niveau cible.
//! \param dir          Direction dans laquelle le joueur entre.
void GameCore::changeLevel(const QPoint targetLevel, const GameFramework::Direction dir) {
    m_pGameCanvas->stopTick();
    for (const auto level: m_pLevel->neighbouringLevels()) {
        if (level->levelId() != targetLevel)
            continue;

        m_pPlayer->pack();
        m_pLevel->deleteLater();
        m_pLevel = level->build(this, m_pPlayer, dir);
        m_pLevel->initialize();
        break;
    }
    m_pGameCanvas->startTick();
}

//! Retourne le niveau actuel.
//! \return Niveau actuel.
Level *GameCore::currentLevel() const {
    return m_pLevel;
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pLevel;
    m_pLevel = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyPressed(const int key) {
    emit notifyKeyPressed(key);
    if (key == Qt::Key_Escape) {
        m_pGameCanvas->stopTick();
        if (m_pLevel != nullptr) {
            m_pPlayer->pack();
            m_pLevel->deleteLater();
        }

        delete m_pPlayer;

        m_pPlayer = new Player();
        connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::keyPressed);
        connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::keyReleased);
        connect(this, &GameCore::notifyMouseButtonPressed, m_pPlayer, &Player::mouseButtonPressed);
        connect(m_pPlayer, &Player::playerDied, this, &GameCore::onPlayerDied);
        m_pLevel = LevelBuilder(QPoint(0, 0)).build(this, m_pPlayer, GameFramework::NEUTRAL);
        m_pPlayer->initialize();
        m_pLevel->initialize();
        m_pGameCanvas->startTick();
    }
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(const int key) {
    emit notifyKeyReleased(key);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

//! Traite la destruction d'un sprite.
//! \param pSprite Sprite qui doit être détruit.
void GameCore::spriteQueuedForDeletion(Sprite *pSprite) {
    m_pLevel->scene()->removeSpriteFromScene(pSprite);
    m_pLevel->scene()->unregisterSpriteFromTick(pSprite);

    if (const auto pPlatform = dynamic_cast<Platform *>(pSprite))
        m_pPlatforms.removeAll(pPlatform);

    delete pSprite;
}

//! Retourne le canvas de l'application
GameCanvas *GameCore::canvas() const {
    return m_pGameCanvas;
}

//! Fonction à appeler quand le joueur meurt
void GameCore::onPlayerDied() {
    m_pGameCanvas->stopTick();
    m_pPlayer->pack();
    m_pLevel->deleteLater();
    m_pLevel = nullptr;
    auto newScene = m_pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    m_pGameCanvas->setCurrentScene(newScene);
    m_pPlayer->deleteLater();
    m_pPlayer = nullptr;

    auto deathMessagePixmap = QPixmap(600, 200);
    deathMessagePixmap.fill(QColorConstants::Transparent);
    QPainter messagePainter(&deathMessagePixmap);
    messagePainter.setFont(QFont("Arial", 32));
    messagePainter.setPen(QPen(QColorConstants::Red));
    messagePainter.drawText(QRectF(0, 0, 600, 200), Qt::AlignCenter, "Vous êtes mort !");
    auto spriteMsg = new Sprite(deathMessagePixmap);
    newScene->addSpriteToScene(spriteMsg);
    spriteMsg->setPos(SCENE_WIDTH / 2.0 - spriteMsg->width() / 2.0, 200 - spriteMsg->height() / 2.0);

    const int toolTipWidth = 600;
    const int toolTipHeight = 200;
    const int keySpriteWidth = 40;

    auto tooltipPixmap = QPixmap(toolTipWidth, toolTipHeight);
    tooltipPixmap.fill(QColorConstants::Transparent);
    QPainter tooltipPainter(&tooltipPixmap);
    tooltipPainter.setFont(QFont("Arial", 16));
    tooltipPainter.setPen(QPen(QColorConstants::White));
    QRect requiredBounding;
    tooltipPainter.drawText(
            QRect(
                    0,
                    0,
                    toolTipWidth / 2.0,
                    toolTipHeight
            ),
            Qt::AlignLeft | Qt::AlignVCenter,
            "Appuyez sur",
            &requiredBounding
    );

    auto keySprite = QImage();
    keySprite.load(
            QString("%1/keys/Esc_Key_Light.png")
                    .arg(GameFramework::imagesPath())
    );

    tooltipPainter.drawImage(
            QPoint(
                    requiredBounding.width(),
                    toolTipHeight / 2.0 - keySpriteWidth / 2.0
            ),
            keySprite.scaledToWidth(keySpriteWidth, Qt::SmoothTransformation)
    );


    tooltipPainter.drawText(
            QRectF(
                    requiredBounding.width() + keySpriteWidth,
                    0,
                    toolTipWidth / 2.0 - keySpriteWidth,
                    toolTipHeight
            ),
            Qt::AlignLeft | Qt::AlignVCenter,
            "pour recharger le jeu."
    );
    auto spriteTooltip = new Sprite(tooltipPixmap);
    newScene->addSpriteToScene(spriteTooltip);
    spriteTooltip->setPos(
            SCENE_WIDTH / 2.0 - spriteTooltip->shape().boundingRect().width() / 2.0,
            400 - toolTipHeight / 2.0
    );

    m_pGameCanvas->startTick();
}
