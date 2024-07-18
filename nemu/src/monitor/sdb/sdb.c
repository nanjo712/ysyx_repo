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

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>
#include "sdb.h"
#include <common.h>
#include <utils.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[32];
  word_t old_value;
} WP;

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
bool check_wp();
WP* get_from_no(int no);
WP *new_wp();
void free_wp(WP *wp);
void print_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  set_nemu_state(NEMU_QUIT, cpu.pc, 0);
  return -1;
}

static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  int n = 1;
  if (arg != NULL) {
    sscanf(arg, "%d", &n);
  }
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  if (strcmp(arg, "r") == 0) {
    isa_reg_display();
  }
  else if (strcmp(arg, "w") == 0) {
    print_wp();
  }
  return 0;
}

static int cmd_x(char *args)
{
  char *arg = strtok(NULL, " ");
  int n=0;
  if (arg==NULL) return 1;
  n=atoi(arg);
  arg = strtok(NULL, " ");
  if (arg==NULL) return 1;
  vaddr_t address;
  bool flag;
  address = expr(arg, &flag);
  if (!flag) return 1;
  for (int i=0;i<n;i++)
  {
    word_t result = vaddr_read(address, 4);
    printf("%08x: %08x\n", address, result);
    address += 4;
  }
  return 0;
}

static int cmd_p(char *args) {
  bool flag;
  unsigned result=expr(args, &flag);
  if (!flag) printf("Invalid expression\n");
  else printf("%u\n", result);
  return 0;
}

static int cmd_w(char *args) {
  WP *wp = new_wp();
  strcpy(wp->expr, args);
  bool flag;
  wp->old_value = expr(args, &flag);
  printf("Watchpoint %d: %s\n", wp->NO, wp->expr);
  return 0;
}

static int cmd_d(char *args) {
  int n;
  sscanf(args, "%d", &n);
  WP *wp = get_from_no(n);
  free_wp(wp);
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "si [n] : Execute n instruction (Default n=1)", cmd_si },
  { "x",  "x N EXPR : Show N word in memory at address EXPR", cmd_x },
  { "info", "info r : Print register state", cmd_info },
  { "info", "info w : Print watchpoint information", cmd_info },
  { "p", "p EXPR : Print the value of EXPR", cmd_p },
  { "w", "w EXPR : Set a watchpoint for EXPR", cmd_w },
  { "d", "d N : Delete watchpoint N", cmd_d },
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

int record[20000];
int rec_cnt = 0;

void test_expr()
{
  char *buf = malloc(65536+128);
  FILE *f = fopen("/home/woshiren/ysyx-workbench/nemu/tools/gen-expr/build/input", "r");
  int cnt = 0; 
  while (fgets(buf, 65536+128, f)!=NULL)
  {
    char *s=strtok(buf, " ");
    // printf("Test %d: %s ", cnt, s);
    unsigned long result = strtoul(s, NULL, 10);
    char *expr_s = s + strlen(s) + 1 ;
    // printf("%s", expr_s);
    expr_s[strlen(expr_s)-1] = '\0';
    unsigned long result_m = expr(expr_s, NULL);
    // printf("Expected: %ld, Got: %ld\n", result, result_m);
    if (result!=result_m) 
    {
      // printf("Failed %d\n",cnt);
      record[rec_cnt++] = cnt;
    }
    else 
    {
      // printf("Passed %d\n",cnt);
    }
    cnt++;
  }

  printf("Total %d tests, %d passed, %d failed\n", cnt, cnt-rec_cnt, rec_cnt);
  for (int i=0;i<rec_cnt;i++)
  {
    printf("%d ", record[i]);
  }
  free(buf);
  fclose(f);
}

void sdb_mainloop() {
  test_expr();

  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { 
      printf("Unknown command '%s'\n", cmd); 
    }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
