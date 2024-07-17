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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NUMBER,
  TK_ADD,
  TK_SUB,
  TK_MUL,
  TK_DIV,
  TK_LEFT_BRACKET,
  TK_RIGHT_BRACKET,
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_ADD},         // plus
  {"==", TK_EQ},        // equal
  {"\\-", TK_SUB},         // sub
  {"\\*", TK_MUL},         // mul
  {"\\/", TK_DIV},         // div
  {"\\(", TK_LEFT_BRACKET},         // left bracket
  {"\\)", TK_RIGHT_BRACKET},         // right bracket
  {"[0-9]+", TK_NUMBER},         // number
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32768];   
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;
          case TK_ADD: tokens[nr_token].type = TK_ADD; nr_token++; break;
          case TK_SUB: tokens[nr_token].type = TK_SUB; nr_token++; break;
          case TK_MUL: tokens[nr_token].type = TK_MUL; nr_token++; break;
          case TK_DIV: tokens[nr_token].type = TK_DIV; nr_token++; break;
          case TK_LEFT_BRACKET: tokens[nr_token].type = TK_LEFT_BRACKET; nr_token++; break;
          case TK_RIGHT_BRACKET: tokens[nr_token].type = TK_RIGHT_BRACKET; nr_token++; break;
          case TK_NUMBER: 
            if (substr_len >= 32) {
              printf("number is too long\n");
              return false;
            }
            tokens[nr_token].type = TK_NUMBER;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static int op_priority(int type) {
  switch (type) {
    case TK_NUMBER: return 0x3f3f3f3f;
    case TK_ADD: return 1;
    case TK_SUB: return 1;
    case TK_MUL: return 2;
    case TK_DIV: return 2;
    case TK_LEFT_BRACKET: return 0x3f3f3f3f;
    case TK_RIGHT_BRACKET: return 0x3f3f3f3f;
    default: return 0x3f3f3f3f;
  }
}

static bool check_parentheses(int p, int q) {
  int i, cnt = 0;
  if (tokens[p].type != TK_LEFT_BRACKET || tokens[q].type != TK_RIGHT_BRACKET) {
    return false;
  }
  for (i = p + 1; i < q; i++) {
    if (tokens[i].type == TK_LEFT_BRACKET) {
      cnt++;
    } else if (tokens[i].type == TK_RIGHT_BRACKET) {
      cnt--;
    }
    if (cnt < 0) {
      return false;
    }
  }
  return cnt == 0;
}

static int dominant_operator(int p, int q) {
  int i, op = p, cnt = 0, min_priority = 0x3f3f3f3f;
  for (i = p; i <= q; i++) {
    if (tokens[i].type == TK_LEFT_BRACKET) {
      cnt++;
    } else if (tokens[i].type == TK_RIGHT_BRACKET) {
      cnt--;
    } else if (cnt == 0 && op_priority(tokens[i].type) <= min_priority && 
               (!(tokens[i].type == TK_ADD || tokens[i].type == TK_SUB) || 
                tokens[i-1].type == TK_RIGHT_BRACKET || tokens[i-1].type == TK_NUMBER || i == p)
              ) {
      min_priority = op_priority(tokens[i].type);
      op = i;
    }
  }
  return op;
}

static int eval(int p, int q) {
  if (p > q) {
    printf("Bad expression with p=%d and q=%d\n",p,q);
    assert(0);
  } else if (p == q) {
    int num = 0;
    sscanf(tokens[p].str, "%d", &num);
    return num;
  } else if (check_parentheses(p, q)) {
    return eval(p + 1, q - 1);
  } else {
    int op = dominant_operator(p, q);
    if (p==op)
    {
      switch (tokens[op].type) {
        case TK_ADD: return eval(p + 1, q);
        case TK_SUB: return -eval(p + 1, q);
        default: assert(0);
      }
    }
    else 
    {
      int val1 = eval(p, op - 1);
      int val2 = eval(op + 1, q);
      switch (tokens[op].type) {
        case TK_ADD: return val1 + val2;
        case TK_SUB: return val1 - val2;
        case TK_MUL: return val1 * val2;
        case TK_DIV: return val1 / val2;
        default: assert(0);
      }
    }
  }
}


int expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  int result = eval(0, nr_token - 1);
  *success = true;
  return result;
  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
