#include <iostream>
#include <cmath> 
#include <fstream> //для создания .cvs файла
#include <iomanip> //для задания точности записи в cvs файл
using namespace std;

struct Result {
    double H, T, p, ro, g, a;
};

Result atmosphere(double h); //вычисления
void testing(); //создание .cvs таблицы

int main() {
    double h;                   // Геометрическая высота, м
    
    while (true) {
        cout << "\nInput geometrical height h (from -2000 to 85000), m: ";
        if (!(cin >> h)) {  // Великий синий кит делать проверка на вводима значеня, нефрит стержень успех высокий ура
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            cout << "incorrect value\n"; 
            continue; 
        }

        if (h == -3000.0) break; // Условие выхода
        
        if (h == -4000.0) testing(); // Секретная команда для создания таблицы
        
        if (h < -2000.0 || h > 85000.0) {
            cout << "incorrect value\n";
            continue;
        }

        Result res = atmosphere(h);
        // Пункт 7
        if (!isfinite(res.H) || !isfinite(res.T) || !isfinite(res.p) || !isfinite(res.ro) || !isfinite(res.g) || !isfinite(res.a)) {
            cout << "Values are not finite\n";
            continue; // или break, в зависимости от логики
        }
        if (res.T <= 0.0 || res.p <= 0.0 || res.ro <= 0.0 || res.g <= 0.0 || res.a <= 0.0) {
            cout << "Values (T, p, ro, g, a) must be > 0\n";
            continue;
        }
        
        // Выводы делайте сами
        printf("H  = %15.6f m'\n", res.H);
        printf("T  = %15.6f K\n", res.T);
        printf("p  = %15.6f Pa\n", res.p);
        printf("ro = %15.6f kg/m^3\n", res.ro);
        printf("g  = %15.6f m/s^2\n", res.g);
        printf("a  = %15.6f m/s\n", res.a);
        printf("--------------------------\n");
    }

    return 0;
}

// Функция вычислений
Result atmosphere(double h) {
    
    // Константы (все заглавными для отличия)
    const double RAD   = 6356767.0; 
    const double G0    = 9.80665;
    const double RGAS  = 287.05287;
    const double K     = 1.4;
    const double T0    = 288.15;
    const double P0    = 101325.0;

    //таблицы для выбора Hi и Ti и Beta
    const double HiTAB[6]   = {11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0};
    const double TiTAB[6]   = {216.65, 216.65, 228.65, 270.65,  270.65, 214.65};
    const double BetaTAB[6] = {-0.0065, 0.0, 0.0010, 0.0028, 0.0, -0.0028};
    
    // Рассчитываем давления на границах слоёв (P_TAB)
    double P_TAB[7];                          // Пункт 3 (вместе с TiTAB выше)
    P_TAB[0] = P0;
    for (int i = 0; i < 6; ++i) {
        double dH = HiTAB[i] - (i == 0 ? 0.0 : HiTAB[i-1]);
        double T_prev = (i == 0 ? T0 : TiTAB[i-1]);
        
        if (BetaTAB[i] == 0.0) {
            P_TAB[i+1] = P_TAB[i] * exp(-G0 * dH / (RGAS * T_prev));
        } else {
            P_TAB[i+1] = P_TAB[i] * pow(TiTAB[i] / T_prev, -G0 / (RGAS * BetaTAB[i]));
        }
    }

    double H = RAD * h / (RAD + h);           // Пункт 2
    double g = G0 * pow(RAD / (RAD + h), 2);  // Пункт 2
    
    // для нижнего слоя (H < 11000)           // Пункт 4 (весь блок)
    double Hi = 0.0; 
    double Ti = T0;  
    double beta = BetaTAB[0]; 
    double Pi = P0;
    // Выбор слоя (перебор снизу вверх)
    if (H >= 11000.0) { Hi = HiTAB[0]; Ti = TiTAB[0]; beta = BetaTAB[0]; Pi = P_TAB[1]; }
    if (H >= 20000.0) { Hi = HiTAB[1]; Ti = TiTAB[1]; beta = BetaTAB[1]; Pi = P_TAB[2]; }
    if (H >= 32000.0) { Hi = HiTAB[2]; Ti = TiTAB[2]; beta = BetaTAB[2]; Pi = P_TAB[3]; }
    if (H >= 47000.0) { Hi = HiTAB[3]; Ti = TiTAB[3]; beta = BetaTAB[3]; Pi = P_TAB[4]; }
    if (H >= 51000.0) { Hi = HiTAB[4]; Ti = TiTAB[4]; beta = BetaTAB[4]; Pi = P_TAB[5]; }
    if (H >= 71000.0) { Hi = HiTAB[5]; Ti = TiTAB[5]; beta = BetaTAB[5]; Pi = P_TAB[6]; }

    double T = Ti + beta * (H - Hi);          // Пункт 5
    double p;                                 // Пункт 5
        if (beta != 0.0) p = Pi * pow(T / Ti, -G0 / (RGAS * beta));
        else p = Pi * exp(-G0 * (H - Hi) / (RGAS * Ti));
    
    double ro = p / (RGAS * T);               // Пункт 6
    double a  = sqrt(K * RGAS * T);           // Пункт 6

    Result res;
    res.H = H;
    res.T = T;
    res.p = p;
    res.ro = ro;
    res.g = g;
    res.a = a;
    return res; //Вывод значений, проверки находятся в main

    // if (!isfinite(H) || !isfinite(T) || !isfinite(p) || !isfinite(ro) || !isfinite(g) || !isfinite(a)) {  // Пункт 7
    //     cout << "Values are not finite u moron\n";
    //     return;
    // }
    // if (T <= 0.0 || p <= 0.0 || ro <= 0.0 || g <= 0.0 || a <= 0.0) {  // Пункт 7
    //     cout << "Values (T, p, ro, g, a) must be > 0, which they are not\n";
    //     return;
    // }
    
    /* Здесь была проверка вывода
    printf("H  = %15.6f m\n", H);
    printf("T  = %15.6f K\n", T);
    printf("p  = %15.6f Pa\n", p);
    printf("ro = %15.6f kg/m^3\n", ro);
    printf("g  = %15.6f m/s^2\n", g);
    printf("a  = %15.6f m/s\n", a);
    printf("------------------------\n");
    */
}

void testing() {
    ofstream fout("C:\\profile.csv");
    
    if (!fout.is_open()) { //проверка на то что файл огткрылся
    cout << "file cant be opened, run .exe/VScode with admin rights\n";
    return;

    fout << "h;H;T;p;ro;g;a\n"; // Заголовки столбцов
    fout << fixed << setprecision(12); //точность: 12 знаков после запятой

    for (int j = 0; j <= 870; ++j) {
        double h = -2000.0 + 100.0 * j;
        
        Result res = atmosphere(h);
        
        // Запись в файл. 
        fout << h << ";"
             << res.H << ";"
             << res.T << ";"
             << res.p << ";"
             << res.ro << ";"
             << res.g << ";"
             << res.a << "\n";
    }

    fout.close();
    cout << "Da file is da made\n";
}
