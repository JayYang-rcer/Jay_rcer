function [Speed] = threeB(C, T, Hz, DOF)
    N_all = Hz*T; % 总共有多少点
    
    %初始化速度结构体变量
    Speed.y  = zeros(1,N_all-1); %函数值
    Speed.v  = zeros(1,N_all-1); %速度
    Speed.a  = zeros(1,N_all-1); %加速度
    
    NC=length(C); %原始控制点数
    RC=[C(:,1),C(:,1),C(:,1),C(:,2:NC-1),C(:,NC),C(:,NC),C(:,NC)];
    NC=NC+1;
    
    for i = 1:N_all-1
        t_now = i / N_all * T;
        %归一化时间
        k = floor(t_now / T * NC);
        t = t_now - k * T / NC;
        t = t * NC / T;
        
        %四个位置样条函数
        f1s=(1-t)^3/6; 
        f2s=(3*t^3-6*t^2+4)/6;
        f3s=(-3*t^3+3*t^2+3*t+1)/6;
        f4s=t^3/6;      
        
        Speed.y(i) = RC(DOF,k+1)*f1s + RC(DOF,k+2)*f2s + RC(DOF,k+3)*f3s + RC(DOF,k+4)*f4s;
        if i == 1
            Speed.v(i) = 0;
            Speed.a(i) = 0;
        else
            Speed.v(i) = (Speed.y(i) - Speed.y(i-1)) / (T / N_all);
            Speed.a(i) = (Speed.v(i) - Speed.v(i-1)) / (T / N_all);
        end
    end
end

