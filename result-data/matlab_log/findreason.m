clc;
clear;
load("data.mat");
m = 0;
n = 0;
X = motor_data.sky_state;
for i = 1:length(X)
    if X(i) == 1 && m == 0
        m = i+5;
    end
    if X(i) == 2 && n == 0
        n = i-1;
    end
end
m = 9000;
n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
plot(t1,motor_data.x_state(m:n,1),'LineWidth',3);
hold on;
plot(t1,motor_data.torque_hope(m:n,1),'LineWidth',3);
plot(t1,motor_data.current_actual_float(m:n,1),'LineWidth',3);
% ylabel('x-state','torque','current',14);
legend('x-state','torque','current');
grid on;
