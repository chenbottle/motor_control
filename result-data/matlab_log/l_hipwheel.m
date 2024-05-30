clc;
clear;
load("data.mat");
m = 0;
n = 0;
X = motor_data.sky_state;
for i = 1:length(X)
    if X(i) == 2 && m == 0
        m = i+5;
    end
    if X(i) == 0 && n == 0 && m ~= 0
        n = i-1;
    end
end
% n = m + 300;
n = length(X)-1;
% n = n + 100;
t = 0:0.002:10000;
t1 = t(1:n-m+1);

plot(t1,motor_data.L_hipwheel(m:n,1),'LineWidth',3);
hold on;
plot(t1,motor_data.L_hipwheel(m:n,2),'LineWidth',3);
plot(t1,hope_angle_data.center_distance(m:n,1),'LineWidth',3);
plot(t1,hope_angle_data.center_distance(m:n,2),'LineWidth',3);
plot(t1,motor_data.L_down_diff(m:n,1),'LineWidth',3);
plot(t1,motor_data.L_down_diff(m:n,2),'LineWidth',3);
% plot(t1,motor_data.body_center_y_calc(m:n),'LineWidth',3);
plot(t1,motor_data.center_angle(m:n),'LineWidth',3);
plot(t1,0.1*double(motor_data.sky_state(m:n)),'LineWidth',3);
hold on;
ylabel('knee/rad','FontSize',14);
legend('lhipwheel','rhipwheel','lhipwheel-hope','rhipwheel-hope','lL-down-diff','rL-down-diff','center-angle','sky-state');
grid on;
% 'center-calc',
