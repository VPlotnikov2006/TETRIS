//TETRIS_2022

#ifndef _CLASS_
#define _CLASS_

//{


//Class
//==============================================================================================================================================================
//{
class Figure {
private:
    double x, y;              ///Координаты центра вращения фигуры
    Figure_type type;         ///Тип фигуры
    COLORREF color;           ///Цвет фигуры
    int direction;            ///Направление фигуры
    Figure_state state = MOVE;///Статус фигуры
public:
    Figure(): x(0), y(0), type(SQUARE), color(TX_NULL), direction(0) {} ///Нулевой конструктор
    Figure(double x0, double y0, Figure_type t, COLORREF c, int d): x(x0), y(y0), type(t), color(c), direction(d) {} ///Полноценный конструктор
    Figure& operator= (const Figure &f); ///Оператор присваивания

    void r_left();  ///Функция поворота ввлево
    void r_right(); ///Функция поворота вправо
    void m_left();  ///Функция движения влево
    void m_right(); ///Функция движения вправо
    int m_down();   ///Функция движения вниз,
                    ///Возвращает кол-во стертых рядов,
                    ///При остановке меняет свой статус и если нет выхода за пределы массива останавливается

    void set(Point p); ///Установка координат
    void set(int d);
    void set(Figure_state s);

    void draw(double s); ///Рисование

    int push(); ///Фигура вписывает себя в двумерный массив и возвращает кол-во стертых рядов

    bool check(); ///Проверка корректности постановки фигуры
    Figure_state get(); ///Возвращает статус фигуры

    Point get_pos(); ///Возвращает координаты фигуры
    void set_default(); ///Фигура появляется в точке спавна
    Figure_type get_type();
    Figure gen_shadow();
    int get_d() {
        return direction;
    }
};
//}
//==============================================================================================================================================================


//Initialization
//==============================================================================================================================================================
//{
Figure& Figure::operator= (const Figure &f) {
    x = f.x;
    y = f.y;
    type = f.type;
    color = f.color;
    direction = f.direction;
    state = f.state;
    return *this;
}

void Figure::r_left() {
    if (state == STATIC || state == OUT_OF_AREA)
        return;
    int new_direction = (direction + 3) % 4;
    Figure cpy(x, y, type, color, new_direction);
    if (cpy.check())
        direction = new_direction;
}

void Figure::r_right() {
    if (state == STATIC || state == OUT_OF_AREA)
        return;
    int new_direction = (direction + 5) % 4;
    Figure cpy(x, y, type, color, new_direction);
    if (cpy.check())
        direction = new_direction;
}

void Figure::m_left() {
    if (state == STATIC || state == OUT_OF_AREA)
        return;
    Figure cpy(x - BLOCK_SIZE, y, type, color, direction);
    if(cpy.check())
        x = x - BLOCK_SIZE;
}

void Figure::m_right() {
    if (state == STATIC || state == OUT_OF_AREA)
        return;
    Figure cpy(x + BLOCK_SIZE, y, type, color, direction);
    if(cpy.check())
        x = x + BLOCK_SIZE;
}

int Figure::m_down() {
    if (state == STATIC || state == OUT_OF_AREA)
        return 0;
    Figure cpy(x, y + BLOCK_SIZE, type, color, direction);
    if(cpy.check()) {
        y = y + BLOCK_SIZE;
    }
    else {
        return push();
    }
    return 0;
}

Figure Figure::gen_shadow() {
    Figure shadow(x, y, type, RGB(50, 50, 50), direction);
    shadow.set(SHADOW);
    while(shadow.get() != STATIC)
        shadow.m_down();
    shadow.set(SHADOW);
    return shadow;
}

void Figure::set(Point p) {
    x = p.x;
    y = p.y;
}

void Figure::set(Figure_state s) {
    state = s;
}

void Figure::draw(double s = 1) {
    for (auto k : dict[type][direction]) {
        sqr_draw({x + k.x * BLOCK_SIZE, y + k.y * BLOCK_SIZE}, color, s);
    }
}

int Figure::push() {
    if (state == SHADOW || state == STATIC) {
        state = STATIC;
        return 0;
    }
    bool flag = 1;
    vector<vector<pair<bool, COLORREF>>> cpy = static_arr;
    for (auto k : dict[type][direction]) {
        double X = x + k.x * BLOCK_SIZE;
        double Y = y + k.y * BLOCK_SIZE;
        X -= X_LEFT - BLOCK_SIZE * 3;
        Y -= Y_UP;
        int j = (int) (X) / BLOCK_SIZE;
        int i = (int) (Y) / BLOCK_SIZE;
        if (i == 0) {
            flag = 0;
        }
        if (i < 0) {
            state = OUT_OF_AREA;
            static_arr = cpy;
            return 0;
        }
        static_arr[i][j].first = 1;
        static_arr[i][j].second = color;
    }
    if (flag)
        state = STATIC;
    else
        state = OUT_OF_AREA;
    return update();
}

bool Figure::check() {
    bool ans = 1;
    for (auto k : dict[type][direction]) {
        double X = x + k.x * BLOCK_SIZE;
        double Y = y + k.y * BLOCK_SIZE;
        X -= X_LEFT - BLOCK_SIZE * 3;
        Y -= Y_UP;
        int j = (int) (X) / BLOCK_SIZE;
        int i = (int) (Y) / BLOCK_SIZE;
        if(static_arr[i][j].first)
            ans = 0;
    }
    return ans;
}

Figure_state Figure::get() {
    return state;
}

Point Figure::get_pos() {
    return {x, y};
}

void Figure::set_default() {
    x = start_pos[type].x * BLOCK_SIZE + X_LEFT;
    y = start_pos[type].y * BLOCK_SIZE + Y_UP;
}

Figure_type Figure::get_type() {
    return type;
}

void Figure::set(int d){
    direction = d;
}
//}
//==============================================================================================================================================================
//}
//Этот код написал Плотников Владимир, электронной подписью является число - 589201
//Открытый ключ: {1001117521, 7}
//Зашифрованная подпись - 525272584

#endif
