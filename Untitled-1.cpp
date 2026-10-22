// u=arct(25-5/270)
//dx/dt=Vcostheta
//dy/dt=Vsintheta
//mdV/dt=-Xa-Gsintheta
//mVdtheta/dt=-gcostheta

#include <cstdio> 
#include <cmath>
using namespace std;

int main() {
    double t = 0.0, V = 80.0, theta = atan2(5, 270), x = 0.0, y = 0.0, m = 0.05, a = 0.005, G = m*9.80665, h = 0;      // Н.У.
    double dt = 0.05;     // шаг 
    int    step = 0;      // счётчик шагов
    double Vx = V * cos(theta), Vy = V * sin(theta);

    while (true) { 
        
        theta = atan2(Vy, Vx);
        x += Vx*dt;
        y += Vy*dt;
        Vx += (-V*a/m*cos(theta))*dt;
        Vy += (-V*a/m*sin(theta) - G)*dt;
        if (theta > 0) h = y;

        t += dt;
        ++step;

        printf("step %d, t = %10.2f, x = %10.3f, y = %10.3f, theta = %10.4f\n", step, t, x, y, theta);

        if (y < 0) break;
    }
    
        printf("h = %10.3f\n", h);
}