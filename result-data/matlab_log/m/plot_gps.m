clc;
clear;
load("jump_step.mat");
m = 1100;%pid 300 jacobi 330 dynamic 500 90
n = m+2200;
h1=plot(state_msgs.gps_wheel_xyz(m:n,3),'LineWidth',3);
hold on;
xlim([0,2200]);
h2=plot(state_msgs.gps_body_xyz(m:n,3),'LineWidth',3);
title('jump-step','FontSize',25);
xlabel('Ê±¿ÌÊý','FontSize',16);
ylabel('z/m','FontSize',16);
hold on;
% i = m;
% j = 1;
% while i <= n
% plot([j,j],[state_msgs.gps_wheel_xyz(i,3),state_msgs.gps_body_xyz(i,3)],'Color',[0.75,0.75,0.75]);
% i=i+1;
% end
legend([h1,h2],'rwheel','body');
grid on;
set(gca,'XMinorGrid','on');
set(gca,'YMinorGrid','on');