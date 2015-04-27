CFLAGS = -W -Wall -Wextra -g
LDLIBS = -lm -pthread

exo1 = exo1 miroir miroir_2

exo2 = moyenne

exo3 = exo3_1 exo3_2

exo4 =  exo4_1 

exo5 = exo5_1

all     : $(exo1) $(exo2) $(exo3) $(exo4)

$(exo1) : % : %.o

$(exo2) : % : %.o

$(exo3) : % : %.o

$(exo4) : % : %.o

clean   : 
	rm *.o $(exo1) $(exo2) $(exo3)
