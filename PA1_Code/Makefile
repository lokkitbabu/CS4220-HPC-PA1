MPICXX = mpic++
CXXFLAGS = -Wall -O3 -Wextra -Werror=return-type -Wfatal-errors -Werror

SRC = pi.cpp pi_calc.cpp
EXEC = pi

all: $(EXEC)

$(EXEC): $(SRC)
	$(MPICXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(EXEC)
