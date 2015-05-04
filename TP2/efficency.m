close all; clear all;

N = 25;
t = zeros(1,N);

for i = 1:N
   x = 0;
   i
   for j=1:10 
       j
       command = sprintf( '(time ./fractal %d)2>&1|grep real',i);
       [status,cmdout] = system(command);
       l = length(cmdout);
       str = cmdout(l-6:l-2);
       x = x +sscanf(str,'%f');
   end
   t(i) = x/10;
end

plot(t);
grid on
plot(t)
grid on;
xlabel('Nombre de threads')
ylabel('Le temps (s)')