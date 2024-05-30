clc;
clear;
load("pid_jump.mat");
m = 500;
n = m + 420;
h1 = plot(state_msgs.torque_hope(m:n,3));
hold on;
h2 = plot(state_msgs.torque_actual(m:n,3));
title('knee-torque');
xlabel('ʱ����','FontSize',16);
ylabel('torque/N��m','FontSize',16);
legend([h1,h2],'torquehope','torqueactual');
grid on;