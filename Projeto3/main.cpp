// Código base (inicial) para implementação do TP3 de CG
#include <GL/glut.h>

// Tamanho da janela
const int width = 800;
const int height = 600;

// Posições e dimensões das barras
float barWidth = 20.0f;
float barHeight = 100.0f;
float bar1Y = height / 2 - barHeight / 2;
float bar2Y = height / 2 - barHeight / 2;

// Posição e velocidade da bola
float ballX = width / 2;
float ballY = height / 2;
float ballSize = 20.0f;

// Velocidades
float barSpeed = 10.0f;
float ballXSpeed = 5.0f;
float ballYSpeed = 5.0f;

// Placar
int score1 = 0;
int score2 = 0;

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
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

        // Desenha o placar
        glRasterPos2f(width / 2 - 50, height - 50);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score1);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '-');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score2);

        // Desenha as barras
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para a barra do jogador 1
        drawBar(20.0f, bar1Y);

        glColor3f(0.0f, 0.0f, 1.0f); // Azul para a barra do jogador 2
        drawBar(width - barWidth - 20.0f, bar2Y);

        // Desenha a bola
        glColor3f(0.0f, 1.0f, 0.0f); // Verde para a bola
        drawBall(ballX, ballY, ballSize / 2);
    }

    glutSwapBuffers();
}

void update(int value) {
    // Lógica de atualização do jogo, incluindo movimentação das barras e bola

    // Espaço para a lógica de movimentação das barras

    // Espaço para a lógica de movimentação e colisão da bola

    // Espaço para a lógica de contagem de pontos

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Aproximadamente 60 FPS
}

void handleKeysDown(unsigned char key, int x, int y) {
    // Espaço para configurar o estado das teclas ao pressionar
}

void handleKeysUp(unsigned char key, int x, int y) {
    // Espaço para configurar o estado das teclas ao soltar
}

// Função para evitar o redimensionamento da janela
void reshape(int w, int h) {
    glutReshapeWindow(width, height);
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
}

int main(int argc, char** argv) {
    Init(argc, argv);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
