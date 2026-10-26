// u=arct(25-5/270)
//dx/dt=Vcostheta
//dy/dt=Vsintheta
//mdV/dt=-Xa-Gsintheta
//mVdtheta/dt=-gcostheta

#include <cstdio> 
#include <cmath>
using namespace std;

int main() {
    double t = 0.0, V = 80.0, theta = atan2(5, 270), x = 0.0, y = 0.0, m = 0.05, a = 0.005, G = m*9.80665;      // Н.У.
    double h = 0.0, Zero1 = 0.0, Zero2 = 0.0;      // Измеряемые величины
    double dt = 0.001;     // шаг 
    int    step = 0;      // счётчик шагов
    double Vx = V * cos(theta), Vy = V * sin(theta);


    FILE *file = fopen("graph.csv", "w"); // Открываем файл
    fprintf(file, "x\ty\n"); // Заголовки

    while (true) { 
        //printf("Vx = %10.3f, Vy = %10.3f, theta = %10.4f\n", Vx, Vy, theta);
        theta = atan2(Vy, Vx);
        x += Vx*dt;
        y += Vy*dt;
        Vx += (-V*a/m*cos(theta))*dt;
        Vy += (-V*a/m*sin(theta) - G)*dt;
        if (theta > 0) h = y;
        if (h == y && y < 0.02) Zero1 = x;
        if (h > y && y > 0.02) Zero2 = x;

        fprintf(file, "%f\t%f\n", x, y); // Данные через табуляцию
        t += dt;
        ++step;

        printf("step %d, t = %10.2f, x = %10.3f, y = %10.3f, h = %10.3f, theta = %10.4f\n, ", step, t, x, y, h, theta);
        if (y < 0) break;
    }
    
    fclose(file);
        printf("h = %10.3f, Zero1 = %10.3f, Zero2 = %10.3f, distance = %10.3f\n", h, Zero1, Zero2, x);
}
