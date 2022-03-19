#include "base.h"
#include "line_clear.h"
#include "figure_class.h"
#include "random.h"
#include "score.h"
#include "button.h"
#include "key.h"
#include "record.h"
#include "tutorial.h"

//Типы фигур:
//Линия - LINE
//Г-образная - L_SHAPED1
//Отраженная Г-образная - L_SHAPED2
//Квадрат - SQUARE
//Z-образная - Z_SHAPED2
//Отраженная Z-оразная - Z_SHAPED1
//Т-образная T_SHAPED

//{
void play();                                     ///Запуск игры
__attribute__((__noreturn__)) void tetris_exit();///Выход из игры (с сохранением всего)
__attribute__((__noreturn__)) void menu();       ///Открытие основного меню

int main() {
    build();
    key_build();
    settings_load();
    records_load();
    txCreateWindow(1080, 720);
    menu();
}

__attribute__((__noreturn__)) void menu() {
    Button Play_game({200 + 100, 200}, {400 + 100, 300}, "PLAY",     TX_NULL, TX_BLACK);
    Button Records  ({200 + 100, 400}, {400 + 100, 500}, "RECORDS",  TX_NULL, TX_BLACK);
    Button Settings ({500 + 100, 200}, {700 + 100, 300}, "SETTINGS", TX_NULL, TX_BLACK);
    Button Exit     ({500 + 100, 400}, {700 + 100, 500}, "EXIT",     TX_NULL, TX_BLACK);
    txSelectFont ("Serifiqo 4F Free Capitals", 50, 50 / 3, 1000);
    txSetFillColor(RGB(150, 150, 150));
    txSetColor(TX_BLACK);
    txClear();
    txTextOut(500, 100, "TETRIS");
    Play_game.draw();
    Records.draw();
    Settings.draw();
    Exit.draw();
    while(!Exit.get()) {
        if (Play_game.get())
            play();
        if (Records.get())
            open_records();
        if (Settings.get())
            open_tutorial();
        txSetFillColor(RGB(95, 158, 160));
        txSetColor(TX_BLACK);
        txClear();
        txSelectFont ("Serifiqo 4F Free Capitals", 150, 150 / 3, 1000);
        txTextOut(390, 50, "TETRIS");
        txSelectFont ("Serifiqo 4F Free Capitals", 50, 50 / 3, 1000);
        Play_game.draw();
        Records.draw();
        Settings.draw();
        Exit.draw();
        txSleep(10);
    }
    tetris_exit();
}

__attribute__((__noreturn__)) void tetris_exit() {
    settings_save();
    records_save();
    txDisableAutoPause();
    exit(0);
}

void play() {
    srand (time (NULL));
    build();
    Figure main_figure;
    Figure next;
    Figure shadow;
    Figure hold = rand_figure();
    hold.set({900, 596});
    Figure t;
    Score score;
    main_figure = rand_figure();
    shadow = main_figure.gen_shadow();
    next = rand_figure();
    next.set({900, 180 + 50});
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        while(true) {
            for(int i = 0; i < 3 && !GetAsyncKeyState(VK_ESCAPE); i++) {
                if (GetAsyncKeyState(to_char(ml))) {
                    main_figure.m_left();
                    shadow = main_figure.gen_shadow();
                }
                if (GetAsyncKeyState(to_char(mr))) {
                    main_figure.m_right();
                    shadow = main_figure.gen_shadow();
                }
                if (GetAsyncKeyState(to_char(md))) {
                    score.update(main_figure.m_down());
                    shadow = main_figure.gen_shadow();
                }
                if (GetAsyncKeyState(to_char(rl))) {
                    main_figure.r_left();
                    shadow = main_figure.gen_shadow();
                }
                if (GetAsyncKeyState(to_char(rr))) {
                    main_figure.r_right();
                    shadow = main_figure.gen_shadow();
                }
                if (GetAsyncKeyState(to_char(h))) {
                    Point h_pos = hold.get_pos();
                    Point m_pos = main_figure.get_pos();
                    h_pos += shift[hold.get_type()];
                    m_pos += shift[main_figure.get_type()];
                    int d = main_figure.get_d();
                    t = hold;
                    hold = main_figure;
                    main_figure = t;
                    hold.set(h_pos - shift[hold.get_type()]);
                    main_figure.set(m_pos - shift[main_figure.get_type()]);
                    hold.set(0);
                    if (!main_figure.check()) {
                        t = hold;
                        hold = main_figure;
                        main_figure = t;
                        hold.set(h_pos - shift[hold.get_type()]);
                        main_figure.set(m_pos - shift[main_figure.get_type()]);
                        hold.set(0);
                        main_figure.set(d);
                    }
                    shadow = main_figure.gen_shadow();
                }
                txSetFillColor(RGB(150, 150, 150));
                txClear();
                if (main_figure.get() == MOVE) {
                    shadow.draw();
                    main_figure.draw();

                }
                static_draw();
                txSetColor(TX_BLACK, 2);
                txSelectFont ("Serifiqo 4F Free Capitals", 35, 10, 1000);
                txTextOut (20, 250, "Press <<ESCAPE>> to exit to menu");
                txTextOut (20, 300, "After the game ends, ");
                txTextOut (20, 350, "A dialog box will open");
                txTextOut (20, 400, "Look for him");
                txSelectFont ("Serifiqo 4F Free Capitals", 100);
                txTextOut(810 + 25, 80 - 40, "NEXT");
                txTextOut(810 + 25, 306 + 200 - 100, "HOLD");
                txRectangle(810, 90 + 50, 990, 270 + 50);
                txRectangle(810, 306 + 200, 990, 486 + 200);
                next.draw();
                hold.draw();
                score.draw({100, 10});
                txSleep(100);
            }
            if(main_figure.get() != MOVE || GetAsyncKeyState(VK_ESCAPE))
                break;
            score.update(main_figure.m_down());
            txSetFillColor(RGB(150, 150, 150));
            txClear();
            if (main_figure.get() == MOVE) {
                shadow.draw();
                main_figure.draw();

            }
            static_draw();
            txSetColor(TX_BLACK, 2);
            txSetFillColor(RGB(150, 150, 150));
            txSelectFont ("Serifiqo 4F Free Capitals", 35, 10, 1000);
            txTextOut (20, 250, "Press <<ESCAPE>> to exit to menu");
            txTextOut (20, 300, "After the game ends, ");
            txTextOut (20, 350, "A dialog box will open");
            txTextOut (20, 400, "Look for him");
            txSelectFont ("Serifiqo 4F Free Capitals", 100);
            txTextOut(810 + 25, 80 - 40, "NEXT");
            txTextOut(810 + 25, 306 + 200 - 100, "HOLD");
            txRectangle(810, 90 + 50, 990, 270 + 50);
            txRectangle(810, 306 + 200, 990, 486 + 200);
            next.draw();
            hold.draw();
            score.draw({100, 10});
            txSleep(100);
            if(main_figure.get() != MOVE || GetAsyncKeyState(VK_ESCAPE))
                break;
        }
        if (main_figure.get() == STATIC){
            main_figure = next;
            main_figure.set_default();
            next = rand_figure();
            next.set({900, 180 + 50});
            shadow = main_figure.gen_shadow();
        }
        else {
            break;
        }
    }
    if (MessageBox(NULL, "Would you like to record your score?","Record?", MB_YESNO) == IDYES) {
        const char* nik = txInputBox("Under what name do you want to do it?", "Record!", "PRO_GAME_MASTER_2010");
        Record r(string(nik), score.get());
        record_add(r);
    }
}

//}
//Этот код написал Плотников Владимир, электронной подписью является число - 456709
//Открытый ключ: {506477, 7}
//Зашифрованная подпись - 267686
