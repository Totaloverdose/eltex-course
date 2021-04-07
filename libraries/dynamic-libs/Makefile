LIBPATH := ./lib
OBJS := $(addprefix $(LIBPATH)/, sum.o subtraction.o division.o multiplication.o)
LIBNAME := $(LIBPATH)/libmycalc.so

main: main.c libmycalc.h $(LIBNAME)
	gcc main.c libmycalc.h -o main -L $(LIBPATH) -l mycalc -Wl,-rpath,$(LIBPATH)

$(LIBNAME): $(OBJS)
	gcc -shared $(OBJS) -o $(LIBNAME)

$(LIBPATH)/%.o: $(LIBPATH)/%.c
	gcc -c $< -o $@
