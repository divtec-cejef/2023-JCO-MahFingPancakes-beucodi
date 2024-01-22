/*! \mainpage 2023-JCO-MahFingPancakes-beucodi
 *
 * \author Odin Beuchat
 * \section intro_sec Introduction
 *
 * Ce projet est un jeu vidéo de type "MetroidVania", c'est-à-dire un jeu d'action/plateforme
 * dans lequel le joueur doit explorer un monde ouvert, découvrir des zones cachées et
 * acquérir de nouvelles capacités qui lui permettront d'explorer de nouvelles zones.
 *
 * Le but de ce projet est de développer un jeu vidéo en utilisant le framework Qt, ainsi
 * qu'explorer le langage C++ et les concepts de la programmation orientée objet.
 *
 * \section architecture_sec Architecture
 * La classe MainFrm se charge d'afficher la fenêtre de l'application. Elle utilise le
 * fichier `mainfrm.ui` pour construire l'interface utilisateur.
 * Par défaut, cette interface utilisateur intègre un élément de type GameView
 * qui se charge d'afficher la scene graphique (qui elle est de type GameScene).
 *
 * Au moment de la construction (MainFrm()) d'une instance MainFrm, un cadre
 * de jeu (GameCanvas) est créé.
 * Ce cadre de jeu gère la cadence du jeu (le tick) et délègue toute la logique
 * du jeu à un objet de type GameCore, qu'il crée.
 *
 * GameCore est en charge de créer un objet de type LevelBuilder, qui va chercher le fichier
 * de données du premier niveau (0-0.lvl), le lire et créer ses objets. Gamecore va ensuite
 * appeler LevelBuilder::build(), qui va instancier un objet de type Level, et lui passer
 * tous les objets créés. Level::Level() s'occupe de créer la GameScene avec GameCanvas::createScene(),
 * puis l'affecte au GameCanvas, au travers du pointeur de GameCore, qui est passé en paramètre à
 * LevelBuilder::build().
 *
 * Pendant que le niveau est créé, LevelBuilder::build() lance un thread qui s'occupe de parcourir
 * les portes du niveau, et de créer un LevelBuilder pour chacun des niveaux correspondants.
 * Ensuite, les LevelBuilder sont affectés à une liste dans le Level. A chaque création de LevelBuilder,
 * le fichier de niveau est automatiquement lu et traité. Ainsi, les LevelBuilder liés sont prêts à être
 * utilisés.
 *
 * Voici un diagramme de séquence qui montre la séquence de démarrage du jeu.
 *
 * @startuml "Diagramme de séquence" width=100%
 * hide footbox
 * actor Joueur
 * participant MainFrm
 * participant GameView
 * participant GameCanvas
 * participant GameCore
 * participant LevelBuilder
 * participant fstream
 * participant Level
 * participant GameScene
 * participant LevelBuilder as LB
 *
 * Joueur ->> MainFrm: Démarrer le jeu
 * activate MainFrm
 * create GameView
 * MainFrm --> GameView: <<create>>
 * activate GameView
 * GameView -> GameView: init()
 * activate GameView
 * deactivate GameView
 * GameView -->> MainFrm: <<return>>
 * deactivate GameView
 *
 * create GameCanvas
 * MainFrm -->> GameCanvas: <<create>>
 * activate GameCanvas
 * GameCanvas -> GameCanvas: initDetailedInfos()
 * activate GameCanvas
 * deactivate GameCanvas
 * [<<-- GameCanvas: <<event>> \n onInit()
 * GameCanvas -->> MainFrm: <<return>>
 * deactivate GameCanvas
 * deactivate MainFrm
 * [-->> GameCanvas: <<event>> \n onInit()
 * activate GameCanvas
 * create GameCore
 * GameCanvas --> GameCore: <<create>>
 * activate GameCore
 * deactivate GameCanvas
 *
 * create LevelBuilder
 * GameCore --> LevelBuilder: <<create>>
 * activate LevelBuilder
 *
 * LevelBuilder -> fstream : Lis le fichier de niveau
 * activate fstream
 * fstream --> LevelBuilder: <<return>> \n les lignes du fichier
 * deactivate fstream
 *
 * loop for each line
 * 	LevelBuilder --> LevelBuilder: <<create>> \n l'élément correspondant
 * 	activate LevelBuilder
 * 	LevelBuilder --> LevelBuilder: Ajoute l'élément à la liste
 * 	deactivate LevelBuilder
 * end
 *
 * LevelBuilder -->> GameCore: <<return>>
 * deactivate LevelBuilder
 *
 * GameCore -> LevelBuilder: build()
 * activate LevelBuilder
 * create Level
 * LevelBuilder --> Level: <<create>>
 * activate Level
 * Level -> GameCanvas: createScene()
 * activate GameCanvas
 * create GameScene
 * GameCanvas --> GameScene: <<create>>
 * GameCanvas -->> Level: <<return>> \n GameScene
 * deactivate GameCanvas
 * Level -> GameCanvas: setCurrentScene(GameScene)
 * activate GameCanvas
 * GameCanvas -->> Level: <<return>>
 * deactivate GameCanvas
 * Level -->> LevelBuilder: <<return>>
 * deactivate Level
 *
 * loop for each created sprite
 * 	LevelBuilder -> GameScene: addSpriteToScene(sprite)
 * 	activate GameScene
 * 	GameScene -->> LevelBuilder: <<return>>
 * 	deactivate GameScene
 * end
 *
 *
 * deactivate LevelBuilder
 * LevelBuilder -->> LevelBuilder: new QThread(LevelBuilder::discoverLevels)
 *
 * activate LevelBuilder
 * loop for each sprite
 * 	alt sprite is a door
 * 		create LB
 * 		LevelBuilder --> LB: <<create>>
 * 		activate LB
 * 		LB -> fstream: Lis le fichier de niveau
 * 		activate fstream
 * 		fstream -->> LB: <<return>>
 * 		deactivate fstream
 *
 * 		loop for each line
 * 			LB -> LB: <<create>> \n l'élément correspondant
 * 			activate LB
 * 			LB -->> LB: Ajoute l'élément à la liste
 * 			deactivate LB
 * 		end
 *
 * 		LB -->> LevelBuilder: <<return>>
 * 		deactivate LB
 * 		LevelBuilder -> Level: appendLevel(LevelBuilder)
 * 		activate Level
 * 		Level -->> LevelBuilder: <<return>>
 * 		deactivate Level
 * 	end
 * end
 * LevelBuilder -->> GameCore: <<return>> \n Level
 * deactivate LevelBuilder
 * GameCore --> LevelBuilder: ~LevelBuilder()
 * destroy LevelBuilder
 * GameCore -> GameCanvas: startTick()
 * activate GameCanvas
 * deactivate GameCore
 * deactivate GameCanvas
 *
 * loop
 * [-->> GameCanvas: onTick()
 * activate GameCanvas
 * GameCanvas -> GameCore: tick()
 * activate GameCore
 * deactivate GameCore
 * GameCanvas -> GameScene: tick()
 * activate GameScene
 * deactivate GameScene
 * end
 * @enduml
 *
 * La plupart des jeux d'action ont besoin d'un timing régulier, permettant de
 * déplacer les sprites, détecter les collisions et analyser l'état du jeu.
 * C'est la cadence du jeu.
 *
 * La classe GameCanvas est capable de générer un tel timing, appelé tick. Chaque fois
 * qu'un tick survient, la méthode GameCore::tick() est automatiquement appelée par GameCanvas.
 * Ensuite, ce tick est propagé aux différents sprites de la scène, afin qu'ils puissent
 * y réagir.
 *
 * Par défaut, le tick survient toutes les 20 ms (environ). L'intégralité du code qui
 * est exécuté à chaque tick ne devrait donc pas durer plus de 20 ms.
 * Avec une cadence de 20 ms, on obtient 50 images par secondes.
 *
 * Les méthodes suivantes permettent de manipuler ce tick :
 * - GameCanvas::startTick() : Cette fonction démarre le timing, ce qui a pour effet
 * d'appeler la fonction GameCore::tick() de façon régulière.
 * - GameCanvas::stopTick() : Cette fonction interrompt le timing. La fonction GameCore::tick() n'est plus appelée.
 *
 * Voici un diagramme de classes simplifié qui offre une vue globale des classes qui
 * compose de projet.
 * @startuml "Diagramme de classes simplifié" width=100%
hide circle
skinparam classAttributeIconSize 0
!theme plain

class LevelBuilder {
	+ LevelBuilder(levelId: QPoint)
	+ ~LevelBuilder
	+ build(pCode: GameCore *, pPlayer: Player *, enteredFrom: Direction): Level *
	+ LevelId(): QPoint
	- m_pSprites: QList<Sprite *>
	- m_levelId: QPoint
	- loadNeighbouringLevels()
	- m_pLevel: Level *
	- m_discoveryThread: QThread
	- m_spawnPoint: QPointF
}

Level "1" -o "0..1" LevelBuilder

class Body {
	+ Body(rImagePath: QString, pParent: QGraphicsItem*)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ isAirborne(): boolean
	+ velocity(): QPointF
	+ setVelocity(velocity: QPointF)
	# m_velocity: QPointF
	# m_acceleration: QPointF
	# m_maxSpeedX: qreal
	# m_terminalVelocity: qreal
	# computeGravity(elapsedTimeInMilliseconds: long long)
	- collideWithPlatform(platform: Platform *)
}

Sprite <|-- Body

class Door {
	+ Door(pos: QPoint, target: QPoint, dir: Direction)
	+ travel()
	+ targetLevel(): QPoint
	- m_targetLevel: QPoint
	- m_pImage: QImage *
	- m_dir: Direction
	-- signals --
	+doorEntered(targetLevel: QPoint, dir: Direction)
}

Sprite <|-- Door

class Enemy {
	# Enemy(rImagePath: QString, pos: QPoint, pParent: QGraphicsItem *)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ getDamage(): int
	+ linkPlayer(pPlayer: Player *)
	# m_pPlayer: Player *
	# m_damage: int
	# die()
	# getMinCooldown(): int
	# getMaxCooldown(): int
	- m_moveCooldown: int
	- m_hasAttacked: boolean
	-- slots --
	{abstract} +moveTowardPlayer()
}

Entity <|-- Enemy

class Entity {
	# Entity(rImagePath: QString, pParent: QGraphicsItem *)
	+ takeDamage(damage: int)
	+ pack()
	+ unpack()
	+ initialize()
	{abstract} # die()
	# m_maxHealth: int
	# m_health: int
	# m_pHealthBar: QList<HeartIcon *>
}

Body <|-- Entity

class FallingPlatform {
	+ FallingPlatform(rect: QRect)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ collisionSide(body: Body *): Direction
	- m_isFalling: boolean
	- m_fallSpeed: qreal
}

Platform <|-- FallingPlatform

class FragilePlatform {
	+ FragilePlatform(rect: QRect)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ collisionSide(body: Body *): Direction
	- m_isBreaking: boolean
	- m_breakingState: int
}

Platform <|-- FragilePlatform

class GameCore <<QObject>> {
	+ GameCore(pGameCanvas: GameCanvas *, parent: QObject*)
	+ ~GameCore()
	+ keyPressed(key: int)
	+ keyReleased(key: int)
	+ mouseMoved(newMousePosition: QPointF)
	+ mouseButtonPressed(mousePosition: QPointF, buttons: MouseButtons)
	+ mouseButtonReleased(mousePosition: QPointF, buttons: MouseButtons)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ currentLevel(): Level *
	+ canvas(): GameCanvas *
	- m_pGameCanvas: GameCanvas *
	- m_pPlayer: Player *
	- m_pPlatforms: QList<Platform *>
	- m_pLevel: Level *
	-- slots --
	+ changeLevel(targetLevel: QPoint, Direction dir)
	+ spriteQueuedForDeletion(pSprite: Sprite *)
	+ onPlayerDied()
	-- signals --
	+ notifyMouseMoved(newMousePosition: QPointF)
	+ notifyMouseButtonPressed(mousePosition: QPointF, buttons: MouseButtons)
	+ notifyMouseButtonReleased(mousePosition: QPointF, buttons: MouseButtons)
	+ notifyKeyPressed(key: int)
	+ notifyKeyReleased(key: int)
}

GameCore --> LevelBuilder

class HeartCharge {
	+ HeartCharge(id: QString)
}

Item <|-- HeartCharge

class HeartIcon {
	+ HeartIcon(pParent: QGraphicsItem *)
	{static} + SCALE: qreal
}

HudIcon <|-- HeartIcon

class HudIcon {
	+ HudIcon(rImagePath: QString, pParent: QGraphicsItem *)
}

Sprite <|-- HudIcon

class Item {
	# Item(rImagePath: QString, id: QString, parent: QGraphicsItem *)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ getId(): QString
	# m_id: QString
	{static} # m_size: qreal
}

Body <|-- Item

class JumpCharge {
	+ JumpCharge(id: QString)
}

Item <|-- JumpCharge

class JumpIcon {
	+ JumpIcon(pParent: QGraphicsItem *)
}

HudIcon <|-- JumpIcon

class JumpingPancake {
	+ JumpingPancake(pos: QPoint, pParent: QGraphicsItem *)
	+ tick(elapsedTimeInMilliseconds: long long)
	+ moveTowardPlayer()
	{static} # JUMP_FORCE: qreal
	{static} # ENNEMI_WIDTH: qreal
	{static} # MAX_SPEED_X: qreal
	{static} # DISTANCE_TO_PLAYER_RATIO: qreal
	# getMinCooldown(): int
	# getMaxCooldown(): int
	- m_jumpAnimationFrame: int
	- m_jumpAnimationCooldown: int
	- jumpWithAnimation()
}

Enemy <|-- JumpingPancake

JumpingPancake <|-- FinalBoss

class Level <<QObject>> {
	+ Level(pCanvas: GameCanvas *, pPlayer: Player *, levelId: QPoint)
	+ ~Level()
	+ neighbouringLevels(): QList<LevelBuilder>
	+ scene(): GameScene *
	+ levelId(): QPoint
	+ appendLevel(level: LevelBuilder *)
	+ setSpawnPoint(spawnPoint: QPointF)
	+ initialize()
	- m_levelId: QPoint
	- m_pCanvas: GameCanvas *
	- m_pScene: GameScene *
	- m_pConnectedLevels: QList<LevelBuilder *>
	- m_pPlayer: Player *
	- m_spawnPoint: QPointF
}

class MindSignal {
	+ MindSignal(pParent: QGraphicsItem *)
	+ setDirection(goalPos: QPointF)
	+ tick(elapsedTimeInMilliseconds: long long)
	- m_deltaPos: QPointF
	{static} - ATTACK_SPREAD_SPEED: qreal
	{static} - ATTACK_SIZE: qreal
	{static} - ATTACK_POWER: qreal
	-- signals --
	+ queueForDeletion(a: MindSignal *)
}

Sprite <|-- MindSignal

class Platform {
	# Platform(rect: QRect)
	+ collisionSide(body: Body*): Direction
	# m_pImage: QImage *
	-- signals --
	+ queuedForDeletion(platform: Platform *)
}

Sprite <|-- Platform

class Player {
	+ Player()
	+ jump()
	+ tick(elapsedTimeInMilliseconds: long long)
	+ pack()
	+ unpack()
	+ initialize()
	+ setSpawnPoint(spawnPoint: QPointF)
	- m_playerInput: QPointF
	- m_keysPressed: QList<int>
	- m_maxJumpIcons: int
	- m_jumpIcons: int
	- m_hasReleasedJump: boolean
	- m_spawnPoint: QPointF
	- m_invincibilityTimeLeft: int
	{static} - PLAYER_JUMP_FORCE: qreal
	{static} - ACCELERATION: qreal
	{static} - PLAYER_FRICTION: qreal
	- m_pJumpIconsSprites: QList<JumpIcon *>
	{static} - INVINCIBILITY_TIME: int
	- m_pAttackSprites: QList<MindSignal *>
	- die()
	- takeDamage(damage: int)
	- updateJumpIcons()
	- resetPos()
	- manageEnnemyCollisions()
	-- slots --
	+ keyPressed(key: int)
	+ keyReleased(key: int)
	+ mouseButtonPressed(mousePosition: QPointF, buttons: MouseButtons)
	+ mindSignalRemoved(obj: MindSignal *)
	-- signals --
	+ playerDied()
}

Entity <|-- Player

enum PlayerData {
	{static} + PICKED_UP_UPGRADES
}

Player +-- PlayerData

class RegenPod {
	+ RegenPod()
	+ tick(elapsedTimeInMilliseconds: long long)
	- m_blinkCounter: int
}
Sprite <|-- RegenPod

class SolidPlatform {
	+ SolidPlatform(rect: QRect)
}

Platform <|-- SolidPlatform

class TransparentPlatform {
	+ TransparentPlatform(rect: QRect)
	+ collisionSide(body: Body *): Direction
}

Platform <|-- TransparentPlatform

class Sprite <<QObject>> <<QGraphicsPixmapItem>> {
	+ addAnimationFrame()
	+ setCurrentAnimationFrame()
	+ currentAnimationFrame()
	+ clearAnimationFrame()
	+ showNextAnimationFrame()
	+ setAnimationSpeed()
	+ startAnimation()
	+ stopAnimation()
	+ isAnimationRunning()

	+ globalBoundingBox()
	+ globalShape()

	+ setParentScene()
	+ parentScene()

	+ tick()
	+ setTickHandler()
	+ tickHandler()
	+ removeTickHandler()
}

GameScene "0..1" *-- "*" Sprite

class GameScene <<QGraphicsScene>> {
	- m_SpriteList: liste de Sprite
	+ addSpriteToScene()
	+ sprites()
	+ spriteAt()
	+ collidingSprites()
	+ createText()
	+ setBackgroundImage()
	+ tick()
}

GameCanvas *-- "1..*" GameScene
GameCanvas *-- "1" GameCore

class GameCanvas <<QObject>> {
	+ startTick(int interval)
	+ stopTick()

	+ createScene()
	+ setCurrentScene()
	+ currentScene()

	+ startMouseTracking()
	+ stopMouseTracking()
	+ currentMousePosition

	# eventFilter()
}

class MainFrm <<QWidget>> {


}

MainFrm *-- "1" GameCanvas
MainFrm --* "1" GameView

class GameView <<QGraphicsView>> {
	+ setFitToScreenEnabled()
	+ isFitToScreenEnabled()
	+ setClipSceneEnabled()
	+ isClipSceneEnabled()
}

GameCanvas -> GameView

GameView --> GameScene: Affiche

GameScene "1" --* "0..1" Level

LevelBuilder "0..1" *-- "*" Sprite
 * @enduml
 *
 * \section afaire_sec Travail à réaliser
 * Développer le jeu au sein de la classe GameCore, en spécialisant la classe Sprite et en créant toutes les autres classes nécessaires au jeu.
 *
 * Les initialisations peuvent être faites dans le constructeur de GameCore : GameCore::GameCore().
 *
 * Si l'initialisation du jeu implique de nombreuses instructions, ajouter à GameCore
 * des fonctions privées d'initialisation qui seront appelées depuis le constructeur.
 *
 * Au sein de la classe GameCore, la variable membre m_pCanvas permet d'accéder
 * aux fonctions de la classe GameCanvas.
 *
 * La classe GameScene, qui représente un espace de jeu en deux dimensions, met
 * à disposition plusieurs fonctions utiles pour le jeu :
 *
 * - GameScene::setWidth() : Permet de déterminer la largeur, en pixels, de la surface de jeu. La fonction GameScene::width() permet de la relire.
 * - GameScene::setHeight() : Permet de déterminer la hauteur, en pixels, de la surface de jeu. La fonction GameScene::height() permet de la relire.
 * - GameScene::addSpriteToScene() : Cette fonction intègre le sprite donné à la scène. La scène en prend la propriété
 *   et se chargera donc de le détruire lorsque l'application se termine.
 * - GameScene::collidingSprites() : Cette fonction permet d'obtenir une liste de sprites en collision avec un sprite donné.
 * - GameScene::spriteAt() : Cette fonction permet de récupérer le sprite se trouvant à la position donnée.
 * - GameScene::createText() : Cette fonction ajoute à la scène un texte et retourne un pointeur permettant de manipuler ce texte.
 *
 * La classe GameCanvas intercepte les événements produits par le clavier (appui et
 * relâche d'une touche, voir \ref input_keyboard) et la souris (voir \ref input_mouse).
 *
 * \section sprite_sec Les objets animés
 * Un jeu est essentiellement constitué d'élément graphiques animés qui se déplacent sur
 * la surface de jeu.
 * Qt met à disposition plusieurs classes et fonctionnalités permettant de gérer de
 * nombreux éléments graphiques. Ces éléments graphiques sont de type QGraphicsItem.
 *
 * Pour simplifier le développement, la classe Sprite spécialise QGraphicsItem et
 * met à disposition quelques fonctionnalités de base telles que des méthodes de
 * détection de collision (Sprite::collidingSprites()) ou d'animation (Sprite::addAnimationFrame(),
 * Sprite::startAnimation() et Sprite::stopAnimation()).
 *
 * \section input Evénements du clavier et de la souris
 *
 * \subsection input_keyboard Clavier
 * Lorsqu'une touche du clavier est appuyée, la méthode GameCore::keyPressed() est
 * automatiquement appelée. Si la touche reste appuyée, cette méthode n'est pas appelée
 * continuellement (ce comportement peut être modifié dans GameCanvas::keyPressed() et
 * GameCanvas::keyReleased()).
 *
 * Lorsque la touche du clavier est relâchée, la méthode GameCore::keyReleased() est automatiquement appelée.
 *
 * Ces deux méthodes reçoivent le paramètre `Key` qui représente le code numérique de la touche appuyée.
 *
 * GameCore se charge d'émettre le signal `notifyKeyPressed()` lorsqu'une touche est appuyée
 * et le signal `notifyKeyReleased()` lorsqu'une touche est relâchée, ce qui permet d'y
 * connecter les objets intéressés par cette information.
 *
 * \subsection input_mouse Souris
 * Lorsqu'un bouton de la souris est appuyé, la méthode GameCore::mouseButtonPressed() est automatiquement appelée. La méthode
 * reçoit alors la position de la souris en paramètre et la liste des boutons appuyés.
 *
 * Lorsqu'un bouton de la souris est relâché, la méthode GameCore::mouseButtonReleased() est automatiquement appelée. La méthode
 * reçoit alors la position de la souris en paramètre et la liste des boutons appuyés.
 *
 * Par défaut, les événements de déplacement de la souris ne sont pas détectés, pour des raisons de performance.
 * Pour les détecter, il faut enclencher le suivi de la souris avec la méthode GameCanvas::startMouseTracking(). Le suivi peut être
 * stoppé avec la méthode GameCanvas::stopMouseTracking().
 *
 * Lorsque le suivi de la souris est enclenché, la méthode GameCore::mouseMoved() est appelée chaque fois que la souris est déplacée.
 *
 * GameCore se charge d'émettre les signaux `notifyMouseButtonPressed()`,
 * `notifyMouseButtonReleased()` et `notifyMouseMoved()`, ce qui permet d'y connecter
 * les objets intéressés par les informations concernant la souris.
 *
 * \section res_sec Ressources
 * Pour fonctionner correctement, le jeu a besoin d'un certain nombre de ressources, telles que des images, des sons,
 * des fichiers de configuration, etc.
 * Ces ressources sont placées dans un répertoire `res`.
 *
 * Le fichier resources.h met à disposition des fonctions utilitaires (dans l'espace de nommage GameFramework) permettant d'accéder
 * à ce répertoire `res`.
 *
 * Toutefois, si une de ces ressources venait à être introuvable, il est important que le programme ne plante pas lamentablement et
 * que l'erreur soit gérée de la façon la plus propre possible.

 * \section images_sec Les images
 * Qt met différentes classes à disposition pour manipuler des images.
 *
 * Pour ce projet, ce sont des images bitmap qui seront utilisées. La classe QPixmap permet de les manipuler facilement.
 *
 * Il est très facile de créer un objet QPixmap à partir d'un fichier (png ou jpeg) : il suffit de passer le chemin du fichier au constructeur :
 *
 *     QPixmap monImage("chemin_du_fichier/image.png");
 *
 * Il est également possible d'instancier une image sur le tas :
 *
 *     QPixmap* pMonImage = new QPixmap("chemin_du_fichier/image.png");
 *
 * \section screen_mode Les modes d'affichage
 * Par défaut, la zone de jeu est affichée au sein d'une fenêtre classique Windows et peut cohabiter avec des composants graphiques classiques
 * comme des boutons, des champs de texte et autres.
 *
 * Il est possible de faire en sorte que le jeu s'affiche en plein écran, selon différents modes :
 *
 * - Mode plein écran fenêtré : la fenêtre de jeu prend toute la surface de l'écran, mais garde sa bordure. L'utilisateur peut modifier sa taille.
 * Pour obtenir ce mode, dans la fonction main(), choisir le ligne de code `w.showMaximized()` et commenter les autres.
 * - Mode plein écran total : la fenêtre de jeu prend toute la surface de l'écran, ses bordures et sa barre de titre disparaissent. Pour obtenir
 * ce mode, dans la fonction main(), choisir la ligne de code `w.showFullScreen()` et commenter les autres.
 * - Mode fenêtré : c'est le mode par défaut : le jeu s'affiche dans une fenêtre classique de windows. Pour obtenir ce mode, dans la fonction
 * main(), choisir la ligne de code `w.show()` et commenter les autres.
 *
 * Indépendamment de ces modes, deux réglages peuvent également être faits :
 *
 * - Faire que la surface de jeu affichée se dimensionne automatiquement en fonction de la taille de l'écran (la taille de le scène n'est pas modifiée).
 * Pour cela, ajouter dans MainFrm::MainFrm() la ligne de code `ui->grvGame->setFitToScreenEnabled(true);`.
 * - Supprimer les marges de l'affichage de la surface de jeu. Pour cela, ajouter dans MainFrm::MainFrm() la ligne de code `ui->verticalLayout->setContentsMargins(QMargins(0,0,0,0));`.
 *
 * \section utilities Les fonctions utilitaires
 * En plus des fonctions utilitaires liées aux resources (\ref res_sec), le fichier utilities.h met à disposition des fonctions
 * utiliaires diverses, en particulier des fonctions permettant de connaître les dimensions de l'écran et le rapport largeur/hauteur.
 *
 * Les fonctions GameFramework::hideMouseCursor() et GameFramework::showMouseCursor() permettent de cacher, respectivement afficher le
 * curseur de la souris.
 *
 */

#include "mainfrm.h"
#include "resources.h"

#include <QApplication>

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("cejef-divtec");
    QCoreApplication::setOrganizationDomain("divtec.ch");
    QCoreApplication::setApplicationName("2023-JCO-MahFingPancakes-beucodi");
    QGuiApplication::setApplicationDisplayName("2023-JCO-MahFingPancakes-beucodi");

    qDebug() << "App dir path : " << qApp->applicationDirPath();
    qDebug() << "App library paths : " << qApp->libraryPaths();
    qDebug() << "Image path : " << GameFramework::imagesPath();

    if (GameFramework::resourcesPath().isEmpty()) {
        qCritical() << "Dossier des ressources introuvable : Fin d'exécution du programme.";
        return -1;
    }

    MainFrm w;
    w.showMaximized();

    // Pour un mode d'affichage fenêtré, plein écran
    // w.showMaximized();

    // Pour un mode d'affichage non-fenêtré, plein écran
    // w.showFullScreen();

    return a.exec();
}
