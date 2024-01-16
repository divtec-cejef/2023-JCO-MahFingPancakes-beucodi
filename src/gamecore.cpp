/**
  Fichier qui contient toute la logique du jeu.

  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <QSettings>
#include <QThread>

#include "gamescene.h"
#include "gamecanvas.h"
#include "levelbuilder.h"
#include "level.h"
#include "utilities.h"

#include "player.h"
#include "transparentplatform.h"

constexpr int SCENE_WIDTH = 1280;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent)
{
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    m_pPlayer = new Player();
    m_pPlayer->setScale(40.0 / m_pPlayer->width());

    connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::keyPressed);
    connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::keyReleased);
    connect(m_pPlayer, &Player::playerDied, this, &GameCore::onPlayerDied);

    m_pLevel = LevelBuilder(QPoint(0, 0)).build(this, m_pPlayer, GameFramework::NEUTRAL);
    m_pPlayer->initialize();
    m_pLevel->initialize();

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
    qDebug() << "GameCore Thread:" << QThread::currentThreadId();
}

//! Change le niveau actuel.
//! \param targetLevel  Position du niveau cible.
//! \param dir          Direction dans laquelle le joueur entre.
void GameCore::changeLevel(const QPoint targetLevel, const GameFramework::Direction dir)
{
    m_pGameCanvas->stopTick();
    for (const auto level : m_pLevel->neighbouringLevels())
    {
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
Level* GameCore::currentLevel() const
{
    return m_pLevel;
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore()
{
    delete m_pLevel;
    m_pLevel = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyPressed(const int key)
{
    emit notifyKeyPressed(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(const int key)
{
    emit notifyKeyReleased(key);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds)
{
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition)
{
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons)
{
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons)
{
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

//! Traite la destruction d'un sprite.
//! \param pSprite Sprite qui doit être détruit.
void GameCore::spriteQueuedForDeletion(Sprite* pSprite)
{
    m_pLevel->scene()->removeSpriteFromScene(pSprite);
    m_pLevel->scene()->unregisterSpriteFromTick(pSprite);

    if (const auto pPlatform = dynamic_cast<Platform*>(pSprite))
        m_pPlatforms.removeAll(pPlatform);

    delete pSprite;
}

//! Retourne le canvas de l'application
GameCanvas* GameCore::canvas() const
{
    return m_pGameCanvas;
}

//! Fonction à appeler quand le joueur meurt
void GameCore::onPlayerDied()
{
    m_pGameCanvas->stopTick();
    m_pPlayer->pack();
    m_pLevel->deleteLater();
    qDebug() << m_pLevel->scene()->sprites().length();
    m_pLevel = LevelBuilder(QPoint(0, 0)).build(this, m_pPlayer, GameFramework::NEUTRAL);
    qDebug() << m_pLevel->scene()->sprites().length();
    m_pGameCanvas->startTick();
}
