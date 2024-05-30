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
% n = m + 300;
n = length(X);
t = 0:0.002:10000;
t1 = t(1:n-m+1);
subplot(3,1,1);
plot(t1,motor_data.theta_ground(m:n,1),'LineWidth',3);
hold on;
plot(t1,motor_data.theta_ground(m:n,2),'LineWidth',3);
plot(t1,hope_angle_data.angle_hope(m:n,1),'LineWidth',3);
ylabel('ground/rad','FontSize',14);
legend('actual1','actual2','hope');
grid on;

subplot(3,1,2);
plot(t1,motor_data.angle_actual_rad(m:n,3),'LineWidth',3);
hold on;
plot(t1,motor_data.angle_actual_rad(m:n,4),'LineWidth',3);
plot(t1,hope_angle_data.angle_hope(m:n,2),'LineWidth',3);
% xlabel('t','FontSize',14);
ylabel('knee/rad','FontSize',14);
legend('actual3','actual4','hope');
grid on;

subplot(3,1,3);
plot(t1,-motor_data.angle_actual_rad(m:n,5),'LineWidth',3);
hold on;
plot(t1,-motor_data.angle_actual_rad(m:n,6),'LineWidth',3);
plot(t1,hope_angle_data.angle_hope(m:n,3),'LineWidth',3);
xlabel('t s','FontSize',14);
ylabel('hip/rad','FontSize',14);
hold on;
legend('actual5','actual6','hope');
grid on;
