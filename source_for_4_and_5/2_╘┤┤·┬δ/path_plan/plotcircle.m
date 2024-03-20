function [] = plotcircle(x,y,R,r,g,b)

theta=0:0.1:2*pi;
Circle1=x+R*cos(theta);
Circle2=y+R*sin(theta);
c=[123,14,52];
plot(Circle1,Circle2,'c','linewidth',1);hold on;
fill(Circle1,Circle2,[r,g,b]);
axis equal
end


