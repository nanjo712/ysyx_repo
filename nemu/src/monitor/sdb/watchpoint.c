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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[32];
  word_t old_value;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
// static限制作用域，本质上是为了封装，
// 这些链表的维护由watchpoint.c完成，外部无法直接访问

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

bool check_wp()
{
  WP *p=head;
  bool tag = false;
  while (p!=NULL)
  {
    bool success=true;
    word_t new_value=expr(p->expr,&success);
    if (!success)
    {
      Log("Invalid expression at watchpoint %d: %s\n",p->NO,p->expr);
      continue;
    }
    if (new_value!=p->old_value)
    {
      Log("Watchpoint %d: %s",p->NO,p->expr);
      Log("Old value: %u",p->old_value);
      Log("New value: %u",new_value);
      p->old_value=new_value;
      tag=true;
    }
    p=p->next;
  }
  return tag;
}

WP* get_from_no(int no)
{
  return &wp_pool[no];
}

WP *new_wp()
{
  if (free_==NULL)
  {
    Log("No free watchpoint");
    assert(0);
    return NULL;
  }
  WP* new_wp=free_;
  free_=free_->next; // free_指向下一个空闲的watchpoint
  new_wp->next=head;
  head=new_wp; // 头插法，head指向新的watchpoint
  return new_wp;
}

void free_wp(WP *wp)
{
  WP *p=head;
  if (p==wp)
  {
    head=head->next;
    wp->next=free_;
    free_=wp; // 归还空闲watchpoint
    return;
  }
  while (p->next!=NULL)
  {
    if (p->next==wp)
    {
      p->next=wp->next;
      wp->next=free_;
      free_=wp;
      return;
    }
    p=p->next;
  }
}

void print_wp()
{
  Log("Watchpoint    Expression    Value");
  for (WP *i=head;i!=NULL;i=i->next)
  {
    Log("%-13d%-14s%-14u",i->NO,i->expr,i->old_value);
  }
}

