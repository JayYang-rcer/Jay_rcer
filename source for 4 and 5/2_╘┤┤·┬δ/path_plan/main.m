function [Speedx , Speedy, Speedyaw] = main(n, T, Hz, x, y, yaw, Robot_R)
    C = [x; y; yaw];
    Speedx = threeB(C, T, Hz, 1);
    Speedy = threeB(C, T, Hz, 2);
    Speedyaw = threeB(C, T, Hz, 3);
%     Speedmotor1.v = cosd(Speedyaw.y-45).*Speedy.v - sind(Speedyaw.y-45).*Speedx.v + Robot_R*Speedyaw.v;
%     Speedmotor2.v = cosd(Speedyaw.y+45).*Speedy.v - sind(Speedyaw.y+45).*Speedx.v + Robot_R*Speedyaw.v;
%     Speedmotor3.v = cosd(Speedyaw.y-45).*Speedy.v + sind(Speedyaw.y-45).*Speedx.v + Robot_R*Speedyaw.v;
%     Speedmotor4.v = cosd(Speedyaw.y+45).*Speedy.v + sind(Speedyaw.y+45).*Speedx.v + Robot_R*Speedyaw.v;
%     Speedmotor1.a = diff([0 Speedmotor1.v])*Hz;
%     Speedmotor2.a = diff([0 Speedmotor2.v])*Hz;
%     Speedmotor3.a = diff([0 Speedmotor3.v])*Hz;
%     Speedmotor4.a = diff([0 Speedmotor4.v])*Hz;
    
    figure(1);
    subplot(2, 1, 1);grid on;
    plot(1/Hz:1/Hz:T-1/Hz, sqrt(Speedx.v.^2+Speedy.v.^2),'-g','LineWidth',1);xlabel('速度');
    subplot(2, 1, 2);grid on;
    plot(1/Hz:1/Hz:T-1/Hz, sqrt(Speedx.a.^2+Speedy.a.^2),'-r','LineWidth',1);xlabel('加速度');
    
    figure(2);
    hl = plot(Speedx.y , Speedy.y ,'y-','LineWidth',3);
    for i = n:-1:1
        hp(i) = patch('xdata',x(i),'ydata',y(i),...
            'linestyle','none','facecolor','none',...
            'marker','o','markerEdgecolor','k',...
            'buttonDownFcn',@drag,'userdata',i);
    end
    ax = gca;
    xl = get(ax,'xlim');
    yl = get(ax,'ylim');
    idx = [];
    of = [];
    function drag(src,~)
        idx = get(src,'userdata');
        of = get(gcbf,{'WindowButtonMotionFcn','WindowButtonUpFcn'});
        set(gcbf,'WindowButtonMotionFcn',@move,'WindowButtonUpFcn',@drop);
    end
    function move(~,~)
        cp = get(ax,'currentPoint');
        xn = min(max(cp(1),xl(1)),xl(2));
        yn = min(max(cp(3),yl(1)),yl(2));
        set(hp(idx),'xdata',xn,'ydata',yn)
        x(idx) = xn;
        y(idx) = yn;
        C = [x; y; yaw];
        
        Speedx = threeB(C, T, Hz, 1);
        Speedy = threeB(C, T, Hz, 2);
        Speedmotor1.v = cosd(Speedyaw.y-45).*Speedy.v - sind(Speedyaw.y-45).*Speedx.v + Robot_R*Speedyaw.v;
        Speedmotor2.v = cosd(Speedyaw.y+45).*Speedy.v - sind(Speedyaw.y+45).*Speedx.v + Robot_R*Speedyaw.v;
        Speedmotor3.v = cosd(Speedyaw.y-45).*Speedy.v + sind(Speedyaw.y-45).*Speedx.v + Robot_R*Speedyaw.v;
        Speedmotor4.v = cosd(Speedyaw.y+45).*Speedy.v + sind(Speedyaw.y+45).*Speedx.v + Robot_R*Speedyaw.v;
        Speedmotor1.a = diff([0 Speedmotor1.v])*Hz;
        Speedmotor2.a = diff([0 Speedmotor2.v])*Hz;
        Speedmotor3.a = diff([0 Speedmotor3.v])*Hz;
        Speedmotor4.a = diff([0 Speedmotor4.v])*Hz;
        
        hl.XData = Speedx.y;
        hl.YData = Speedy.y;
        
        figure(1);
        subplot(2, 1, 1);grid on;
        plot(1/Hz:1/Hz:T-1/Hz, sqrt(Speedx.v.^2+Speedy.v.^2),'-g','LineWidth',1);xlabel('速度');
        subplot(2, 1, 2);grid on;
        plot(1/Hz:1/Hz:T-1/Hz, sqrt(Speedx.a.^2+Speedy.a.^2),'-r','LineWidth',1);xlabel('加速度');
    end
    function drop(src,~)
        set(src,'WindowButtonMotionFcn',of{1},'WindowButtonUpFcn',of{2});  
%         figure(3);
%         subplot(2, 2, 1);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor1.v,'-b','LineWidth',1);xlabel('Motor1-v');
%         subplot(2, 2, 2);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor2.v,'-b','LineWidth',1);xlabel('Motor2-v');
%         subplot(2, 2, 3);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor3.v,'-b','LineWidth',1);xlabel('Motor3-v');
%         subplot(2, 2, 4);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor4.v,'-b','LineWidth',1);xlabel('Motor4-v');
%         figure(4);
%         subplot(2, 2, 1);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor1.a,'-r','LineWidth',1);xlabel('Motor1-a');
%         subplot(2, 2, 2);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor2.a,'-r','LineWidth',1);xlabel('Motor2-a');
%         subplot(2, 2, 3);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor3.a,'-r','LineWidth',1);xlabel('Motor3-a');
%         subplot(2, 2, 4);grid on;
%         plot(1/Hz:1/Hz:T-1/Hz, Speedmotor4.a,'-r','LineWidth',1);xlabel('Motor4-a');
        clc;x, y
    end
end