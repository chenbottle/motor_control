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
    if X(i) == 3 && n == 0
        n = i-1;
    end
end
% n = m + 300;
n = length(X)-1;
t = 0:0.002:10000;
t1 = t(1:n-m+1);

plot(t1,motor_data.L_hipwheel(m:n,1),'LineWidth',3);
hold on;
plot(t1,motor_data.L_hipwheel(m:n,2),'LineWidth',3);
plot(t1,motor_data.L_down_diff(m:n,1)/2 + motor_data.L_down_diff(m:n,2)/2,'LineWidth',3);
% plot(t1,motor_data.L_down_diff(m:n,2),'LineWidth',3);
plot(t1,0.1*double(motor_data.sky_state(m:n)),'LineWidth',3);
hold on;
ylabel('knee/rad','FontSize',14);
legend('lhipwheel0','lhipwheel1','L-down-diff','sky-state');
grid on;
% 'center-calc',
