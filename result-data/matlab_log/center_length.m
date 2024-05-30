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
% n = m + 300;
% n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
plot(t1,motor_data.body_center_y_ac(m:n),'LineWidth',3);
hold on;
plot(t1,motor_data.body_center_y_calc(m:n),'LineWidth',3);
plot(t1,motor_data.center_distance(m:n),'LineWidth',3);
xlabel('t s','FontSize',14);
ylabel('distance m','FontSize',14);
hold on;
legend('body-center-y-ac','body-center-y-calc','center-distance');
grid on;
