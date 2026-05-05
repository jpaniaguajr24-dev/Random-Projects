#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QString>
#include <QVector>

// This class is the whole Math Ninja game window
class MathNinjaGame : public QWidget
{
private:
    // These are the words/text we see on the screen
    QLabel *titleLabel;
    QLabel *questionLabel;
    QLabel *scoreLabel;
    QLabel *livesLabel;
    QLabel *timerLabel;
    QLabel *messageLabel;

    // These are the buttons the player clicks
    QPushButton *answerButton1;
    QPushButton *answerButton2;
    QPushButton *answerButton3;
    QPushButton *answerButton4;
    QPushButton *restartButton;

    // This timer counts down every second
    QTimer *gameTimer;

    // These keep track of the game information
    int score;
    int lives;
    int timeLeft;
    int correctAnswer;

public:
    // This runs when the game first starts
    MathNinjaGame(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Starting game values
        score = 0;
        lives = 3;
        timeLeft = 10;
        correctAnswer = 0;

        // Window title and size
        setWindowTitle("Math Ninja");
        resize(600, 500);

        // This makes the game look colorful
        setStyleSheet(
            "QWidget {"
            "   background-color: #1e1e2f;"
            "   color: white;"
            "   font-family: Arial;"
            "}"
            "QLabel {"
            "   font-size: 22px;"
            "}"
            "QPushButton {"
            "   background-color: #ffcc00;"
            "   color: black;"
            "   font-size: 22px;"
            "   font-weight: bold;"
            "   border-radius: 12px;"
            "   padding: 15px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #ffaa00;"
            "}"
        );

        // Create the title
        titleLabel = new QLabel("Math Ninja");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: #ffcc00;");

        // Create the question text
        questionLabel = new QLabel("Question goes here");
        questionLabel->setAlignment(Qt::AlignCenter);
        questionLabel->setStyleSheet("font-size: 34px; font-weight: bold;");

        // Create score, lives, and timer text
        scoreLabel = new QLabel("Score: 0");
        livesLabel = new QLabel("Lives: 3");
        timerLabel = new QLabel("Time: 10");

        // Create message text
        messageLabel = new QLabel("Choose the correct answer!");
        messageLabel->setAlignment(Qt::AlignCenter);
        messageLabel->setStyleSheet("font-size: 20px; color: #90ee90;");

        // Create the answer buttons
        answerButton1 = new QPushButton();
        answerButton2 = new QPushButton();
        answerButton3 = new QPushButton();
        answerButton4 = new QPushButton();

        // Create the restart button
        restartButton = new QPushButton("Restart Game");
        restartButton->setStyleSheet(
            "background-color: #00c2ff;"
            "color: black;"
            "font-size: 20px;"
            "font-weight: bold;"
            "border-radius: 12px;"
            "padding: 12px;"
        );

        // This row holds score, lives, and time
        QHBoxLayout *statsLayout = new QHBoxLayout();
        statsLayout->addWidget(scoreLabel);
        statsLayout->addStretch();
        statsLayout->addWidget(livesLabel);
        statsLayout->addStretch();
        statsLayout->addWidget(timerLabel);

        // This grid puts the 4 answer buttons in a square
        QGridLayout *buttonLayout = new QGridLayout();
        buttonLayout->addWidget(answerButton1, 0, 0);
        buttonLayout->addWidget(answerButton2, 0, 1);
        buttonLayout->addWidget(answerButton3, 1, 0);
        buttonLayout->addWidget(answerButton4, 1, 1);

        // This puts everything on the screen from top to bottom
        QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->addWidget(titleLabel);
        mainLayout->addLayout(statsLayout);
        mainLayout->addSpacing(20);
        mainLayout->addWidget(questionLabel);
        mainLayout->addSpacing(20);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(messageLabel);
        mainLayout->addWidget(restartButton);

        // Add the layout to the window
        setLayout(mainLayout);

        // Create the timer
        gameTimer = new QTimer(this);

        // Every second, run updateTimer()
        connect(gameTimer, &QTimer::timeout, this, &MathNinjaGame::updateTimer);

        // When button 1 is clicked, check its answer
        connect(answerButton1, &QPushButton::clicked, this, [=]() {
            checkAnswer(answerButton1->text().toInt());
        });

        // When button 2 is clicked, check its answer
        connect(answerButton2, &QPushButton::clicked, this, [=]() {
            checkAnswer(answerButton2->text().toInt());
        });

        // When button 3 is clicked, check its answer
        connect(answerButton3, &QPushButton::clicked, this, [=]() {
            checkAnswer(answerButton3->text().toInt());
        });

        // When button 4 is clicked, check its answer
        connect(answerButton4, &QPushButton::clicked, this, [=]() {
            checkAnswer(answerButton4->text().toInt());
        });

        // When restart is clicked, restart the whole game
        connect(restartButton, &QPushButton::clicked, this, &MathNinjaGame::restartGame);

        // Start the first question
        startNewQuestion();

        // Start the timer. 1000 means 1 second.
        gameTimer->start(1000);
    }

private:
    // This makes a new math question
    void startNewQuestion()
    {
        // Reset the clock for each new question
        timeLeft = 10;
        timerLabel->setText("Time: " + QString::number(timeLeft));

        // Pick two random numbers from 1 to 12
        int number1 = QRandomGenerator::global()->bounded(1, 13);
        int number2 = QRandomGenerator::global()->bounded(1, 13);

        // Pick a random math operation
        // 1 means add, 2 means subtract, 3 means multiply
        int operation = QRandomGenerator::global()->bounded(1, 4);

        QString symbol;

        // Make an addition question
        if (operation == 1)
        {
            correctAnswer = number1 + number2;
            symbol = "+";
        }
        // Make a subtraction question
        else if (operation == 2)
        {
            correctAnswer = number1 - number2;
            symbol = "-";
        }
        // Make a multiplication question
        else
        {
            correctAnswer = number1 * number2;
            symbol = "x";
        }

        // Show the question on the screen
        questionLabel->setText(QString::number(number1) + " " + symbol + " " + QString::number(number2) + " = ?");

        // This list will hold the 4 answer choices
        QVector<int> answers;

        // Add the real correct answer first
        answers.push_back(correctAnswer);

        // Add 3 wrong answers
        while (answers.size() < 4)
        {
            // Make a wrong answer close to the correct one
            int wrongAnswer = correctAnswer + QRandomGenerator::global()->bounded(-10, 11);

            // Only add it if it is not the correct answer and not already in the list
            if (wrongAnswer != correctAnswer && !answers.contains(wrongAnswer))
            {
                answers.push_back(wrongAnswer);
            }
        }

        // Mix up the answers so the correct answer is not always first
        for (int i = 0; i < answers.size(); i++)
        {
            int randomIndex = QRandomGenerator::global()->bounded(answers.size());

            int temp = answers[i];
            answers[i] = answers[randomIndex];
            answers[randomIndex] = temp;
        }

        // Put the answers on the buttons
        answerButton1->setText(QString::number(answers[0]));
        answerButton2->setText(QString::number(answers[1]));
        answerButton3->setText(QString::number(answers[2]));
        answerButton4->setText(QString::number(answers[3]));

        // Make sure the answer buttons can be clicked
        enableAnswerButtons(true);
    }

    // This checks if the player clicked the right answer
    void checkAnswer(int selectedAnswer)
    {
        // If the answer is correct
        if (selectedAnswer == correctAnswer)
        {
            score += 10;
            scoreLabel->setText("Score: " + QString::number(score));

            messageLabel->setText("Correct! Ninja strike!");
            messageLabel->setStyleSheet("font-size: 20px; color: #90ee90;");
        }
        // If the answer is wrong
        else
        {
            lives--;
            livesLabel->setText("Lives: " + QString::number(lives));

            messageLabel->setText("Wrong! The answer was " + QString::number(correctAnswer));
            messageLabel->setStyleSheet("font-size: 20px; color: #ff6961;");
        }

        // If the player has no lives left, end the game
        if (lives <= 0)
        {
            endGame();
        }
        // If the player still has lives, make a new question
        else
        {
            startNewQuestion();
        }
    }

    // This runs every second
    void updateTimer()
    {
        // Take away 1 second
        timeLeft--;

        // Show the new time
        timerLabel->setText("Time: " + QString::number(timeLeft));

        // If time runs out
        if (timeLeft <= 0)
        {
            lives--;
            livesLabel->setText("Lives: " + QString::number(lives));

            messageLabel->setText("Too slow! The answer was " + QString::number(correctAnswer));
            messageLabel->setStyleSheet("font-size: 20px; color: #ff6961;");

            // If no lives are left, end the game
            if (lives <= 0)
            {
                endGame();
            }
            // If the player still has lives, go to the next question
            else
            {
                startNewQuestion();
            }
        }
    }

    // This ends the game
    void endGame()
    {
        // Stop the timer
        gameTimer->stop();

        // Show game over text
        questionLabel->setText("Game Over!");
        messageLabel->setText("Final Score: " + QString::number(score));

        // Stop the player from clicking answers
        enableAnswerButtons(false);

        // Pop-up message that shows the final score
        QMessageBox::information(
            this,
            "Game Over",
            "Your final score was: " + QString::number(score)
        );
    }

    // This starts the game over
    void restartGame()
    {
        // Reset everything back to the beginning
        score = 0;
        lives = 3;
        timeLeft = 10;

        scoreLabel->setText("Score: 0");
        livesLabel->setText("Lives: 3");

        messageLabel->setText("Choose the correct answer!");
        messageLabel->setStyleSheet("font-size: 20px; color: #90ee90;");

        // Start a new question and start the timer again
        startNewQuestion();
        gameTimer->start(1000);
    }

    // This turns the answer buttons on or off
    void enableAnswerButtons(bool enabled)
    {
        answerButton1->setEnabled(enabled);
        answerButton2->setEnabled(enabled);
        answerButton3->setEnabled(enabled);
        answerButton4->setEnabled(enabled);
    }
};

// This is where the program starts
int main(int argc, char *argv[])
{
    // This starts the Qt app
    QApplication app(argc, argv);

    // Create the game window
    MathNinjaGame game;

    // Show the game window
    game.show();

    // Keep the app running until the player closes it
    return app.exec();
}
