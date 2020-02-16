/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: First Last, student#, first.last@tuni.fi                 #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>
#include <QList>


/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent *);


private slots:

    /* \brief Start the game.
     */
    void on_playButton_clicked();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */



    void on_instructionButton_clicked();

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();
    void MakeFood();
    void SnakeHead();
    void SnakeBody();
    void moveSnake();
    void checkFood();
    void checkCollision();
    void moveBody();
    void moveBodies();
    void gameOver();

    int WIDTH_;                         /**< WIDTH of area */
    int HEIGHT_;                        /**< HEIGHT of area */
    int timerId;
    int dots;                           /**< Length of body */
    int point_ = 0;                     /**< Score */
    int food_x;                         /**< x Position of Food */
    int food_y;                         /**< y Position of Food */
    int DELAY ;                         /**< Speed milisecond */
    int second_;                        /**< Timer */
    bool inGame;
    bool addBody;

    QPointF prevPos;
    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */
    QGraphicsRectItem* snakeHead_;      /**< The snake Head in the scene. */
    QGraphicsRectItem* snakeBody_;      /**< The snake Body in the scene. */
    QList<QGraphicsRectItem*> snakeBodies; /**< The List of Snake's body. */
    QList<QGraphicsEllipseItem*> FOODS; /**< The List of Food. */
    QGraphicsScene scene_;              /**< Manages drawable objects. */
    QTimer timer_;                      /**< Triggers the Snake to move. */
    QPoint direction_ = QPoint(-1,0);
    QGraphicsEllipseItem* food_;        /**< The food item in the scene. */
};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
