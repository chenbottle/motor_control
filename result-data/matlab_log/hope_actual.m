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
% m = 1;
n = length(X);
t = 0:0.001:20;
t = t(1:n-m+1);
subplot(3,1,1);
plot(t,hope_angle_data.angle_hope(m:n,4),'LineWidth',3);
hold on;
plot(t,motor_data.center_distance(m:n),'LineWidth',3);
ylabel('center-distance/m','FontSize',16);
title('hope-actual','FontSize',25);
legend('hope','actual','FontSize',20);
grid on;
set(gca,'XMinorGrid','on');
set(gca,'YMinorGrid','on');

subplot(3,1,2);
plot(t,hope_angle_data.angle_hope(m:n,2),'LineWidth',3);
hold on;
plot(t,motor_data.angle_actual_rad(m:n,3),'LineWidth',3);
ylabel('knee-theta/rad','FontSize',16);
legend('hope','actual','FontSize',20);
grid on;
set(gca,'XMinorGrid','on');
set(gca,'YMinorGrid','on');

subplot(3,1,3);
plot(t,hope_angle_data.angle_hope(m:n,3),'LineWidth',3);
hold on;
plot(t,motor_data.angle_actual_rad(m:n,5),'LineWidth',3);
ylabel('hip-theta/rad','FontSize',16);
xlabel('t s','FontSize',16);
legend('hope','actual','FontSize',20);
grid on;
set(gca,'XMinorGrid','on');
set(gca,'YMinorGrid','on');
% xlim([0,2200]);