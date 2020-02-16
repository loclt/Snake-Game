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
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: First Last, student#, first.last@tuni.fi                 #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent):

    QMainWindow(parent){
    inGame = true;
    addBody = false;
    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);
    ui_.spinBoxWidth->setMinimum(0);
    ui_.spinBoxWidth->setMaximum(10);
    ui_.spinBoxHeigth->setMinimum(0);
    ui_.spinBoxHeigth->setMaximum(10);
    ui_.spinBoxDelay->setMinimum(100);
    ui_.spinBoxDelay->setMaximum(1500);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnake);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    // TODO: Read the event to see which key got pressed and store the result.
    // You can remove this functio
    QMainWindow::keyReleaseEvent(event);
    if(event->key() == Qt::Key_W) {
        direction_ = QPoint(0,-1);
    }else if(event->key() == Qt::Key_S) {
        direction_ = QPoint(0,1);
    }else if(event->key() == Qt::Key_D) {
        direction_ = QPoint(1,0);
    }else if(event->key() == Qt::Key_A) {
        direction_ = QPoint(-1,0);
    }
    checkCollision();
    checkFood();
}

void MainWindow::timerEvent(QTimerEvent *e) {
    /*
     * Using timerEvent() as a loop in other to check collision
     * and update the position of snake.
     */
    Q_UNUSED(e);
    if (inGame) {
        ++second_;
        ui_.lcdNumberTimer->setStyleSheet("background-color: darkCyan");
        ui_.lcdNumberTimer->display(second_);
        checkFood();
        checkCollision();
        moveSnake();
        if (addBody == true){
            moveBody();
        }
    }
}

void MainWindow::on_playButton_clicked() {
    /*
     * Generate variables:
     * dots = 0
     * second_ = 0 (for Timer LCD)
     * Drawing Snake's Head and the position of its
     * Random position of Food basing on WIDTH and HEIGTH
     * SpinBox and Button will be disable after click Play
     */
    WIDTH_ = ui_.spinBoxWidth->value();   // Get value Width
    HEIGHT_ = ui_.spinBoxHeigth->value(); // Get value Height
    ui_.spinBoxWidth->setEnabled(false);  // Disable SpinBoxWidth after play
    ui_.spinBoxHeigth->setEnabled(false); // Disable SpinBoxHeight after play
    ui_.playButton->setEnabled(false);    // Disable PlayButton after play
    DELAY = ui_.spinBoxDelay->value();    // Get value DELAY(milisecond)
    ui_.spinBoxDelay->setEnabled(false);  // Disable SpinBoxSpeed after play
    dots = 0;
    second_= 0;
    SnakeHead(); //Draw Snake's Head
    // Generate the first position of Snake's Head
    snakeHead_->setPos(WIDTH_-1, HEIGHT_-2);
    // Generate the first position of Food
    MakeFood(); // Draw Food
    food_x= qrand()% WIDTH_;
    food_y= qrand()% HEIGHT_;
    food_-> setPos(food_x,food_y);

    adjustSceneArea();
    timerId = startTimer(DELAY);
}


void MainWindow::SnakeHead(){
    /*
     * Draw Snake's Head
     * Background: Black
     */
    const QRectF snake_Head(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::black);
    snakeHead_ = scene_.addRect(snake_Head, pen, brush);
}

void MainWindow::SnakeBody(){
    /*
     * Draw Snake's Body
     * Background: Green
     * Append each Body after creating to QList snakeBodies.
     */
    const QRectF snake_Body(0, 0, 1, 1);
    const QPen pen(Qt::white, 0);
    const QBrush brush(Qt::green);
    snakeBody_ = scene_.addRect(snake_Body, pen, brush);
    snakeBodies.prepend(snakeBody_);
}

void MainWindow::MakeFood(){
    /*
     * Draw Food
     * Check: Ellipse
     * Background: Red
     * Append each Food after creating to QList FOODS
     * in other to clear the old one.
     */
    const QPen penFood(Qt::yellow, 0);
    const QBrush brushFood(Qt::red);
    const QRect food(0, 0, 1, 1);
    food_ = scene_.addEllipse(food,penFood,brushFood);
    FOODS.prepend(food_);
}

void MainWindow::moveSnake() {
    /*
     * Move graphical Snake's Head left in the scene
     */
    const QPointF old_snake_pos = snakeHead_->scenePos();
    const QPointF new_snake_pos = old_snake_pos + direction_;
    snakeHead_->setPos(new_snake_pos);
}

void MainWindow::moveBody(){
    /*
     * Move graphical Snake's Body left in the scene basing
     * on the previous position.
     */
    const QPointF old_snakeBody_pos = snakeHead_->scenePos();
    for (size_t i = 1, n = snakeBodies.size()-1; i < n; ++i){
        snakeBodies[i]->setPos(snakeBodies[i+1]->pos());
    }
    // move front body to previous position of head
    snakeBodies.last()->setPos(old_snakeBody_pos-direction_);
}

void MainWindow::checkFood() {
    /*
     * Check the Snake's Head postion and Food position
     * If it matchs each other, Snake's Body will add one more
     * and the old food will be removed and be replaced by the new one.
     */
    if (snakeHead_->x() == food_->x() && snakeHead_->y() == food_->y()) {
        dots++;
        point_++;
        ui_.lcdNumberScore->setStyleSheet("background: green");
        ui_.lcdNumberScore->display(point_);
        addBody = true;
        SnakeBody();
        snakeBodies.prepend(snakeBody_);
        //connect(&timer_, &QTimer::timeout, this, &MainWindow::moveBody);
        food_x = qrand() % WIDTH_; // random potision of Food
        food_y = qrand() % HEIGHT_; // random potision of Food
        MakeFood();
        food_-> setPos(food_x,food_y);
        FOODS.prepend(food_);
        for (size_t i = 0, n = FOODS.size()-1; i < n; ++i){
            if (i ==0){
                FOODS.last()->setPos(food_->scenePos());
            }else{
                FOODS[i-1]->setPos(food_->scenePos());
            }
        }
    }
}

void MainWindow::checkCollision() {
    /*
     * Check collision when the Snake's Head go to limit of SIZE area.
     * 1> Snake's Head hits its Body
     * 2,3,4,5> Snake's Head hits the wall
     */
    for (int z = dots; z > 0; z--) {
        if ((snakeHead_->x() == snakeBodies[z]->x()) &&
                (snakeHead_->y() == snakeBodies[z]->y())){
            inGame = false;
            gameOver();
        }
    }
    if (snakeHead_->y() >= HEIGHT_) {
        inGame = false;
        gameOver();
    }
    if (snakeHead_->y() < 0) {
        inGame = false;
        gameOver();
    }
    if (snakeHead_->x() >= WIDTH_) {
        inGame = false;
        gameOver();
    }
    if (snakeHead_->x() < 0) {
        inGame = false;
        gameOver();
    }
}

void MainWindow::gameOver(){
    /*
     * Pop up a message Box in other to
     * inform the result of player
     */
    QMessageBox msgBox;
    msgBox.information(this,tr("Game Over"),
                       QString("You Lose!\nScore: %1\nSpent Time: %2").arg(point_).arg(second_));
    close();
}

void MainWindow::adjustSceneArea() {
    // TODO: Replace the area's size with the play field's actual size.
    const QRectF area(0, 0, WIDTH_, HEIGHT_);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}

void MainWindow::on_instructionButton_clicked()
{
    QMessageBox msgInstruction;
    QString info = ("1. The rule of game:\nControl Snake to eat all Foods in area."
                    " If Snake hits the wall or its body, You will lost."
                    " A MessageBox will appear and inform your spending time and points"
                    "\n\n2. How to Play\n"
                    "Direction by:\n"
                    "W: Up\nS: Down\nD: Right\nA: Left"
                    "\n\n3.Features:\n- Adjust Size of Area (Width & Height)"
                    "\n- Adjust Delay: The smaller the number, the faster the speed"
                    "\n- Timer: count playing time"
                    "\n- Score: count points while play game");
    msgInstruction.information(this,tr("Instruction"),info);
}
