clearvars
close all
clc

%% Police/Referee Whistle

% number of peas inside the whistle
%num_peas = 1;
fs = 44100; % sampling frequency in hertz
f0 = 1800; % fundamental frequency in hertz
f0_2 = 2*1800;
f0_3 = 3*1800;
dB = [0 -10 -25 -20 6];
dB_mag = db2mag(dB);
t = 1:1/fs:5; % time passed by
% initial velocitys
int_pea_vel_x = 8;
int_pea_vel_y = 8;
% Loss factor for velocity
los_fac = 0.98;
% int_pea_vel_y = 8 * rand;
% initial positions
r = 0.03; % radius
theta = 2 * pi * rand;
int_pea_pos_x = r * rand * cos(theta);
int_pea_pos_y = r * rand * sin(theta);
% initial direction
dir = 2 * pi * rand;
% draw circle
%t = linspace(0,0.03 * pi);
%plot(cos(t),sin(t))
A = pi * r;
th = 0:pi/50:2*pi;
xunit = r * cos(th);
yunit = r * sin(th);
% figure
% hold on
% plot(xunit, yunit);
% plot(int_pea_pos_x, int_pea_pos_y, 'gO')
% hold off
% position of opening
x_op = 0.02;
y_op = 0.02;

% white noise
wn = 2*rand(1,length(t)); % producing white noise
wn = wn - mean(wn); % make sure that x < 1

final_dis = [];

% moving pea
for a = 1:length(t)-1
    if a == 1
        % change x and y position according to time step
        delta_t = t(a + 1) - t(a);
        x = (int_pea_vel_x + (int_pea_pos_x/delta_t)) * delta_t;
        y = (int_pea_vel_y + (int_pea_pos_y/delta_t)) * delta_t;
%         pea_x = x;
%         pea_y = y;
          % plotting the circle and start position of pea
%         figure
%         plot(xunit, yunit)
%         plot(x, y, 'gO')
        % get the next x and y position
        next_x = x;
        next_y = y;
    else
        % after first round compute next x and y based on the previous
        % values
        delta_t = t(a + 1) - t(a);
        next_x = next_x + int_pea_vel_x * delta_t;
        next_y = next_y + int_pea_vel_y * delta_t;
%         next_x = (int_pea_vel_x + (next_x/delta_t)) * delta_t;
%         next_y = (int_pea_vel_y + (next_y/delta_t)) * delta_t;
          % draw changes of pea position
%         clf
%         hold on
%         plot(xunit, yunit)
%         plot(next_x, next_y, 'gO')
%         drawnow
        % check if pea is next to the edge of the circle
        next_r = sqrt(next_x^2 + next_y^2);
        if next_r >= r 
            % if so then reverse the last step
            next_x = next_x - int_pea_vel_x * delta_t;
            next_y = next_y - int_pea_vel_y * delta_t;
            %             next_x = (int_pea_vel_x - (next_x/delta_t)) * delta_t;
%             next_y = (int_pea_vel_y - (next_y/delta_t)) * delta_t;
%             int_pea_vel_x = int_pea_vel_x * los_fac;
%             int_pea_vel_y = int_pea_vel_y * los_fac;
%             var = -1 + (1+1)*rand;
%             if var <= 0
%                 int_pea_vel_x = -int_pea_vel_x;
%             else
%                 int_pea_vel_y = -int_pea_vel_y;
%             end
            % rotate velocities and reverse only one velocity value
            % multiply that with a los factor due to friction
            phi = -atan2(next_x, next_y);            
            cosphi = cos(phi);
            sinphi = sin(phi);
            temp1 = (sinphi * int_pea_vel_x) - (cosphi * int_pea_vel_y);
            temp2 = (cosphi * int_pea_vel_x) + (sinphi * int_pea_vel_y);
            temp1 = temp1 * los_fac;
            temp2 = temp2 * los_fac;
            int_pea_vel_x = (-sinphi * temp1) + (cosphi * temp2);
            int_pea_vel_y = (cosphi * temp1) + (sinphi * temp2);
        end
    end
%     pea_x = [pea_x next_x];
%     pea_y = [pea_y next_y];
    % compute the distance from pea to fipple
    dis = sqrt((next_x - x_op)^2 + (next_y - y_op)^2);
    final_dis = [final_dis dis];
    
    
    %     next_r = sqrt(next_x^2 + next_y^2);
    %     next_th = arctan(next_y/next_x);
    
    
end
hold off
% compute frequency harmonics
sig = (dB_mag(1) * cos(2 * pi * t * f0)) + (dB_mag(2) * cos(2 * pi * t * f0_2)) + (dB_mag(3) * cos(2 * pi * t * f0_3));
% map to scale of white noise
final_dis = final_dis * 1/0.06;

% compute signal and noise output with changing amplitude depending on
% position to the fipple, noise is reduced at all times
out_sig = sig(1:end-1) .* (db2mag((1-final_dis) * 6));
out_wn = wn(1:end-1) .* (db2mag(((1-final_dis) * 2) - 20));
% plot it
figure
hold on
plot(out_sig)
plot(out_wn)
hold off
% add signal and noise together and playback
out_final = out_sig + out_wn;
soundsc(out_final,fs)