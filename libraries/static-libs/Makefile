LIBDIR := ./lib
OBJS := $(addprefix $(LIBDIR)/, division.o multiplication.o subtraction.o sum.o)
LIBNAME := $(LIBDIR)/libmycalc.a

main: main.c libmycalc.h $(LIBNAME)
	gcc main.c -o main -L $(LIBDIR) -l mycalc

$(LIBNAME): $(OBJS)
	ar rc $(LIBNAME) $(OBJS)

$(LIBDIR)/%.o: $(LIBDIR)/%.c
	gcc -c $< -o $@
