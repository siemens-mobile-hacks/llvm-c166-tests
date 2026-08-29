extern void llvm_crt_init_proxy(void);
extern void c166_test_main(void);

void main(void) {
  llvm_crt_init_proxy();
  c166_test_main();
}
