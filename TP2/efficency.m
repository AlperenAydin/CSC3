close all; clear all;

N = 100;
t = zeros(1,N);

for i = 1:N
    i
   command = sprintf( '(time ./fractal %d)2>&1|grep real',i); 
   [status,cmdout] = system(command);
   l = length(cmdout);
   str = cmdout(l-6:l-2);
   t(i)= sscanf(str,'%f');
   
end

plot(t);
grid on
plot(t)
grid on;
xlabel('Nombre de threads')
ylabel('Le temps (s)')