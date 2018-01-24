IDIR = -I/home/catherine/Documents/Markers
CC = clang++ -std=c++11
CFLAGS = $(IDIR) `pkg-config opencv --cflags`  `pkg-config eigen3 --cflags --libs`
ODIR = obj

LIBS = `pkg-config opencv --libs` -L/usr/local/lib/ -lGLEW -lglfw -lGL -L/usr/lib/x86_64-linux-gnu/ 



_OBJ =  main.o affineTransform.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.cpp ;
	$(CC) -c -o $@ $< $(CFLAGS) 


run: $(OBJ) ;
	clang++ -o $@ $^ $(CFLAGS) $(LIBS)

# .PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

