clc;
clear;
load("/home/chen/Documents/knowledge/2wl_control/result-data/matlab_log/data.mat");
m = 4836;%pid 300 jacobi 330 dynamic 500
n = 4875;
subplot(3,1,1);
plot(motor_data.theta_ground(1,m:n),'LineWidth',3);
hold on;
% xlim([14.8,17.1]);
plot(hope_angle_data.angle_hope(m:n,1),'LineWidth',3);
% title('jump');
% xlabel('时刻数','FontSize',14);
ylabel('ground/rad','FontSize',14);
legend('actual','hope');
grid on;
subplot(3,1,2);
plot(motor_data.angle_actual_rad(m:n,3),'LineWidth',3);
hold on;
plot(motor_data.angle_actual_rad(m:n,4),'LineWidth',3);
plot(hope_angle_data.angle_hope(m:n,2),'LineWidth',3);
% xlabel('时刻数','FontSize',14);
ylabel('knee/rad','FontSize',14);
legend('l','r','hope');
grid on;
subplot(3,1,3);
plot(-motor_data.angle_actual_rad(m:n,5),'LineWidth',3);
hold on;
plot(-motor_data.angle_actual_rad(m:n,6),'LineWidth',3);
% +(180-75.01)/180*pi)
plot(hope_angle_data.angle_hope(m:n,3),'LineWidth',3);
xlabel('时刻数','FontSize',14);
ylabel('hip/rad','FontSize',14);
hold on;
legend('l','r','hope');
grid on;