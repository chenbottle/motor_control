x1=0;
y1=0.187;
x2=0.16;
y2=0.2;
x3=0.32;
y3=0.48;
A=[1 x1 x1^2 x1^3 x1^4 0 0 0 0 0;
   1 x2 x2^2 x2^3 x2^4 0 0 0 0 0;
   0 0 0 0 0 1 x2 x2^2 x2^3 x2^4;
   0 0 0 0 0 1 x3 x3^2 x3^3 x3^4;
   0 1 2*x2 3*x2^2 4*x2^3 0 -1 -2*x2 -3*x2^2 -4*x2^3;
   0 0 2 6*x2 12*x2^2 0 0 -2 -6*x2 -12*x2^2;
%    0 0 0 6 24*x2 0 0 0 -6 -24*x2;
   0 0 2 6*x1 12*x1^2 0 0 0 0 0;
   0 1 2*x1 3*x1^2 4*x1^3 0 0 0 0 0;
   0 0 0 0 0 0 0 2 6*x3 12*x3^2;
   0 0 0 0 0 0 1 2*x3 3*x3^2 4*x3^3];
Y=[y1 y2 y2 y3 0 0 15 0 -9.81 3]';
X=inv(A)*Y;
m1=linspace(x1,x2,100);
deltat = (x2-x1)/100;
n1=X(1)+X(2)*m1+X(3)*m1.^2+X(4)*m1.^3 + X(5)*m1.^4;
n1dot = X(2)+2*X(3)*m1+3*X(4)*m1.^2 + 4*X(5)*m1.^3;
n1dott = 2*X(3)+6*X(4)*m1 + 12*X(5)*m1.^2;

m2=linspace(x2,x3,100);
n2=X(6)+X(7)*m2+X(8)*m2.^2+X(9)*m2.^3 + X(10)*m2.^4;
n2dot = X(7)+2*X(8)*m2+3*X(9)*m2.^2 + 4*X(10)*m2.^3;
n2dott = 2*X(8)+6*X(9)*m2 + 12*X(10)*m2.^2;
subplot(3,1,1);
plot(m1,n1);
hold on;
plot(m2,n2);

subplot(3,1,2);
plot(m1,n1dot);
hold on;
plot(m2,n2dot);

subplot(3,1,3);
plot(m1,n1dott);
hold on;
plot(m2,n2dott);