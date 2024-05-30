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
    if X(i) == 3 && n == 0
        n = i-1;
    end
end
% m =  1;
% n = n + 30;
n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
subplot(3,1,1);
plot(t1,motor_data.current_actual_float(m:n,1),'LineWidth',3);
hold on;
plot(t1,-motor_data.current_actual_float(m:n,2),'LineWidth',3);
ylabel('wheel A','FontSize',14);
legend('current0','current1');
grid on;

subplot(3,1,2);
plot(t1,motor_data.current_actual_float(m:n,3),'LineWidth',3);
hold on;
plot(t1,-motor_data.current_actual_float(m:n,4),'LineWidth',3);
plot(t1,motor_data.sky_state(m:n)*10,'LineWidth',3);
ylabel('knee A','FontSize',14);
legend('current2','current3');
grid on;

subplot(3,1,3);
plot(t1,motor_data.current_actual_float(m:n,5),'LineWidth',3);
hold on;
plot(t1,-motor_data.current_actual_float(m:n,6),'LineWidth',3);

xlabel('t s','FontSize',14);
ylabel('hip A','FontSize',14);
hold on;
legend('current4','current5');
grid on;
