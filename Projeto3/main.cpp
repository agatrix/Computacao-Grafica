#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <chrono>

std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::seconds accumulatedTime(0);     

enum GameState { MENU, GAME, PAUSE, GAME_OVER };
GameState currentState = MENU;

// Tamanho da janela
const int width = 800;
const int height = 600;

// Posições e dimensões das barras
float barWidth = 20.0f;
float barHeight = 100.0f;
float bar1Y = (float)height / 2 - barHeight / 2;
float bar2Y = (float)height / 2 - barHeight / 2;

// Posição e velocidade da bola
float ballX = (float)width / 2;
float ballY = (float)height / 2;
float ballSize = 20.0f;
float ballXSpeed = 5.0f;
float ballYSpeed = 5.0f;
const float initialBallXSpeed = 5.0f;
const float initialBallYSpeed = 5.0f;
const float speedIncreaseRate = 0.01f; // Taxa de aumento de velocidade

// Velocidades
float barSpeed = 15.0f;

// Placar
int score1 = 0;
int score2 = 0;

// Teclas pressionadas
bool upPressed = false;
bool downPressed = false;
bool up2Pressed = false;
bool down2Pressed = false;

// Estado do jogo
bool gameOver = false;
bool gamePaused = false;
bool gameMenu = true;
int winner = 0; // 0: none, 1: player1, 2: player2

// Alternar a direção inicial da bola
bool ballDirectionUp = true;

// Função para desenhar uma barra
void drawBar(float x, float y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + barWidth, y);
    glVertex2f(x + barWidth, y + barHeight);
    glVertex2f(x, y + barHeight);
    glEnd();
}

// Função para desenhar a bola
void drawBall(float x, float y, float size) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i += 30) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + cos(angle) * size, y + sin(angle) * size);
    }
    glEnd();
}

// Função para exibir mensagens de texto
void drawText(const char* text, float x, float y, void* font) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawMenu() {
    drawText("PONG GAME",(float)width / 2 - 100, (float)height / 2 + 50, GLUT_BITMAP_HELVETICA_18);
    drawText("Press 'S' to Start the Game",(float)width / 2 - 120, (float)height / 2, GLUT_BITMAP_HELVETICA_18);
    drawText("Press 'Q' to Quit", (float)width / 2 - 120, (float)height / 2 - 50, GLUT_BITMAP_HELVETICA_18);
}

void drawGame(){
    if (gameOver) {
        // Desenha a mensagem de vitória
        if (winner == 1) {
            drawText("Player 1 Wins!", (float)width / 2 - 70, (float)height / 2, GLUT_BITMAP_HELVETICA_18);
        } else if (winner == 2) {
            drawText("Player 2 Wins!", (float)width / 2 - 70, (float)height / 2, GLUT_BITMAP_HELVETICA_18);
        }
        drawText("Press 'R' to Restart", (float)width / 2 - 100, (float)height / 2 - 30, GLUT_BITMAP_HELVETICA_18);
        drawText("Press 'M' to Menu", (float)width / 2 - 100, (float)height / 2 - 50, GLUT_BITMAP_HELVETICA_18);
    } else if (gamePaused) {
        // Desenha a mensagem de pausa
        drawText("Game Paused", (float)width / 2 - 60, (float)height / 2, GLUT_BITMAP_HELVETICA_18);
        drawText("Press SPACE to Resume", (float)width / 2 - 90, (float)height / 2 - 30, GLUT_BITMAP_HELVETICA_18);
    } else {
        // Desenha o placar
        glRasterPos2f((float)width / 2 - 50, height - 50);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '-');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score2);

        // Desenha o timer
        int minutos = 0;
        int segundos = 0;
        if (!gamePaused && !gameOver) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = accumulatedTime + std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
            minutos = elapsedTime.count() / 60;
            segundos = elapsedTime.count() % 60;
        } else if(gameOver){
            auto elapsedTime = accumulatedTime;
            minutos = elapsedTime.count() / 60;
            segundos = elapsedTime.count() % 60;
        }
        glRasterPos2f((float)width / 2 - 50, height - 30);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + (minutos / 10));
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + (minutos % 10));
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ':');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + (segundos / 10));
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + (segundos % 10));

        // Desenha as barras
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para a barra do jogador 1
        drawBar(20.0f, bar1Y);

        glColor3f(0.0f, 0.0f, 1.0f); // Azul para a barra do jogador 2
        drawBar(width - barWidth - 20.0f, bar2Y);

        // Desenha a bola
        glColor3f(0.0f, 1.0f, 0.0f); // Verde para a bola
        drawBall(ballX, ballY, ballSize / 2);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentState) {
        case MENU:
            drawMenu();
            break;
        case GAME:
            drawGame();
            break;
    }
    glutSwapBuffers();
}

void resetGame() {
    // Reinicializa as variáveis do jogo
    score1 = 0;
    score2 = 0;
    ballX = (float)width / 2;
    ballY = (float)height / 2;
    ballXSpeed = initialBallXSpeed;
    ballYSpeed = ballDirectionUp ? initialBallYSpeed : -initialBallYSpeed;
    ballDirectionUp = !ballDirectionUp;
    bar1Y = (float)height / 2 - barHeight / 2;
    bar2Y = (float)height / 2 - barHeight / 2;
    gameOver = false;
    gamePaused = false;
    winner = 0;

    //reiniciar Timer
    accumulatedTime = std::chrono::seconds(0);
    startTime = std::chrono::steady_clock::now();
}

void update(int value) {
    if (gameOver || gamePaused || gameMenu) {
        glutPostRedisplay();
        glutTimerFunc(16, update, 0); // Continue chamando update para manter o display
        return;
    }

    // Movimentação da bola
    ballX += ballXSpeed;
    ballY += ballYSpeed;

    // Verifica colisão com o topo e a base
    if (ballY - ballSize / 2 < 0 || ballY + ballSize / 2 > height) {
        ballYSpeed = -ballYSpeed;
    }

    // Verifica colisão com as barras
    if (ballX - ballSize / 2 < 20.0f + barWidth && ballY > bar1Y && ballY < bar1Y + barHeight) {
        ballXSpeed = -ballXSpeed;
        ballX = 20.0f + barWidth + ballSize / 2; // Corrige a posição da bola
    }

    if (ballX + ballSize / 2 > width - 20.0f - barWidth && ballY > bar2Y && ballY < bar2Y + barHeight) {
        ballXSpeed = -ballXSpeed;
        ballX = width - 20.0f - barWidth - ballSize / 2; // Corrige a posição da bola
    }

    // Verifica se a bola saiu do campo
    if (ballX < 0) {
        score2++;
        ballX = (float)width / 2;
        ballY = (float)height / 2;
        ballXSpeed = -initialBallXSpeed;
        ballYSpeed = ballDirectionUp ? initialBallYSpeed : -initialBallYSpeed;
        ballDirectionUp = !ballDirectionUp;

        if (score2 >= 10) {
            gameOver = true;
            winner = 2;
        }
    } else if (ballX > width) {
        score1++;
        ballX = (float)width / 2;
        ballY = (float)height / 2;
        ballXSpeed = initialBallXSpeed;
        ballYSpeed = ballDirectionUp ? initialBallYSpeed : -initialBallYSpeed;
        ballDirectionUp = !ballDirectionUp;

        if (score1 >= 10) {
            gameOver = true;
            winner = 1;
        }
    }

    // Movimentação das barras
    if (upPressed && bar1Y + barHeight < height) {
        bar1Y += barSpeed;
    }
    if (downPressed && bar1Y > 0) {
        bar1Y -= barSpeed;
    }

    if (up2Pressed && bar2Y + barHeight < height) {
        bar2Y += barSpeed;
    }
    if (down2Pressed && bar2Y > 0) {
        bar2Y -= barSpeed;
    }

    // Aumenta a velocidade da bola com o tempo
    ballXSpeed += (ballXSpeed > 0 ? speedIncreaseRate : -speedIncreaseRate);
    ballYSpeed += (ballYSpeed > 0 ? speedIncreaseRate : -speedIncreaseRate);

    // Garante que a velocidade não seja negativa
    if (fabs(ballXSpeed) < speedIncreaseRate) ballXSpeed = (ballXSpeed > 0 ? speedIncreaseRate : -speedIncreaseRate);
    if (fabs(ballYSpeed) < speedIncreaseRate) ballYSpeed = (ballYSpeed > 0 ? speedIncreaseRate : -speedIncreaseRate);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Aproximadamente 60 FPS
}

void handleKeysDown(unsigned char key, int x, int y) {
    switch (currentState) {
        case MENU:
            if (key == 's' || key == 'S') {
                gameMenu = !gameMenu;
                currentState = GAME;
                startTime = std::chrono::steady_clock::now();
            } else if (key == 'q' || key == 'Q') {
                exit(0);
            }
            break;
        case GAME:
            if (key == 'w' || key == 'W') {
                upPressed = true;
            }
            if (key == 's' || key == 'S') {
                downPressed = true;
            }
            if (key == 'r' || key == 'R') {
                if (gameOver) {
                    resetGame();
                }
            }
            if (key == 'm' || key == 'M'){
                if(gameOver){
                    gameMenu = true;
                    currentState = MENU;
                }
            }
            if (key == ' ') {
                //Trava o timer para quando o jogo pausa
                if (gamePaused) {
                // Retoma o jogo
                    startTime = std::chrono::steady_clock::now();
                } else {
                    // Pausa o jogo
                    auto currentTime = std::chrono::steady_clock::now();
                    accumulatedTime += std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
                }

                // Alterna entre pausar e retomar o jogo
                gamePaused = !gamePaused;
            }
            if (key == 27) { // ESC key
                exit(0); // Encerra o programa
            }
            break;
    }
}

void handleKeysUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        upPressed = false;
    }
    if (key == 's' || key == 'S') {
        downPressed = false;
    }
}

void handleSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            up2Pressed = true;
            break;
        case GLUT_KEY_DOWN:
            down2Pressed = true;
            break;
    }
}

void handleSpecialKeysUp(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            up2Pressed = false;
            break;
        case GLUT_KEY_DOWN:
            down2Pressed = false;
            break;
    }
}

// Função para evitar o redimensionamento da janela
void reshape(int w, int h) {
    if (w != width || h != height) {
        glutReshapeWindow(width, height);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glutPostRedisplay();
}

void Init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("TP3 - Jogo Pong");
    glClearColor(0.0, 0.0, 0.0, 1.0); // Cor de fundo preta
    startTime = std::chrono::steady_clock::now();
}

int main(int argc, char** argv) {
    Init(argc, argv);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutSpecialFunc(handleSpecialKeys); // Função para teclas especiais (setas)
    glutSpecialUpFunc(handleSpecialKeysUp); // Função para liberação das teclas especiais
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

