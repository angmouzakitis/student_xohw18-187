all: run_readok.x run_readdenied.x run_writedenied.x

run_readok.x: fw_clinic1_access.c
	gcc -D_READOK -o $@ fw_clinic1_access.c

run_readdenied.x: fw_clinic1_access.c
	gcc -D_READDEDENIED -o $@ fw_clinic1_access.c

run_writedenied.x: fw_clinic1_access.c
	gcc -D_WRITEDENIED -D_READOK -o $@ fw_clinic1_access.c
clean: 
	-@rm -f run_*
	-@rm -f logs_all

runtests: all
	@printf '<<< run_readok.x >>>\n' > logs_all
	@./run_readok.x >> logs_all 2>&1 || true
	@printf '\n\n<<< run_readdenied.x >>>\n' >> logs_all
	@./run_readdenied.x >> logs_all 2>&1 || true
	@printf '\n\n<<< run_writedenied.x >>>\n' >> logs_all
	@./run_writedenied.x >> logs_all 2>&1 || true
	@printf "Logs at 'logs_all'\n\n"
	@bash tests_verification.sh

