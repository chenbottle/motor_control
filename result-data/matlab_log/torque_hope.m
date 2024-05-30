clc;
clear;
load("data.mat");
m = 0;
n = 0;
X = motor_data.sky_state;
for i = 1:length(X)
    if X(i) == 3 && m == 0
        m = i+5;
    end
    if X(i) == 0 && n == 0
        n = i-1;
    end
end
% n = m + 300;
n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
plot(t1,motor_data.torque_hope(m:n,3),'LineWidth',3);
hold on;
plot(t1,motor_data.torque_hope(m:n,4),'LineWidth',3);
ylabel('torque N.m','FontSize',14);
legend('hope1','hope2');
grid on;
