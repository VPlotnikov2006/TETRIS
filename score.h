//TETRIS_2022

#ifndef _SCORE_
#define _SCORE_
//{
class Score {
private:
    int score;               ///Текущий счёт
public:
    Score(int s): score(s) {}///Нормальный конструктор
    Score(): score(0) {}     ///Нулевой конструктор
    void update(int n);      ///Конвертация разрушенных рядов в очков
    void draw(Point p1);     ///Рисование счёта
    int get();               ///Возвращает текущий счёт
};

void Score::update(int n) {
    if( n == 1 )
        score += 100;
    if( n == 2 )
        score += 300;
    if( n == 3 )
        score += 700;
    if( n == 4 )
        score += 1500;
}

void Score::draw(Point p) {
    txSelectFont ("Serifiqo 4F Free Capitals", 100);
    txSetColor(TX_WHITE);
    char chet[10];
    sprintf(chet,"%d",score);
    txTextOut(p.x, p.y, "Score");
    txTextOut(p.x, p.y + 10 + txGetTextExtentY(chet), chet);
}

int Score::get(){
    return score;
}
//}
//Этот код написал Венгеров Кирилл, электронной подписью является число - 1948
//Открытый ключ: {61534471, 11}
//Зашифрованная подпись - 41637548

#endif
