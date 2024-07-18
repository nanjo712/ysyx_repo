/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// this should be enough
static char buf[65536] = {};
static char buf_with_u[131072] = {};
static char code_buf[131072 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static int max_tokens = 6000;
static int use_tokens = 0;

static inline int choose(int n) {
  return rand() % n;
}

static void gen(char c) {
  buf_with_u[strlen(buf_with_u)] = c;
  buf[strlen(buf)] = c;
}

static void gen_num() {
  use_tokens ++;
  int len = 1 + choose(10);
  if (len == 1) {
    gen(choose(10) + '0');
  } else {
    gen(choose(9) + '1');
    for (int i = 1; i < len; i ++) {
      gen(choose(10) + '0');
    }
  }
}

static void gen_rand_op() {
  use_tokens ++;
  switch (choose(4)) {
    case 0: gen('+'); break;
    case 1: gen('-'); break;
    case 2: gen('*'); break;
    case 3: gen('/'); break;
  }
}

static void gen_blank() {
  if (choose(2)) gen(' '); 
}

static void gen_rand_expr() {
  gen_blank();
  if (max_tokens-use_tokens <= 2) 
  {
    gen_num();
  } 
  else switch (choose(3)) {
    case 0: strcat(buf_with_u, "(unsigned)");
            gen_num(); break;
    case 1: use_tokens+=2u; gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
  gen_blank();
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    memset(buf, 0, sizeof(buf));
    memset(buf_with_u, 0, sizeof(buf_with_u));
    use_tokens = 0;
    gen_rand_expr();
    sprintf(code_buf, code_format, buf_with_u);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Wall -Werror /tmp/.code.c -o /tmp/.expr > /dev/null 2>&1"); 
    if (ret != 0) 
    {
      perror("[WARNING] Got a ERROR, regenerating.");  
      i = i-1; 
      continue;
    }

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);   

    int result;
    ret = fscanf(fp, "%d", &result);
    int status = pclose(fp);
    if (status != 0) 
    {
      perror("[WARNING] Got a FPE, regenerating.");  
      i = i-1; 
      continue;   
    }
    else
    {
      printf("%u %s\n", result, buf);
      char *s = malloc(65536+128);
      sprintf(s, "[INFO] Successfully generated: %d", i);
      perror(s);
    }
  }
  return 0;
}
