/**
  Fichier qui contient toute la logique du jeu.

  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>

#include <QDebug>
#include <QSettings>

#include "fallingplatform.h"
#include "fragileplatform.h"
#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"

#include "player.h"
#include "solidplatform.h"
#include "transparentplatform.h"

const int SCENE_WIDTH = 1280;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent)
{
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;

    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);

    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));

    // Instancier et initialiser les sprite ici :
    setupPlayer();
    setupPlatforms();

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
}

//! Instancie et initialise le joueur.
void GameCore::setupPlayer()
{
    m_pPlayer = new Player();
    m_pScene->addSpriteToScene(m_pPlayer);
    m_pPlayer->setScale(40.0 / m_pPlayer->width());
    m_pPlayer->updateJumpCharges();

    connect(this, &GameCore::notifyKeyPressed, m_pPlayer, &Player::keyPressed);
    connect(this, &GameCore::notifyKeyReleased, m_pPlayer, &Player::keyReleased);
}

//! Instancie, lie et initialise les plateformes.
void GameCore::setupPlatforms()
{
    m_pPlatforms.append(new SolidPlatform(QRect(0, 200, 200, 20)));
    m_pPlatforms.append(new TransparentPlatform(QRect(200, 200, 100, 20)));
    m_pPlatforms.append(new SolidPlatform(QRect(300, 200, 200, 20)));
    m_pPlatforms.append(new SolidPlatform(QRect(0, 400, 500, 20)));
    m_pPlatforms.append(new FallingPlatform(QRect(500, 400, 200, 20)));
    m_pPlatforms.append(new FragilePlatform(QRect(500, 200, 200, 20)));

    for (auto pPlatform : m_pPlatforms)
    {
        m_pScene->addSpriteToScene(pPlatform);
        m_pScene->registerSpriteForTick(pPlatform);
        connect(pPlatform, &Platform::queuedForDeletion,
                this, &GameCore::spriteQueuedForDeletion);
    }
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore()
{
    delete m_pScene;
    m_pScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key)
{
    emit notifyKeyPressed(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key)
{
    emit notifyKeyReleased(key);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds)
{
    m_pPlayer->tick(static_cast<qreal>(elapsedTimeInMilliseconds));
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
    m_pScene->removeSpriteFromScene(pSprite);
    m_pScene->unregisterSpriteFromTick(pSprite);

    if (auto pPlatform = dynamic_cast<Platform*>(pSprite))
        m_pPlatforms.removeAll(pPlatform);

    delete pSprite;
}
