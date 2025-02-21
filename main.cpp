#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <fstream>

// Cross-platform include for GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float waveTime = 0.0f; 
float objectX = 0.0f;
float objectY = -0.8f;
float velocityY = 0.0f;
float jumpHeight = 0.1f; 
float gravity = 0.0f; 
float leaves[6][4]; 
float leafSpeed = 0.01f; 
float leafWidth = 0.15f; 
int score = 0;
int gameOver = 0;


void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

//    رسم پس‌زمینه بازی با موج‌های پویا
void drawBackground() {
    // تعداد بخش‌هایی که پس‌زمینه به آن‌ها تقسیم می‌شود هرچی بیشتر باشه موج ها نرم تر و طبیعی ترند
    int numSegments = 500; 

    // محاسبه عرض هر مستطیل در طول محور x
    float segmentWidth = 2.0f / numSegments; 

    //  شروع رسم اشکال چهارضلعی همان گونه که در کدهای سرکلاس گفته شد QUADS
    //برای رسم چهارضلعی کاربرد دارد
    glBegin(GL_QUADS); 

    // حلقه برای رسم هر مستطیل
    for (int i = 0; i < numSegments; i++) {
        // محاسبه مختصات x سمت چپ مستطیل
        float x1 = -1.0f + i * segmentWidth;

        // محاسبه مختصات x سمت راست مستطیل
        float x2 = x1 + segmentWidth;

        float yOffset1 = 0.2f * sin(5.0f * x1 + waveTime); 

        float yOffset2 = 0.2f * sin(5.0f * x2 + waveTime); 

        // تنظیم رنگ قسمت پایین مستطیل (آبی روشن‌تر)
        glColor3f(0.4f, 0.7f, 1.0f); 

        // تعریف رأس پایین-چپ مستطیل (با در نظر گرفتن موج)
        glVertex2f(x1, -0.7f + yOffset1); 

        // تعریف رأس پایین-راست مستطیل
        glVertex2f(x2, -0.7f + yOffset2); 

        // تنظیم رنگ قسمت بالای مستطیل (آبی تیره‌تر)
        glColor3f(0.1f, 0.4f, 0.8f);  

        // تعریف رأس بالا-راست مستطیل
        glVertex2f(x2, 0.7f + yOffset2); 

        // تعریف رأس بالا-چپ مستطیل
        glVertex2f(x1, 0.7f + yOffset1); 
    }

    glEnd(); 
}


// قورباغه
void drawFrog() {
    float frogWidth = 0.10f; //  عرض قورباغه
    float frogHeight = leafWidth * 0.75f; // ارتفاع قورباغه

    // بدن قورباغه
    glColor3f(0.2f, 0.5f, 0.01f); // سبز تیره
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(objectX + frogWidth * cos(theta), objectY + frogHeight * sin(theta));
    }
    glEnd();

    // زبان 
    glColor3f(1.0f, 0.6f, 0.7f); // صورتی روشن
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(
            objectX - frogWidth * 0.09f + frogWidth * 0.2f * cos(theta), // کاهش شعاع لپ
            objectY - frogHeight * 0.5f + frogHeight * 0.15f * sin(theta)
        );
    }
    glEnd();

    

    // پا چپ
    glColor3f(0.2f, 0.5f, 0.01f); // سبز تیره
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(
            objectX - frogWidth * 0.75f + frogWidth * 0.59f * cos(theta),
            objectY - frogHeight * 0.3f + frogHeight * 0.39f * sin(theta)
        );
    }
    glEnd();

    // پا راست
    glColor3f(0.2f, 0.5f, 0.01f); // سبز تیره
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(
            objectX + frogWidth * 0.75f + frogWidth * 0.59f * cos(theta),
            objectY - frogHeight * 0.3f + frogHeight * 0.39f * sin(theta)
        );
    }
    glEnd();


    // چشم چپ
    glColor3f(0.0f, 0.0f, 0.0f); 
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(objectX - frogWidth * 0.75f + frogWidth * 0.15f * cos(theta), objectY + frogHeight * 0.7f + frogHeight * 0.15f * sin(theta));
    }
    glEnd();

    //چشم راست
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(objectX + frogWidth * 0.75f + frogWidth * 0.15f * cos(theta), objectY + frogHeight * 0.7f + frogHeight * 0.15f * sin(theta));
    }
    glEnd();

    // لبخند
    glColor3f(0.0f, 0.0f, 0.0f); 
    glLineWidth(4.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 180; i <= 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(objectX + frogWidth * 0.5f * cos(theta), objectY - frogHeight * 0.09f + frogHeight * 0.3f * sin(theta));
    }
    glEnd();
}
//برگ نیلوفر
void drawLilyPad(float x, float y, float width, float brightness) {
    glPushMatrix(); // ذخیره وضعیت ماتریس
    glTranslatef(x, y, 0.0f); // انتقال به مکان برگ

    glBegin(GL_POLYGON); // رسم برگ
    glColor3f(0.0f, brightness, 0.0f); // تنظیم رنگ با روشنایی
    for (int i = 0; i <= 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(width * cos(theta), (width * 0.6f) * sin(theta));
    }
    glEnd();

    // رسم رگبرگ‌ها
    glColor3f(0.01f, 0.4f, 0.5f); 
    glBegin(GL_LINES);
    for (int i = 0; i < 5; i++) {
        float theta = i * (360.0f / 5) * M_PI / 180.0f;
        glVertex2f(0.0f, 0.0f);
        glVertex2f(width * cos(theta), (width * 0.6f) * sin(theta));
    }
    glEnd();

    glPopMatrix(); // بازیابی ماتریس ذخیره شده
}

void drawLeaves() {
    for (int i = 0; i < 5; i++) {
        drawLilyPad(leaves[i][0], leaves[i][1], leafWidth, leaves[i][3]);
    }
}

//فریم
void drawFrame() {
    // تنظیم رنگ فریم
    glColor3f(1.0f, 0.3f, 0.0f); // نارنجی مایل به قرمز
    float thickness = 0.02f; // ضخامت فریم

    glBegin(GL_QUADS);

    // فریم بالا
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f - thickness);
    glVertex2f(-1.0f, 1.0f - thickness);

    // فریم پایین
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f + thickness);
    glVertex2f(-1.0f, -1.0f + thickness);
    

    // فریم چپ
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(-1.0f + thickness, 1.0f);
    glVertex2f(-1.0f + thickness, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    

    // فریم راست
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f - thickness, 1.0f);
    glVertex2f(1.0f - thickness, -1.0f);
    glVertex2f(1.0f, -1.0f);
    

    glEnd();
}


//تابع نمایش دهنده
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawFrame();
    if (gameOver) {
        glColor3f(0.6f, 0.0f, 0.0f);
        drawText("Game Over!", -0.15f, 0.3f); 
    
    

    // نمایش امتیاز
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(scoreText, -0.2f, -0.1f);

    // نمایش دستورالعمل
    drawText("try again game with 'enter' ", -0.2f, -0.3f);

    glutSwapBuffers();
    return;
}


    drawFrog();
    drawLeaves();

    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(scoreText, -0.9f, 0.9f);

    glutSwapBuffers();
}

// همان تایمر
void update(int value) {
    if (!gameOver) {
        if (gravity != 0.0f) {
            velocityY += gravity;
            objectY += velocityY;
        }
         waveTime += 0.01f;
         if (waveTime > 2 * M_PI) waveTime -= 2 * M_PI; 

        // حرکت برگ ها
        for (int i = 0; i < 6; i++) {
            leaves[i][1] -= leafSpeed;

            if (leaves[i][1] < -1.0f) {
                leaves[i][1] = 1.0f; 
                leaves[i][0] = (rand() % 200 - 100) / 100.0f; 
                leaves[i][2] = 0.0f; 
            }
        }

        for (int i = 0; i < 5; i++) {//بررسی هر جامپ روی هر برگ و اضافه کردن امتیاز
            if (fabs(objectY - leaves[i][1]) < 0.1f && fabs(objectX - leaves[i][0]) < leafWidth) {
    velocityY = jumpHeight;

    if (leaves[i][2] == 0.0f) {
        score++;
        leaves[i][2] = 1.0f;

        //  با تاایید هر جامپ روی همان برگ افزایش روشنایی برگ
        leaves[i][3] = std::min(leaves[i][3] + 0.2f, 1.0f);      

        if (score % 10 == 0) {
            leafSpeed += 0.001f;
        }
    }
    break;
}

        }

       
        if (objectY < -1.0f) {
            gameOver = 1;
        }

        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
    }
}

// تابع  کلید اصلی
void handleKeys(unsigned char key, int x, int y) {
    if (!gameOver) {
        if (key == ' ') {
            if (gravity == 0.0f) {
                gravity = -0.005f; //استارت
            }
            velocityY = jumpHeight;
        } else if (key == 'z' || key == 'Z') {
            objectX -= 0.1f;
            if (objectX < -1.0f) objectX = -1.0f;
        } else if (key == 'C' || key == 'c') {
            objectX += 0.1f;
            if (objectX > 1.0f) objectX = 1.0f;
        }
    } else if (key == '\r' || key == ' ') {
        objectX = 0.0f;
        objectY = -0.8f;
        velocityY = 0.0f;
        gravity = 0.0f;
        leafSpeed = 0.005f;
        for (int i = 0; i < 5; i++) {
            leaves[i][0] = (rand() % 200 - 100) / 100.0f;
            leaves[i][1] = (rand() % 200) / 100.0f;
        }
        score = 0;
        gameOver = 0;
        glutTimerFunc(16, update, 0);
    }
}
//مقادیر اولیه
//آماده‌سازی متغیرها و تنظیمات گرافیکی.
void initialize() {
    srand(time(0)); // مقداردهی اولیه برای اعداد تصادفی
    for (int i = 0; i < 6; i++) {
        leaves[i][0] = (rand() % 200 - 100) / 100.0f; // مکان تصادفی x برگ‌ها
        leaves[i][1] = (rand() % 200) / 100.0f; // مکان تصادفی y برگ‌ها
        leaves[i][2] = 0.0f; // وضعیت برگ (پرش نشده) یعنی اگر پرشی اتفاق نیوفتاد تیره بمان
        leaves[i][3] = 0.6f; // روشنایی اولیه برگ‌ها
    }
    
    glClearColor(0.2f, 0.6f, 1.0f, 1.0f); // تنظیم رنگ پس‌زمینه
    glMatrixMode(GL_PROJECTION); //   projectionتغییر ماتریس به حالت 
    glLoadIdentity(); // بازنشانی ماتریس به حالت پیش‌فرض
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // تنظیم سیستم در محیط دو‌بعدی
}

//تابع اصلی که دارای فراخوانی های توابع دیگر معمولا مولفه های داخل ان ثابت 
//یعنی نیاز نیست برنامه نویس بنویسه هی میتونه کپی کنه
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(800, 680); // تنظیم اندازه پنجره
    glutCreateWindow("Frog Game"); // ایجاد پنجره با عنوان بازی

    initialize();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeys);
    glutTimerFunc(16, update, 0);

    glutMainLoop();// کاربرد ان برای این هی بچرخه و بازی ادامه پیدا کند
    return 0;
}

