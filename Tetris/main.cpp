#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int BLOCK_SIZE = 30;

class TetrisGame : public QWidget
{
private:
    vector<vector<int>> board;

    vector<vector<vector<int>>> pieces =
    {
        // I piece
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },

        // O piece
        {
            {1, 1},
            {1, 1}
        },

        // T piece
        {
            {0, 1, 0},
            {1, 1, 1},
            {0, 0, 0}
        },

        // L piece
        {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        },

        // J piece
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },

        // S piece
        {
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}
        },

        // Z piece
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        }
    };

    vector<vector<int>> currentPiece;

    int pieceX;
    int pieceY;
    int score;
    bool gameOver;

    QTimer *timer;

public:
    TetrisGame(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("Qt Tetris");
        setFixedSize(BOARD_WIDTH * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE + 50);

        board = vector<vector<int>>(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));

        score = 0;
        gameOver = false;

        srand(time(0));

        spawnPiece();

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &TetrisGame::gameLoop);
        timer->start(500);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);

        painter.fillRect(rect(), QColor(25, 25, 25));

        drawBoard(painter);
        drawPiece(painter);
        drawScore(painter);

        if (gameOver)
        {
            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 24, QFont::Bold));
            painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
        }
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (gameOver)
            return;

        if (event->key() == Qt::Key_Left)
        {
            if (canMove(pieceX - 1, pieceY, currentPiece))
                pieceX--;
        }
        else if (event->key() == Qt::Key_Right)
        {
            if (canMove(pieceX + 1, pieceY, currentPiece))
                pieceX++;
        }
        else if (event->key() == Qt::Key_Down)
        {
            if (canMove(pieceX, pieceY + 1, currentPiece))
                pieceY++;
        }
        else if (event->key() == Qt::Key_Up)
        {
            vector<vector<int>> rotated = rotatePiece(currentPiece);

            if (canMove(pieceX, pieceY, rotated))
                currentPiece = rotated;
        }

        update();
    }

private:
    void drawBoard(QPainter &painter)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                QRect blockRect(
                    x * BLOCK_SIZE,
                    y * BLOCK_SIZE,
                    BLOCK_SIZE,
                    BLOCK_SIZE
                );

                if (board[y][x] == 1)
                {
                    painter.fillRect(blockRect, QColor(0, 180, 255));
                }
                else
                {
                    painter.fillRect(blockRect, QColor(40, 40, 40));
                }

                painter.setPen(QColor(70, 70, 70));
                painter.drawRect(blockRect);
            }
        }
    }

    void drawPiece(QPainter &painter)
    {
        for (int y = 0; y < currentPiece.size(); y++)
        {
            for (int x = 0; x < currentPiece[y].size(); x++)
            {
                if (currentPiece[y][x] == 1)
                {
                    QRect blockRect(
                        (pieceX + x) * BLOCK_SIZE,
                        (pieceY + y) * BLOCK_SIZE,
                        BLOCK_SIZE,
                        BLOCK_SIZE
                    );

                    painter.fillRect(blockRect, QColor(255, 180, 0));
                    painter.setPen(Qt::black);
                    painter.drawRect(blockRect);
                }
            }
        }
    }

    void drawScore(QPainter &painter)
    {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));

        QString scoreText = "Score: " + QString::number(score);

        painter.drawText(
            10,
            BOARD_HEIGHT * BLOCK_SIZE + 30,
            scoreText
        );
    }

    void gameLoop()
    {
        if (gameOver)
            return;

        if (canMove(pieceX, pieceY + 1, currentPiece))
        {
            pieceY++;
        }
        else
        {
            lockPiece();
            clearLines();
            spawnPiece();
        }

        update();
    }

    void spawnPiece()
    {
        int randomIndex = rand() % pieces.size();

        currentPiece = pieces[randomIndex];

        pieceX = BOARD_WIDTH / 2 - 2;
        pieceY = 0;

        if (!canMove(pieceX, pieceY, currentPiece))
        {
            gameOver = true;
            timer->stop();
        }
    }

    bool canMove(int newX, int newY, vector<vector<int>> piece)
    {
        for (int y = 0; y < piece.size(); y++)
        {
            for (int x = 0; x < piece[y].size(); x++)
            {
                if (piece[y][x] == 1)
                {
                    int boardX = newX + x;
                    int boardY = newY + y;

                    if (boardX < 0 || boardX >= BOARD_WIDTH)
                        return false;

                    if (boardY >= BOARD_HEIGHT)
                        return false;

                    if (boardY >= 0 && board[boardY][boardX] == 1)
                        return false;
                }
            }
        }

        return true;
    }

    void lockPiece()
    {
        for (int y = 0; y < currentPiece.size(); y++)
        {
            for (int x = 0; x < currentPiece[y].size(); x++)
            {
                if (currentPiece[y][x] == 1)
                {
                    int boardX = pieceX + x;
                    int boardY = pieceY + y;

                    if (boardY >= 0 && boardY < BOARD_HEIGHT)
                    {
                        board[boardY][boardX] = 1;
                    }
                }
            }
        }
    }

    void clearLines()
    {
        for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
        {
            bool fullLine = true;

            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                if (board[y][x] == 0)
                {
                    fullLine = false;
                    break;
                }
            }

            if (fullLine)
            {
                board.erase(board.begin() + y);
                board.insert(board.begin(), vector<int>(BOARD_WIDTH, 0));

                score += 100;

                y++;
            }
        }
    }

    vector<vector<int>> rotatePiece(vector<vector<int>> piece)
    {
        int rows = piece.size();
        int cols = piece[0].size();

        vector<vector<int>> rotated(cols, vector<int>(rows, 0));

        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                rotated[x][rows - 1 - y] = piece[y][x];
            }
        }

        return rotated;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TetrisGame window;
    window.show();

    return app.exec();
}
