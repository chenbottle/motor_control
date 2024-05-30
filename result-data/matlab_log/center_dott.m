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
    if X(i) == 0 && n == 0 && m ~= 0
        n = i-1;
    end
end
% m =  1;
% n = n + 30;
n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
plot(t1,motor_data.body_center_dott_y(m:n),'LineWidth',3);
hold on
% plot(t1,motor_data.center_dott_y(m:n),'LineWidth',3);
plot(t1,motor_data.sky_state(m:n)*5,'LineWidth',3);
xlabel('t s','FontSize',14);
ylabel('hip/rad','FontSize',14);
hold on;
legend('body-center-dott-y','center-dott-y');
grid on;
