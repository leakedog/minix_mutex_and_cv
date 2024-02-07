SRC=src
BIN=bin

ALL= \
	$(BIN)/lock_unlock \
	$(BIN)/simple_many \
	$(BIN)/many_procs \
	$(BIN)/many_pm \
	$(BIN)/check_fifo \
	$(BIN)/simple_cv \
	$(BIN)/many_cv \
	$(BIN)/many_cv2 \
	$(BIN)/tictac \
	$(BIN)/tictactac \
	$(BIN)/prod_cons \
	$(BIN)/lock_hang \
	$(BIN)/sigint \
	$(BIN)/lock_exit \
	$(BIN)/mlocks_exit \
	$(BIN)/lock_kill \
	$(BIN)/mlocks_kill \
	$(BIN)/cond_kill \
	$(BIN)/mconds_kill \


all: $(ALL)

clean:
	rm -rf $(ALL)
	rm -f out/*

$(BIN)/lock_unlock: $(SRC)/lock_unlock.c
	cc $? -o $@

$(BIN)/simple_many: $(SRC)/simple_many.c
	cc $? -o $@

$(BIN)/many_procs: $(SRC)/many_procs.c
	cc $? -o $@

$(BIN)/many_pm: $(SRC)/many_pm.c
	cc $? -o $@

$(BIN)/check_fifo: $(SRC)/check_fifo.c
	cc $? -o $@

$(BIN)/simple_cv: $(SRC)/simple_cv.c
	cc $? -o $@

$(BIN)/many_cv: $(SRC)/many_cv.c
	cc $? -o $@

$(BIN)/many_cv2: $(SRC)/many_cv2.c
	cc $? -o $@

$(BIN)/tictac: $(SRC)/tictac.c
	cc $? -o $@

$(BIN)/tictactac: $(SRC)/tictactac.c
	cc $? -o $@

$(BIN)/prod_cons: $(SRC)/prod_cons.c
	cc $? -o $@

$(BIN)/lock_hang: $(SRC)/lock_hang.c
	cc $? -o $@

$(BIN)/sigint: $(SRC)/sigint.c
	cc $? -o $@

$(BIN)/lock_exit: $(SRC)/lock_exit.c
	cc $? -o $@

$(BIN)/mlocks_exit: $(SRC)/mlocks_exit.c
	cc $? -o $@

$(BIN)/lock_kill: $(SRC)/lock_kill.c
	cc $? -o $@

$(BIN)/mlocks_kill: $(SRC)/mlocks_kill.c
	cc $? -o $@

$(BIN)/cond_kill: $(SRC)/cond_kill.c
	cc $? -o $@

$(BIN)/mconds_kill: $(SRC)/mconds_kill.c
	cc $? -o $@

